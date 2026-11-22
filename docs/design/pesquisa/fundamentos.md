# Fundamentos de design de interface para o redesenho do Kanban

Tudo o que está abaixo vem de páginas que abri de fato (lista no fim), de verificações feitas nesta máquina com Windows 11 ou de contas de contraste que fiz pela fórmula da WCAG. Quando a afirmação é inferência minha, está marcado.

**Fontes que não consegui acessar:**
- **Artigo "7 Practical Tips for Cheating at Design" no Medium:** deu HTTP 403. Os espelhos (readmedium, web.archive.org) foram bloqueados pela política de permissões. Por isso não cito o artigo diretamente. Uso o site oficial do Refactoring UI, as duas prévias oficiais do livro, a compilação de tweets do Steve Schoger e uma resenha do livro.
- **Material Design (m2/m3):** as páginas vieram sem conteúdo, porque dependem de JS. Para espaçamento usei Atlassian, Carbon e Primer.

---

## 1. Achados por tema

### 1.1 Refactoring UI (Wathan e Schoger)

**Hierarquia sem caixas**
- O livro tem os capítulos "Not all elements are equal", "Size isn't everything", "De-emphasize to emphasize" e "Labels are a last resort" (refactoringui.com).
- Tweet do Schoger de 19/09/2017: "Font size isn't always the best way to emphasize or de-emphasize text, try using color and font weight instead" (digitalsynopsis).
- Segundo a resenha no dev.to, às vezes basta tirar a ênfase dos elementos vizinhos para destacar o principal.

**Rótulos**
- Rótulo e valor podem virar uma frase só: "12 left in stock" no lugar de "In stock: 12".
- Em dashboards o rótulo fica, mas como conteúdo secundário: menor, mais claro ou mais leve. Citação: "The data itself is what matters" (prévia oficial "labels-are-a-last-resort").
- Isso ataca diretamente os rótulos em CAIXA ALTA espalhados pela tela.

**Menos bordas**
- Capítulo "Use fewer borders": bordas demais deixam a interface poluída. As alternativas são sombra, fundo de cor contrastante ou mais espaço (refactoringui.com).
- Tweet de 16/08/2017: "Too many borders can make a design look really busy."

**Espaço**
- Capítulos "Start with too much white space", "Establish a spacing and sizing system" e "Avoid ambiguous spacing" (refactoringui.com).
- A escala parte de um valor base, com passos pequenos no começo e maiores no fim (dev.to).

**Sombras**
- Capítulos "Emulate a light source", "Use shadows to convey elevation" e "Shadows can have two parts".
- Quanto menor e menos borrada a sombra, mais perto do fundo o elemento parece estar (dev.to).
- Um leve deslocamento vertical deixa a sombra mais natural (tweet de 20/06/2017).

**Cor**
- Monte os tons antes de começar ("Define your shades up front"):
  - 8 a 10 cinzas, sem preto puro;
  - 1 ou 2 cores primárias com 5 a 10 tons cada;
  - cores de destaque para estados (perigo, alerta, sucesso), "use them sparingly".
- O tom base (500) deve servir como fundo de botão. O 900 vai para texto e o 100 para fundos tingidos. "Trust your eyes, not the numbers" (prévia oficial "building-your-color-palette").
- Cinzas com matiz: "Try saturating your greys with a bit of blue or brown for a cooler or warmer feel" (tweet de 19/03/2018). Capítulo "Greys don't have to be grey".
- Não use texto cinza sobre fundo colorido. Tinja o texto com o matiz do fundo (capítulo do livro e tweet de 12/06/2017).

**Tipografia**
- "Most interfaces use too many font sizes."
- Linhas de 45 a 75 caracteres.
- Linhas mais longas pedem entrelinha maior (dev.to).

**Ícones nos cards de métricas**
- O Schoger sugere pôr uma forma com fundo atrás de ícones pequenos em vez de ampliá-los (tweet de 16/11/2017).
- Esse é justamente o padrão "ícone em quadradinho colorido" que foi criticado. Ele resolve o problema do ícone ampliado, mas usado em todo card vira ruído. Isso é inferência minha.

### 1.2 Laws of UX (lawsofux.com), aplicadas às 4 telas

