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
* Aplicar ferramentas como MobSF, Android Profiler e Android Lint em um app open source.
* Documentar os resultados obtidos, classificando os problemas por severidade e propondo melhorias.

## Ferramentas Utilizadas

* **Estáticas:** MobSF, Android Lint e SonarQube
* **Dinâmicas:** Android Profiler
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
     * Armazenamento inseguro *(Confidentiality)*
     * Componentes exportados indevidamente *(Confidentiality, Resistance)*
     * Permissões perigosas *(Confidentiality, Authenticity)*
     * Falta de verificação de integridade de dados *(Integrity)*
     * Falta de autenticação ou validação de origem *(Authenticity)*
     * Ausência de proteção contra engenharia reversa *(Resistance)*
     * Falta de logs de ações sensíveis *(Accountability, Non-repudiation)*
   * Detectar problemas de desempenho:
     * Más práticas de código *(Time Behaviour)*
     * Vazamentos de memória *(Resource Utilization)*
     * Uso excessivo de recursos *(Resource Utilization)*
3. **Auditoria Dinâmica**

   * Monitorar CPU, memória e rede *(Resource Utilization)*
   * Medir tempo de resposta e execução *(Time Behaviour)*
   * Avaliar comportamento sob carga *(Capacity)*
   * Monitorar transmissão de dados sensíveis *(Confidentiality, Authenticity)*
   * Detectar modificação ou violação em tempo de execução *(Resistance, Integrity)*
4. **Documentação dos Resultados**

   * Tabelas de vulnerabilidades e problemas de desempenho
   * Classificação por impacto (baixa, média, alta)
   * Propostas de melhoria e relação com atributos da ISO/IEC 25010

## Fundamentação Teórica

A pesquisa será dividida em três frentes principais, cada uma atribuída a um dos pesquisadores:

### 1. Qualidade de Software com base na ISO/IEC 25010

Nesta trabalho de auditoria, o foco será nos atributos de qualidade **Desempenho (Performance Efficiency)** e **Segurança (Security)**, conforme definidos na ISO/IEC 25010.

Para cada um dos subatributos listados abaixo, serão abordados três pontos:

1. **Contextualização do subatributo**
2. **Problemas causados pela ausência de sua aplicação**
3. **Formas de aplicá-lo na prática**

#### Desempenho (Performance Efficiency)

* **Time Behaviour**
* **Resource Utilization**
* **Capacity**

#### Segurança (Security)

* **Confidentiality**
* **Integrity**
* **Authenticity**
* **Non-repudiation**
* **Accountability**
* **Resistance**

### 3. Ferramentas e Técnicas

* Ferramentas de análise estática e dinâmica.
* Comparação entre Android Lint, MobSF, SonarQube, Android Profiler.
* Vantagens, limitações e contexto ideal de uso.

## Resultados e Discussão

Serão apresentados os principais problemas encontrados nas análises estática e dinâmica, seguidos por uma análise crítica de seus impactos:

* **Quadros e gráficos** mostrando vulnerabilidades e problemas de performance
* **Classificação por severidade** e frequência
* **Propostas de correção** alinhadas às boas práticas
* **(Opcional)** Comparação antes/depois se correções forem aplicadas

## Cronograma das Atividades

| Semana | Data  | Atividades de Pesquisa                                  | Atividades Práticas                                     |
| ------ | ----- | ------------------------------------------------------- | -------------------------------------------------------- |
| 1      | 09/06 | Levantamento teórico inicial                           | Escolha do app e configuração do ambiente              |
| 2      | 16/06 | Estudo da ISO/IEC 25010 e definição dos atributos     | Instalação e testes preliminares com ferramentas       |
| 3      | 23/06 | Escrita da fundamentação teórica                     | Início da Análise Estática com MobSF, Lint, SonarQube |
| 4      | 30/06 | Continuação da escrita e organização da metodologia | Análise Estática — consolidação de achados          |
| 5      | 28/07 | Revisão teórica e alinhamento com metodologia         | Início da Análise Dinâmica com Android Profiler       |
| 6      | 04/08 | Consolidação das Fundamentações e Relevância       | Análise Dinâmica — coleta e registro de dados         |
| 7      | 11/08 | Revisão geral do conteúdo escrito                     | Consolidação e comparação dos resultados             |

## Materiais de Estudo

1. **Documentação das Ferramentas:**
   * Android Lint : [https://developer.android.com/studio/write/lint]()
   * Android Profiler: [https://developer.android.com/studio/profile]()
   * MobSF: [https://github.com/MobSF/Mobile-Security-Framework-MobSF]()
   * SonarQube: [https://www.sonarsource.com/products/sonarqube/]()
2. **Referências Normativas:**
   * ISO/IEC 25010 - Systems and software engineering — Systems and software Quality Requirements and Evaluation (SQuaRE)
     * LINK: [https://www.iso.org/standard/35733.html]()
3. **Artigos Científicos e Técnicos:**
