
# Um Estudo Sobre os Riscos da Dependência Cega de IA para o Desenvolvimento de Software

## Introdução

A Inteligência Artificial tem revolucionado diversas áreas da tecnologia, e o desenvolvimento de software não é exceção, com a popularização de assistentes de programação, como ChatGPT e Copilot, desenvolvedores podem gerar código rapidamente, otimizar tarefas repetitivas e solucionar problemas com mais eficiência. No entanto, essa facilidade levanta um debate importante: até que ponto a IA no desenvolvimento de software pode ser benéfica, e quando ela se torna um problema?

Este trabalho explora os riscos da utilização indiscriminada da IA como ferramenta principal no desenvolvimento de software, especialmente quando usada por indivíduos sem conhecimento técnico suficiente para validar as soluções propostas, por meio de um estudo de caso prático, analisaremos o desenvolvimento de um scanner de IP para identificar portas abertas e protocolos de rede, feito por um usuário sem experiência prévia na área de redes.

A criação de um sistema sem um planejamento adequado, validação técnica e compreensão sólida de seus fundamentos pode resultar em código ineficiente, inseguro e difícil de manter e à medida que o projeto se torna mais complexo, esses problemas tendem a se agravar, comprometendo não apenas seu desempenho, mas também sua confiabilidade e escalabilidade.

Com isso pretende-se demonstrar como a aceitação cega de códigos gerados por IA pode resultar em problemas que o próprio programador não consegue solucionar, criando um ciclo vicioso de dependência, a pesquisa destaca tanto os benefícios da IA no desenvolvimento de software quanto os riscos associados ao seu uso inconsequente, evidenciando a importância do conhecimento técnico para interpretar, validar e corrigir o código gerado.

## Objetivos

### Objetivo Geral

Analisar os impactos do uso da Inteligência Artificial no desenvolvimento de software, apontando tanto seus benefícios quanto os riscos associados à dependência excessiva, especialmente quando utilizada sem conhecimento técnico adequado para validar as soluções geradas.

### Objetivos Específicos

* Demonstrar, por meio de um estudo de caso, os desafios e limitações enfrentados por um usuário sem conhecimento prévio de redes ao desenvolver um scanner de IP com auxílio da IA.
* Discutir a importância do conhecimento técnico para interpretar, validar e corrigir o código gerado por IA, evitando a criação de sistemas frágeis e ineficientes.
* Apresentar uma reflexão sobre o papel da IA como ferramenta auxiliar, enfatizando a necessidade de um desenvolvimento consciente e estruturado.

## Ferramentas Utilizadas

* **Linguagem:** Python
* **Bibliotecas:** Ipaddress e Nmap
* **Ferramentas de IA:** ChatGPT e Github Copilot(Com complemento de código e chat)

## Desenvolvimento

Desenvolver um scanner de portas que verifica quais serviços estão ativos em um determinado IP ou rede local e gera um relatório automático sobre os serviços encontrados.

### Tarefas

* Criar um script que escaneie portas abertas em um alvo especificado.
* Gerar um relatório simples sobre os serviços detectados.
* Implementar um log automático para registrar os resultados.
* Utilizar um padrão de projeto para modularizar o código.

### Etapas do Projeto

1. **Configuração Inicial**
   Criar um script básico em Python para aceitar um endereço IP ou domínio como entrada.
2. **Implementação da Varredura de Portas**
   Usar socket para verificar quais portas estão abertas.
   Integrar a biblioteca nmap para identificar os serviços ativos.
3. **Geração do Relatório**
   Criar um arquivo CSV ou JSON contendo os resultados da varredura.
   Salvar IP escaneado, portas abertas e serviços detectados.
4. **Refinamento e Testes**
   Melhorar a interface do script (ex.: menus interativos).
   Adicionar um modo de escaneamento rápido e um mais detalhado.
5. **Exemplo de Uso:**
   $ python scanner.py --target 192.168.1.1
   [+] Escaneando 192.168.1.1...
   [+] Porta 22 (SSH) - Aberta
   [+] Porta 80 (HTTP) - Aberta
   [+] Relatório salvo em: scan_192.168.1.1.json

## Análise

