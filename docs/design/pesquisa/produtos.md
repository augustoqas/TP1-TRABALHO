# Pesquisa: como produtos reais desenham quadros Kanban e métricas de fluxo

Abri todas as URLs da lista final com WebFetch. Algumas páginas não abriram: Mobbin (403), artigo de WIP da base de conhecimento do Businessmap (403), linear.app/docs/priority (404), artigo da Viget sobre o Card Table (429), basecamp.com/features/card-table (404) e o corpo do post da Atlassian "Priority icons are more accessible" (só o título carregou).

Quase tudo vem de documentação textual, não de screenshots, porque as galerias de imagens bloquearam o acesso. Por isso a **contagem exata de cores** em cada interface não dá para medir. Relato onde a cor aparece segundo a documentação, e marco o que é inferência ou conhecimento prévio.

---

## 1. Linear

- **Cor:** o redesenho de 2024 gera os temas a partir de só 3 variáveis (base, accent, contraste) em LCH, contra 98 valores por tema antes. Também reduziu de propósito o azul da marca para um visual "more neutral and timeless" [L1]. O refresh de 2026 trocou o cinza azulado por um cinza mais quente, menos saturado [L2]. A cor aparece de forma funcional:
  - status: cada status de workflow tem cor própria, editável [L3];
  - ícone de prazo: cinza normal, laranja se vence em até 1 semana, vermelho se vence hoje ou está atrasado [L3];
  - ícone de SLA: vai de cinza para amarelo, laranja e vermelho [L3];
  - bloqueios: flag laranja em "Blocked by" e vermelha em "Blocks" [L3].
- **Prioridade:** 5 níveis (No priority, Low, Medium, High, Urgent). Urgent também envia notificação ao responsável. "Sem prioridade" vai para o fim da ordenação [L3]. Um artigo de terceiros confirma "different icon shapes" por nível [L7]. Os glifos não estão descritos em nenhuma fonte que consegui abrir. Pelo meu conhecimento prévio, não verificado: barras tipo sinal de celular para Low/Medium/High e um quadrado com "!" para Urgent.
- **WIP:** a doc do board não fala de limite de WIP nem de contagem por coluna [L4].
- **Cartão:** a descrição nunca aparece no cartão. Se faltar espaço, nem todas as propriedades aparecem. O detalhe completo abre com "peek" (tecla Espaço) [L4]. As propriedades visíveis são escolhidas pelo usuário: ID, status, responsável, prioridade, prazo, estimativa, labels, "time in status" etc. [L5]. O cartão é um subconjunto configurável de campos, e o ID (ex.: ENG-123) é um deles.
- **Métricas:** o Insights fica num painel lateral direito (Cmd/Ctrl+Shift+I) ou em tela cheia. Cada medida mostra **gráfico + tabela**, ligados entre si no hover [L6]:
  - cycle time (início → conclusão) e lead time (criação → conclusão) são **scatterplots**;
  - no hover aparecem marcadores de percentil 25/50/75/95;
  - clicar num ponto abre a issue;
  - há também burn-up / CFD [L6].
- **Permissões:** os papéis são Owner, Admin, Member e Guest. Team owners controlam configurações do time [L8]. A doc **não descreve** como a UI comunica a falta de permissão [L8].
- **Tipografia e estrutura:** Inter Display nos títulos e Inter no resto [L1]. Dois princípios do refresh de 2026 [L2]:
  - "Don't compete for attention you haven't earned": a navegação recua (sidebar escurecida) e o conteúdo se destaca;
  - "Structure should be felt, not seen": bordas e separadores suavizados e reduzidos.
- **O que evita** [L2]:
  - excesso de bordas, que tinham "quietly proliferated";
  - ícones demais e decorativos (reduziram a quantidade e o tamanho);
  - fundos coloridos nos ícones de time;
  - cor saturada.

## 2. Jira (Cloud / Data Center)

