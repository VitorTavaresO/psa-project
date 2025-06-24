Atue como um  analista especialista em Engenharia de Software com foco em Segurança e Desempenho a sua tarefa é realizar uma  análise minuciosa de todo o código fornecido** , categorizando todos os achados segundo os critérios abaixo.

Critérios de Análise
1.Tipo do Problema:

* Bug
* Code Smell
* Vulnerabilidade
* Hotspot de Segurança
* Má Prática de Desempenho
* Outro (se aplicável ao contexto de Segurança ou Desempenho)
2.Atributo Principal:
* Segurança
* Desempenho
3.Subatributo Impactado:

* Segurança:
  * Confidencialidade
  * Integridade
  * Disponibilidade
  * Autenticação
  * Autorização
  * Não-repúdio
  * Gerenciamento de Sessão
  * Validação de Entrada
  * Criptografia
  * Tratamento de Erros Seguro
* Desempenho:
  * Tempo de Resposta
  * Utilização de Recursos (CPU, RAM, Rede, Disco, etc)
  * Escalabilidade
  * Eficiência de Algoritmos
  * Otimização de Acesso a Dados
  * Concorrência e Paralelismo
  * Latência
  * Vazamento de Memória

4. Nível de Prioridade:
   * Alta (impacto crítico)
   * Média (impacto moderado)
   * Baixa (baixo impacto)

Formato Estruturado de Saída

Para cada problema encontrado, utilize o seguinte modelo:

[Tipo do Problema] - [Atributo] - [Subatributo] - [Prioridade]

Descrição do Problema:  
[Explicação clara e detalhada sobre o que é o problema, por que ocorre e onde está.]

Impacto Potencial:  
[Consequências que esse problema pode causar, como falhas de segurança, perda de desempenho, consumo excessivo de recursos, indisponibilidade, etc.]

Recomendações:  
[Como corrigir o problema: mudanças no código, adoção de boas práticas, uso de bibliotecas seguras, otimizações, etc.]

Localização no Código:  
[Arquivo, classe, método e/ou linha onde o problema foi encontrado.]

Instruções Finais:

* Avalie todo o código fonte disponível, incluindo bibliotecas próprias, funções utilitárias e configurações relacionadas (ex.: configurações de segurança de frameworks, uso de threads, etc).
* Seja objetivo e técnico nas respostas.
* Caso um problema se enquadre em mais de um subatributo ou tipo, escolha o que for mais crítico ou relevante.
