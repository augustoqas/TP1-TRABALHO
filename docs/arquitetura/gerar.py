# -*- coding: utf-8 -*-
"""
Gera o Modelo de Arquitetura a partir de modelo.py.

    python gerar.py           -> diagrama.svg, arquitetura.html e arquitetura.pdf
    python gerar.py --png     -> tambem diagrama.png (2400 px, para conferencia)
    python gerar.py --sem-pdf -> so SVG e HTML

Somente biblioteca padrao (Python 3.8+). O PDF e impresso pelo Microsoft Edge
(ou Google Chrome) em modo headless; nada e baixado.

Este arquivo contem apenas codigo. Para mudar o conteudo, edite modelo.py.
"""
import html
import math
import re
import shutil
import subprocess
import sys
import time
import unicodedata
from pathlib import Path

AQUI = Path(__file__).resolve().parent
sys.path.insert(0, str(AQUI))
import modelo as M  # noqa: E402

AVISOS = []


def aviso(msg):
    AVISOS.append(msg)


# ==========================================================================
# Texto
# ==========================================================================
def esc(s):
    return html.escape(str(s), quote=True)


def marcar(s):
    """Escapa HTML e aplica a mini-marcacao `codigo` e **negrito**."""
    s = esc(s)
    s = re.sub(r"`([^`]+)`", r"<code>\1</code>", s)
    s = re.sub(r"\*\*([^*]+)\*\*", r"<strong>\1</strong>", s)
    return s


def largura(txt, tam, negrito=False, mono=False):
    """Estimativa (conservadora) da largura do texto em Segoe UI / Consolas."""
    if mono:
        return 0.55 * tam * len(txt)
    tot = 0.0
    for ch in unicodedata.normalize("NFD", txt):
        if unicodedata.combining(ch):
            continue
        if ch in "iljI.,:;'|!":
            f = 0.27
        elif ch in "ftr()[]-/ ":
            f = 0.36
        elif ch in "mw":
            f = 0.82
        elif ch in "MW":
            f = 0.92
        elif ch.isupper():
            f = 0.66
        elif ch.isdigit():
            f = 0.56
        else:
            f = 0.55
        tot += f
    return tot * tam * (1.07 if negrito else 1.0)


def quebrar(txt, larg_max, tam):
    linhas, atual = [], ""
    for p in txt.split():
        teste = (atual + " " + p).strip()
        if atual and largura(teste, tam) > larg_max:
            linhas.append(atual)
            atual = p
        else:
            atual = teste
    if atual:
        linhas.append(atual)
    return linhas


def faixa(ids):
    """[16..26] -> '16–26'; [1, 3] -> '1, 3'."""
    ids = sorted(set(ids))
    if not ids:
        return ""
    if len(ids) > 2 and ids == list(range(ids[0], ids[-1] + 1)):
        return f"{ids[0]}–{ids[-1]}"
    return ", ".join(str(i) for i in ids)


# ==========================================================================
# Indices do modelo + validacao
# ==========================================================================
MOD = {m["id"]: m for m in M.MODULOS}
PAC = {p["id"]: p for p in M.PACOTES}
INTF = {i["nome"]: i for i in M.INTERFACES}


def validar_modelo():
    erros = []
    for m in M.MODULOS:
        if m["pacote"] not in PAC:
            erros.append(f"módulo {m['id']}: pacote '{m['pacote']}' inexistente")
        if "coluna" in m and m["coluna"] not in M.COLUNAS:
            erros.append(f"módulo {m['id']}: coluna '{m['coluna']}' não está em COLUNAS")
    for i in M.INTERFACES:
        if i["provida_por"] not in MOD:
            erros.append(f"interface {i['nome']}: provida_por '{i['provida_por']}' inexistente")
        for c in i.get("requerida_por", []):
            if c not in MOD:
                erros.append(f"interface {i['nome']}: requerida_por '{c}' inexistente")
    for d in M.DEPENDENCIAS:
        for k in ("de", "para"):
            if d[k] not in MOD:
                erros.append(f"dependência {d}: '{d[k]}' inexistente")
        if d["tipo"] == "secundaria" and not d.get("motivo"):
            erros.append(f"dependência secundária {d['de']}->{d['para']} sem 'motivo'")
    if erros:
        sys.exit("Erros em modelo.py:\n  - " + "\n  - ".join(erros))


def provê(mid):
    return [i for i in M.INTERFACES if i["provida_por"] == mid]


def requer(mid):
    return [i for i in M.INTERFACES if mid in i.get("requerida_por", [])]


def deps(de=None, para=None, tipo=None):
    return [d for d in M.DEPENDENCIAS
            if (de is None or d["de"] == de) and (para is None or d["para"] == para)
            and (tipo is None or d["tipo"] == tipo)]


def rotulo_mod(mid):
    m = MOD[mid]
    return m.get("rotulo") or f"{PAC[m['pacote']]['prefixo']} {m['nome']}"


def servicos_da_interface(i):
    ids = [mt["id"] for mt in i["metodos"] if mt.get("id") is not None]
    texto = faixa(ids)
    if not texto:  # herda da IServico* requerida pelo modulo provedor
        herd = [x for x in requer(i["provida_por"])]
        ids = [mt["id"] for x in herd for mt in x["metodos"] if mt.get("id") is not None]
        texto = faixa(ids) or i.get("servicos_texto") or \
            ", ".join(x.get("servicos_texto", "") for x in herd if x.get("servicos_texto"))
    if i.get("servicos_obs"):
        texto += f" ({i['servicos_obs']})"
    return texto or "—"


def rotulo_metodo(mt):
    if mt.get("rotulo"):
        return mt["rotulo"]
    return f"{mt['nome']} ({mt['id']})" if mt.get("id") is not None else mt["nome"]


def profundidade(pid):
    d, p = 0, PAC[pid]
    while p["pai"]:
        d, p = d + 1, PAC[p["pai"]]
    return d


# ==========================================================================
# Layout do diagrama
# ==========================================================================
W_BASE = 1200       # largura minima do SVG em unidades (1 un ~ 0,23 mm no A4 paisagem)
W = W_BASE          # recalculada em layout() se as colunas nao couberem
MARG = 10           # margem externa
INSET = 12          # recuo de um pacote aninhado
TAB_H = 22          # altura da aba do pacote
PAD_SUP = 12        # espaco entre a aba/topo do pacote e os componentes
PAD_INF = 12        # espaco entre os componentes e a base do pacote
ZONA = 56           # faixa livre entre niveis (para os conectores)
GAP_COL = 18        # espaco horizontal entre componentes vizinhos
ALT_COMP = 60       # altura de um componente
R_BOLA, R_SOQ = 8, 13
F_ROT = 12.5        # fonte dos rotulos de interface
F_NOME, F_PEQ = 15, 11
F_PAC = 13
NOTA_L = 560


def calcular_spans():
    """Colunas ocupadas por cada modulo (indices inicial, final)."""
    idx = {c: n for n, c in enumerate(M.COLUNAS)}
    span = {}
    for m in M.MODULOS:
        if "coluna" in m:
            span[m["id"]] = (idx[m["coluna"]], idx[m["coluna"]])
    for _ in range(len(M.MODULOS)):
        for m in M.MODULOS:
            if m["id"] in span:
                continue
            viz = [i["provida_por"] for i in requer(m["id"])]
            viz += [c for i in provê(m["id"]) for c in i.get("requerida_por", [])]
            viz += [d["de"] for d in deps(para=m["id"], tipo="primaria")]
            viz += [d["para"] for d in deps(de=m["id"], tipo="primaria")]
            cols = [k for v in viz if v in span for k in span[v]]
            if cols:
                span[m["id"]] = (min(cols), max(cols))
    falta = [m["id"] for m in M.MODULOS if m["id"] not in span]
    if falta:
        sys.exit(f"Não foi possível posicionar {falta}: informe 'coluna' em modelo.py.")
    por_nivel = {}
    for m in M.MODULOS:
        a, b = span[m["id"]]
        for outro in por_nivel.get(m["nivel"], []):
            c, d = span[outro]
            if not (b < c or d < a):
                sys.exit(f"Módulos {outro} e {m['id']} ocupam as mesmas colunas no nível "
                         f"{m['nivel']}: mude 'nivel' ou 'coluna' em modelo.py.")
        por_nivel.setdefault(m["nivel"], []).append(m["id"])
    return span