- **Cor no cartão:**
  - a cor de cartão é **uma faixa fina à esquerda**, por tipo, prioridade, responsável ou JQL; cartão sem regra fica cinza [J1];
  - cartão sinalizado (flag/impedimento) fica **amarelo inteiro** [J2];
  - "Days in column" usa pontos que ficam vermelhos com o tempo, e 4 pontos vermelhos significam 20 dias ou mais [J1].
- **Prioridade:** fica na camada inferior do cartão, junto com tipo, responsável e estimativa. O resumo fica sempre no topo e cabem até 3 campos extras [J1]. Pelo resultado de busca do post da comunidade (corpo não aberto), os ícones antigos eram setas quase iguais que só mudavam de cor, e foram redesenhados para serem distinguíveis **sem depender de cor**.
- **WIP:** "column constraints" com mínimo e máximo.
  - Máximo excedido deixa o **cabeçalho da coluna vermelho**; mínimo não atingido deixa **amarelo** [J3]. No Data Center a coluna inteira muda de cor [J4].
  - Os valores ficam no topo da coluna, e ativar a regra também mostra a contagem [J4].
  - **Só aviso, não bloqueia.** Não muda o que é exibido e conta itens escondidos por filtro [J3][J4].
  - Só administrador do board ou do espaço configura [J3].
- **Métricas:**
  - **Control Chart** [J5]: um ponto por item (ou cluster), eixo Y em tempo decorrido. Tem linha azul de média móvel (janela de 20% dos itens, mínimo 5) e **faixa azul sombreada de desvio-padrão**. Escala linear abaixo de 30 dias e raiz cúbica acima. As colunas escolhidas definem se a medida é cycle time ou lead time.
  - **CFD** [J6]: área empilhada, uma cor por coluna. Banda que alarga indica gargalo. Período ajustável arrastando sobre o "Overview".
  - **Cycle time report** [J7]: usa **mediana** semanal comparada às 12 semanas anteriores, mais scatterplot da semana e tabela de itens. Fica em Reports.
- **Permissões:** quando falta transição ou permissão, o arrastar falha ou o cartão volta ao lugar (relatos da comunidade em resultados de busca; o thread aberto [J8] só trouxe o título). Não achei doc oficial sobre ação escondida vs. desabilitada.
- **O que evita:** pouco documentado. Os limites de 3 campos extras por cartão e da faixa de cor estreita mostram contenção [J1].

## 3. Trello

- **Cor:** é o produto mais colorido.
  - **30 cores de label** (eram 10). O texto fica sobre um tom mais claro para legibilidade, e há modo daltônico que acrescenta padrão/textura à cor [T1].
  - O selo de prazo segue uma escala de cor: cinza se faltam mais de 24 h, amarelo se vence em até 24 h, vermelho se atrasou há menos de 24 h, rosa claro se atrasou há mais, verde se concluído [T2].
- **Prioridade:** não existe campo nativo. Usa-se label colorida (inferência a partir de [T1]).
- **WIP:** pelo Power-Up "List Limits", que mostra a contagem por lista e **pinta a lista inteira de amarelo** ao exceder. É aviso, não bloqueio [T3].
- **Cartão:** selos de descrição, comentários, anexos e checklist (a contagem fica verde quando a checklist está completa) e prazo. Esses ícones padrão **não podem ser ocultados**, e há pedido de usuários para isso (visto só em resultado de busca, jira.atlassian.com/browse/TRELLO-630).
- **Métricas:** não há métricas de fluxo nativas.
- **Permissões:** o papel **Observer** vê, comenta e vota, mas não move nem edita cartões, não marca checklist e não muda configurações [T4]. A doc não diz como a UI comunica isso.
- **O que evita:** quase nada. É a referência do "muito badge" e do excesso de cor.

## 4. GitHub Projects

