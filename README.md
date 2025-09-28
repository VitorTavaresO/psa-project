
# Dependência de Ferramentas de IA em Práticas de Computação: Relato sobre Lições de um Estudo aplicado em Sala de Aula

Este repositório acompanha o estudo sobre os riscos da utilização indiscriminada de Inteligência Artificial (IA) no desenvolvimento de software, analisando tanto os benefícios quanto os desafios enfrentados por usuários sem conhecimento técnico suficiente. O projeto inclui um **estudo de caso**: desenvolvimento de um scanner de IP para identificação de portas abertas e serviços de rede.

---

## 📌 Introdução

A Inteligência Artificial tem revolucionado diversas áreas da tecnologia, e o desenvolvimento de software não é exceção. Assistentes de programação como ChatGPT e GitHub Copilot permitem gerar código rapidamente, otimizar tarefas repetitivas e solucionar problemas com eficiência.

No entanto, o uso cego da IA pode gerar **código ineficiente, inseguro e difícil de manter**, especialmente quando o usuário não tem conhecimento técnico para validar ou interpretar as soluções propostas. Este repositório demonstra, por meio de um estudo de caso prático, como a dependência da IA pode impactar negativamente o desenvolvimento de software.

---

## 🎯 Objetivos

### Geral

Analisar os impactos do uso da IA no desenvolvimento de software, destacando benefícios e riscos associados à dependência excessiva, especialmente sem conhecimento técnico adequado.

### Específicos

- Demonstrar, por meio de estudo de caso, os desafios de um usuário sem conhecimento de redes ao desenvolver um scanner de IP com IA.
- Discutir a importância do conhecimento técnico para validar e corrigir códigos gerados por IA.
- Refletir sobre o papel da IA como ferramenta auxiliar, enfatizando o desenvolvimento consciente e estruturado.

---

## 🛠 Ferramentas Utilizadas

- **Linguagem:** Python
- **Bibliotecas:** ipaddress, Nmap
- **IA:** ChatGPT, GitHub Copilot (com complemento de código e chat)

---

## 💻 Desenvolvimento

O objetivo do projeto é desenvolver um **scanner de portas** que:

- Verifica serviços ativos em IPs ou redes locais.
- Gera relatório automático sobre os serviços encontrados.

### Tarefas

- Criar script que escaneie portas abertas.
- Gerar relatório simples (CSV ou JSON) com resultados.
- Implementar log automático.
- Modularizar o código com padrão de projeto.

### Etapas

1. **Configuração Inicial:** Script Python recebe IP/domínio.
2. **Varredura de Portas:** Uso de `socket` e `nmap` para identificar serviços.
3. **Geração do Relatório:** Salva IP, portas abertas e serviços detectados em CSV/JSON.
4. **Refinamento e Testes:** Menus interativos e modos de escaneamento rápido/detalhado.

**Exemplo de uso:**

```bash
$ python scanner.py --target 192.168.1.1
[+] Escaneando 192.168.1.1...
[+] Porta 22 (SSH) - Aberta
[+] Porta 80 (HTTP) - Aberta
[+] Relatório salvo em: scan_192.168.1.1.json
```