def niveis_do_pacote(pid):
    nv = {m["nivel"] for m in M.MODULOS if m["pacote"] == pid}
    for p in M.PACOTES:
        if p["pai"] == pid:
            nv |= niveis_do_pacote(p["id"])
    return nv


def conexoes(span):
    """Lista de conectores com posicao em 'unidades de coluna'."""
    con = []

    def centro_sobreposicao(a, b):
        (x0, x1), (y0, y1) = span[a], span[b]
        lo, hi = max(x0, y0), min(x1, y1)
        if lo > hi:
            sys.exit(f"{a} e {b} não compartilham coluna; o diagrama só liga módulos "
                     f"verticalmente alinhados. Ajuste 'coluna' em modelo.py.")
        return (lo + hi + 1) / 2.0

    for i in M.INTERFACES:
        p = i["provida_por"]
        cons = i.get("requerida_por", [])
        if not cons:
            a, b = span[p]
            con.append({"tipo": "pirulito", "intf": i["nome"], "prov": p, "cons": None,
                        "pos": (a + b + 1) / 2.0, "nivel_sup": MOD[p]["nivel"] - 1})
        for c in cons:
            nc, np_ = MOD[c]["nivel"], MOD[p]["nivel"]
            if abs(nc - np_) != 1:
                sys.exit(f"{c} requer {i['nome']} de {p}, mas não estão em níveis vizinhos.")
            con.append({"tipo": "bola_soquete", "intf": i["nome"], "prov": p, "cons": c,
                        "pos": centro_sobreposicao(p, c), "nivel_sup": min(nc, np_)})
    for d in deps(tipo="primaria"):
        nd, np_ = MOD[d["de"]]["nivel"], MOD[d["para"]]["nivel"]
        if np_ - nd != 1:
            sys.exit(f"Dependência {d['de']}->{d['para']}: o destino deve estar no nível "
                     f"logo abaixo da origem.")
        con.append({"tipo": "uso", "de": d["de"], "para": d["para"],
                    "pos": centro_sobreposicao(d["de"], d["para"]), "nivel_sup": nd})
    return con


def layout():
    span = calcular_spans()
    con = conexoes(span)
    niveis = sorted({m["nivel"] for m in M.MODULOS})
    pac_niveis = {p["id"]: niveis_do_pacote(p["id"]) for p in M.PACOTES}
    n = len(M.COLUNAS)
    dmax = max(profundidade(p["id"]) for p in M.PACOTES)

    # ---------- horizontal: X0 e largura de coluna ----------
    abas_cruzadas = [MARG + profundidade(p["id"]) * INSET + largura(p["nome"], F_PAC, True) + 20
                     for p in M.PACOTES if min(pac_niveis[p["id"]]) > niveis[0]]
    T = max(abas_cruzadas, default=0) + 16
    rotulos = [(c["pos"], largura(c["intf"], F_ROT)) for c in con if "intf" in c]
    rotulos += [(c["pos"], largura("«use»", F_PEQ)) for c in con if c["tipo"] == "uso"]
    pos_min = min(c["pos"] for c in con)
    # largura minima de coluna: texto dos componentes de 1 coluna e rotulos
    # (o rotulo de uma interface nao pode alcancar a linha da coluna vizinha)
    def larg_texto(m):
        sub = largura(m.get("classe", ""), F_PEQ, mono=True) + 8 if m.get("classe") else \
            largura(m.get("descricao", ""), F_PEQ) + 8
        return max(sub, largura(m["nome"], 12, True) + 2 * 28)
    colw_min = max([larg_texto(m) + GAP_COL for m in M.MODULOS if span[m["id"]][0] == span[m["id"]][1]]
                   + [R_SOQ + 6 + w + 8 for _, w in rotulos])
    # o SVG cresce (e e reduzido na pagina) quando ha muitas colunas
    global W
    W = W_BASE
    while True:
        colw = (W - 2 * MARG) / n
        x0 = MARG
        for _ in range(50):
            x0 = max(MARG + dmax * INSET + 14, T - pos_min * colw)
            lim = [(W - MARG - 10 - (R_SOQ + 6) - w - x0) / pos for pos, w in rotulos]
            lim.append((W - MARG - dmax * INSET - 12 - x0) / n)
            colw = min(lim)
        if colw >= colw_min or W > 4 * W_BASE:
            break
        W += 20

    def xcol(pos):
        return x0 + pos * colw

    # ---------- vertical ----------
    y = MARG
    pac_y = {}  # pid -> [aba, topo, base]
    zonas, nivel_y = {}, {}
    nota = None
    nota_cfg = getattr(M, "NOTA_DIAGRAMA", None)

    def ordem(lista, desc):
        return sorted(lista, key=lambda pid: profundidade(pid), reverse=desc)

    def fechar(nv):
        nonlocal y, nota
        for pid in ordem([p for p in pac_niveis if max(pac_niveis[p]) == nv], True):
            y += PAD_INF
            pac_y[pid].append(y)
            if nota_cfg and nota_cfg["ancora"] == pid:
                y += 22
                nota = {"y": y, "ancora_y": pac_y[pid][2]}
                linhas = quebrar(nota_cfg["texto"].format(**SECOES), NOTA_L - 28, F_PEQ + 0.5)
                nota["linhas"] = linhas
                nota["h"] = 14 + len(linhas) * 15
                y += nota["h"]

    def abrir(nv):
        nonlocal y
        for pid in ordem([p for p in pac_niveis if min(pac_niveis[p]) == nv], False):
            pac_y[pid] = [y]
            y += TAB_H
            pac_y[pid].append(y)
            y += PAD_SUP

    for k, nv in enumerate(niveis):
        if k:
            fechar(niveis[k - 1])
            z0 = y
            y += ZONA
            zonas[niveis[k - 1]] = (z0, y)
        abrir(nv)
        nivel_y[nv] = y
        y += ALT_COMP
    fechar(niveis[-1])

    caixas = {}
    for m in M.MODULOS:
        a, b = span[m["id"]]
        xa, xb = xcol(a) + GAP_COL / 2, xcol(b + 1) - GAP_COL / 2
        caixas[m["id"]] = (xa, nivel_y[m["nivel"]], xb - xa, ALT_COMP)
    pac_x = {}
    for p in M.PACOTES:
        d = profundidade(p["id"])
        pac_x[p["id"]] = (MARG + d * INSET, W - MARG - d * INSET)
    if nota:
        xa, xb = pac_x[nota_cfg["ancora"]]
        nota["x"], nota["w"] = xb - NOTA_L, NOTA_L
    return {"span": span, "con": con, "caixas": caixas, "zonas": zonas, "pac_y": pac_y,
            "pac_x": pac_x, "xcol": xcol, "colw": colw, "y_fim": y, "nota": nota}


# ==========================================================================
# Desenho SVG
# ==========================================================================
def f(v):
    return f"{v:.1f}".rstrip("0").rstrip(".")


class Svg:
    def __init__(self):
        self.p = []

    def add(self, s):
        self.p.append(s)

    def linha(self, x1, y1, x2, y2, cls="d-l"):
        self.add(f'<line class="{cls}" x1="{f(x1)}" y1="{f(y1)}" x2="{f(x2)}" y2="{f(y2)}"/>')

    def ret(self, x, y, w, h, cls, extra=""):
        self.add(f'<rect class="{cls}" x="{f(x)}" y="{f(y)}" width="{f(w)}" height="{f(h)}"{extra}/>')

    def texto(self, x, y, t, cls, ancora="start"):
        a = "" if ancora == "start" else f' text-anchor="{ancora}"'
        self.add(f'<text class="{cls}" x="{f(x)}" y="{f(y)}"{a}>{esc(t)}</text>')