- **Cor:** o design system Primer usa cor **por papel semântico**: accent (links/foco), success, attention (avisos e trabalho em andamento), danger, e open/closed/done para estados. Os neutros cuidam da estrutura (fundos nos passos 1–6, bordas nos passos 7–8, texto nos passos 9–10) [G1].
- **Prioridade:** não há campo nativo. É um single-select criado pelo time (inferência a partir de [G2]: as colunas vêm de um campo single-select ou iteration).
- **WIP:** limite por coluna e por view, definido no menu "…" da coluna.
  - **Contagem e limite aparecem no topo da coluna**, e o valor fica **destacado em vermelho** quando excede [G2][G3].
  - **Não impede** pessoas nem automações de adicionar cartões [G2].
  - Somas de campos numéricos também aparecem no topo da coluna [G2].
- **Cartão:** os campos visíveis são escolhidos no menu da view [G2].
- **Métricas:** os Insights têm gráficos "current" (estado atual) e "historical" (no tempo). O padrão é um burn-up com Open, Completed e Not planned. **Não há cycle time** [G4].
- **Permissões:** papéis de projeto No access, Read, Write e Admin. Read só vê, Write vê e edita, Admin também adiciona colaboradores [G5].

## 5. Plane

- **Prioridade:** Urgent, High, Medium, Low e None. Dá para agrupar e ordenar o board por prioridade [P1].
- **Cartão:** muitas propriedades liga/desliga: ID, tipo, responsável, datas, labels, prioridade, estado, contagem de sub-itens e anexos, estimativa etc. Há opção "Show empty groups" [P1].
- **WIP:** a doc não menciona [P1].
- **Métricas:** cada aba abre com **totais em números** (todos, iniciados, backlog, não iniciados, concluídos). Depois vêm gráficos de barras, área (criados vs. resolvidos por mês), scatter, linha e radar, com breakdown por prioridade em **gráfico + tabela** exportável para CSV [P2].
  - **Não há cycle time nem lead time** [P2].
  - Analytics do projeto só para Admin e Member; exportar só para Admin [P2].
- **Permissões:** papéis Admin, Member, Contributor, Commenter e Guest [P3].
  - Commenter vê e comenta, mas não cria nem edita [P3].
  - Guest de projeto nem vê work items [P3].
  - O único comportamento de UI documentado é que, sem permissão, "Members see no project creation option": a ação é **escondida** [P3].

## 6. Basecamp (Card Table)

- **Estrutura:** "Triage" no topo para entrada de trabalho, coluna especial "Not Now", colunas do fluxo e "Done". Cada coluna pode ganhar uma seção **"On Hold"** para trabalho bloqueado, que continua no contexto da coluna [B1]. As colunas têm **cor customizável** [B1]. O cartão é uma página com discussão, arquivos e "steps" atribuíveis a pessoas [B1][B2].
- **WIP:** não encontrei limite de WIP em nenhuma fonte aberta.
- **Densidade:** no celular, o time decidiu dar mais espaço ao Triage, porque decidir o que entra no fluxo é a tarefa mais difícil, e quis "remove all unnecessary distractions". Expandir o Triage colapsa as colunas do fluxo, e vice-versa. Estados vazios e menus de contexto foram cortados da v1 onde não eram essenciais [B3].
- **Métricas:** nenhuma métrica de fluxo encontrada.
- **O que evita:** o produto se posiciona contra sistemas "bloated, complicated, and confusing" [B4].

## 7. Businessmap (Kanbanize) — referência de métricas de fluxo maduras

- **WIP:** o topo da coluna fica vermelho quando o limite é excedido. A recomendação é só exceder com motivo, e **registrar o motivo** para o time ver. Também existem limites de WIP **por pessoa** [K1]. A página da base de conhecimento sobre os comportamentos "Always allow" e similares deu 403. Pelo resultado de busca, "Always allow" mantém o limite como indicador, com o cabeçalho vermelho.
- **Métricas:**
  - **scatter de cycle time** com um ponto por cartão, eixo X na data de conclusão e Y em dias, cor por tipo de trabalho e **linhas de percentil**, lidas como "X% de chance de ficar pronto em N dias" [K2];
  - filtros por tipo, prioridade e responsável [K2];
  - também CFD e heat map de cycle time [K3].

---

## Tabela comparativa