| Lei | Definição (lawsofux) | Aplicação |
|---|---|---|
| Proximidade | Objetos próximos são percebidos como grupo | Agrupe por espaço, não por card: título + metadado colados, blocos separados por 24–32 px |
| Região Comum | Área com limite claro forma grupo; um fundo também cria região | Use região só nas colunas do quadro (fundo levemente tingido). Cartões e métricas não precisam de caixa cada um |
| Similaridade | Elementos parecidos parecem relacionados | Se toda pílula tem a mesma forma colorida, prioridade, status e tag viram uma coisa só. Diferencie por forma e texto |
| Von Restorff | O item diferente é lembrado; enfatize com parcimônia e não só por cor | Uma única coisa destacada por tela: a coluna que estourou o WIP, ou o CTA de login |
| Hick | Tempo de decisão cresce com opções; destaque a recomendada | Login com 1 ação primária. Lista do DEV sem ações de edição |
| Jakob | Usuários esperam que funcione como produtos que já conhecem | Convenção do Jira: o limite aparece no topo da coluna e a coluna muda de cor quando o máximo é excedido (docs Jira Server) |
| Tesler | Complexidade irredutível deve ser absorvida pelo sistema | A regra de WIP deve ser imposta e mostrada pela UI ("4/4", alvo de soltar desabilitado), não explicada em banner |
| Carga cognitiva (extrínseca) | Elementos que não ajudam a entender consomem recursos | Tirar banners, tooltips redundantes, badges decorativos e caixas |
| Fitts | Tempo depende de distância e tamanho do alvo | Alvos de arrastar/soltar grandes (coluna inteira). WCAG 2.5.8 pede alvo ≥ 24×24 px (AA) |
| Doherty | Resposta em < 400 ms | Feedback imediato ao mover cartão (otimista) |
| Estética-usabilidade | Interfaces bonitas parecem mais fáceis | Justifica o esforço visual, mas pode mascarar problemas em teste |

Ressalva honesta sobre Tesler: a própria página recomenda orientação contextual, "such as tooltips". A crítica, portanto, não é contra tooltips em si. Ela vale para tooltips e banners que explicam algo que o estado visual já deveria mostrar.

### 1.3 WCAG 2.2 (W3C)

- **1.4.3 Contraste mínimo (AA):**
  - texto ≥ **4.5:1**;
  - texto grande ≥ **3:1**. Texto grande é ≥ 18 pt (≈ 24 px) ou ≥ 14 pt negrito (≈ 18.5 px negrito);
  - não há arredondamento: 4.499:1 reprova;
  - isentos: componente inativo, texto decorativo e logotipo.
- **1.4.11 Contraste não textual (AA):** ≥ **3:1** contra a cor adjacente para:
  - o que identifica um componente e seu estado (inclusive foco);
  - partes de gráficos necessárias para entender o conteúdo.

  Uma borda de input só precisa de 3:1 se for a única pista do controle. Exemplo da W3C: borda #767676 passa, #AAA sobre branco (≈ 2.3:1) falha. Botão com texto não precisa de borda. Componente desabilitado é isento.
- **1.4.1 Uso de cor (A):** "Color is not used as the only visual means of conveying information". Falhas comuns:
  - F81: campo obrigatório ou com erro indicado só por cor;
  - F13: gráfico que depende só de cor.

  Erro e sucesso precisam de pista além da cor, mesmo com 3:1.
- **1.4.8 Apresentação visual (AAA, referência):**
  - ≤ 80 caracteres por linha;
  - entrelinha ≥ 1,5 em parágrafos;
  - texto não justificado.
- **2.5.8 Tamanho do alvo (AA):** ≥ 24×24 px CSS, com exceção quando há espaçamento suficiente.

**Contas que fiz (fórmula WCAG):**
- O cinza "padrão de template" #9CA3AF (gray-400 do Tailwind) dá **2.54:1** sobre branco e reprova para texto. #94A3B8 dá 2.56:1.
- #767676 dá 4.54:1 e passa.

### 1.4 Escala tipográfica

**Razões do typescale.com:**
- 1.067 (Minor Second)
- 1.125 (Major Second)
- **1.2 (Minor Third)**
- **1.25 (Major Third)**
- 1.333 (Perfect Fourth)
- 1.414
- 1.5
- 1.618