A análise será conduzida com o objetivo de identificar os impactos da dependência da Inteligência Artificial no desenvolvimento de software, avaliando tanto os benefícios quanto os desafios enfrentados por um usuário sem conhecimento prévio da área de redes ao construir um scanner de IP com auxílio da IA.

1. **Monitoramento das Interações**

   * Um dos membros da equipe será responsável por acompanhar o participante que utilizará a IA para desenvolver o sistema.
   * Durante o desenvolvimento, serão registradas todas as interações realizadas com a IA, incluindo as perguntas feitas, os códigos gerados, as edições aplicadas e as dificuldades enfrentadas.
   * Será analisado o processo de tomada de decisão do participante, observando quando e como ele aceita, modifica ou rejeita as respostas da IA.
   * As interações problemáticas serão documentadas, destacando casos onde o código gerado pela IA apresentou erros, falhas de segurança, inconsistências ou dificuldades de implementação.
2. **Registro de Resultados e Problemas Identificados**

   * As principais dificuldades encontradas pelo participante serão registradas, como erros de funcionamento, falhas na interpretação do código gerado e necessidade de múltiplas interações para resolver um problema.
   * Será avaliada a capacidade do participante de compreender e corrigir os erros por conta própria ou se ele precisa recorrer novamente à IA, criando um ciclo de dependência.
3. **Análise Externa e Conclusões**

   * Após o período de desenvolvimento e monitoramento, um terceiro membro da equipe, que não participou diretamente da criação do código nem do acompanhamento, realizará uma análise crítica aprofundada.
   * Esse membro avaliará os dados coletados, buscando padrões de dependência da IA, impactos na qualidade do código e desafios enfrentados pelo participante.
   * Serão extraídos insights sobre os benefícios proporcionados pela IA, como agilidade no desenvolvimento e auxílio na solução de problemas, assim como as problemáticas, como a falta de compreensão do código gerado e os riscos de segurança e manutenção.
   * A análise final buscará relacionar esses aspectos com conceitos da disciplina de  **Projeto Avançado de Software** , destacando a importância de um desenvolvimento estruturado e da validação técnica no uso de IA.

Essa abordagem permitirá uma avaliação detalhada dos efeitos da dependência da IA no desenvolvimento de software, oferecendo uma visão equilibrada entre seus benefícios e os riscos associados ao seu uso sem conhecimento técnico adequado.

## Cronograma das Atividades

* **1° Dia** :

  * Definição do tema do estudo, das ferramentas a serem utilizadas e do direcionamento para o desenvolvimento da aplicação.
  * Planejamento da estrutura de monitoramento das interações e definição das métricas a serem observadas durante o desenvolvimento.
* **2° Dia** :

  * Início do desenvolvimento prático: o participante terá o primeiro período de tempo dedicado para tentar construir a aplicação utilizando a IA.
  * Acompanhamento das interações com a IA e registros das decisões tomadas pelo participante, incluindo as dificuldades enfrentadas.
* **3° Dia** :

  * Continuação do desenvolvimento: o participante terá o tempo restante para finalizar a solução proposta.
  * Ao final, será realizada uma análise mais aprofundada, onde um membro da equipe realizará uma avaliação crítica das interações registradas e dos resultados obtidos.
  * Documentação dos principais pontos observados durante o desenvolvimento e as falhas ou sucessos encontrados, com ênfase na dependência da IA e suas implicações no código gerado.

## Materiais de Estudo

1. Artigos Jornalístico:
   1. Humans do it better: GitClear analyzes 153M lines of code, finds risks of AI.
      **LINK: <https://arc.dev/talent-blog/impact-of-ai-on-code/>**

2. Artigos Científicos:
   1. The Next Frontier in Software Development: AI-Augmented Software Development Processes
      **LINK: <https://ieeexplore.ieee.org/stamp/stamp.jsp?tp=&arnumber=10176194>**

   2. Future of software development with generative AI

      **LINK: <https://link.springer.com/article/10.1007/s10515-024-00426-z>**

   3. Impact of Generative AI on the Software Development Lifecycle (SDLC)
      **LINK: <https://papers.ssrn.com/sol3/papers.cfm?abstract_id=4536700>**