| Produto | Cores (estado normal) | Prioridade | Limite de WIP | Métricas | Permissões |
|---|---|---|---|---|---|
| Linear | Base neutra (3 variáveis de tema), acento azul contido; cor só em status, prazo, SLA e bloqueio [L1][L2][L3] | 5 níveis com ícone de formas distintas [L3][L7] | Não documentado [L4] | Painel lateral; scatter de cycle e lead time com percentis + tabela; burn-up/CFD [L6] | Papéis documentados; comunicação na UI não documentada [L8] |
| Jira | Faixa fina à esquerda do cartão; amarelo para flag; pontos vermelhos de "dias na coluna" [J1][J2] | Ícone na base do cartão, redesenhado para não depender de cor (busca) [J1] | Mín./máx. no topo; cabeçalho vermelho (excedeu) ou amarelo (abaixo do mínimo); só aviso [J3][J4] | Control Chart (média móvel + faixa de desvio-padrão), CFD, relatório de cycle time com mediana [J5][J6][J7] | Só admin configura colunas e limites [J3] |
| Trello | Muitas: 30 cores de label, escala de cor no prazo, modo daltônico [T1][T2] | Sem campo nativo (usa label) | Power-Up: lista inteira amarela; só aviso [T3] | Nenhuma nativa | Observer só vê e comenta [T4] |
| GitHub Projects | Cor semântica por papel (Primer); neutros para a estrutura [G1] | Sem campo nativo (single-select) [G2] | "contagem/limite" no topo; número vermelho; não bloqueia [G2][G3] | Burn-up e gráficos atuais/históricos; sem cycle time [G4] | Read / Write / Admin [G5] |
| Plane | Não verificado | Urgent…None; agrupa por prioridade [P1] | Não documentado | Totais em números + gráficos + tabela; sem cycle time [P2] | Commenter só lê e comenta; ação escondida sem permissão [P3] |
| Basecamp | Cor por coluna (customizável) [B1] | Não existe | Não existe; usa "On Hold" e "Not Now" [B1] | Nenhuma | Não verificado |
| Businessmap | Não verificado | Usada como filtro nos gráficos [K2] | Topo vermelho; motivo registrado; limite por pessoa [K1] | Scatter com percentis, CFD, heat map [K2][K3] | Não verificado |

---

## Princípios práticos para o redesenho