def icone_componente(s, x, y, k=1.0):
    """Icone UML 2 de componente: retangulo com duas abas a esquerda."""
    s.ret(x + 4 * k, y, 13 * k, 17 * k, "d-ico")
    s.ret(x, y + 3 * k, 8 * k, 4 * k, "d-ico")
    s.ret(x, y + 10 * k, 8 * k, 4 * k, "d-ico")


def componente(s, mod, cx):
    x, y, w, h = cx
    cor = PAC[mod["pacote"]].get("componente")
    s.ret(x, y, w, h, "d-comp", f' style="fill:{cor}"' if cor else "")
    icone_componente(s, x + w - 24, y + 7)
    meio = x + w / 2
    s.texto(meio, y + 15, "«component»", "d-est", "middle")
    # nome longo: reduz a fonte para nao encostar no icone (minimo 12)
    tam = F_NOME
    while tam > 11.5 and largura(mod["nome"], tam, True) > w - 2 * 28:
        tam -= 0.5
    estilo = "" if tam == F_NOME else f' style="font-size:{f(tam)}px"'
    s.add(f'<text class="d-nome" x="{f(meio)}" y="{f(y + 33.5)}" text-anchor="middle"{estilo}>'
          f'{esc(mod["nome"])}</text>')
    if mod.get("classe"):
        s.texto(meio, y + 50, mod["classe"], "d-classe", "middle")
        larg = largura(mod["classe"], F_PEQ, mono=True)
    else:
        s.texto(meio, y + 50, mod.get("descricao", ""), "d-desc", "middle")
        larg = largura(mod.get("descricao", ""), F_PEQ)
    # linha de baixo: margem de 4 de cada lado; nome: livre do icone (28 de cada lado)
    larg = max(larg + 8, largura(mod["nome"], tam, True) + 2 * 28)
    if larg > w:
        aviso(f"texto do componente {mod['id']} pode não caber na caixa ({larg:.0f} > {w:.0f})")


def soquete_vertical(s, x, yb, abre_para_baixo=True):
    if abre_para_baixo:   # consumidor acima: arco por cima
        s.add(f'<path class="d-soq" d="M {f(x - R_SOQ)} {f(yb)} A {R_SOQ} {R_SOQ} 0 0 1 '
              f'{f(x + R_SOQ)} {f(yb)}"/>')
    else:
        s.add(f'<path class="d-soq" d="M {f(x - R_SOQ)} {f(yb)} A {R_SOQ} {R_SOQ} 0 0 0 '
              f'{f(x + R_SOQ)} {f(yb)}"/>')


def bola(s, x, y):
    s.add(f'<circle class="d-bola" cx="{f(x)}" cy="{f(y)}" r="{R_BOLA}"/>')


def seta_aberta(s, x, y, dx, dy):
    """Ponta de seta aberta (duas hastes) apontando na direcao (dx, dy)."""
    n = math.hypot(dx, dy)
    ux, uy = dx / n, dy / n
    px, py = -uy, ux
    c, l = 11, 6
    s.add(f'<polyline class="d-ponta" points="{f(x - ux * c + px * l)},{f(y - uy * c + py * l)} '
          f'{f(x)},{f(y)} {f(x - ux * c - px * l)},{f(y - uy * c - py * l)}"/>')


def nota_uml(s, x, y, w, h, linhas=None):
    d = 11
    s.add(f'<path class="d-nota" d="M {f(x)} {f(y)} H {f(x + w - d)} L {f(x + w)} {f(y + d)} '
          f'V {f(y + h)} H {f(x)} Z"/>')
    s.add(f'<path class="d-nota-dobra" d="M {f(x + w - d)} {f(y)} V {f(y + d)} H {f(x + w)}"/>')
    for k, t in enumerate(linhas or []):
        s.texto(x + 12, y + 19 + 15 * k, t, "d-nota-t")


def desenhar_svg(L):
    s = Svg()
    segs, caixas_txt = [], []   # para a verificacao de colisoes

    # --- pacotes (externos primeiro) ---
    for p in sorted(M.PACOTES, key=lambda p: profundidade(p["id"])):
        aba, topo, base = L["pac_y"][p["id"]]
        xa, xb = L["pac_x"][p["id"]]
        tw = largura(p["nome"], F_PAC, True) + 20
        estilo = f' style="fill:{p["fundo"]};stroke:{p["borda"]}"'
        s.ret(xa, topo, xb - xa, base - topo, "d-pac", estilo)
        s.ret(xa, aba, tw, TAB_H, "d-pac", estilo)
        s.add(f'<text class="d-pac-t" x="{f(xa + 10)}" y="{f(aba + 15.5)}" '
              f'style="fill:{p["forte"]}">{esc(p["nome"])}</text>')
        segs += [("h", xa, xb, topo), ("h", xa, xb, base)]
        caixas_txt.append((f"aba {p['nome']}", xa, aba, tw, TAB_H, "aba"))

    # --- conectores ---
    cx = L["caixas"]
    for c in L["con"]:
        x = L["xcol"](c["pos"])
        z0, z1 = L["zonas"][c["nivel_sup"]]
        ym = (z0 + z1) / 2
        if c["tipo"] in ("bola_soquete", "pirulito"):
            prov = cx[c["prov"]]
            prov_abaixo = MOD[c["prov"]]["nivel"] > c["nivel_sup"]
            y_prov = prov[1] if prov_abaixo else prov[1] + prov[3]
            yb = ym
            s.linha(x, y_prov, x, yb + (R_BOLA if prov_abaixo else -R_BOLA))
            segs.append(("v", x, min(y_prov, yb), max(y_prov, yb)))
            if c["cons"]:
                cons = cx[c["cons"]]
                y_cons = cons[1] + cons[3] if prov_abaixo else cons[1]
                s.linha(x, y_cons, x, yb - R_SOQ if prov_abaixo else yb + R_SOQ)
                soquete_vertical(s, x, yb, prov_abaixo)
                segs.append(("v", x, min(y_cons, yb), max(y_cons, yb)))
            bola(s, x, yb)
            tx = x + R_SOQ + 6
            s.texto(tx, yb + 4.5, c["intf"], "d-intf")
            caixas_txt.append((c["intf"], tx, yb - 9, largura(c["intf"], F_ROT), 13.5, "rot"))
        else:
            a, b = cx[c["de"]], cx[c["para"]]
            y1, y2 = a[1] + a[3], b[1]
            s.linha(x, y1, x, y2 - 1, "d-uso")
            seta_aberta(s, x, y2, 0, 1)
            s.texto(x + 6, ym + 4, "«use»", "d-use")
            segs.append(("v", x, y1, y2))
            caixas_txt.append(("«use»", x + 6, ym - 7, largura("«use»", F_PEQ), 12, "rot"))

    # --- componentes ---
    for m in M.MODULOS:
        componente(s, m, cx[m["id"]])
        x, y, w, h = cx[m["id"]]
        caixas_txt.append((m["id"], x, y, w, h, "comp"))

    # --- nota ---
    nt = L["nota"]
    if nt:
        nota_uml(s, nt["x"], nt["y"], nt["w"], nt["h"], nt["linhas"])
        xa = nt["x"] + nt["w"] / 2
        s.linha(xa, nt["ancora_y"], xa, nt["y"], "d-ancora")

    # --- legenda ---
    y_leg = L["y_fim"] + 16
    h_leg = legenda(s, MARG, y_leg, W - 2 * MARG)
    altura = y_leg + h_leg + MARG

    verificar_colisoes(segs, caixas_txt)
    corpo = "\n".join(s.p)
    return corpo, altura


