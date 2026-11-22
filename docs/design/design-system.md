# Fluxo: design system (v2, direção "Ameixa")

Referência visual: [design-system.png](design-system.png).

Princípio geral: **base neutra, cor só onde carrega significado.** A marca aparece na ação principal, na seleção e no foco. Alerta, perigo e sucesso são cores separadas da marca e nunca aparecem sozinhas: sempre com texto ou forma (WCAG 1.4.1).

## 1. Cor

Contrastes calculados pela fórmula de luminância relativa da WCAG 2.2. Limites: texto normal ≥ 4,5:1; texto grande (≥ 24 px, ou ≥ 18,5 px em 600) e componentes/estados ≥ 3:1.

### Marca

| Token | Hex | Uso | Contraste |
|---|---|---|---|
| `brand` | #6E2F63 | Botão primário, foco, seleção, link de ação, marca | branco sobre ela 9,33:1; ela sobre branco 9,33:1; sobre ink-50 8,33:1 |
| `brand-700` | #58244F | Botão pressionado; texto de item selecionado | branco 11,80:1; sobre brand-50 10,21:1 |
| `brand-100` | #EBD9E7 | Halo externo do foco (decorativo; o anel brand é a pista) | brand sobre ela 6,94:1 |
| `brand-50` | #F5ECF3 | Fundo de item selecionado na lista | brand 8,08:1; ink-600 6,75:1 |

### Neutros (matiz ameixa)

| Token | Hex | Uso | Contraste |
|---|---|---|---|
| `ink-900` | #231E23 | Texto principal | 16,39:1 branco; 15,74:1 ink-25; 14,62:1 ink-50 |
| `ink-800` | #3B343B | Timestamps, valores em tabela | 12,07:1 branco |
| `ink-700` | #4A434B | Descrição, texto de apoio forte, iniciais | 9,56:1 branco; 8,11:1 sobre ink-100 |
| `ink-600` | #575058 | Texto secundário, rótulos de métrica, desenvolvedor no cartão | 7,80:1 branco; 6,96:1 ink-50; 6,75:1 brand-50 |
| `ink-500` | #736B74 | Metadado, identificador, rótulo de campo, contagem de coluna | 5,14:1 branco; 4,94:1 ink-25; 4,59:1 ink-50. **Reprova sobre brand-50 (4,45:1): lá use ink-600** |
| `ink-400` | #8C838D | Borda de campo (única pista do controle) | 3,65:1 branco; 3,26:1 ink-50 (≥ 3:1) |
| `ink-300` | #B8AFB9 | Borda do botão secundário, slot tracejado, barra da média | 2,13:1 (decorativo: o botão é identificado pelo texto) |
| `ink-200` | #DDD6DD | Barras "apagadas" do ícone de prioridade | 1,42:1 (o texto ao lado carrega a informação) |
| `ink-150` | #E9E4E9 | Divisores, borda do cartão, linha da barra superior | 1,25:1 (decorativo) |
| `ink-100` | #EFEBEF | Fundo das iniciais do usuário | — |
| `ink-50` | #F4F1F4 | Fundo da coluna do quadro | — |
| `ink-25` | #FBFAFB | Fundo da página | — |
| `white` | #FFFFFF | Cartão, campo, painel de detalhe | — |

### Semânticas

| Token | Hex | Uso | Contraste |
|---|---|---|---|
| `warn-fg` / `warn-bg` | #8A4B00 / #FFF3D6 | Cabeçalho de coluna no limite de WIP, alvo de soltar recusado | 6,17:1 |
| `warn-line` | #B26A00 | Linha inferior do cabeçalho no limite, contorno tracejado do alvo recusado | 4,24:1 branco; 3,84:1 sobre warn-bg; 3,78:1 sobre ink-50 |
| `danger` / `danger-bg` | #B3261E / #FDECEA | Prioridade alta, erro de campo, ação Excluir | 6,54:1 branco; 5,72:1 danger-bg; 5,83:1 ink-50 |
| `success` / `success-bg` | #2E6B30 / #E9F4E8 | Só confirmação pontual (ex.: "Cartão criado.") | 5,69:1; 6,44:1 branco |

Regras:
- Uma cor de estado por região. No quadro, só a coluna Fazendo no limite e a prioridade Alta têm cor.
- **Feito não é verde.** Concluído se comunica pela coluna, pelo ícone de check e pelo texto. Verde em toda a coluna seria só decoração.
- Sem cor por pessoa. Iniciais sempre em ink-100/ink-700.

## 2. Tipografia