**Sistemas reais:**
- **Atlassian:** body 14/20, small 12/16, large 16/24; headings 12/16, 14/20, 16/20, 20/24, 24/28, 28/32, 32/36. Tem também tokens próprios para **métricas**: 16/20, 24/28, 28/32 em Bold.
- **Windows 11:** 12/16, 14/20, 18/24, 20/28, 28/36, 40/52. Pesos só Regular e Semibold, sem Bold. **Sentence case em toda a UI.** Mínimo 12 px Regular / 14 px Semibold. 50–60 caracteres por linha.
- **Primer (GitHub):** entrelinhas alinhadas a uma grade de 4 px; cerca de 80 caracteres ou menos por linha; texto alinhado à esquerda; "evite usar cor como principal meio de ênfase".

**Entrelinha e comprimento de linha:**
- Butterick (Practical Typography): entrelinha de 120–145% e linha de 45–90 caracteres.
- Refactoring UI: 45–75 caracteres.

**Caixa alta (Butterick):**
- Use com parcimônia (títulos curtos, rótulos pequenos) e sempre com espaçamento entre letras e kerning.
- Parágrafo em caixa alta "é tipografia que se autoderrota".

**Números tabulares (MDN):**
- `font-variant-numeric: tabular-nums` corresponde ao recurso OpenType `tnum` e dá dígitos de largura igual.
- Só funciona se a fonte tiver `tnum`.

### 1.5 Grid e espaçamento

- **Atlassian:** base 8 px, com tokens de 0, 2, 4, 6, 8, 12, 16, 20, 24, 32, 40, 48, 64 e 80.
  - 0–8 px: UI compacta (ícone com texto, padding de badge);
  - 12–24 px: botões, itens de card;
  - 32–80 px: layout de página.
  - "Use tokens no lugar de valores crus".
- **Carbon (IBM):** 2, 4, 8, 12, 16, 24, 32, 40, 48, 64, 80, 96, 160 (múltiplos de 2, 4 e 8). "Evite sair da escala". Espaço comunica relação e hierarquia.
- **Primer:** passos de 2 px até 48 e depois 64, 80, 96…
  - pilhas: condensed 8, normal 16, spacious 24;
  - altura de controle: 24, 28, **32**, 40, 48;
  - raio: 3, **6** (padrão), 12;
  - foco com contorno de 2 px;
  - breakpoint xlarge = **1280 px**.

### 1.6 Fontes

**Licença aberta.** Verifiquei os recursos OpenType no Fontsource ou no repositório oficial:

| Fonte | Personalidade (fonte oficial) | Números tabulares | Licença / onde obter |
|---|---|---|---|
| **Instrument Sans** | Neo-grotesca: "precision with subtle notes of playfulness". Eixo de largura 75–100 (condensa para colunas densas). 12 conjuntos estilísticos | **Sim (`tnum`)**, confirmado no README oficial | OFL 1.1, github.com/Instrument/instrument-sans e Google Fonts |
| **Atkinson Hyperlegible Next** | Feita pelo Braille Institute para baixa visão. Distingue 1/I/l e 0/O. 7 pesos, variável 200–800 | **Sim** (Fontsource) | OFL 1.1 na versão do Google (repo googlefonts/atkinson-hyperlegible-next). O download no site do Braille Institute tem EULA próprio |
| **Public Sans** | Neutra, do governo dos EUA (USWDS), derivada da Libre Franklin. Métricas próximas do SF Pro | **Sim** (README cita "tabular numerals") | OFL 1.1, github.com/uswds/public-sans. **Não é mais mantida** |
| **IBM Plex Sans** | "Neutral, yet friendly Grotesque". Eixos de peso e largura. Tem zero cortado | `tnum` **não aparece** no Fontsource, então não confirmei | OFL 1.1, github.com/IBM/plex |
| **Source Sans 3** | Adobe, "intended to work well in user interfaces" | `tnum` não listado. Lista `pnum`, o que sugere dígitos tabulares por padrão (inferência). Tem `zero` | OFL 1.1, github.com/adobe-fonts/source-sans |
| **Figtree** | "Friendly, simple geometric" (Erik Kennedy). 300–900 | **Sim** (Fontsource) | OFL 1.1 |
| **Manrope** | Sans moderna, 200–800 | **Sim** (Fontsource) | OFL 1.1 |
| **Geist** | Vercel, estilo suíço. O README diz que mira títulos e tamanhos grandes. Muito associada à "estética Vercel" | **Sim** (Fontsource) | OFL 1.1, vercel.com/font |

