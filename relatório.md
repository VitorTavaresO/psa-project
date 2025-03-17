# Relatório 17-03

### Configuração Inicial

- Primeiro passado realizado pela aluna foi solicitar ao ChatGPT uma estrutura de pastas para o projeto.
- Em sequência em um primeiro teste do Github Copilot ela questionou sobre o que seria o Factory Method.
- Solicitou ao Copilot o seguinte tópico: "Criar um script básico em Python para aceitar um endereço IP ou domínio como entrada."
- Aceitou a inserção do código Python gerado pelo Copilot, sem buscar o que fazia cada etapa do código.

  ```python
  def main():
      # Solicita ao usuário um endereço IP ou domínio
      target = input("Digite o endereço IP ou domínio para escanear: ")

      # Valida se o usuário forneceu uma entrada
      if not target.strip():
          print("Erro: Nenhum endereço IP ou domínio foi fornecido.")
          return

      # Exibe o alvo fornecido
      print(f"Alvo recebido: {target}")
      # Aqui você pode adicionar a lógica de escaneamento de portas ou outras funcionalidades

  if _name_ == "_main_":
      main()
  ```

- Realizou um teste se o código fazia o que foi solicitado ao agente.
- Buscou entender brevemente o código do Input criado.
- Solicitou ao Copilot o que significava e para que servia o .strip() utilizado.
- Solicitou ao ChatGPT quais os tipos de dados que deveriam ser aceitos nesse input, informação essa que não foi dita na explicação do projeto.
- Informou ao Copilot que iria trabalhar apenas com IP e então integrou a alteração de código fornecida.

  ```python
  import ipaddress

  def main():
      # Solicita ao usuário um endereço IP
      target = input("Digite o endereço IP para escanear: ")

      # Valida se o usuário forneceu uma entrada
      if not target.strip():
          print("Erro: Nenhum endereço IP foi fornecido.")
          return

      # Valida se o endereço fornecido é um IP válido
      try:
          ipaddress.ip_address(target)
          print(f"Alvo recebido: {target}")
          # Aqui você pode adicionar a lógica de escaneamento de portas ou outras funcionalidades
      except ValueError:
          print("Erro: O endereço fornecido não é um IP válido.")

  if _name_ == "_main_":
      main()
  ```

- Uma informação acrescentada pelo Copilot foi que seria recomendado usar a biblioteca **ipaddress** nativa do Python para verificar se o ip é válido.
- Ela selecionou o código if name para questionar seu uso e necessidade.

### Implementação da Varredura de Portas