Fontes instaladas no Windows, sem download:
- **Bahnschrift** (DIN 1451, sinalização) em títulos, números de métrica e contagens de WIP, sempre com `font-variant-numeric: tabular-nums`.
- **Segoe UI Variable** (`"Segoe UI Variable Text", "Segoe UI", system-ui`) no texto.
- **Consolas** para identificadores (PRJ001, QDR001, CRT014) e timestamps (`07-MAR-2026-17:30`).

| Estilo | Fonte | Tamanho / entrelinha | Peso | Onde |
|---|---|---|---|---|
| display | Bahnschrift | 56 / 56 | 600 | Só a marca "Fluxo" no login |
| metric-lg | Bahnschrift | 40 / 48 (unidade 20) | 350, tnum | Tempo de ciclo e lead time no detalhe do cartão |
| metric | Bahnschrift | 32 / 36 (unidade 18) | 350, tnum | Médias do quadro no cabeçalho |
| page | Bahnschrift | 28 / 32 (36/44 no detalhe de projeto) | 600 | Título da tela |
| section | Bahnschrift | 18 / 24 | 600 | "Linha do tempo", "Dados", "Quadros" |
| column | Bahnschrift | 16 / 20 | 600, tnum | Cabeçalho de coluna e "3/3" |
| lead | Segoe UI Variable | 16 / 24 | 400 | Descrição da entidade, slogan do login |
| body | Segoe UI Variable | 14 / 20 | 400 e 600 | Texto, nome do cartão (600), tabela |
| label | Segoe UI Variable | 13 / 16 | 400 | Rótulo de métrica, breadcrumb |
| meta | Segoe UI Variable | 12 / 16 | 400 | Rótulo de campo, legenda, metadado do cartão |
| mono | Consolas | 12,5–13 / 16 | 400 | Identificadores e timestamps |

Regras:
- Dois pesos no texto (400 e 600), como a rampa do Windows 11. Nos números grandes, Bahnschrift 350: o tamanho já dá ênfase, e o peso leve evita o "número gritado".
- Unidade menor e mais clara que o número (`2d 8h 30min`): o número lê primeiro, a unidade depois.
- Hierarquia por peso e cor antes de tamanho. Rótulo sempre menos enfático que o dado.
- Texto corrido com no máximo ~75 caracteres por linha.

## 3. Grid e espaçamento

- Tela de referência 1280 × 800. Margem lateral 32 px. Sem sidebar: barra superior de 48 px com marca, navegação (Projetos, Pessoas) e usuário.
- Quadro: 3 colunas iguais (≈ 395 px) com 16 px entre elas.
- Detalhe de cartão: conteúdo + coluna de dados de 288 px, separados por 48 px e um divisor.
- Projetos: lista de 352 px + detalhe com conteúdo limitado a 720 px.

| Token | px | Uso |
|---|---|---|
| `s1` | 4 | Ícone–texto, rótulo–valor |
| `s2` | 8 | Entre cartões, dentro de componentes |
| `s3` | 12 | Padding do cartão, itens de grupo |
| `s4` | 16 | Padding de coluna, gap de colunas |
| `s6` | 24 | Entre grupos |
| `s8` | 32 | Margem da tela, blocos |
| `s12` | 48 | Entre seções, entre métricas |
| `s16` | 64 | Respiro de página |

Controles: altura 32 px (36 no campo de busca, 40 no login), alvo ≥ 24 × 24 (WCAG 2.5.8).

## 4. Raios e elevação

| Token | Valor | Uso |
|---|---|---|
| `r-sm` | 4 px | Marcadores, barras |
| `r-md` | 6 px | Cartão, botão, campo |
| `r-lg` | 12 px | Coluna do quadro |
| `e-drag` | `0 1px 2px rgba(35,30,35,.14), 0 10px 28px rgba(35,30,35,.18)` | Só o cartão sendo arrastado |
| `e-overlay` | `0 1px 2px …/.10, 0 4px 12px …/.10` | Menus e diálogos |

Coluna: fundo tingido, sem borda e sem sombra (região comum). Cartão: branco com borda de 1 px ink-150, sem sombra. Métricas e dados: texto direto na página, sem caixa.

## 5. Componentes

**Barra superior.** Marca (ícone de três colunas: vazia, meio cheia, cheia = A fazer → Fazendo → Feito) + "Fluxo" em Bahnschrift brand; navegação com item ativo em 600 + sublinhado brand de 2 px (não só cor); iniciais neutras + nome à direita.