**Monoespaçadas para identificadores**
- **JetBrains Mono:** x-height maximizada, o que ajuda em tamanho pequeno. 8 pesos. OFL.
- **IBM Plex Mono:** zero cortado (`zero`), OFL.
- **Geist Mono:** "readability in code and tabular content", OFL.

**Fontes instaladas no Windows — verificação local**

Abri os arquivos em C:\WINDOWS\Fonts e li as tabelas fvar, GSUB e hmtx com um parser próprio.

| Fonte | Status nesta máquina | Recursos verificados |
|---|---|---|
| **Segoe UI Variable** | Instalada (SegUIVar.ttf) | Eixos wght 300–700 e opsz 5–36 (tamanho óptico automático). **Tem `tnum`**. Por padrão todos os dígitos têm a mesma largura (1104 unidades) **exceto o "1"** (776), então ative `tabular-nums` mesmo assim. No CSS os nomes são "Segoe UI Variable Text", "… Small" e "… Display" |
| **Bahnschrift** | Instalada | Digitalização da norma DIN 1451 (sinalização de estradas alemãs). Eixos wght 300–700 e wdth 75–100. **Pesos "duplexados"**: a largura não muda entre Light e Bold, segundo a Microsoft, o que é útil para dados. Dígitos **proporcionais por padrão**, mas **tem `tnum`** |
| **Sitka** | Instalada | Serifada com tamanho óptico (opsz 6–27,5). Tem `tnum`. Tem caráter, mas não serve para UI densa |
| **Corbel / Candara** | Instaladas | Têm `tnum`, `lnum` e `onum`, com dígitos proporcionais por padrão. Aparência mais "Office" |
| **Consolas** | Instalada | Mono padrão disponível |
| **Cascadia Code/Mono** | **Não estão** em C:\Windows\Fonts nem no registro, embora a lista oficial do Windows 11 as inclua | Vêm com o Windows Terminal |
| **Aptos** | **Não instalada** | Ela vem do Office, não do Windows |

### 1.7 Cor de marca

**Atlassian Design System**
- Separa as cores em papéis:
  - *accent*: sem significado e intercambiável;
  - *success*, *warning* ("prevenir um erro"), *danger* ("perigo ou erro grave"), *information*, *discovery*;
  - neutros para a maior parte de fundos, texto e formas;
  - *brand*: ações primárias.
- "Don't use an accent when the color has semantic meaning."
- Seleção deve ter pista além da cor (check, sublinhado, borda).

**Primer**
- Tokens funcionais success, attention, danger e **done**, cada um com variantes `-emphasis` e `-muted`.

