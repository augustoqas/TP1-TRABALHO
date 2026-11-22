# Justificativas do redesenho (v2)

Formato: **o que mudou** · por quê · referência. As URLs vêm de [pesquisa/produtos.md](pesquisa/produtos.md) e [pesquisa/fundamentos.md](pesquisa/fundamentos.md) (só páginas abertas de fato).

## Identidade

1. **Ameixa (#6E2F63) no lugar do azul SaaS.** É o matiz mais distante de vermelho, âmbar e verde, então a marca nunca se confunde com um estado. Neutros levemente tingidos de ameixa dão unidade sem acrescentar cor. Ref.: Atlassian, papéis de cor (https://atlassian.design/foundations/color); "Greys don't have to be grey" (https://www.refactoringui.com/, https://digitalsynopsis.com/design/useful-ui-ux-design-tips/).
2. **Cor só com significado.** Na tela do quadro só há duas cores fora dos neutros e da marca: o alerta da coluna cheia e a prioridade Alta. Assim o alerta é lido de primeira. Ref.: Linear, "Don't compete for attention you haven't earned" (https://linear.app/now/behind-the-latest-design-refresh); Primer, cor por papel (https://primer.style/product/getting-started/foundations/color-usage/); Von Restorff (https://lawsofux.com/von-restorff-effect/).
3. **Bahnschrift nos títulos e números.** É uma DIN de sinalização instalada no Windows, com pesos de largura igual e `tnum`. Dá identidade de "quadro" sem baixar fonte, e os números de métrica e o "3/3" não pulam. Ref.: https://learn.microsoft.com/en-us/typography/font-list/bahnschrift; https://developer.mozilla.org/en-US/docs/Web/CSS/font-variant-numeric.
4. **Marca desenhada com função:** três colunas (vazia, meio cheia, cheia), que são o próprio fluxo A fazer → Fazendo → Feito. Substitui o logotipo genérico em quadrado azul.

## Login

5. **Sem painel dividido, sem ilustração.** Uma coluna centrada: marca em 56 px, uma frase, dois campos, um botão. A identidade vem da tipografia e da cor da marca. Ref.: Lei de Hick, uma ação primária (https://lawsofux.com/hicks-law/); carga cognitiva (https://lawsofux.com/cognitive-load/).
6. **Removidos o divisor "ou", "Lembrar meu e-mail", o olho, os ícones nos campos e a nota sobre papéis.** Nenhum corresponde a um recurso do sistema. O foco no campo de senha usa anel brand de 2 px (≥ 3:1). Ref.: WCAG 1.4.11 (https://www.w3.org/WAI/WCAG22/Understanding/non-text-contrast.html).

## Estrutura geral

7. **Sem sidebar, sem barra de título falsa e sem rodapé de status.** Uma barra de 48 px com marca, Projetos, Pessoas e usuário. Libera ≈ 220 px de largura para o quadro, e quadro e cartão passam a ser navegados dentro do projeto. Ref.: Linear, "Structure should be felt, not seen" (https://linear.app/now/behind-the-latest-design-refresh).
8. **Contexto dito uma vez.** O breadcrumb mostra só o pai (PRJ001 Portal Aluno › QDR001) e o título mostra o atual. Saíram o bloco "Projeto atual" e o subtítulo repetido. Ref.: Proximidade e carga cognitiva (https://lawsofux.com/law-of-proximity/, https://lawsofux.com/cognitive-load/).
9. **Menos caixas.** Só a coluna (região comum) e o cartão (objeto físico do Kanban) têm forma. Métricas, dados e linha do tempo ficam soltos na página e se separam por espaço. Raio varia por função (6 no cartão, 12 na coluna) e sombra só no cartão arrastado. Ref.: "Use fewer borders" (https://www.refactoringui.com/); Região Comum (https://lawsofux.com/law-of-common-region/); escala de raios do Primer (https://primer.style/foundations/primitives/size).
10. **Sentence case em toda a interface.** "A fazer", "Tempo de ciclo médio", sem caixa alta espaçada. Ref.: Atlassian Lozenge (https://atlassian.design/components/lozenge/usage); rampa do Windows 11 (https://learn.microsoft.com/en-us/windows/apps/design/signature-experiences/typography); Butterick sobre caixa alta (https://practicaltypography.com/all-caps.html).

## Quadro (tela principal)

11. **WIP comunicado pelo estado da coluna.** "Fazendo 3/3" no cabeçalho, com fundo de alerta, linha inferior, ícone e "No limite". Saíram o card de métrica de WIP, a pílula e o tooltip: um lugar, quatro pistas. Ref.: GitHub Projects, limite no topo da coluna (https://docs.github.com/en/issues/planning-and-tracking-with-projects/customizing-views-in-your-project/customizing-the-board-layout); Jira, cabeçalho colorido (https://support.atlassian.com/jira-software-cloud/docs/configure-columns/); WCAG 1.4.1 (https://www.w3.org/WAI/WCAG22/Understanding/use-of-color.html).
12. **Bloqueio mostrado no momento da ação.** O mockup congela o arrasto de CRT007 sobre Fazendo. O alvo de soltar aparece hachurado e tracejado, com "Sem vaga: limite de 3 cartões" e cursor de bloqueio, e a origem em A fazer fica como slot vazio. Diferente do Jira e do GitHub, o Fluxo **recusa** a entrada, e a interface mostra isso no ponto da ação em vez de num banner. Ref.: Lei de Tesler (https://lawsofux.com/teslers-law/); princípio 4 da pesquisa de produtos; Fitts, a coluna inteira como alvo (https://lawsofux.com/fittss-law/).
13. **Sem "Mover →" em cada cartão.** Mover é arrastar (ou usar o teclado no cartão focado). Nove botões iguais viravam textura e, desabilitados, pediam tooltip. Ref.: carga cognitiva (https://lawsofux.com/cognitive-load/); Jakob, convenção de arrastar em Kanban (https://lawsofux.com/jakobs-law/).
14. **Cartão com 4 dados:** nome, identificador, prioridade e desenvolvedor. Descrição, tamanho e timestamps vão para o detalhe. Ref.: Jira, resumo + até 3 campos (https://support.atlassian.com/jira-software-cloud/docs/customize-cards); Linear, descrição fora do cartão (https://linear.app/docs/board-layout).
15. **Prioridade por forma + texto.** Três, duas ou uma barra mais o rótulo, e só Alta em vermelho. Saíram as pílulas coloridas para todos os níveis. Ref.: Atlassian, "Don't use color alone" (https://atlassian.design/components/lozenge/usage); formas distintas do Linear (https://screensdesign.com/articles/mobile-task-priority-picker-ux/).
16. **Métricas como números, não cards.** "Tempo de ciclo médio 1d 22h 0min" e "Lead time médio 4d 11h 55min" em Bahnschrift 32, com rótulo pequeno acima e a base do cálculo ("Média dos 3 cartões em Feito"). Sem ícone em quadradinho. Ref.: "Labels are a last resort" (https://www.refactoringui.com/previews/labels-are-a-last-resort); tokens de métrica da Atlassian (https://atlassian.design/foundations/typography); totais no topo do Plane (https://docs.plane.so/core-concepts/analytics).
17. **Gestor vê suas ações no contexto.** "Editar quadro" ao lado do título e "Novo cartão" no cabeçalho de A fazer, que é onde o cartão nasce. Excluir fica dentro de Editar. Ref.: Proximidade (https://lawsofux.com/law-of-proximity/).
18. **Feito sem verde e avatares sem cor.** A posição na coluna já diz o estado, e a cor por pessoa não significa nada. Ref.: Asana, "bursts of color" só em conclusão pontual (https://asana.com/inside-asana/teamwork-is-beautiful-introducing-asanas-new-look); Primer (https://primer.style/product/getting-started/foundations/color-usage/).

## Detalhe do cartão

19. **Linha do tempo proporcional com as definições desenhadas.** A distância entre os pontos segue o tempo real (45 % de espera, 55 % de trabalho). Chaves marcam "Tempo de ciclo" (início → término) e "Lead time" (entrada → término), e quem vê entende a métrica sem legenda. Ref.: Linear Insights, cycle × lead (https://linear.app/docs/insights); definições do Businessmap (https://businessmap.io/kanban-resources/kanban-software/kanban-lead-cycle-time).
20. **Comparação com a média sem chip colorido.** Número grande do cartão, duas barras neutras na mesma escala (cartão × média do quadro) com o valor escrito e a frase "10h 30min acima da média do quadro." Acima e abaixo da média não são bom nem ruim por si, por isso não ganham verde ou vermelho. Ref.: WCAG 1.4.1 (https://www.w3.org/WAI/WCAG22/Understanding/use-of-color.html); mediana e média como referência (https://support.atlassian.com/jira-software-cloud/docs/view-and-understand-your-cycle-time-report).
21. **Todos os campos, só os que existem.** Identificador, nome, descrição, prioridade, estado, tamanho, desenvolvedor (com email), quadro e os três timestamps. Saíram "criado por", "movido por" e o aviso "Estado final", porque o check e a falta de ação de mover já dizem isso.

## Projetos (visão do desenvolvedor)

22. **Ações de edição ausentes, não desabilitadas.** Lucas Pereira (desenvolvedor) não vê "Novo projeto", "Editar", "Excluir" nem "Novo quadro". Também saíram o banner de papel e o tooltip "Ação restrita". Ref.: Plane, "Members see no project creation option" (https://docs.plane.so/workspaces-and-users/roles); Lei de Hick (https://lawsofux.com/hicks-law/).
23. **A regra se explica pelo dado.** "Gestor do projeto: Ana Ribeiro · ana.ribeiro@unb.br" fica ao lado de início e término. Quem não é o gestor entende por que não há edição e sabe a quem pedir, sem o rodapé "fale com a gestora". O rótulo é o nome do atributo (como "Desenvolvedor" no cartão), porque o sistema não guarda gênero.
24. **Lista mostra identificadores; detalhe mostra tudo.** Na lista, cada linha traz identificador + nome. A seleção tem fundo, barra lateral e peso (não só cor). Na tabela de quadros, "Fazendo / limite de WIP" usa a mesma convenção do quadro (3/3 + "No limite"), e a linha inteira abre o quadro, sem "Abrir quadro →" repetido. Ref.: Similaridade e Jakob (https://lawsofux.com/law-of-similarity/, https://lawsofux.com/jakobs-law/).

## Acessibilidade e dados

25. **Contrastes calculados.** Todo texto fica ≥ 4,5:1. Metadado em ink-500 dá 5,14:1 no branco e 4,59:1 na coluna, e sobre brand-50 troca para ink-600 (6,75:1). A borda de campo ink-400 dá 3,65:1. Ref.: WCAG 1.4.3 e 1.4.11 (https://www.w3.org/WAI/WCAG22/Understanding/contrast-minimum.html, https://www.w3.org/WAI/WCAG22/Understanding/non-text-contrast.html).
26. **Rótulo de exibição ≠ valor armazenado.** O enum continua `MEDIA`, `MEDIO`, `A FAZER` no sistema. A interface mostra "Média", "Médio", "A fazer", porque é texto para pessoas: acento correto e sentence case. O mapeamento é fixo e não altera nenhum dado. Ref.: Atlassian Lozenge (https://atlassian.design/components/lozenge/usage); Windows 11, sentence case (https://learn.microsoft.com/en-us/windows/apps/design/signature-experiences/typography).
27. **Dados de exemplo válidos e coerentes.** Nomes com até 15 letras/espaço e sem acento ("Portal Aluno", "Testes de login"), descrições com até 30 caracteres terminadas em ponto, IDs AAA999, timestamps `DD-MES-AAAA-HH:MM`. As médias batem com os cartões em Feito:
    - CRT001 (02-MAR-2026-09:30 → 03-MAR-2026-10:00 → 05-MAR-2026-16:00): ciclo 2d 6h 0min, lead 3d 6h 30min;
    - CRT002 (03-MAR-2026-10:15 → 05-MAR-2026-09:00 → 07-MAR-2026-17:30): ciclo 2d 8h 30min, lead 4d 7h 15min;
    - CRT003 (04-MAR-2026-14:00 → 09-MAR-2026-08:30 → 10-MAR-2026-12:00): ciclo 1d 3h 30min, lead 5d 22h 0min;
    - médias: 1d 22h 0min e 4d 11h 55min.