1. **Base neutra, cor só com significado.** Fundo, bordas e texto em escala de cinza. Reserve cor para estados que pedem ação: limite de WIP estourado, item parado, prazo. *Fontes:* Linear [L1][L2], Primer [G1].
2. **Prioridade por forma + texto, nunca só por cor.** ALTA/MEDIA/BAIXA com ícone de formas distintas (ex.: 3/2/1 barras) e rótulo em tooltip ou texto. Só ALTA pode ganhar cor de destaque. *Fontes:* Atlassian Lozenge, "Don't use color alone" [J9]; redesenho dos ícones do Jira (busca); formas distintas no Linear [L7].
3. **WIP como "n/limite" no cabeçalho da coluna**, por exemplo "FAZENDO 3/3". Ao exceder, pinte só o número ou o cabeçalho, não a coluna inteira. *Fontes:* GitHub [G2][G3], Jira Cloud [J3].
4. **Limite de WIP que avisa em vez de travar, e que só o gestor contorna.** Os grandes produtos não bloqueiam. Se a regra do sistema exigir bloqueio, explique o motivo no ponto da ação. Se permitir exceder, peça e registre o motivo. *Fontes:* Jira [J3], GitHub [G2], Businessmap [K1].
5. **Cartão enxuto, com 3 a 4 dados.** Título no topo; numa linha inferior, ID curto, ícone de prioridade, tamanho (P/M/G) e avatar ou iniciais. Detalhes e timestamps ficam no painel de detalhe. *Fontes:* Jira, resumo + 3 campos + camada inferior [J1]; Linear, descrição fora do cartão e "peek" [L4].
6. **Mostrar o envelhecimento no próprio cartão.** Um indicador discreto de tempo em FAZENDO aproxima a métrica de cycle time do quadro. *Fontes:* "Days in column" do Jira [J1]; "time in status" do Linear [L5].
7. **Métricas = número-resumo + gráfico + tabela ligados.** Comece por números grandes: mediana e média de cycle time, mediana e média de lead time, WIP atual. Abaixo, um scatter (um ponto por cartão) e uma tabela de cartões. Clicar num ponto abre o cartão. *Fontes:* Linear Insights [L6], totais no topo do Plane [P2], cycle time report do Jira [J7].
8. **Prefira mediana e percentis à média isolada.** Mostre a média, mas destaque a mediana e algo como "85% em até N dias", que é mais honesto com outliers. *Fontes:* percentis do Linear [L6], mediana do Jira [J7], percentis do Businessmap [K2], média móvel com desvio-padrão do Jira [J5].
9. **Para quem não é gestor, esconder as ações de edição.** Um DESENVOLVEDOR não precisa ver botões de editar projeto, quadro ou cartão. Um aviso curto e discreto ("Somente o gestor do projeto pode alterar este quadro") evita a frustração de tentar e falhar. *Fontes:* Plane, "Members see no project creation option" [P3]; Trello e GitHub, papéis só-leitura [T4][G5]; relatos de drag-and-drop falhando sem explicação no Jira [J8].
10. **Estrutura sentida, não desenhada.** Poucas bordas, separação por espaço e fundos levemente diferentes. Sem sombras pesadas e sem ícones decorativos. *Fontes:* Linear 2026 [L2].
11. **Sem CAIXA ALTA em rótulos e selos.** Use só a primeira letra maiúscula e mostre "Alta", "Fazendo" na UI, mesmo que o enum seja ALTA ou FAZENDO. *Fonte:* Atlassian Lozenge, "Don't use title case capitalization or all caps" [J9].
12. **Estados vazios e colunas com intenção.** Coluna vazia com uma frase curta, sem ilustração. Projeto sem quadros mostra a próxima ação só para o gestor. A coluna mais "decisória" pode ganhar destaque. *Fontes:* foco no Triage do Basecamp [B3]; "Show empty groups" no Linear [L4] e no Plane [P1].

---

## Referências (abertas de fato)