**Produtos de fluxo de trabalho com marca não azul**
- **Asana**, post oficial: trocou o verde frio por "an empowering, warm coral". A ideia é "a clean white canvas" com "bursts of color in those fulfilling moments", e a cor é usada "so it never disrupts your focus". É o modelo a seguir: neutros dominam, a cor aparece só em momentos de conclusão.
- **Linear**, página oficial de marca: cor primária "a subtle desaturated blue", reservada para fundos, e "Monochrome usage is preferred". É azul, mas é o oposto do "azul SaaS saturado". Mostra que dá para ter marca quase monocromática.
- **Todoist (vermelho):** só vi em resultados de busca (fontes de terceiros, ≈ #E44432). Não abri fonte oficial, então trate como não verificado.
- **Jira:** é azul e serve de contraexemplo. A convenção de WIP dele está nos docs: vermelho quando o máximo é excedido, amarelo quando o mínimo não é atingido, limite exibido no topo da coluna.

---

## 2. Recomendação concreta

### (a) Fontes

**Principal: Instrument Sans**
- `tnum` confirmado, personalidade "precisão com leve brincadeira" e eixo de largura para cabeçalhos de coluna e tabelas.
- Como o render é offline, empacote os .woff2 no projeto via `@font-face` local. A OFL permite.
- Alternativa com argumento acadêmico de acessibilidade: **Atkinson Hyperlegible Next**.

**Fallback instalado: Segoe UI Variable**
```css
font-family: "Instrument Sans", "Segoe UI Variable Text", "Segoe UI", system-ui, sans-serif;
font-variant-numeric: tabular-nums; /* nas métricas, contagens WIP, datas e durações */
```

**Opção "só fontes instaladas" com identidade**
- **Bahnschrift** nos títulos e números de métrica: estética DIN e sinalização, o que combina com a ideia de "quadro". Sempre com `tabular-nums`.
- **Segoe UI Variable Text** no corpo.

**Monoespaçada**
- **JetBrains Mono** (ou IBM Plex Mono, pelo zero cortado) para IDs como `KB-142`.
- Fallback instalado: **Consolas**. Cascadia não está instalada aqui.

### (b) Escala tipográfica

Base 14 px, razão ≈ 1.2, arredondada para a grade de 4 px.

| Uso | Tamanho / entrelinha | Peso |
|---|---|---|
| Metadado, legenda, rótulo secundário | 12 / 16 | 400 (cor ink-500) |
| Corpo, texto do cartão, tabela | **14 / 20** | 400; 600 para ênfase |
| Título de cartão, campos, subtítulo | 16 / 24 | 600 |
| Título de seção ou coluna | 20 / 28 | 600 |
| Título de página | 24 / 32 | 600 |
| Número de métrica (tempo de ciclo, lead time) | 28 / 32 a 32 / 40 | 600, `tabular-nums` |
| Título do login (opcional) | 36 / 44 | 600 |

**Regras:**
- Só 2 pesos (400 e 600), como a rampa do Windows.
- Sentence case em toda a UI.
- Caixa alta, no máximo, em um único tipo de rótulo de 12 px com espaçamento de letras levemente aberto. O valor exato (≈ 0,04 em) é escolha minha; a fonte só diz "sempre espaçar".
- Hierarquia por peso e cor antes de tamanho.
- Texto corrido do detalhe do cartão com máximo de 60–75 caracteres (≈ 560–640 px a 14 px).

### (c) Grid e espaçamento

- **Base 4 px, ritmo de 8 px.** Tokens: 4, 8, 12, 16, 24, 32, 48, 64 (subconjunto de Atlassian e Carbon). Nada fora da escala.
- **Uso:**
  - 4–8 px dentro de componentes;
  - 12–16 px entre itens de um grupo;
  - 24–32 px entre grupos;
  - 48–64 px na margem e no respiro da página.
- **Em 1280×800:** margem lateral de 32 px.
  - Quadro: 3 colunas iguais com espaço de 16 px. Sem barra lateral, cada coluna fica ≈ (1280 − 64 − 32) / 3 ≈ 395 px.
  - Com barra lateral de 240 px, a conta é refeita.
- **Controles:** altura 32 px (40 px na ação primária do login), alvo ≥ 24×24 (WCAG 2.5.8).
- **Raios variados por função** em vez de um raio único em tudo: 4 em chips, 6 em controles e cartões, 12 só em painéis grandes (baseado na escala do Primer).
- **Elevação:** só o cartão arrastado e os overlays têm sombra.
  - Colunas: fundo tingido, sem borda e sem sombra.
  - Métricas: texto sobre o fundo, separadas por espaço ou por um divisor de 1 px, sem caixa nem ícone em quadradinho.

### (d) Contraste (WCAG 2.2)

- **Texto normal ≥ 4.5:1. Texto grande ≥ 3:1** (≥ 24 px, ou ≥ 18,5 px em 600/700).
- **Bordas de input, anel de foco e indicadores de estado ≥ 3:1** com a cor adjacente.
  - Divisores decorativos podem ser claros (≈ 1.4:1).
  - Mas aí o input precisa de outra pista, ou de uma borda ≥ 3:1 (ex.: #868F8C dá 3.25:1).
- **Nunca só cor:**
  - WIP estourado = número "5/4" + ícone + texto curto ("acima do limite") + cor;
  - prioridade alta = texto ou ícone + cor;
  - concluído = texto e estado, não só verde.
- **Cuidado com o cinza secundário sobre fundo tingido.** Na direção A, ink-500 dá 4.92:1 no branco, mas só **4.29:1** sobre brand-50 e reprova. Lá use ink-600.

### (e) Direções de cor de marca

Contas minhas pela fórmula WCAG. Em todas as direções:
- os neutros têm leve matiz da marca (Refactoring UI);
- as cores semânticas são separadas da marca (Atlassian).

**A. "Petróleo" (verde-azulado escuro)**
- Por quê: lembra fluxo e calma sem cair no azul SaaS, e funciona bem com neutros esverdeados.
- Tokens:
  - brand #0F5F5A (branco sobre ela 7.49:1), brand-700 #0B4A46, brand-50 #E6F2F0;
  - ink-900 #1B2422 (15.9:1), ink-600 #4B5754 (7.5:1), ink-500 #68736F (4.92:1).
- Risco: fica perto do verde de sucesso. Solução: deixe "FEITO" sem verde, só com menos ênfase (cinza e check). O sucesso verde fica para confirmações pontuais.

**B. "Ameixa" (berinjela)**
- Por quê: é o matiz mais longe de vermelho, âmbar e verde. Não briga com nenhuma cor semântica e dá um tom sóbrio e acadêmico.
- Tokens:
  - brand #6E2F63 (branco 9.33:1), brand-700 #58244F, brand-50 #F5ECF3;
  - ink-900 #231E23, ink-600 #575058 (7.8:1), ink-500 #736B74 (5.14:1).
- É a opção mais segura do ponto de vista semântico.

**C. "Terracota"**
- Por quê: é quente e "de papel", como o cartão físico do kanban, e é o mesmo raciocínio do coral da Asana.
- Tokens:
  - brand #A3401C (branco 6.35:1), brand-700 #843214, brand-50 #FBEEE8;
  - neutros "pedra" ink-900 #27211D, ink-600 #5D554E (7.3:1), ink-500 #786F68 (4.92:1).
- Risco: fica perto do vermelho de perigo. Nesse caso o perigo precisa sempre de ícone e texto, e a marca não pode ser usada em estados.

**Semânticas comuns às três**

| Papel | Texto / fundo | Contraste |
|---|---|---|
| Alerta / WIP | #8A4B00 sobre #FFF3D6 | 6.17:1 |
| Linha de alerta | #B26A00 | 4.24:1 no branco, passa os 3:1 de componente |
| Perigo / prioridade alta | #B3261E sobre #FDECEA | 5.72:1 |
| Sucesso | #2E6B30 sobre #E9F4E8 | 5.69:1 |

Uso: um tom de cor por tela; badges só onde existe significado.

---

## 3. Referências (todas abertas, salvo indicação)

**Refactoring UI**
- https://www.refactoringui.com/ — lista oficial de capítulos (hierarquia, "Use fewer borders", espaço, cinzas, sombras) e trecho sobre bordas.
- https://www.refactoringui.com/previews/building-your-color-palette — 8–10 cinzas, 5–10 tons por cor, papéis do 100/500/900.
- https://www.refactoringui.com/previews/labels-are-a-last-resort — combinar rótulo e valor, tirar ênfase do rótulo.
- https://digitalsynopsis.com/design/useful-ui-ux-design-tips/ — tweets do Schoger com datas (cor e peso, bordas, cinzas saturados, sombras, ícones).
- https://dev.to/updivision/book-review-refactoring-ui-by-adam-wathan-steve-schoger-2f7m — resenha: sombras em duas camadas, escala, 45–75 caracteres.
- https://wiert.me/2020/03/30/7-practical-tips-for-cheating-at-design-refactoring-ui-medium/ — aberta, mas só tem o título do artigo e um parágrafo do blogueiro; não usada.
- Não acessíveis: medium.com/refactoring-ui/… (403); readmedium e web.archive (bloqueados pela política); refactoringui.com/previews (404).

**Laws of UX**
- https://lawsofux.com/ — lista das leis.
- Páginas de cada lei: /law-of-proximity/, /law-of-common-region/, /law-of-similarity/, /von-restorff-effect/, /hicks-law/, /jakobs-law/, /teslers-law/, /cognitive-load/, /fittss-law/, /doherty-threshold/, /aesthetic-usability-effect/.

**WCAG 2.2 (W3C)**
- https://www.w3.org/WAI/WCAG22/Understanding/contrast-minimum.html — 4.5:1, 3:1, definição de texto grande.
- https://www.w3.org/WAI/WCAG22/Understanding/non-text-contrast.html — 3:1 para componentes e estados; exemplo #767676 contra #AAA.
- https://www.w3.org/WAI/WCAG22/Understanding/use-of-color.html — 1.4.1, falhas F81, F73 e F13.
- https://www.w3.org/WAI/WCAG22/Understanding/visual-presentation.html — 80 caracteres, entrelinha 1,5 (AAA).
- https://www.w3.org/WAI/WCAG22/Understanding/target-size-minimum.html — 24×24 px (AA).

**Tipografia**
- https://typescale.com/ — razões da escala modular.
- https://practicaltypography.com/all-caps.html, /line-length.html, /line-spacing.html — caixa alta, 45–90 caracteres, entrelinha 120–145%.
- https://developer.mozilla.org/en-US/docs/Web/CSS/font-variant-numeric — tabular-nums = `tnum`.

**Sistemas de design**
- https://atlassian.design/foundations/spacing — tokens base 8.
- https://atlassian.design/foundations/typography — tokens de texto e de métrica.
- https://atlassian.design/foundations/color — papéis semânticos, "não só cor", 4.5:1 e 3:1.
- https://primer.style/foundations/primitives/size — espaçamento, raio, alturas de controle, breakpoints.
- https://primer.style/foundations/typography — grade de 4 px, cerca de 80 caracteres, cor não é o principal meio de ênfase.
- https://primer.style/foundations/color — tokens success, attention, danger, done (página só lista variáveis).
- https://carbondesignsystem.com/elements/spacing/overview/ — escala Carbon.
- Material (m2 e m3): abertas, mas sem conteúdo (página em JS).

**Fontes de terceiros**
- https://github.com/Instrument/instrument-sans — eixos, `tnum`, OFL.
- https://github.com/uswds/public-sans — origem, tabular, OFL, não mantida.
- https://github.com/IBM/plex — famílias, OFL.
- https://www.ibm.com/plex/ — personalidade, pouca informação técnica.
- https://github.com/adobe-fonts/source-sans — UI, OFL, VF.
- https://github.com/erikdkennedy/figtree — "friendly geometric", OFL.
- https://github.com/googlefonts/atkinson-hyperlegible-next — OFL 1.1.
- https://www.brailleinstitute.org/freefont/ — intenção de design, versões Next e Mono, EULA.
- https://vercel.com/font e https://github.com/vercel/geist-font — Geist Sans e Mono, OFL.
- https://github.com/JetBrains/JetBrainsMono — x-height, OFL.
- https://github.com/microsoft/cascadia-code — Code e Mono, vem com o Windows Terminal.
- Recursos OpenType listados no Fontsource: https://fontsource.org/fonts/{ibm-plex-sans, public-sans, source-sans-3, geist, manrope, ibm-plex-mono, figtree, atkinson-hyperlegible-next}/about.
- Não acessíveis: Google Fonts (vazio), ibm.com/design/language/typography/typeface (truncada), github.com/sharanda/manrope (404).

**Fontes do Windows**
- https://learn.microsoft.com/en-us/typography/font-list/bahnschrift — DIN 1451, eixos, pesos duplexados.
- https://learn.microsoft.com/en-us/windows/apps/design/signature-experiences/typography — Segoe UI Variable, rampa do Windows 11, sentence case, mínimos.
- https://learn.microsoft.com/en-us/typography/fonts/windows_11_font_list — fontes que vêm com o Windows 11.

**Marcas e Kanban**
- https://asana.com/inside-asana/teamwork-is-beautiful-introducing-asanas-new-look — coral e "bursts of color".
- https://linear.app/brand — azul dessaturado, uso monocromático preferido.
- https://confluence.atlassian.com/jirasoftwareserver/configuring-columns-938845277.html — limites de coluna (vermelho/amarelo, limite no topo).
- https://basecamp.com/about/press — aberta, sem informação de cor.
- Todoist: só resultado de busca, nenhuma página oficial aberta.

**Verificação local**
- Os contrastes foram calculados com a fórmula de luminância relativa da WCAG.
