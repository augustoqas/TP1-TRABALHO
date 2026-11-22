# Crítica da versão anterior (v1)

Telas analisadas: a primeira versão dos mockups (não versionada no repositório).

"Design de IA" aqui quer dizer escolhas que aparecem por padrão de template, sem decisão: elas preenchem a tela, mas não ajudam a pessoa a entender o sistema. Para cada problema: o que é e por que é vício.

## Identidade e cor

1. **Paleta padrão sem identidade.** Azul-marinho na sidebar, azul vivo no botão e badges pastéis (roxo, verde, amarelo, cinza). É a paleta de qualquer template SaaS de Tailwind (ver item 4). *Vício:* a cor foi escolhida por costume, não por significado. Nada distingue o Fluxo de outro painel.
2. **Cor sem significado.** Badge roxo para GESTOR, azul-claro para DESENVOLVEDOR, verde para FEITO, amarelo para MÉDIA, ícones de métrica em lilás e azul. *Vício:* quando tudo tem cor, a cor deixa de avisar. O único estado que pede atenção (WIP no limite) compete com uma dúzia de manchas coloridas (Von Restorff perde força).
3. **Avatares com cor aleatória** (AR rosa, LP azul, RS verde, BL amarelo, MC lilás). *Vício:* a cor sugere uma categoria que não existe. É ruído decorativo copiado de geradores de avatar.
4. **Cinzas de template.** Os `.html` usam literalmente a escala slate do Tailwind (#0f172a, #334155, #64748b, #94a3b8, #e2e8f0) e o azul #2563eb. O texto secundário em #94a3b8 (rodapé, legendas, "Fluxo v1.0", "ou") dá 2,56:1 no branco e reprova a WCAG 1.4.3. *Vício:* contraste e paleta herdados do template, sem conta.

## Excesso de elementos

5. **Pílulas e badges em tudo.** Prioridade, tamanho, estado, papel, contagem de coluna, "Limite de WIP: 3", "3 A FAZER / 3 FAZENDO / 3 FEITO". *Vício:* a mesma forma (pílula colorida) para coisas diferentes apaga a diferença entre elas (Lei da Similaridade).
6. **Rótulos em CAIXA ALTA espaçada** ("NAVEGAÇÃO", "PROJETO ATUAL", "QUADRO", "ENTRADA · A FAZER", "MÉDIO", "ALTA"). *Vício:* é o atalho visual mais comum para "parecer organizado". Caixa alta lê mais devagar e, em todo lugar, não hierarquiza nada.
7. **Tudo em cards brancos iguais**, mesmo raio e mesma sombra: métricas, linha do tempo, dados do cartão, lista e detalhe de projeto, cada campo do projeto. *Vício:* a caixa substitui a decisão de agrupar por espaço. A tela vira uma grade de retângulos sem hierarquia.
8. **Ícones dentro de quadradinhos coloridos** nos cards de métricas (relógio, seta, alerta). *Vício:* é o padrão "dashboard genérico". O ícone não acrescenta informação ao rótulo "Tempo de ciclo médio".
9. **Faixa colorida na borda dos cartões e no topo das colunas** (cinza, laranja e verde por estado). *Vício:* repete com cor o que a coluna já diz pela posição.
10. **"Mover →" repetido em todo cartão.** Nove botões iguais na tela. *Vício:* ação repetida vira textura e desperdiça espaço. Nos cartões de A FAZER o botão aparecia desabilitado, o que exigia tooltip para explicar.
11. **Contadores inúteis na sidebar** (Pessoas 6, Projetos 4, Quadros 3, Cartões 9). *Vício:* número exibido porque "dashboards têm números", sem decisão que dependa dele.

## Redundância

12. **Contexto repetido três vezes:** breadcrumb + título com subtítulo "Projeto PRJ001 · Portal do Aluno" + bloco "PROJETO ATUAL" na sidebar. E a barra de título da janela repetia de novo. *Vício:* falta de confiança na própria hierarquia.
13. **Alerta de WIP triplicado:** card de métrica vermelho "3/3 limite atingido" + badge "3/3" no cabeçalho + pílula "Limite atingido" + tooltip. *Vício:* gritar o mesmo aviso em quatro lugares não o deixa mais claro. Ensina a ignorá-lo.
14. **Rodapé/barra de status** com "Conectado como…", contagem de cartões e data. *Vício:* tudo isso já estava no topo da tela. É enchimento herdado de apps desktop antigos.
15. **Barra de título de janela falsa** (ícone, título, minimizar, maximizar, fechar). *Vício:* encena um "app" desenhando cromo do sistema operacional, gasta 32 px e não é design da interface.
16. **Card "Cartões no quadro 9" com 3 pílulas por estado.** Repete as contagens dos cabeçalhos das colunas logo abaixo.

## Explicar em vez de mostrar

17. **Tooltips e banners explicando regras.** "Limite de WIP atingido… Conclua um cartão em FAZENDO", "Você está conectado como DESENVOLVEDOR: pode consultar…", "Ação restrita ao GESTOR", "Novo quadro: somente GESTOR", "Estado final — não pode mais ser movido", "Precisa de alguma alteração? Fale com a gestora". *Vício:* a interface descreve as regras em vez de comunicá-las pelo estado (Lei de Tesler e carga cognitiva extrínseca).
18. **Ações desabilitadas com tooltip para o desenvolvedor** (Editar, Excluir, Criar projeto). *Vício:* mostra ao usuário o que ele não pode fazer e depois pede desculpas. Produtos como o Plane simplesmente não exibem a ação.

## Login

19. **Tela dividida ao meio com painel escuro e ilustração genérica** de mini-quadro. *Vício:* é o layout de login mais repetido da web. Metade da tela não serve à tarefa (entrar).
20. **Divisor "ou" sem segunda opção.** *Vício:* elemento copiado de logins com "Entrar com Google". Aqui separa nada de nada.
21. **"Lembrar meu e-mail" e ícone de olho decorativos**, além de ícones de envelope e cadeado dentro dos campos. *Vício:* recursos que o sistema não tem, colocados porque "logins têm isso".
22. **Nota sobre papéis GESTOR/DESENVOLVEDOR no login.** Explica uma regra de cadastro na tela de entrada.
23. **Seta decorativa no botão "Entrar →".**

## Tipografia

24. **Hierarquia tipográfica fraca.** Título, números de métrica e nomes em pesos e tamanhos parecidos, tudo na mesma família. Os números de métrica não usavam algarismos tabulares. *Vício:* sem contraste de escala e peso, o olho não sabe por onde começar, e as caixas tentam compensar.

## Dados de exemplo que violam o próprio sistema

25. **Nomes fora do formato** (até 15 letras e espaço): "Sprint 3 – Backend" (dígito e travessão), "Autenticação com e-mail e senha" (acento, hífen, 31 caracteres), "API de matrícula em disciplinas", "Exportar histórico em PDF", "Sistema de Estágios", "App da Biblioteca" (17), "Ouvidoria Online" (16).
26. **Descrições fora do formato** (até 30 caracteres): "Login por e-mail e senha com bloqueio após 5 tentativas." (56 caracteres, com acento e hífen); "Portal web de matrículas." com acento.
27. **Valores de domínio com acento** nos badges: "MÉDIO", "MÉDIA". O sistema guarda MEDIO e MEDIA. A v1 misturou rótulo e valor sem critério, e ainda em caixa alta.
28. **Dados que o sistema não guarda:** "Criado por Mariana Costa", "Movido por Rafael Souza", "Projeto em andamento", "120 dias de duração", "25/30 caracteres". *Vício:* inventar riqueza para a tela parecer cheia. O professor vai perguntar onde esse dado está no modelo.
29. **Datas sem formato do sistema na lista** ("02-MAR-2026 → 30-JUN-2026", sem hora) e com seta decorativa.

## Outros

30. **Sidebar espelhando as tabelas do banco** (Pessoas, Projetos, Quadros, Cartões). Quadro e cartão não existem fora de um projeto. Navegar por eles soltos não corresponde a nenhuma tarefa.
31. **Gráfico de barra de WIP na tabela de quadros** (barra vermelha, laranja, vazia). Mais uma codificação só por cor.
