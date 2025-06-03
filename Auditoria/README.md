# Auditoria de Qualidade em Aplicações Android: Uma Abordagem Prática com Foco em Desempenho e Segurança

## Introdução

Com o crescimento exponencial do uso de dispositivos móveis, a qualidade das aplicações Android tornou-se um fator crítico para a experiência do usuário e para o sucesso de um aplicativo, problemas como lentidão, consumo excessivo de recursos ou falhas de segurança podem comprometer seriamente a usabilidade e a confiabilidade de um app.

Neste contexto, o presente trabalho propõe uma auditoria prática de qualidade em aplicações Android, com foco em dois atributos fundamentais definidos pela norma ISO/IEC 25010: desempenho e segurança. Utilizando ferramentas como MobSF, Android Profiler, Firebase Performance e Android Lint, este estudo busca identificar problemas reais em um aplicativo open source, analisando tanto aspectos estáticos quanto dinâmicos da aplicação.

A iniciativa visa não apenas demonstrar a importância da análise de qualidade no ciclo de desenvolvimento de software mobile, mas também capacitar a equipe na utilização de ferramentas práticas de auditoria. Os resultados obtidos serão discutidos em termos de impacto nos atributos de qualidade e potencial de correção.

## Objetivos

### Objetivo Geral

Analisar a qualidade de aplicações Android, com foco nos atributos de desempenho e segurança, por meio de uma auditoria prática utilizando ferramentas de análise estática e dinâmica.

### Objetivos Específicos

* Estudar os principais atributos de qualidade conforme a norma ISO/IEC 25010, com ênfase em desempenho e segurança.
* Investigar as principais falhas de projeto em aplicações Android relacionadas a esses atributos.
* Aplicar ferramentas como MobSF, Android Profiler, Firebase Performance e Android Lint em um app open source.
* Documentar os resultados obtidos, classificando os problemas por severidade e propondo melhorias.

## Ferramentas Utilizadas

* **Estáticas:**
* **Dinâmicas:**
* **Ambiente:**

## Desenvolvimento

### Equipe e Tarefas

| Papel    | Responsáveis | Atividades principais                                                                                                    |
| -------- | ------------- | ------------------------------------------------------------------------------------------------------------------------ |
| Pesquisa | 3 pessoas     | Fundamentação teórica, levantamento de ferramentas, problemas comuns, escrita das seções teóricas e metodológicas |
| Prática | 2 pessoas     | Auditoria com testes em app open source, documentação dos resultados                                                  |

### Etapas do Projeto

1. **Escolha do App Open Source**
   Critérios:

   * App não trivial, com funcionalidades completas.
   * Código aberto, compilável no Android Studio.
   * Documentação mínima necessária.
2. **Auditoria Estática**

   * Detectar falhas de segurança:
     * Armazenamento inseguro
     * Componentes exportados indevidamente
     * Permissões perigosas
   * Detectar problemas de desempenho:
     * Más práticas
     * Vazamentos de memória
     * Problemas de desempenho
3. **Auditoria Dinâmica**

   * Monitorar CPU, memória e rede
   * **(Opcional)** Integrar com **Firebase Performance Monitoring** para:
     * Verificar lentidões, ANRs e tempos de resposta
4. **Documentação dos Resultados**

   * Tabelas de vulnerabilidades e problemas de desempenho
   * Classificação por impacto (baixa, média, alta)
   * Propostas de melhoria e relação com atributos da ISO/IEC 25010

## Fundamentação Teórica

A pesquisa será dividida em três frentes principais, cada uma atribuída a um dos pesquisadores:

### 1. Arquitetura Android e Qualidade

* Ciclo de vida de componentes
* Problemas como vazamento de memória, ANRs, travamentos
* Práticas recomendadas de desenvolvimento

### 2. ISO/IEC 25010 e Atributos de Qualidade

* Qualidade de software e sua importância
* Desempenho e segurança como atributos principais
* Relação entre falhas comuns e perda de qualidade percebida

### 3. Ferramentas e Técnicas

* Ferramentas de análise estática e dinâmica
* Comparação entre Android Lint, MobSF, SonarQube, Profiler e Firebase
* Vantagens, limitações e contexto ideal de uso

## Resultados e Discussão

Serão apresentados os principais problemas encontrados nas análises estática e dinâmica, seguidos por uma análise crítica de seus impactos:

* **Quadros e gráficos** mostrando vulnerabilidades e problemas de performance
* **Classificação por severidade** e frequência
* **Propostas de correção** alinhadas às boas práticas
* **(Opcional)** Comparação antes/depois se correções forem aplicadas

## Cronograma das Atividades

| Semana | Atividades de Pesquisa                           | Atividades Práticas                           |
| ------ | ------------------------------------------------ | ---------------------------------------------- |
| 1      | Levantamento teórico e ferramentas              | Escolha do app e configuração do ambiente    |
| 2      | Escrita da fundamentação teórica              | Análise Estática                             |
| 3      | Revisão teórica e aprofundamento metodológico | Análise Dinâmica                             |
| 4      | Conclusão do artigo e revisão final            | Consolidação dos resultados e documentação |

## Materiais de Estudo

1. **Documentação das Ferramentas:**
2. **Referências Normativas:**
   * ISO/IEC 25010 - Systems and software engineering — Systems and software Quality Requirements and Evaluation (SQuaRE)
3. **Artigos Científicos e Técnicos (opcional):**
