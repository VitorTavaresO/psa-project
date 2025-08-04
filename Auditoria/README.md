# **Auditoria de Qualidade em Aplicações Android: Uma Abordagem Prática com Foco em Desempenho, Segurança e IA**

## **Introdução**

O crescente uso de dispositivos móveis torna a qualidade das aplicações Android um fator crítico para a experiência do usuário e para a sustentabilidade dos sistemas. Problemas como lentidão, consumo excessivo de recursos e falhas de segurança impactam diretamente a usabilidade, a confiabilidade e a reputação de um app.

Este trabalho propõe uma  **auditoria prática de qualidade em aplicações Android** , com foco nos atributos definidos pela norma  **ISO/IEC 25010** , especialmente **desempenho** e  **segurança** .

Além do uso de ferramentas tradicionais de análise estática e dinâmica (MobSF, Android Lint, SonarQube, Android Profiler), o projeto também traz uma abordagem inovadora:  **a aplicação de ferramentas de Inteligência Artificial (IA), especificamente o** GitHub Copilot, na avaliação da qualidade do software.

O objetivo é comparar os apontamentos gerados pelas ferramentas tradicionais com aqueles fornecidos por IA, discutindo a eficácia, as limitações e os ganhos de cada abordagem na prática da auditoria de software.

---

## **Objetivos**

### **Objetivo Geral**

Realizar uma auditoria de qualidade em aplicações Android, com foco em desempenho e segurança, comparando os resultados obtidos por ferramentas tradicionais de qualidade de software com análises realizadas por IA.

### **Objetivos Específicos**

* Estudar os atributos de qualidade da norma  **ISO/IEC 25010** , com ênfase em **Desempenho** e  **Segurança** .
* Aplicar ferramentas de auditoria estática e dinâmica para levantamento de problemas.
* Utilizar ferramentas de **Inteligência Artificial** para realizar análises complementares.
* Elaborar dois relatórios por membro: um com os achados das ferramentas de qualidade de software e outro com os apontamentos da IA.
* Comparar os resultados obtidos pelas duas abordagens.
* Discutir os pontos fortes, limitações e complementaridade entre IA e ferramentas tradicionais.
* Propor melhorias para o aplicativo analisado com base nos resultados.

---

## **Ferramentas Utilizadas**

### **Ferramentas de Qualidade de Software:**

* **Estáticas:** MobSF, Android Lint, SonarQube
* **Dinâmicas:** Android Profiler

### **Ferramentas de IA:**

* GitHub Copilot---

## **Desenvolvimento — Etapas do Projeto**

### 1. **Escolha do App Open Source**

* App não trivial, com código aberto e documentação suficiente.
* Deve ser compilável no Android Studio e representar um cenário realista.

### 2. **Auditoria com Ferramentas de Qualidade de Software**

* **Análise Estática:**
  * Verificação de vulnerabilidades (armazenamento inseguro, permissões, componentes exportados, etc.).
  * Identificação de problemas de desempenho (uso de memória, CPU, má práticas, vazamento de memória, etc.).
* **Análise Dinâmica:**
  * Monitoramento de CPU, memória, rede e tempo de resposta.
  * Avaliação de comportamento sob carga e monitoramento de transmissão de dados sensíveis.

### 3. **Auditoria com Inteligência Artificial**

* Utilizar IA para realizar análises manuais ou semiautomáticas do código, perguntando sobre:
  * Boas práticas de segurança e desempenho.
  * Vulnerabilidades.
  * Sugestões de melhoria de código.
  * Problemas de arquitetura, organização, legibilidade ou consumo de recursos.
* Registrar os apontamentos, evidências e recomendações feitas pela IA.

### 4. **Documentação dos Resultados**

* Cada membro gera:
  * **Relatório da Ferramenta de Qualidade de Software.**
  * **Relatório dos Apontamentos da IA.**
* Consolidar os resultados:
  * Quadro comparativo entre IA e ferramentas.
  * Classificação dos problemas por impacto (baixa, média, alta) e tipo (desempenho, segurança, outros).

### 5. **Discussão e Propostas de Melhoria**

* Avaliar pontos fortes, limitações e complementaridade das abordagens.
* Propor correções e melhorias para o projeto analisado.

---

## **Fundamentação Teórica**

Mantém-se os mesmos pilares, agora adicionando uma frente sobre IA:

### 1. **Qualidade de Software e ISO/IEC 25010**

* Atributos: **Desempenho** e  **Segurança** .
* Subatributos:
  * *Performance Efficiency:* Time Behaviour, Resource Utilization, Capacity.
  * *Security:* Confidentiality, Integrity, Authenticity, Non-repudiation, Accountability, Resistance.

### 2. **Ferramentas de Qualidade de Software**

* Análise das ferramentas tradicionais, suas vantagens e limitações.

### 3. **Uso de Inteligência Artificial na Análise de Código**

* Potencial, limites, riscos e vantagens do uso de IA como apoio na auditoria de qualidade de software.

---


## **Resultados e Discussão**

### **Entregas do Projeto**

Os resultados desta auditoria serão organizados em quatro componentes principais:

#### 1. **Relatório Geral das Ferramentas**

* Consolidação dos achados de todas as ferramentas tradicionais utilizadas
* Análise estática: MobSF, Android Lint, SonarQube
* Análise dinâmica: Android Profiler
* Classificação dos problemas por severidade e categoria
* Métricas de qualidade e indicadores de desempenho

#### 2. **Relatório da IA Copilot Claude Sonnet 4**

* Análises e recomendações fornecidas pela IA
* Apontamentos sobre segurança, desempenho e boas práticas
* Sugestões de refatoração e otimização
* Identificação de padrões arquiteturais e problemas estruturais
* Avaliação de legibilidade e manutenibilidade do código

#### 3. **Tabela Comparativa**

* Comparação sistemática entre os achados das ferramentas tradicionais e da IA
* Análise de convergências e divergências nos apontamentos
* Avaliação da precisão e relevância de cada abordagem
* Identificação de pontos fortes e limitações de cada método
* Discussão sobre complementaridade das técnicas

#### 4. **Conjunto de Propostas de Melhorias**

* Recomendações priorizadas para correção dos problemas identificados
* Plano de ação estruturado por impacto e esforço de implementação
* Sugestões de melhorias arquiteturais e de código
* Propostas para otimização de desempenho e fortalecimento da segurança
* Diretrizes para prevenção de problemas futuros

### **Análise Crítica**

* Eficácia comparativa entre ferramentas tradicionais e IA na detecção de problemas
* Discussão sobre a viabilidade prática de cada abordagem
* Recomendações para integração de IA em processos de qualidade de software

---

## *Cronograma Atualizado — Incluindo Análise com IA*

| Semana | Data          | **Atividades Práticas**                                                                 |
| ------ | ------------- | ---------------------------------------------------------------------------------------------- |
| 1      | 09/06         | Escolha do app e configuração do ambiente                                                    |
| 2      | 16/06         | Instalação e testes preliminares com ferramentas                                             |
| 3-4    | 21/06 e 28/06 | **Execução da Auditoria Completa com Ferramentas (Estática e Dinâmica)**             |
| 5      | 28/07         | **Análise com IA — coleta de apontamentos e geração dos relatórios de cada membro** |
| 6      | 04/08         | Ajustes finais nas propostas, consolidação dos relatórios e fechamento dos materiais        |
| 7      | 11/08         | **Revisão geral, preparação dos vídeos e organização para apresentação final**   |