**Botões.**
- Primário: fundo brand, texto branco 600. No máximo um por tela.
- Secundário: fundo branco, borda ink-300, texto ink-900.
- Quieto: só texto brand (ex.: "Novo cartão" no cabeçalho de A fazer, "Criar conta").
- Perigo: só texto danger ("Excluir"). Confirmação em diálogo.
- Estados: foco = anel brand de 2 px afastado 2 px; pressionado = brand-700.
- **Sem permissão = ausente.** Não há estado "desabilitado com tooltip" para regras de papel.

**Campo.** Rótulo 14/600 acima; caixa branca com borda ink-400 (3,65:1).
- Foco: borda + anel brand, halo brand-100.
- Erro: borda danger + ícone + mensagem que diz como corrigir ("Use um número de 1 a 25.").

**Cartão (no quadro).** Quatro dados: nome (14/600), identificador (Consolas, ink-500), prioridade (ícone + texto), desenvolvedor (ink-600, à direita). Descrição, tamanho e timestamps ficam no detalhe.
- Estados: normal; arrastando (rotação de 1,5°, `e-drag`); origem do arrasto (slot tracejado ink-300).
- Sem botão "Mover" no cartão: mover é arrastar (ou atalho de teclado no cartão focado).

**Indicador de prioridade.** Três barras crescentes: 3 cheias = Alta (danger, texto 600), 2 = Média, 1 = Baixa (ink-600). A forma distingue sem cor, o texto confirma, e só Alta ganha cor.

**Cabeçalho de coluna.**
- Normal: nome (Bahnschrift 16/600) + contagem em ink-500.
- No limite (só em Fazendo): contagem vira "n/limite" (3/3) em 600, fundo warn-bg, linha inferior warn-line de 2 px, ícone de bloqueio e o texto "No limite".
- O WIP aparece **só aqui** (e na tabela de quadros do projeto, que é outra tela).

**Alvo de soltar recusado.** Só durante o arrasto sobre a coluna Fazendo cheia: área com contorno tracejado warn-line, hachura diagonal, texto "Sem vaga: limite de 3 cartões", cursor de bloqueio. Ao soltar, o cartão volta à origem. A regra aparece no momento e no lugar da ação.

**Linha de métrica.** Rótulo 13 px ink-600 acima, número Bahnschrift 32 (ou 40) com unidades menores. Comparação com a média do quadro em texto ("10h 30min acima da média do quadro.") e em duas barras na mesma escala (este cartão em brand, média em ink-300) com o valor escrito. Sem chip colorido.

**Linha do tempo (detalhe do cartão).** Eixo proporcional ao tempo real. Entrada (círculo vazio) → Início (anel brand) → Término (círculo cheio com check). A espera é tracejada, o trabalho é linha cheia. Abaixo, chaves marcam o "Tempo de ciclo" (início → término) e o "Lead time" (entrada → término): a definição de cada métrica aparece no desenho.

**Lista com seleção.** Linha selecionada com fundo brand-50, barra brand de 3 px à esquerda e nome em 600 brand-700 (três pistas além da cor).

## 6. Regras de conteúdo

- **Sentence case** em títulos, rótulos, botões e estados. Nenhum rótulo em caixa alta espaçada.
- **Rótulo de exibição ≠ valor armazenado.** O sistema guarda `A FAZER`, `FAZENDO`, `FEITO`, `ALTA`, `MEDIA`, `BAIXA`, `GRANDE`, `MEDIO`, `PEQUENO`, `GESTOR`, `DESENVOLVEDOR`. A interface exibe "A fazer", "Fazendo", "Feito", "Alta", "Média", "Baixa", "Grande", "Médio", "Pequeno". O mapeamento é fixo e fica na camada de apresentação.
- **Dados do usuário aparecem como estão guardados.** Identificador em Consolas (PRJ001), timestamp `DIA-MES-AAAA-HH:MM` (05-MAR-2026-09:00), email em minúsculas.
- **Rótulo de campo é o nome do atributo**, não flexionado pelo valor ("Gestor do projeto: Ana Ribeiro", "Desenvolvedor: Davi Martins"). O sistema não guarda gênero.
- **Rótulo vira frase quando possível:** "Média dos 3 cartões em Feito", "4h 40min abaixo da média do quadro."
- **Regra pelo estado, não por aviso.** Sem banner de papel, sem tooltip de restrição, sem "fale com o gestor". Quem não pode editar não vê a ação; o dado "Gestor do projeto" explica por quê.
- **Não inventar dados.** A tela só mostra atributos que existem no modelo (nada de "criado por", "em andamento" ou "dias de duração").
- **Durações:** `Nd Nh Nmin`, com algarismos tabulares.
