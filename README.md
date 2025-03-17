# Projeto Avançado de Software

## Ciclo 1 - Scanner de Portas com Relatório Automático com uso de IA

### Descrição:
Desenvolver um scanner de portas que verifica quais serviços estão ativos em um determinado IP ou rede local e gera um relatório automático sobre os serviços encontrados.

### Objetivos:
- Criar um script que escaneie portas abertas em um alvo especificado.
- Gerar um relatório simples sobre os serviços detectados.
- Implementar um log automático para registrar os resultados.
- Utilizar um padrão de projeto para modularizar o código.

### Tecnologias e Ferramentas:
- Linguagem: Python (usando socket e nmap)
- Bibliotecas: nmap (para varredura), csv/json (para salvar logs)
- Padrão de Projeto: Factory Method (para criar diferentes scanners)

### Etapas do Projeto:
**Configuração Inicial**

Criar um script básico em Python para aceitar um endereço IP ou domínio como entrada.

**Implementação da Varredura de Portas**

Usar socket para verificar quais portas estão abertas.
Integrar a biblioteca nmap para identificar os serviços ativos.

**Geração do Relatório**
Criar um arquivo CSV ou JSON contendo os resultados da varredura.
Salvar IP escaneado, portas abertas e serviços detectados.

**Refinamento e Testes**
Melhorar a interface do script (ex.: menus interativos).
Adicionar um modo de escaneamento rápido e um mais detalhado.

**Exemplo de Uso:**
$ python scanner.py --target 192.168.1.1  
[+] Escaneando 192.168.1.1...  
[+] Porta 22 (SSH) - Aberta  
[+] Porta 80 (HTTP) - Aberta  
[+] Relatório salvo em: scan_192.168.1.1.json  
