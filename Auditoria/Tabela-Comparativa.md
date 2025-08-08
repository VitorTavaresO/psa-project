# Tabela Comparativa Detalhada: Análise Linha por Linha dos Problemas

| **#**                                          | **Categoria/Arquivo**                       | **Relatório Geral**                                           | **Relatório IA**                                 | **Status de Sobreposição** |
| ---------------------------------------------------- | ------------------------------------------------- | -------------------------------------------------------------------- | ------------------------------------------------------- | ---------------------------------- |
| **AUTENTICAÇÃO E CREDENCIAIS**               |                                                   |                                                                      |                                                         |                                    |
| 1                                                    | Tokens/API_KEY em texto plano na memória         | ✅ (#39-40) Heap Dump com TOKEN_CACHED, AUTHENTICATION_INFO, API_KEY | ✅ (#13) Tokens no Keychain iOS sem verificação       | 🔄**IDENTIFICADO EM AMBOS**  |
| 2                                                    | SHA-256 sem salt                                  | ❌ Não identificado                                                 | ✅ (#9) SHA-256 simples para hash de senha              | ⚠️**APENAS IA**            |
| 3                                                    | Credenciais Basic Auth                            | ❌ Não identificado                                                 | ✅ (#20) Basic Auth em Base64 simples                   | ⚠️**APENAS IA**            |
| 4                                                    | Limpeza de dados sensíveis                       | ❌ Não identificado                                                 | ✅ (#18) Limpeza incompleta durante logout              | ⚠️**APENAS IA**            |
| **CRIPTOGRAFIA E2E**                           |                                                   |                                                                      |                                                         |                                    |
| 5                                                    | Chaves privadas E2E expostas                      | ❌ Não identificado                                                 | ✅ (#11) Chaves em texto simples no UserPreferences     | ⚠️**APENAS IA**            |
| 6                                                    | Geração de chaves fracas                        | ❌ Não identificado                                                 | ✅ (#12) Apenas 16 bytes aleatórios para chaves        | ⚠️**APENAS IA**            |
| 7                                                    | Senha E2E sem backup                              | ❌ Não identificado                                                 | ✅ (#6) Sem mecanismos de recuperação segura          | ⚠️**APENAS IA**            |
| 8                                                    | Senha E2E em estado do componente                 | ❌ Não identificado                                                 | ✅ (#23) Mantida sem limpeza                            | ⚠️**APENAS IA**            |
| 9                                                    | Dados sem criptografia adicional                  | ❌ Não identificado                                                 | ✅ (#27) UserPreferences sem proteção                 | ⚠️**APENAS IA**            |
| **SSL/TLS E CERTIFICADOS**                     |                                                   |                                                                      |                                                         |                                    |
| 10                                                   | Configurações SSL permissivas                   | ✅ (#24) Permite texto simples e certificados de usuário            | ✅ (#15) Senhas via Alert.prompt sem validação        | 🔄**IDENTIFICADO EM AMBOS**  |
| 11                                                   | Tratamento de erros SSL                           | ❌ Não identificado especificamente                                 | ✅ (#16) Tratamento inadequado em iOS                   | ⚠️**APENAS IA**            |
| 12                                                   | Chaves privadas de certificados expostas          | ❌ Não identificado                                                 | ✅ (#17) Métodos públicos no Android                  | ⚠️**APENAS IA**            |
| **VALIDAÇÃO DE ENTRADA**                     |                                                   |                                                                      |                                                         |                                    |
| 13                                                   | Problemas com regex                               | ✅ (#1,8,22) Múltiplos BUGs relacionados a regex                    | ✅ (#7) Nomes de sala sem validação robusta           | 🔄**IDENTIFICADO EM AMBOS**  |
| 14                                                   | Validação OAuth insuficiente                    | ❌ Não identificado                                                 | ✅ (#14,36) Estados OAuth e parâmetros OAuth           | ⚠️**APENAS IA**            |
| 15                                                   | Política de senhas fraca                         | ❌ Não identificado                                                 | ✅ (#30) Validação apenas no cliente                  | ⚠️**APENAS IA**            |
| **PERFORMANCE E THREADS**                      |                                                   |                                                                      |                                                         |                                    |
| 16                                                   | Tarefas pesadas na thread principal               | ✅ (#33) Execução de tarefas pesadas no UI Thread                  | ✅ (#10,28) Múltiplas chamadas síncronas bloqueiam UI | 🔄**IDENTIFICADO EM AMBOS**  |
| 17                                                   | Múltiplas threads competindo                     | ✅ (#34-37) Firebase, OkHttp, TaskRunner                             | ✅ (#33) Vazamentos por listeners não removidos        | 🔄**IDENTIFICADO EM AMBOS**  |
| 18                                                   | Operações de criptografia sem paralelização   | ❌ Não identificado                                                 | ✅ (#19) Sem otimização de paralelização            | ⚠️**APENAS IA**            |
| 19                                                   | Queries de banco não otimizadas                  | ❌ Não identificado especificamente                                 | ✅ (#22,40) Database e operações sequenciais          | ⚠️**APENAS IA**            |
| **RATE LIMITING E FORÇA BRUTA**               |                                                   |                                                                      |                                                         |                                    |
| 20                                                   | Múltiplas tentativas sem limitação             | ❌ Não identificado                                                 | ✅ (#4,21) Tentativas de autenticação e login         | ⚠️**APENAS IA**            |
| **LOGGING E AUDITORIA**                        |                                                   |                                                                      |                                                         |                                    |
| 21                                                   | Logs contêm informações sensíveis             | ❌ Não identificado                                                 | ✅ (#29,48) Debug com tokens e senhas                   | ⚠️**APENAS IA**            |
| 22                                                   | Ausência de auditoria                            | ❌ Não identificado                                                 | ✅ (#31,45) Ações sensíveis e sistema insuficiente   | ⚠️**APENAS IA**            |
| **COBERTURA DE TESTES**                        |                                                   |                                                                      |                                                         |                                    |
| 23                                                   | Arquivos sem cobertura de teste                   | ✅ (#2-6) 15 arquivos sem cobertura                                  | ❌ Não abordado                                        | ⚠️**APENAS GERAL**         |
| 24                                                   | Timeouts excessivos em testes                     | ❌ Não identificado especificamente                                 | ✅ (#32,39) 60000ms em operações                      | ⚠️**APENAS IA**            |
| **COMPATIBILIDADE E VERSÕES**                 |                                                   |                                                                      |                                                         |                                    |
| 25                                                   | Android 5.0-5.0.2 vulnerável                     | ✅ (#23) Compatibilidade com API 21 vulnerável                      | ❌ Não identificado                                    | ⚠️**APENAS GERAL**         |
| **CODE SMELLS ESTRUTURAIS**                    |                                                   |                                                                      |                                                         |                                    |
| 26                                                   | God Class (RoomView)                              | ✅ (#11) 16 code smells, complexidade                                | ✅ Implícito em múltiplos problemas                   | 🔄**IDENTIFICADO EM AMBOS**  |
| 27                                                   | Métodos complexos                                | ✅ (#11,12,16,19) Múltiplos arquivos                                | ✅ Implícito em vários problemas                      | 🔄**IDENTIFICADO EM AMBOS**  |
| 28                                                   | Código comentado                                 | ✅ (#7,17) Trechos comentados                                        | ❌ Não identificado especificamente                    | ⚠️**APENAS GERAL**         |
| 29                                                   | Reflexão utilizada                               | ✅ (#27) JsonAdapter.java                                            | ❌ Não identificado                                    | ⚠️**APENAS GERAL**         |
| 30                                                   | Constantes hardcoded                              | ❌ Não identificado especificamente                                 | ✅ (#26,37) Configurações e timeouts                  | ⚠️**APENAS IA**            |
| **PROBLEMAS ESPECÍFICOS DO RELATÓRIO GERAL** |                                                   |                                                                      |                                                         |                                    |
| 31                                                   | BUGs de atribuição setState                     | ✅ (#13,14,19,20) setSwitchValue(switchValue)                        | ❌ Não identificado                                    | ⚠️**APENAS GERAL**         |
| 32                                                   | Conversão de condicionais em boolean             | ✅ (#15,17,18) Evitar vazamento de valor                             | ❌ Não identificado                                    | ⚠️**APENAS GERAL**         |
| 33                                                   | Uso de ByteBuffer.wrap                            | ✅ (#21) Substituir por função construtora                         | ❌ Não identificado especificamente                    | ⚠️**APENAS GERAL**         |
| 34                                                   | Clipboard acessado                                | ✅ (#28) Pode expor informações sensíveis                         | ❌ Não identificado                                    | ⚠️**APENAS GERAL**         |
| 35                                                   | Código nativo carregado                          | ✅ (#29) SystemLibraryLoader.java                                    | ❌ Não identificado                                    | ⚠️**APENAS GERAL**         |
| 36                                                   | TCP Sockets usados                                | ✅ (#30) Okio.java                                                   | ❌ Não identificado                                    | ⚠️**APENAS GERAL**         |
| 37                                                   | Consulta dados sensíveis (SMS/contatos)          | ✅ (#31) UriKt.java                                                  | ❌ Não identificado                                    | ⚠️**APENAS GERAL**         |
| 38                                                   | Message digests fracos                            | ✅ (#32) MD5 pode causar vulnerabilidades                            | ❌ Não identificado                                    | ⚠️**APENAS GERAL**         |
| **PROBLEMAS ESPECÍFICOS DO RELATÓRIO IA**    |                                                   |                                                                      |                                                         |                                    |
| 39                                                   | Campo JoinCode sem proteção contra captura      | ❌ Não identificado                                                 | ✅ (#1) secureTextEntry insuficiente                    | ⚠️**APENAS IA**            |
| 40                                                   | Foco com atraso perceptível                      | ❌ Não identificado                                                 | ✅ (#2) InteractionManager causa atraso                 | ⚠️**APENAS IA**            |
| 41                                                   | Renderização não dinâmica                     | ❌ Não identificado                                                 | ✅ (#3) BaseButton e Gap                                | ⚠️**APENAS IA**            |
| 42                                                   | Mensagens de erro genéricas                      | ❌ Não identificado                                                 | ✅ (#5) Expõem detalhes internos                       | ⚠️**APENAS IA**            |
| 43                                                   | Limitação de usuários sem tratamento dinâmico | ❌ Não identificado                                                 | ✅ (#8) Grandes volumes                                 | ⚠️**APENAS IA**            |
| 44                                                   | Acesso Keychain sem validação                   | ❌ Não identificado                                                 | ✅ (#24) iOS Storage.swift                              | ⚠️**APENAS IA**            |
| 45                                                   | Descriptografia sem validação de integridade    | ❌ Não identificado                                                 | ✅ (#25) Android Encryption.java                        | ⚠️**APENAS IA**            |
| 46                                                   | Falta de paginação eficiente                    | ❌ Não identificado                                                 | ✅ (#35) Carregamento de mensagens                      | ⚠️**APENAS IA**            |
| 47                                                   | Promises não resolvidas adequadamente            | ❌ Não identificado                                                 | ✅ (#38) deferred.ts                                    | ⚠️**APENAS IA**            |
| 48                                                   | Problemas de thread safety                        | ❌ Não identificado especificamente                                 | ✅ (#43,46) Coleções e objetos em memória            | ⚠️**APENAS IA**            |

## Resumo da Sobreposição

### **Problemas Identificados em Ambos (8 categorias):**

* ✅ Tokens/credenciais em texto plano
* ✅ Configurações SSL permissivas
* ✅ Problemas com regex
* ✅ Tarefas pesadas na thread principal
* ✅ Múltiplas threads competindo
* ✅ God Class e métodos complexos

### **Únicos do Relatório Geral (13 categorias):**

* ⚠️ Cobertura de testes (15 arquivos)
* ⚠️ Compatibilidade Android vulnerável
* ⚠️ BUGs específicos de React (setState)
* ⚠️ Problemas nativos (clipboard, TCP sockets, etc.)

### **Únicos do Relatório IA (25 categorias):**

* ⚠️ Criptografia E2E completa (5 problemas)
* ⚠️ Rate limiting e força bruta
* ⚠️ Logging e auditoria
* ⚠️ Validação OAuth
* ⚠️ Performance específica (queries, promises)

### **Taxa de Sobreposição: ~20%**

* **Complementaridade: ~80%** - As análises são altamente complementares
* **Relatório Geral:** Focou em problemas estruturais, testes e compatibilidade
* **Relatório IA:** Aprofundou-se em segurança específica (E2E, OAuth, auditoria)