- [L1] https://linear.app/now/how-we-redesigned-the-linear-ui — temas em LCH com 3 variáveis, azul contido, Inter Display / Inter, menos ruído visual.
- [L2] https://linear.app/now/behind-the-latest-design-refresh — "Don't compete for attention…", "Structure should be felt, not seen", menos bordas e ícones, cinza mais quente.
- (também aberta) https://linear.app/changelog/2026-03-12-ui-refresh — ícones redesenhados, sidebar mais escura.
- [L3] https://linear.app/docs/issue-property — 5 níveis de prioridade; cores de prazo, SLA e flags; cor por status.
- [L4] https://linear.app/docs/board-layout — descrição fora do cartão, peek, agrupamentos, "Show empty groups"; sem WIP.
- [L5] https://linear.app/docs/display-options.md — propriedades exibíveis no cartão (ID, prioridade, time in status…).
- [L6] https://linear.app/docs/insights — scatter de cycle e lead time, percentis 25/50/75/95, gráfico + tabela, painel lateral.
- [L7] https://screensdesign.com/articles/mobile-task-priority-picker-ux/ — Linear usa "different icon shapes" por prioridade.
- [L8] https://linear.app/docs/members-roles.md — papéis; nada sobre como a UI sinaliza falta de permissão.
- (também abertas, sem detalhe visual) https://linear.app/docs/project-priority.md, https://linear.app/changelog/2024-07-25-priority-for-projects-and-micro-adjust, https://linear.app/now/a-design-reset.
- [J1] https://support.atlassian.com/jira-software-cloud/docs/customize-cards — faixa de cor à esquerda, resumo + 3 campos, Days in column com pontos vermelhos.
- [J2] https://support.atlassian.com/jira-software-cloud/docs/flag-an-issue/ — cartão sinalizado fica amarelo.
- [J3] https://support.atlassian.com/jira-software-cloud/docs/configure-columns/ — cabeçalho vermelho ou amarelo, só visual, só admin configura.
- [J4] https://confluence.atlassian.com/jirasoftwareserver/configuring-columns-938845277.html — versão Data Center: coluna muda de cor, contagem exibida.
- [J5] https://support.atlassian.com/jira-software-cloud/docs/view-and-understand-the-control-chart/ — anatomia do Control Chart.
- [J6] https://support.atlassian.com/jira-software-cloud/docs/view-and-understand-the-cumulative-flow-diagram/ — CFD e leitura de gargalo.
- [J7] https://support.atlassian.com/jira-software-cloud/docs/view-and-understand-your-cycle-time-report — mediana semanal, scatter, tabela.
- [J8] https://community.atlassian.com/forums/Jira-questions/I-don-t-get-any-blue-slots-where-I-can-drag-a-card-from-one/qaq-p/827543 — só o título carregou.
- [J9] https://atlassian.design/components/lozenge/usage — sem caixa alta, não usar só cor, lozenge para status e prioridade.
- (também abertas, pouco conteúdo) https://support.atlassian.com/jira/kb/columns-are-greyed-out-in-the-board-team-managed-project/ (bug de workflow, não permissão); https://community.atlassian.com/t5/Jira-articles/Priority-icons-are-more-accessible/ba-p/1732935 (só o título); https://community.atlassian.com/forums/Jira-questions/WIP-Limits-not-highlighting-when-over/qaq-p/2790509 (só o título).
- [T1] https://www.atlassian.com/blog/trello/20-new-trello-label-colors — 30 cores de label, modo daltônico.
- [T2] https://support.atlassian.com/trello/docs/due-soon — escala de cor do selo de prazo.
- [T3] https://trello.substack.com/p/counting-cards-trello-style — List Limits: contagem e lista amarela.
- [T4] https://support.atlassian.com/trello/docs/adding-observers-to-boards/ — o que o Observer pode e não pode fazer.
- (também aberta, sem detalhe de cartão) https://trello.com/guide/trello-101.
- [G1] https://primer.style/product/getting-started/foundations/color-usage/ — cor semântica por papel, escala de neutros.
- [G2] https://docs.github.com/en/issues/planning-and-tracking-with-projects/customizing-views-in-your-project/customizing-the-board-layout — limite no topo, destaque, não bloqueia, somas.
- [G3] https://github.blog/changelog/2023-06-15-github-issues-projects-june-15th-update/ — "value will be highlighted in red".
- [G4] https://docs.github.com/en/issues/planning-and-tracking-with-projects/viewing-insights-from-your-project/about-insights-for-projects — gráficos current e historical, burn-up.
- [G5] https://docs.github.com/en/issues/planning-and-tracking-with-projects/managing-your-project/managing-access-to-your-projects — Read / Write / Admin.
- [P1] https://docs.plane.so/core-concepts/issues/display-options — propriedades, agrupamento, empty groups.
- [P2] https://docs.plane.so/core-concepts/analytics — totais, gráficos, tabela, papéis; sem cycle time.
- [P3] https://docs.plane.so/workspaces-and-users/roles — papéis; ação escondida sem permissão.
- [B1] https://updates.37signals.com/post/new-in-basecamp-card-tables — Triage, Not Now, Done, On Hold, cor de coluna.
- [B2] https://updates.37signals.com/post/new-in-basecamp-steps-for-cards — steps atribuíveis.
- [B3] https://dev.37signals.com/bringing-card-table-to-the-small-screen/ — foco no Triage, colapso de colunas, cortes da v1.
- [B4] https://basecamp.com/articles/why-your-projects-need-a-kanban-board — posicionamento contra o "bloated".
- [K1] https://businessmap.io/limiting-work-in-progress-flexible — topo vermelho, motivo ao exceder, WIP por pessoa.
- [K2] https://businessmap.io/kanban-resources/kanban-analytics/cycle-time-scatter-plot — scatter com linhas de percentil.
- [K3] https://businessmap.io/kanban-resources/kanban-software/kanban-lead-cycle-time — definições, CFD, heat map.