def legenda(s, x, y, larg):
    itens = [
        ("Componente (módulo)", "comp"),
        ("Interface provida (lollipop)", "prov"),
        ("Interface requerida (socket)", "req"),
        ("Conexão requerida–provida", "con"),
        ("Dependência de uso «use»", "uso"),
        ("Pacote (camada)", "pac"),
        ("Nota", "nota"),
    ]
    alt_linha = 27
    fundo_idx = len(s.p)          # a caixa e inserida antes dos simbolos
    titulo_w = largura("Legenda:", 12.5, True) + 22
    x_ini = x + 12 + titulo_w
    larg_itens = [52 + 8 + largura(t, 12) + 20 for t, _ in itens]
    ncol = len(itens)             # maior grade alinhada que cabe na largura
    while ncol > 1:
        cols = [max(larg_itens[k::ncol]) for k in range(ncol)]
        if x_ini + sum(cols) <= x + larg - 8:
            break
        ncol -= 1
    cols = [max(larg_itens[k::ncol]) for k in range(ncol)]
    for k, (texto, tipo) in enumerate(itens):
        lin, col = divmod(k, ncol)
        cx = x_ini + sum(cols[:col])
        cy = y + 4 + alt_linha / 2 + lin * alt_linha
        simbolo(s, tipo, cx, cy)
        s.texto(cx + 60, cy + 4.3, texto, "d-leg")
    linhas = -(-len(itens) // ncol)
    h = linhas * alt_linha + 8
    s.p.insert(fundo_idx, f'<rect class="d-leg-caixa" x="{f(x)}" y="{f(y)}" '
                          f'width="{f(larg)}" height="{f(h)}"/>')
    s.texto(x + 12, y + 4 + alt_linha / 2 + 4.5, "Legenda:", "d-leg-tit")
    return h


def simbolo(s, tipo, x, y):
    if tipo == "comp":
        s.ret(x + 6, y - 10, 40, 20, "d-comp")
        icone_componente(s, x + 31, y - 7, 0.7)
    elif tipo == "prov":
        s.linha(x + 4, y, x + 36, y)
        bola(s, x + 36 + R_BOLA, y)
    elif tipo == "req":
        cxs = x + 34 + R_SOQ
        s.linha(x + 4, y, x + 34, y)
        s.add(f'<path class="d-soq" d="M {f(cxs)} {f(y - R_SOQ)} A {R_SOQ} {R_SOQ} 0 0 0 '
              f'{f(cxs)} {f(y + R_SOQ)}"/>')
    elif tipo == "con":
        cxs = x + 20 + R_SOQ
        s.linha(x + 2, y, x + 20, y)
        s.add(f'<path class="d-soq" d="M {f(cxs)} {f(y - R_SOQ)} A {R_SOQ} {R_SOQ} 0 0 0 '
              f'{f(cxs)} {f(y + R_SOQ)}"/>')
        bola(s, cxs, y)
        s.linha(cxs + R_BOLA, y, x + 52, y)
    elif tipo == "uso":
        s.linha(x + 2, y, x + 49, y, "d-uso")
        seta_aberta(s, x + 50, y, 1, 0)
    elif tipo == "pac":
        s.ret(x + 6, y - 11, 16, 6, "d-pac", ' style="fill:#f2f2f2;stroke:#666"')
        s.ret(x + 6, y - 5, 40, 16, "d-pac", ' style="fill:#f2f2f2;stroke:#666"')
    elif tipo == "nota":
        nota_uml(s, x + 8, y - 10, 34, 20)


def verificar_colisoes(segs, caixas):
    def corta(seg, bx, by, bw, bh):
        if seg[0] == "v":
            _, x, ya, yb = seg
            return bx < x < bx + bw and ya < by + bh and yb > by
        _, xa, xb, y = seg
        return by < y < by + bh and xa < bx + bw and xb > bx

    rot = [c for c in caixas if c[5] == "rot"]
    for nome, bx, by, bw, bh, _ in rot:
        for sg in segs:
            if corta(sg, bx, by, bw, bh):
                aviso(f"rótulo '{nome}' encosta em uma linha do diagrama")
                break
        for outro in caixas:
            if outro[0] == nome:
                continue
            ox, oy, ow, oh = outro[1:5]
            if bx < ox + ow and bx + bw > ox and by < oy + oh and by + bh > oy:
                aviso(f"rótulo '{nome}' sobrepõe '{outro[0]}'")
    for nome, bx, by, bw, bh, tipo in caixas:
        if tipo == "aba":
            for sg in segs:
                if sg[0] == "v" and corta(sg, bx, by, bw, bh):
                    aviso(f"um conector atravessa a {nome}")
                    break
        if tipo in ("rot", "comp") and bx + bw > W - MARG:
            aviso(f"'{nome}' ultrapassa a margem direita do diagrama")


CSS_SVG = """
.d-l{stroke:#2b3238;stroke-width:1.4;fill:none}
.d-uso{stroke:#2b3238;stroke-width:1.3;stroke-dasharray:6 4;fill:none}
.d-ponta{stroke:#2b3238;stroke-width:1.4;fill:none;stroke-linejoin:miter}
.d-ancora{stroke:#6d6a5e;stroke-width:1.1;stroke-dasharray:3 3}
.d-comp{fill:#fff;stroke:#2b3238;stroke-width:1.4}
.d-ico{fill:#fff;stroke:#2b3238;stroke-width:1.1}
.d-pac{stroke-width:1.2}
.d-bola{fill:#fff;stroke:#2b3238;stroke-width:1.5}
.d-soq{fill:none;stroke:#2b3238;stroke-width:1.5}
.d-nota{fill:#fffbea;stroke:#8a845f;stroke-width:1.1}
.d-nota-dobra{fill:none;stroke:#8a845f;stroke-width:1.1}
.d-leg-caixa{fill:#fafafa;stroke:#b9bec3;stroke-width:1}
text{font-family:"Segoe UI",Calibri,Arial,sans-serif;fill:#1e252b}
.d-est{font-size:11px;fill:#56606a}
.d-nome{font-size:15px;font-weight:700}
.d-classe{font-size:11px;font-family:Consolas,"Courier New",monospace;fill:#3b444c}
.d-desc{font-size:11.5px;font-style:italic;fill:#4a4f55}
.d-intf{font-size:12.5px;fill:#1e252b}
.d-use{font-size:11px;fill:#4a4f55}
.d-pac-t{font-size:13px;font-weight:700}
.d-nota-t{font-size:11.5px;fill:#3c3a2c}
.d-leg{font-size:12px}
.d-leg-tit{font-size:12.5px;font-weight:700}
"""


def gerar_svg():
    L = layout()
    corpo, altura = desenhar_svg(L)
    svg = (f'<svg xmlns="http://www.w3.org/2000/svg" viewBox="0 0 {W} {f(altura)}" '
           f'role="img" aria-label="Diagrama de componentes UML do sistema">\n'
           f'<style>{CSS_SVG}</style>\n<rect x="0" y="0" width="{W}" height="{f(altura)}" fill="#fff"/>\n'
           f'{corpo}\n</svg>')
    return svg, altura


# ==========================================================================
# Documento HTML
# ==========================================================================
SECOES = {"secao": "3.4", "tabela": "2"}   # onde fica a tabela de dependencias secundarias


def chip(pid, texto=None):
    p = PAC[pid]
    return (f'<span class="chip" style="background:{p["fundo"]};border-color:{p["borda"]};'
            f'color:{p["forte"]}">{esc(texto or p["prefixo"])}</span>')


def lista_codigo(nomes):
    return ", ".join(f"<code>{esc(n)}</code>" for n in nomes)


def html_visao_geral():
    out = []
    for bloco in M.VISAO_GERAL:
        tipo = bloco[0]
        if tipo == "p":
            out.append(f"<p>{marcar(bloco[1])}</p>")
        elif tipo == "h3":
            out.append(f"<h3>{marcar(bloco[1])}</h3>")
        elif tipo == "ul":
            out.append("<ul>" + "".join(f"<li>{marcar(i)}</li>" for i in bloco[1]) + "</ul>")
        elif tipo == "caixa":
            out.append(f'<div class="caixa"><div class="caixa-tit">{marcar(bloco[1])}</div><ul>'
                       + "".join(f"<li>{marcar(i)}</li>" for i in bloco[2]) + "</ul></div>")
        elif tipo == "fluxo":
            out.append(f'<div class="fluxo"><div class="caixa-tit">{marcar(bloco[1])}</div><ol>'
                       + "".join(f"<li>{marcar(i)}</li>" for i in bloco[2]) + "</ol></div>")
        elif tipo == "referencia":
            out.append('<p class="leg-tab junto">Tabela 1 – Arquitetura de referência do '
                       'enunciado e módulos deste modelo</p>')
            linhas = []
            for r in M.REFERENCIA:
                mods = r["modulos"]
                pid = MOD[mods[0]]["pacote"]
                nomes = ", ".join(esc(MOD[m]["nome"]) for m in mods)
                linhas.append(f"<tr><td><strong>{esc(r['elemento'])}</strong></td>"
                              f"<td>{chip(pid, PAC[pid]['nome'])}</td><td>{nomes}</td></tr>")
            out.append('<table class="tab ref"><thead><tr><th>Elemento de referência</th>'
                       '<th>Pacote</th><th>Módulos</th></tr></thead><tbody>'
                       + "".join(linhas) + "</tbody></table>")
    return "\n".join(out)


def html_modulo(m):
    mid = m["id"]
    rel = []
    pv = provê(mid)
    rq = requer(mid)
    if pv:
        rel.append(f"<span class=\"rel-k\">Provê</span> {lista_codigo(i['nome'] for i in pv)}")
    if rq:
        rel.append(f"<span class=\"rel-k\">Requer</span> {lista_codigo(i['nome'] for i in rq)}")
    usa = deps(de=mid, tipo="primaria")
    if usa:
        rel.append("<span class=\"rel-k\">Usa</span> " +
                   ", ".join(esc(MOD[d["para"]]["nome"]) for d in usa))
    cons = deps(de=mid, tipo="secundaria")
    if cons:
        rel.append("<span class=\"rel-k\">Consulta</span> " +
                   ", ".join(esc(MOD[d["para"]]["nome"]) for d in cons))
    usado = deps(para=mid, tipo="primaria")
    if usado:
        rel.append("<span class=\"rel-k\">Usado por</span> " +
                   ", ".join(esc(rotulo_mod(d["de"])) for d in usado))
    consultado = deps(para=mid, tipo="secundaria")
    if consultado:
        rel.append("<span class=\"rel-k\">Consultado por</span> " +
                   ", ".join(esc(rotulo_mod(d["de"])) for d in consultado))
    p = PAC[m["pacote"]]
    sub = (f'<code class="mod-classe">{esc(m["classe"])}</code>' if m.get("classe")
           else f'<span class="mod-desc">{esc(m.get("descricao", ""))}</span>')
    return (f'<div class="modulo" style="border-left-color:{p["borda"]}">'
            f'<div class="mod-cab"><span class="mod-nome" style="color:{p["forte"]}">'
            f'{esc(m["nome"])}</span>{sub}</div>'
            f'<div class="mod-rel">{" <span class=sep>·</span> ".join(rel)}</div>'
            f'<p>{marcar(m["resp"])}</p></div>')


def html_responsabilidades():
    out = ['<h2 id="sec-resp">3 Responsabilidades dos módulos</h2>',
           f'<p>Os {len(M.MODULOS)} módulos estão agrupados por camada. Para cada um são '
           'indicadas a classe que o implementa, as interfaces que provê e requer e as '
           'dependências de armazenamento; em seguida, a responsabilidade do módulo.</p>']
    n = 0
    for p in M.PACOTES:
        mods = [m for m in M.MODULOS if m["pacote"] == p["id"]]
        if not mods:
            continue
        n += 1
        out.append(f'<h3><span class="marca" style="background:{p["borda"]}"></span>'
                   f'3.{n} {esc(p["titulo_secao"])}</h3>')
        if p["id"] == "armazenamento":
            out.append(f"<p>{marcar(M.ARMAZENAMENTO_COMUM)}</p>")
        out += [html_modulo(m) for m in mods]
    # tabela de dependencias secundarias
    out.append(f'<h3><span class="marca" style="background:{PAC["servico"]["borda"]}"></span>'
               f'{SECOES["secao"]} Dependências secundárias (consultas de consistência)</h3>')
    out.append('<p class="junto">O diagrama mostra apenas a dependência principal de cada módulo '
               'de serviço (o contêiner da própria entidade). Para aplicar as regras de negócio, '
               'os módulos também consultam contêineres de outras entidades:</p>')
    out.append(f'<p class="leg-tab junto">Tabela {SECOES["tabela"]} – Dependências secundárias '
               'da camada de serviço para o armazenamento</p>')
    linhas, ant = [], None
    for d in deps(tipo="secundaria"):
        primeiro = d["de"] != ant
        cel = f"<strong>{esc(rotulo_mod(d['de']))}</strong>" if primeiro else ""
        linhas.append(f'<tr class="{"grupo" if primeiro else ""}"><td>{cel}</td>'
                      f'<td>{esc(MOD[d["para"]]["nome"])}</td><td>{marcar(d["motivo"])}</td></tr>')
        ant = d["de"]
    out.append('<table class="tab dep"><thead><tr><th>Módulo de serviço</th><th>Consulta</th>'
               '<th>Motivo</th></tr></thead><tbody>' + "".join(linhas) + "</tbody></table>")
    return "\n".join(out)


def html_interfaces():
    out = ['<h2 id="sec-intf">4 Interfaces</h2>',
           f'<p>As {len(M.INTERFACES)} interfaces estão declaradas como classes abstratas em '
           '<code>headers/interfaces.hpp</code>. A coluna Serviços traz a numeração do enunciado '
           '(1 a 26); criar conta corresponde ao serviço 1 e a autenticação não é numerada. As '
           'interfaces <code>IApresentacao*</code> atendem os mesmos serviços da '
           '<code>IServico*</code> que o módulo requer.</p>',
           '<p class="leg-tab junto">Tabela 3 – Interfaces entre os módulos</p>']
    linhas = []
    for i in M.INTERFACES:
        prov = i["provida_por"]
        req = " ".join(chip(MOD[c]["pacote"], rotulo_mod(c)) for c in i.get("requerida_por", [])) or "—"
        met = ", ".join(esc(rotulo_metodo(mt)) for mt in i["metodos"])
        linhas.append(f'<tr><td><code class="intf">{esc(i["nome"])}</code></td>'
                      f'<td>{chip(MOD[prov]["pacote"], rotulo_mod(prov))}</td>'
                      f'<td>{req}</td><td>{esc(servicos_da_interface(i))}</td>'
                      f'<td class="met">{met}</td></tr>')
    out.append('<table class="tab intf-tab"><thead><tr><th>Interface</th><th>Provida por</th>'
               '<th>Requerida por</th><th>Serviços</th><th>Métodos</th></tr></thead><tbody>'
               + "".join(linhas) + "</tbody></table>")
    out.append('<h3 id="sec-sig">4.1 Assinaturas C++</h3>')
    out.append('<p class="junto">Transcritas de <code>headers/interfaces.hpp</code> (comentários '
               'Doxygen omitidos). O comentário ao final de cada método indica o número do '
               'serviço.</p>')
    for i in M.INTERFACES:
        out.append(html_codigo_interface(i))
    return "\n".join(out)


TIPOS_CPP = r"\b(Email|Senha|Pessoa|Projeto|Quadro|CartaoAtividade|Identificador|std::vector|IServico\w+)\b"


def realcar(cpp):
    s = esc(cpp)
    s = re.sub(r"\b(virtual|bool|void|const|double|int|class|public)\b", r'<span class="kw">\1</span>', s)
    s = re.sub(TIPOS_CPP, r'<span class="ty">\1</span>', s)
    return s


COL_MAX_CODIGO = 100   # caracteres por linha no bloco de codigo


def formatar_assinatura(sig):
    """Quebra assinaturas longas apos virgulas, alinhando sob o '(' (como no .hpp)."""
    recuo, folga = 4, 7          # recuo do metodo; espaco para o comentario "// 26"
    if recuo + len(sig) + folga <= COL_MAX_CODIGO:
        return realcar(sig)
    abre = sig.index("(")
    partes = sig[abre + 1:].split(", ")
    linhas, atual = [], sig[:abre + 1]
    for k, p in enumerate(partes):
        pedaco = p + ("," if k < len(partes) - 1 else "")
        sep = "" if atual.endswith("(") or not atual.strip() else " "
        if not atual.endswith("(") and recuo + len(atual + sep + pedaco) + folga > COL_MAX_CODIGO:
            linhas.append(atual)
            atual = " " * (abre + 1) + pedaco
        else:
            atual = atual + sep + pedaco
    linhas.append(atual)
    return "\n".join(realcar(l) for l in linhas)


def html_codigo_interface(i):
    serv = servicos_da_interface(i)
    linhas = [f'<div class="cl">{realcar("class " + i["nome"])} {{ '
              f'<span class="cm">// serviços: {esc(serv)}</span></div>',
              f'<div class="cl">{realcar("public:")}</div>']
    for mt in i["metodos"]:
        cm = f' <span class="cm">// {mt["id"]}</span>' if mt.get("id") is not None else ""
        linhas.append(f'<div class="sig">{formatar_assinatura("virtual " + mt["assinatura"] + " = 0;")}{cm}</div>')
    linhas.append(f'<div class="sig">{realcar("virtual ~" + i["nome"] + "() = 0;")}</div>')
    linhas.append('<div class="cl">};</div>')
    return '<div class="codigo">' + "".join(linhas) + "</div>"


def html_convencoes():
    itens = "".join(f'<li><span class="conv-t">{marcar(t)}.</span> {marcar(x)}</li>'
                    for t, x in M.CONVENCOES)
    return ('<h2 id="sec-conv">5 Convenções das interfaces</h2>'
            '<p class="junto">Valem para todas as interfaces de serviço e estão registradas no '
            'topo de <code>headers/interfaces.hpp</code>:</p>'
            f'<ol class="conv">{itens}</ol>')


def html_capa():
    d = M.DOCUMENTO
    n_mod = len(M.MODULOS)
    n_int = len(M.INTERFACES)
    faixas = "".join(f'<span style="background:{p["borda"]}"></span>' for p in M.PACOTES)
    equipe = "".join(f"<li>{esc(n)}</li>" for n in d["equipe"])
    return f"""
<section class="capa-bloco" data-capa>
  <div class="capa-topo">
    <div class="capa-inst">{esc(d['instituicao'])}</div>
    <div class="capa-disc">{esc(d['disciplina'])}</div>
  </div>
  <div class="capa-meio">
    <div class="capa-faixa">{faixas}</div>
    <h1 class="capa-tit">{esc(d['titulo'])}</h1>
    <div class="capa-sub">{esc(d['subtitulo'])}</div>
    <div class="capa-resumo">Estilo em camadas · {n_mod} módulos · {n_int} interfaces · diagrama de componentes UML 2</div>
    {html_sumario()}
  </div>
  <div class="capa-base">
    <div class="capa-eq-t">Equipe</div>
    <ul class="capa-eq">{equipe}</ul>
    <div class="capa-ano">{esc(d['local'])}, {esc(d['ano'])}</div>
  </div>
</section>"""


def html_sumario():
    itens = [("sec-visao", "1 Visão geral"), ("sec-diag", "2 Diagrama de componentes"),
             ("sec-resp", "3 Responsabilidades dos módulos"), ("sec-intf", "4 Interfaces"),
             ("sec-conv", "5 Convenções das interfaces")]
    li = "".join(f'<li><span>{esc(t)}</span><span class="pont"></span>'
                 f'<span class="pg" data-alvo="{a}"></span></li>' for a, t in itens)
    return f'<div class="sumario"><div class="sum-tit">Sumário</div><ol>{li}</ol></div>'


# Area util da pagina paisagem (mm) - usada para dimensionar o SVG.
PAIS_W, PAIS_H_SVG = 277.0, 163.0


def html_diagrama(svg, altura):
    larg = min(PAIS_W, PAIS_H_SVG * W / altura)
    alt = larg * altura / W
    pt = F_ROT * larg / W / 0.3528
    print(f"  diagrama: {larg:.0f} x {alt:.0f} mm; rótulos de interface ≈ {pt:.1f} pt")
    corpo = svg.split("\n", 1)[1]  # remove a linha de abertura e reabre com tamanho
    return f"""
<section class="bloco-diag" data-pagina="paisagem">
  <h2 id="sec-diag">2 Diagrama de componentes</h2>
  <p class="diag-intro">Componentes UML 2 em pacotes por camada, com colunas alinhadas por entidade: cada interface requerida (socket) encaixa na interface provida (lollipop) correspondente.</p>
  <figure>
    <svg xmlns="http://www.w3.org/2000/svg" viewBox="0 0 {W} {f(altura)}" style="width:{larg:.1f}mm;height:{alt:.1f}mm" role="img" aria-label="Diagrama de componentes UML do sistema">
{corpo}
    <figcaption>Figura 1 – Diagrama de componentes UML do sistema</figcaption>
  </figure>
</section>"""


CSS_DOC = """
@page { size: A4; margin: 0; }
@page paisagem { size: A4 landscape; margin: 0; }
* { box-sizing: border-box; }
html, body { margin: 0; padding: 0; background: #fff; }
body { font-family: "Segoe UI", Calibri, Arial, sans-serif; font-size: 10pt; line-height: 1.45;
       color: #1f262c; -webkit-print-color-adjust: exact; print-color-adjust: exact; }
#fonte { display: none; }
@media screen { body { background: #d9dcdf; } .pagina { margin: 8mm auto; box-shadow: 0 1px 6px rgba(0,0,0,.25); } }
.pagina { position: relative; overflow: hidden; background: #fff; break-after: page; }
.pagina:last-child { break-after: auto; }
.pagina.retrato, .pagina.capa { width: 210mm; height: 296.8mm; padding: 17mm 19mm 0 19mm; }
.pagina.retrato .conteudo { height: 262mm; overflow: hidden; }
.pagina.capa { padding: 0; }
.pagina.capa .conteudo { height: 296.8mm; }
.pagina.paisagem { page: paisagem; width: 297mm; height: 209.8mm; padding: 9mm 10mm 0 10mm; }
.pagina.paisagem .conteudo { height: 192mm; overflow: hidden; }
.cabecalho { position: absolute; top: 7mm; left: 19mm; right: 19mm; font-size: 7.5pt; color: #7a838b;
             border-bottom: 0.3pt solid #c9ced3; padding-bottom: 1.2mm; display: flex; justify-content: space-between; }
.paisagem .cabecalho { top: 3mm; left: 10mm; right: 10mm; border: 0; padding: 0; }
.rodape { position: absolute; bottom: 7mm; right: 19mm; font-size: 8pt; color: #6a737b; }
.paisagem .rodape { bottom: 3.5mm; right: 10mm; }
.excede { outline: 2px dashed #d00; }

h2 { font-size: 15pt; font-weight: 600; color: #23313e; margin: 0 0 3mm; padding-bottom: 1.2mm;
     border-bottom: 1.2pt solid #23313e; }
* + h2 { margin-top: 7mm; }
h3 { font-size: 11.5pt; font-weight: 600; color: #26323d; margin: 5mm 0 2mm; display: flex; align-items: center; }
.marca { display: inline-block; width: 3.2mm; height: 3.2mm; border-radius: 0.6mm; margin-right: 2mm; }
p { margin: 0 0 2.4mm; text-align: justify; hyphens: auto; }
ul, ol { margin: 0 0 2.4mm; padding-left: 6mm; }
li { margin-bottom: 1.2mm; }
code { font-family: Consolas, "Courier New", monospace; font-size: 0.92em; color: #24323f;
       background: #f1f3f5; padding: 0 0.6mm; border-radius: 0.6mm; }
strong { font-weight: 600; }

.caixa, .fluxo { border: 0.6pt solid #b8c2cb; border-left: 2.4pt solid #5b7389; background: #f6f8fa;
         padding: 2.6mm 4mm 1mm; margin: 3mm 0 3.5mm; border-radius: 0.8mm; }
.fluxo { border-left-color: #5f8366; background: #f6f9f6; }
.caixa-tit { font-weight: 600; margin-bottom: 1.2mm; color: #23313e; }
.caixa ul, .fluxo ol { margin-bottom: 1mm; }

.tab { width: 100%; border-collapse: collapse; margin: 0 0 4mm; font-size: 8.8pt; line-height: 1.35; }
.tab th { background: #2f3d4a; color: #fff; font-weight: 600; text-align: left; padding: 1.4mm 2mm; }
.tab td { border-bottom: 0.5pt solid #cfd5da; padding: 1.3mm 2mm; vertical-align: top; }
.tab tbody tr:nth-child(even) td { background: #f7f8f9; }
.tab tr { break-inside: avoid; }
.leg-tab { font-size: 8.8pt; font-weight: 600; color: #3a4651; margin: 3mm 0 1.4mm; text-align: left; }
.tab.continua { margin-top: 0; }
.tab.dep tbody tr td { background: #fff; }
.tab.dep tr.grupo td { border-top: 0.9pt solid #8f9aa4; }
.tab.dep td:first-child { width: 30mm; }
.tab.dep td:nth-child(2) { width: 34mm; }
.tab.ref td:first-child { width: 48mm; }
.tab.ref td:nth-child(2) { width: 46mm; }
.intf-tab { font-size: 8.3pt; }
.intf-tab td:first-child { width: 41mm; }
.intf-tab td:nth-child(2), .intf-tab td:nth-child(3) { width: 31mm; }
.intf-tab td:nth-child(4) { width: 20mm; }
code.intf { background: none; padding: 0; font-weight: 600; }
.chip { display: inline-block; border: 0.5pt solid; border-radius: 0.8mm; font-size: 7.4pt; font-weight: 600;
        padding: 0 1.2mm; line-height: 1.45; white-space: nowrap; }

.modulo { border-left: 2.6pt solid; padding: 0.6mm 0 0.4mm 3.2mm; margin: 0 0 3mm; break-inside: avoid; }
.mod-cab { display: flex; align-items: baseline; gap: 2.4mm; }
.mod-nome { font-weight: 700; font-size: 10.5pt; }
.mod-classe { font-size: 8.4pt; background: none; padding: 0; color: #4b5661; }
.mod-desc { font-style: italic; font-size: 8.6pt; color: #5b6168; }
.mod-rel { font-size: 8.4pt; color: #3c4750; margin: 0.4mm 0 0.8mm; }
.mod-rel code { font-size: 8.2pt; }
.rel-k { font-weight: 600; color: #59636c; }
.sep { color: #a0a8af; margin: 0 0.8mm; }
.modulo p { margin: 0; }

.codigo { font-family: Consolas, "Courier New", monospace; font-size: 8pt; line-height: 1.38;
          background: #f6f7f9; border: 0.5pt solid #d3d8dd; border-radius: 0.8mm;
          padding: 2mm 3mm; margin: 0 0 2.6mm; color: #222b33; }
.codigo .cl { white-space: pre-wrap; }
.codigo .sig { white-space: pre; padding-left: 4ch; }
.codigo .kw { color: #1f4f8a; font-weight: 600; }
.codigo .ty { color: #2e6b3c; }
.codigo .cm { color: #8a6d3b; }

.conv { padding-left: 6mm; }
.conv li { margin-bottom: 2mm; text-align: justify; }
.conv-t { font-weight: 600; color: #23313e; }

.bloco-diag h2 { margin-bottom: 1.6mm; }
.diag-intro { font-size: 8.8pt; margin-bottom: 1.6mm; color: #39434c; }
figure { margin: 0; text-align: center; }
figure svg { display: block; margin: 0 auto; }
figcaption { font-size: 9pt; font-weight: 600; color: #2f3a44; margin-top: 1.6mm; }

.capa-bloco { height: 100%; display: flex; flex-direction: column; justify-content: space-between;
              padding: 30mm 24mm 26mm; }
.capa-inst { font-size: 15pt; font-weight: 600; letter-spacing: 0.04em; color: #23313e; }
.capa-disc { font-size: 12pt; color: #4b5661; margin-top: 1.5mm; }
.capa-faixa { display: flex; gap: 1.5mm; margin-bottom: 8mm; }
.capa-faixa span { display: block; width: 22mm; height: 2.2mm; border-radius: 0.6mm; }
.capa-tit { font-size: 34pt; font-weight: 600; color: #1d2a36; margin: 0; line-height: 1.1; border: 0; }
.capa-sub { font-size: 17pt; color: #3b4a58; margin-top: 3mm; }
.capa-resumo { font-size: 10pt; color: #6a747d; margin-top: 7mm; }
.capa-eq-t { font-size: 9pt; text-transform: uppercase; letter-spacing: 0.12em; color: #6a747d; margin-bottom: 2mm; }
.capa-eq { list-style: none; padding: 0; margin: 0 0 9mm; font-size: 12.5pt; line-height: 1.6; }
.capa-ano { font-size: 11pt; color: #4b5661; }

.sumario { margin: 16mm 0 0; width: 125mm; }
.sum-tit { font-size: 9pt; text-transform: uppercase; letter-spacing: 0.12em; color: #6a747d; margin-bottom: 2mm; }
.sumario ol { list-style: none; padding: 0; margin: 0; }
.sumario li { display: flex; align-items: baseline; margin-bottom: 1.2mm; font-size: 10.5pt; color: #2b3640; }
.sumario .pont { flex: 1; border-bottom: 0.8pt dotted #9aa3ab; margin: 0 2mm; }
"""

JS_PAGINAR = r"""
(function () {
  var fonte = document.getElementById('fonte');
  var destino = document.getElementById('paginas');
  var blocos = Array.prototype.slice.call(fonte.children);
  var paginas = [], pagina = null, area = null, tipoAtual = null;
  function novaPagina(tipo) {
    pagina = document.createElement('div');
    pagina.className = 'pagina ' + tipo;
    tipoAtual = tipo;
    if (tipo !== 'capa') {
      var c = document.createElement('div'); c.className = 'cabecalho';
      c.innerHTML = CABECALHO; pagina.appendChild(c);
    }
    area = document.createElement('div'); area.className = 'conteudo'; pagina.appendChild(area);
    if (tipo !== 'capa') { var r = document.createElement('div'); r.className = 'rodape'; pagina.appendChild(r); }
    destino.appendChild(pagina); paginas.push(pagina);
  }
  function cabe() { return area.scrollHeight <= area.clientHeight + 1; }
  function junto(el) { return el && (/^H[1-6]$/.test(el.tagName) || el.classList.contains('junto')); }
  function moverParaNovaPagina(el) {
    area.removeChild(el);
    var levar = [];
    while (area.children.length > 1 && junto(area.lastElementChild)) {
      levar.unshift(area.lastElementChild); area.removeChild(area.lastElementChild);
    }
    novaPagina(tipoAtual);
    levar.forEach(function (x) { area.appendChild(x); });
    area.appendChild(el);
  }
  function dividirTabela(t) {
    // Divide a tabela entre paginas repetindo o cabecalho; cada parte fica com
    // pelo menos 2 linhas (sem linha orfa no fim da pagina nem viuva no inicio).
    var corpo = t.tBodies[0];
    var linhas = Array.prototype.slice.call(corpo.rows);
    linhas.forEach(function (r) { corpo.removeChild(r); });
    var atual = t, c = corpo, i = 0, recemMovida = false;
    while (i < linhas.length) {
      c.appendChild(linhas[i]);
      if (cabe()) { i++; continue; }
      c.removeChild(linhas[i]);
      var n = c.rows.length;
      if (n < 2 && Math.min(2, linhas.length) > n) {
        if (recemMovida || area.children.length === 1) {   // nao cabe nem numa pagina nova
          atual.classList.add('excede');
          for (; i < linhas.length; i++) c.appendChild(linhas[i]);
          return;
        }
        while (c.rows.length) { c.removeChild(c.rows[c.rows.length - 1]); i--; }
        moverParaNovaPagina(atual); recemMovida = true;
        continue;
      }
      if (linhas.length - i === 1 && n > 2) { c.removeChild(c.rows[n - 1]); i--; }
      var nova = atual.cloneNode(false);
      nova.removeAttribute('id');
      nova.classList.add('continua');
      if (atual.tHead) nova.appendChild(atual.tHead.cloneNode(true));
      c = document.createElement('tbody'); nova.appendChild(c);
      novaPagina(tipoAtual);
      area.appendChild(nova); atual = nova; recemMovida = true;
    }
  }
  blocos.forEach(function (b) {
    if (b.hasAttribute('data-capa')) { novaPagina('capa'); area.appendChild(b); return; }
    var tipo = b.getAttribute('data-pagina') || 'retrato';
    if (!pagina || tipoAtual !== tipo || tipo === 'paisagem' || b.hasAttribute('data-quebra')) novaPagina(tipo);
    area.appendChild(b);
    if (cabe()) return;
    if (b.tagName === 'TABLE') { dividirTabela(b); return; }
    if (area.children.length === 1) { b.classList.add('excede'); return; }
    moverParaNovaPagina(b);
    if (!cabe()) b.classList.add('excede');
  });
  var total = paginas.length;
  paginas.forEach(function (p, i) {
    var r = p.querySelector('.rodape'); if (r) r.textContent = 'Página ' + (i + 1) + ' de ' + total;
  });
  Array.prototype.forEach.call(document.querySelectorAll('[data-alvo]'), function (s) {
    var alvo = document.getElementById(s.getAttribute('data-alvo'));
    var p = alvo && alvo.closest('.pagina');
    s.textContent = p ? String(paginas.indexOf(p) + 1) : '?';
  });
  fonte.parentNode.removeChild(fonte);
  document.body.setAttribute('data-paginas', String(total));
})();
"""


def gerar_html(svg, altura):
    d = M.DOCUMENTO
    cab = f"<span>{esc(d['titulo'])} — {esc(d['subtitulo'])}</span><span>{esc(d['disciplina'])}</span>"
    blocos = [
        html_capa(),
        '<h2 id="sec-visao">1 Visão geral</h2>',
        html_visao_geral(),
        html_diagrama(svg, altura),
        html_responsabilidades(),
        html_interfaces(),
        html_convencoes(),
    ]
    js = JS_PAGINAR.replace("CABECALHO", repr(cab))
    return f"""<!DOCTYPE html>
<html lang="pt-BR">
<head>
<meta charset="utf-8">
<title>{esc(d['titulo'])} – {esc(d['subtitulo'])}</title>
<!-- GERADO por gerar.py a partir de modelo.py. Não edite à mão: edite modelo.py e rode python gerar.py -->
<style>{CSS_DOC}</style>
</head>
<body>
<div id="paginas"></div>
<main id="fonte">
{chr(10).join(blocos)}
</main>
<script>{js}</script>
</body>
</html>
"""


# ==========================================================================
# Navegador headless
# ==========================================================================
NAVEGADORES = [
    r"C:\Program Files (x86)\Microsoft\Edge\Application\msedge.exe",
    r"C:\Program Files\Microsoft\Edge\Application\msedge.exe",
    r"C:\Program Files\Google\Chrome\Application\chrome.exe",
    r"C:\Program Files (x86)\Google\Chrome\Application\chrome.exe",
]


def navegador():
    for c in NAVEGADORES:
        if Path(c).exists():
            return c
    for nome in ("msedge", "chrome", "chromium", "google-chrome"):
        c = shutil.which(nome)
        if c:
            return c
    sys.exit("Edge/Chrome não encontrado; ajuste NAVEGADORES em gerar.py.")


def rodar_headless(args, saida, timeout=90):
    """Chama o navegador e espera (polling) o arquivo de saida estabilizar.
    O msedge.exe retorna antes de terminar, por isso nao basta esperar o processo."""
    perfil = AQUI / ".perfil"
    if saida.exists():
        saida.unlink()
    cmd = [navegador(), "--headless", "--disable-gpu", f"--user-data-dir={perfil}",
           "--no-first-run", "--disable-extensions"] + args
    subprocess.run(cmd, stdout=subprocess.DEVNULL, stderr=subprocess.DEVNULL)
    inicio, tam_ant, estavel = time.time(), -1, 0
    while time.time() - inicio < timeout:
        if saida.exists():
            tam = saida.stat().st_size
            if tam > 0 and tam == tam_ant:
                estavel += 1
                if estavel >= 3:
                    break
            else:
                estavel = 0
            tam_ant = tam
        time.sleep(0.4)
    else:
        sys.exit(f"Tempo esgotado esperando {saida.name}.")
    for _ in range(25):   # o navegador pode segurar o perfil por alguns instantes
        try:
            shutil.rmtree(perfil)
            break
        except FileNotFoundError:
            break
        except OSError:
            time.sleep(0.4)


def main():
    try:   # console do Windows (cp1252) nao imprime todos os caracteres
        sys.stdout.reconfigure(encoding="utf-8", errors="replace")
    except Exception:
        pass
    validar_modelo()
    args = sys.argv[1:]
    print("Gerando a partir de modelo.py ...")
    svg, altura = gerar_svg()
    (AQUI / "diagrama.svg").write_text('<?xml version="1.0" encoding="utf-8"?>\n' + svg,
                                       encoding="utf-8", newline="\n")
    doc = gerar_html(svg, altura)
    pagina = AQUI / "arquitetura.html"
    pagina.write_text(doc, encoding="utf-8", newline="\n")
    print("  ok  diagrama.svg, arquitetura.html")
    if "--png" in args:
        prev = AQUI / "diagrama-preview.html"
        larg_px = 2400
        prev.write_text('<!DOCTYPE html><meta charset="utf-8"><style>body{margin:0}svg{display:block;'
                        f'width:{larg_px}px;height:auto}}</style>' + svg, encoding="utf-8")
        alt_px = int(math.ceil(larg_px * altura / W))
        rodar_headless([f"--screenshot={AQUI / 'diagrama.png'}", f"--window-size={larg_px},{alt_px}",
                        "--force-device-scale-factor=1", "--hide-scrollbars", str(prev)],
                       AQUI / "diagrama.png")
        prev.unlink()
        print("  ok  diagrama.png")
    if "--sem-pdf" not in args:
        pdf = AQUI / "arquitetura.pdf"
        rodar_headless(["--no-pdf-header-footer", f"--print-to-pdf={pdf}", str(pagina)], pdf)
        print(f"  ok  arquitetura.pdf ({pdf.stat().st_size // 1024} KB)")
    if AVISOS:
        print("\nAVISOS do layout (confira o diagrama):")
        for a in AVISOS:
            print("  -", a)
    else:
        print("  verificação de layout: sem sobreposições detectadas")


if __name__ == "__main__":
    main()
