# Relatório 17-03

### Configuração Inicial

-
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

- Após concluir a Configuração Inicial, ela solicitou a seguinte questão ao Copilot:
  Como integrar isso: 'Usar socket para verificar quais portas estão abertas. Integrar a biblioteca nmap para identificar os serviços ativos.'?

```python
import ipaddress
import socket
import nmap

def scan_ports_with_socket(target):
    print("\n[+] Escaneando portas abertas com socket...")
    open_ports = []
    for port in range(1, 1025):  # Escaneia as portas de 1 a 1024
        try:
            with socket.socket(socket.AF_INET, socket.SOCK_STREAM) as s:
                s.settimeout(0.5)  # Define um timeout para evitar travamentos
                if s.connect_ex((target, port)) == 0:
                    open_ports.append(port)
                    print(f"[+] Porta {port} aberta")
        except Exception as e:
            print(f"Erro ao escanear a porta {port}: {e}")
    return open_ports

def scan_services_with_nmap(target):
    print("\n[+] Identificando serviços ativos com nmap...")
    nm = nmap.PortScanner()
    try:
        nm.scan(hosts=target, arguments='-sV')  # Escaneia serviços com detecção de versão
        for host in nm.all_hosts():
            print(f"\nHost: {host} ({nm[host].hostname()})")
            print(f"Estado: {nm[host].state()}")
            for proto in nm[host].all_protocols():
                print(f"\nProtocolo: {proto}")
                ports = nm[host][proto].keys()
                for port in ports:
                    service = nm[host][proto][port]
                    print(f"Porta: {port}\tEstado: {service['state']}\tServiço: {service['name']}")
    except Exception as e:
        print(f"Erro ao usar nmap: {e}")

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

        # Escaneia portas abertas com socket
        open_ports = scan_ports_with_socket(target)

        # Identifica serviços ativos com nmap
        if open_ports:
            scan_services_with_nmap(target)
        else:
            print("Nenhuma porta aberta encontrada para escanear serviços.")
    except ValueError:
        print("Erro: O endereço fornecido não é um IP válido.")

if _name_ == "_main_":
    main()
```

- Ela aceitou a inserção do código diretamente sobre o código anterior.
- Ela questionou ao Copilot o que seria a biblioteca Socket apresentada.

  Resposta do Copilot:

  A biblioteca socket é uma biblioteca padrão do Python que fornece uma interface para comunicação de rede usando sockets. Um socket é um   ponto de comunicação entre dois dispositivos (como cliente e servidor) em uma rede. Ele permite que você envie e receba dados entre computadores  conectados por meio de protocolos como TCP ou UDP.

  Principais funcionalidades da biblioteca socket:
  Criação de conexões de rede:

  Permite criar conexões cliente-servidor.
  Suporta protocolos como TCP (conexão confiável) e UDP (conexão sem estado).
  Escaneamento de portas:

  Pode ser usado para verificar se uma porta em um endereço IP está aberta ou fechada, como no seu código.
  Envio e recebimento de dados:

  Permite enviar e receber dados entre dispositivos conectados.
  Comunicação local e remota:
  Pode ser usado para comunicação entre processos no mesmo dispositivo ou entre dispositivos diferentes em uma rede.
- O Copilot informou a necessidade da instalação do nmap.
- Ela executou a instalação por meio do pip.
- Após a execução, ela rodou o programa mirando no ip 8.8.8.8, a execução retornou a porta 53 aberta e depois ficou rodando indefinidamente.
- A aluna questionou ao Copilot porque o processo de leitura das portas estava demorando tanto tempo.
- Como retorno a resposta foi que devido ao set_timeout era de 0.5 por porta e como a leitura é feita das 1024 primeiras portas.
- Uma recomendação dele foi reduzir o set_timeout para 0.1, mas informou que poderia resultar em em falsos negativos.
- Após rodar por um tempo o programa em uma porta que estava com toda certeza aberta, um erro depois de um tempo foi apresentado.
- Traceback (most recent call last):
  File "c:\Users\aline\Downloads\PSA\psa-project\script.py", line 62, in `<module>`
  main()

  ```^^
  File "c:\Users\aline\Downloads\PSA\psa-project\script.py", line 55, in main
  scan_services_with_nmap(target)
  ~~~~~~~~~~~~~~~~~~~~~~~^^^^^^^^
  File "c:\Users\aline\Downloads\PSA\psa-project\script.py", line 21, in scan_services_with_nmap
  nm = nmap.PortScanner()
  File "C:\Users\aline\AppData\Local\Programs\Python\Python313\Lib\site-packages\nmap\nmap.py", line 136, in_init_
  raise PortScannerError(
  f"nmap program was not found in path. PATH is : {os.getenv('PATH')}"
  )
  nmap.nmap.PortScannerError: 'nmap program was not found in path. PATH is : C:\\WINDOWS\\system32;C:\\WINDOWS;C:\\WINDOWS\\System32\\Wbem;C:\\WINDOWS\\System32\\WindowsPowerShell\\v1.0\\;C:\\WINDOWS\\System32\\OpenSSH\\;C:\\Program Files\\Git\\cmd;C:\\Users\\aline\\AppData\\Local\\Programs\\Python\\Python313\\Scripts\\;C:\\Users\\aline\\AppData\\Local\\Programs\\Python\\Python313\\;C:\\Users\\aline\\AppData\\Local\\Programs\\Python\\Launcher\\;C:\\Users\\aline\\AppData\\Local\\Microsoft\\WindowsApps;C:\\Users\\aline\\AppData\\Local\\Programs\\Microsoft VS Code\\bin;c:\\Users\\aline\\.vscode\\extensions\\ms-python.debugpy-2025.4.1-win32-x64\\bundled\\scripts\\noConfigScripts;c:\\Users\\aline\\AppData\\Roaming\\Code\\User\\globalStorage\\github.copilot-chat\\debugCommand'
  ```

- Ela selecionou o erro e questionou ao copilot o que poderia ser o problema e uma possível solução.
- Após um tempo o Copilot apresentou a necessidade de instalar o nmap no Windows também e forneceu o link.
- Após a instação do nmap o copilot instruiu a conferir se está no PATH e rodar o comando

  ```bash
  nmap --version
  ```

- O programa funcionou porém apresentou outro erro, erro esse que a aluna perguntou ao copilot o motivo.
- ~~Nota: A maior dificuldade foi com os erros de funcionamento direto na máquina.~~
- A aluna teve problemas de fazer modificação no código quando solicitado, por não entender todo o funcionamento do mesmo.
- Ao se deparar com um erro de serviços fornecidos, ela foi obrigada novamente a perguntar para o Copilot o porque do erro.
- ~~Nota: Claramente vai se tornando um efeito bolta de neve o uso de IA para gerar o código sem entendimento do seu funcionamento.~~
- Foi possível chegar a uma solução, foi necessário adaptar para IPV6 devido ao acesso ao servidor disponível e essa adaptação e integração foi feita com por meio de pedido ao Copilot "Como fazer para aceitar IPV6"
- Após isso um problema foi identificado em que o nmap não achou o serviço correto associado a porta.
- Ela voltou novamente a questionar o Copilot como resolver esse problema do serviço não ter aparecido.
- Chegou a um resultado desejado com o IPV6 funcionando e as respostas corretas da varreduras de porta.

  ```python


  import ipaddress
  import socket
  import nmap

  def scan_ports_with_socket(target):
      print("\n[+] Escaneando portas abertas com socket...")
      open_ports = []
      try:
          # Detecta automaticamente se o alvo é IPv4 ou IPv6
          family = socket.AF_INET6 if ':' in target else socket.AF_INET
          for port in range(22, 91):  # Escaneia as portas de 22 a 89
              try:
                  with socket.socket(family, socket.SOCK_STREAM) as s:
                      s.settimeout(0.5)  # Define um timeout para evitar travamentos
                      if s.connect_ex((target, port)) == 0:
                          open_ports.append(port)
                          print(f"[+] Porta {port} aberta")
              except Exception as e:
                  print(f"Erro ao escanear a porta {port}: {e}")
      except Exception as e:
          print(f"Erro ao configurar o socket: {e}")
      return open_ports

  def scan_services_with_nmap(target):
      print("\n[+] Identificando serviços ativos com nmap...")
      nm = nmap.PortScanner()
      try:
          # Adiciona o suporte a IPv6 com o argumento -6
          nm.scan(hosts=target, arguments='-sV --version-intensity 5 -p 22 -6')
          for host in nm.all_hosts():
              print(f"\nHost: {host} ({nm[host].hostname()})")
              print(f"Estado: {nm[host].state()}")
              for proto in nm[host].all_protocols():
                  print(f"\nProtocolo: {proto}")
                  ports = nm[host][proto].keys()
                  for port in ports:
                      service = nm[host][proto][port]
                      print(f"Porta: {port}\tEstado: {service['state']}\tServiço: {service['name']}")
      except Exception as e:
          print(f"Erro ao usar nmap: {e}")

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

          # Escaneia portas abertas com socket
          open_ports = scan_ports_with_socket(target)

          # Identifica serviços ativos com nmap
          if open_ports:
              scan_services_with_nmap(target)
          else:
              print("Nenhuma porta aberta encontrada para escanear serviços.")
      except ValueError:
          print("Erro: O endereço fornecido não é um IP válido.")

  if _name_ == "_main_":
      main()
  ```

### Geração do Relatório

- Ela enviou o seguinte prompt para o copilot:

  - Caso eu queira criar um arquivo .csv ou .json contendo os resultados da varredura o que eu teria que fazer
  - Resultado:

  ```python
  import json

   def save_results_to_json(filename, results):
       try:
           with open(filename, mode='w', encoding='utf-8') as file:
               json.dump(results, file, indent=4)
           print(f"Resultados salvos em {filename}")
       except Exception as e:
           print(f"Erro ao salvar resultados no JSON: {e}")
  ```

  - O copilot também reestruturou o scan_services_nmap para ficar melhor adequado para salvar no json.

  ```python
  def scan_services_with_nmap(target):
    print("\n[+] Identificando serviços ativos com nmap...")
    nm = nmap.PortScanner()
    results = []
    try:
        nm.scan(hosts=target, arguments='-sV --version-intensity 5 -p 22-89 -6')
        for host in nm.all_hosts():
            for proto in nm[host].all_protocols():
                ports = nm[host][proto].keys()
                for port in ports:
                    service = nm[host][proto][port]
                    results.append({
                        "port": port,
                        "state": service['state'],
                        "service": service['name']
                    })
    except Exception as e:
        print(f"Erro ao usar nmap: {e}")
    return results
  ```

  - Ela marcou o código e questionou o que seria o json.dump e para que serve.
  - Ao tentar rodar um erro foi apresentado, "No module nmap"
  - Ela copiou o erro apresentando e jogou no Copilot questionando o que poderia ser o erro.
  - Copilot sugeriu ela instalar o python-nmap ou dar um show pelo pip para ver se já não estava instalado.
  - Ela disse que já tinha instalado o aplicativo e já tinha instalado também pelo pip, porém o erro continuava.
  - O erro foi identificado.
  - Ela enviou o seguinte prompt

    - O resultado das portas abertas está dando a 22 e a 90, porém os resultados dos serviços ativos não batem com as portas, eles mostraram os serviços na porta 25 e 80 como fechados.
  - O copilot forneceu uma correção que deu problemas de leituras.
  - Ela enviou outro prompt

    - No caso de eu querer scanear as portas com o socket para passar a lista de portas abertas para scanear os serviços com o nmap, o que eu devo fazer ?

    ```python
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

            # Escaneia portas abertas com socket
            open_ports = scan_ports_with_socket(target)
            print(f"Portas abertas detectadas: {open_ports}")  # Debug print

            # Identifica serviços ativos com nmap
            if open_ports:
                results = scan_services_with_nmap(target, open_ports)
                print(f"Serviços identificados: {results}")  # Debug print

                # Salva os resultados em um arquivo JSON
                save_results_to_json("scan_results.json", results)
            else:
                print("Nenhuma porta aberta encontrada para escanear serviços.")
        except ValueError:
            print("Erro: O endereço fornecido não é um IP válido.")

    ```

  - Ela questionou que o arquivo .json não estava sendo criado.
  - O Copilot recomendou assegurar que a função está sendo chamada.
  - Ela rodou o código com mudanças porém começou a não identificar as portas abertas no servidor.
  - Ela solicitou novamente para gerar um código com os serviços e as portas abertas e inseriu o código diretamente.
  - O código fornecido:

    ```python
    import ipaddress

    import socket
    import nmap
    import json

    def scan_ports_with_socket(target):
        print("\n[+] Escaneando portas abertas com socket...")
        open_ports = []
        try:
            # Detecta automaticamente se o alvo é IPv4 ou IPv6
            family = socket.AF_INET6 if ':' in target else socket.AF_INET
            print(f"Usando família de socket: {'IPv6' if family == socket.AF_INET6 else 'IPv4'}")
            for port in range(22, 91):  # Escaneia as portas de 22 a 89

                try:
                    with socket.socket(family, socket.SOCK_STREAM) as s:
                        s.settimeout(0.5)  # Define um timeout para evitar travamentos
                        if s.connect_ex((target, port)) == 0:
                            open_ports.append(port)
                            print(f"[+] Porta {port} aberta")
                except Exception as e:
                    print(f"Erro ao escanear a porta {port}: {e}")
        except Exception as e:
            print(f"Erro ao configurar o socket: {e}")
        print(f"Portas abertas detectadas: {open_ports}")
        return open_ports

    def scan_services_with_nmap(target, open_ports):
        print("\n[+] Identificando serviços ativos com nmap...")
        nm = nmap.PortScanner()
        results = []
        try:
            # Convert the list of open ports to a comma-separated string
            ports_to_scan = ','.join(map(str, open_ports))
            print(f"Escaneando as portas: {ports_to_scan}")

            # Use the open ports for the Nmap scan
            nm.scan(hosts=target, arguments=f'-sV --version-intensity 5 -p {ports_to_scan}')

            for host in nm.all_hosts():
                print(f"Host: {host}")
                for proto in nm[host].all_protocols():
                    print(f"Protocolo: {proto}")
                    ports = nm[host][proto].keys()
                    for port in ports:
                        service = nm[host][proto][port]
                        results.append({
                            "port": port,
                            "state": service['state'],
                            "service": service['name']
                        })
                        print(f"Porta: {port}, Estado: {service['state']}, Serviço: {service['name']}")
        except Exception as e:
            print(f"Erro ao usar nmap: {e}")
        return results

    def save_results_to_json(filename, results):
        try:
            with open(filename, mode='w', encoding='utf-8') as file:
                json.dump(results, file, indent=4)
            print(f"Resultados salvos em {filename}")
        except Exception as e:
            print(f"Erro ao salvar resultados no JSON: {e}")

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

            # Escaneia portas abertas com socket
            open_ports = scan_ports_with_socket(target)
            print(f"Portas abertas detectadas: {open_ports}")

            # Identifica serviços ativos com nmap
            if open_ports:
                results = scan_services_with_nmap(target, open_ports)
                print(f"Serviços identificados: {results}")

                # Salva os resultados em um arquivo JSON
                save_results_to_json("scan_results.json", results)
            else:
                print("Nenhuma porta aberta encontrada para escanear serviços.")
        except ValueError:
            print("Erro: O endereço fornecido não é um IP válido.")

    if _name_ == "_main_":
        main()

    ```

    - Deu erro na leitura dos serviços
    - Nmap scan info: {'error': ['2804:360:40:2188:19ba:9b3:e4d5:fa89 looks like an IPv6 target specification -- you have to use the -6 option.\r\nWARNING: No targets were specified, so 0 hosts scanned.\r\n'], 'warning': ['WARNING: No targets were specified, so 0 hosts scanned.\r\n'], 'tcp': {'method': 'syn', 'services': '22,90'}}
    - Ela enviou o promp:

      - Como eu posso garantir que o ipv6 pode ser escaneado com os serviços de scan do nmap ?
    - Conseguiu chegar ao resultado esperado com a última correção de código:

    ```python
        import ipaddress

    import socket
    import nmap
    import json

    def scan_ports_with_socket(target):
        print("\n[+] Escaneando portas abertas com socket...")
        open_ports = []
        try:
            # Detecta automaticamente se o alvo é IPv4 ou IPv6
            family = socket.AF_INET6 if ':' in target else socket.AF_INET
            print(f"Usando família de socket: {'IPv6' if family == socket.AF_INET6 else 'IPv4'}")
            for port in range(22, 91):  # Escaneia as portas de 22 a 89

                try:
                    with socket.socket(family, socket.SOCK_STREAM) as s:
                        s.settimeout(0.5)  # Define um timeout para evitar travamentos
                        if s.connect_ex((target, port)) == 0:
                            open_ports.append(port)
                            print(f"[+] Porta {port} aberta")
                except Exception as e:
                    print(f"Erro ao escanear a porta {port}: {e}")
        except Exception as e:
            print(f"Erro ao configurar o socket: {e}")
        print(f"Portas abertas detectadas: {open_ports}")
        return open_ports

    def scan_services_with_nmap(target, open_ports):
        print("\n[+] Identificando serviços ativos com nmap...")
        nm = nmap.PortScanner()
        results = []
        try:
            # Convert the list of open ports to a comma-separated string
            ports_to_scan = ','.join(map(str, open_ports))
            print(f"Escaneando as portas: {ports_to_scan}")

            # Determine if the target is IPv6
            is_ipv6 = ':' in target
            arguments = f'-sV --version-intensity 9 -p {ports_to_scan}'
            if is_ipv6:
                arguments += ' -6'  # Add the IPv6 flag if the target is IPv6

            # Use the open ports for the Nmap scan
            print(f"Comando Nmap: {arguments}")
            nm.scan(hosts=target, arguments=arguments)

            # Debug: Print raw Nmap scan info
            print(f"Nmap scan info: {nm.scaninfo()}")
            print(f"Nmap all hosts: {nm.all_hosts()}")

            for host in nm.all_hosts():
                print(f"Host: {host}")
                for proto in nm[host].all_protocols():
                    print(f"Protocolo: {proto}")
                    ports = nm[host][proto].keys()
                    for port in ports:
                        service = nm[host][proto][port]
                        results.append({
                            "port": port,
                            "state": service['state'],
                            "service": service['name']
                        })
                        print(f"Porta: {port}, Estado: {service['state']}, Serviço: {service['name']}")
        except Exception as e:
            print(f"Erro ao usar nmap: {e}")
        return results

    def save_results_to_json(filename, results):
        try:
            with open(filename, mode='w', encoding='utf-8') as file:
                json.dump(results, file, indent=4)
            print(f"Resultados salvos em {filename}")
        except Exception as e:
            print(f"Erro ao salvar resultados no JSON: {e}")

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

            # Escaneia portas abertas com socket
            open_ports = scan_ports_with_socket(target)
            print(f"Portas abertas detectadas: {open_ports}")

            # Identifica serviços ativos com nmap
            if open_ports:
                results = scan_services_with_nmap(target, open_ports)
                print(f"Serviços identificados: {results}")

                # Salva os resultados em um arquivo JSON
                save_results_to_json("scan_results.json", results)
            else:
                print("Nenhuma porta aberta encontrada para escanear serviços.")
        except ValueError:
            print("Erro: O endereço fornecido não é um IP válido.")

    if _name_ == "_main_":
        main()

    ```

### Menu Interativo

- Prompt :

  - Caso eu queira adicionar um menu interativo com as opções "Escaneamento rápido" que escaneie rapidamente e "Escaneamento detalhado" que seja mais detalhado, o que devo fazer?
- Resultado:

  ```python
  def main():
      while True:
          # Display the menu
          print("\n=== Menu de Escaneamento ===")
          print("1. Escaneamento rápido")
          print("2. Escaneamento detalhado")
          print("3. Sair")

          # Get the user's choice
          choice = input("Escolha uma opção: ").strip()

          if choice == "1":
              print("\n[+] Você escolheu: Escaneamento rápido")
              quick_scan()
          elif choice == "2":
              print("\n[+] Você escolheu: Escaneamento detalhado")
              detailed_scan()
          elif choice == "3":
              print("\n[+] Saindo do programa. Até logo!")
              break
          else:
              print("\n[!] Opção inválida. Tente novamente.")

  def quick_scan():
      # Perform a quick scan
      target = input("\nDigite o endereço IP para escanear (rápido): ").strip()
      if not validate_ip(target):
          return
      open_ports = scan_ports_with_socket(target)
      print(f"Portas abertas detectadas (rápido): {open_ports}")

  def detailed_scan():
      # Perform a detailed scan
      target = input("\nDigite o endereço IP para escanear (detalhado): ").strip()
      if not validate_ip(target):
          return
      open_ports = scan_ports_with_socket(target)
      print(f"Portas abertas detectadas (detalhado): {open_ports}")
      if open_ports:
          results = scan_services_with_nmap(target, open_ports)
          print(f"Serviços identificados (detalhado): {results}")
          save_results_to_json("scan_results_detailed.json", results)
      else:
          print("Nenhuma porta aberta encontrada para escanear serviços.")

  def validate_ip(target):
      # Validate the IP address
      try:
          ipaddress.ip_address(target)
          print(f"Alvo recebido: {target}")
          return True
      except ValueError:
          print("Erro: O endereço fornecido não é um IP válido.")
          return False

    ```

  - Ela solicitou que ao selecionar o escaneamento rápido ele também salve no arquivo json o resultado.

### Código Final

```python

    import ipaddress
    import socket
    import nmap
    import json

    def scan_ports_with_socket(target):
        print("\n[+] Escaneando portas abertas com socket...")
        open_ports = []
        try:
            # Detecta automaticamente se o alvo é IPv4 ou IPv6
            family = socket.AF_INET6 if ':' in target else socket.AF_INET
            print(f"Usando família de socket: {'IPv6' if family == socket.AF_INET6 else 'IPv4'}")
            for port in range(22, 91):  # Escaneia as portas de 22 a 89

                try:
                    with socket.socket(family, socket.SOCK_STREAM) as s:
                        s.settimeout(0.5)  # Define um timeout para evitar travamentos
                        if s.connect_ex((target, port)) == 0:
                            open_ports.append(port)
                            print(f"[+] Porta {port} aberta")
                except Exception as e:
                    print(f"Erro ao escanear a porta {port}: {e}")
        except Exception as e:
            print(f"Erro ao configurar o socket: {e}")
        print(f"Portas abertas detectadas: {open_ports}")
        return open_ports

    def scan_services_with_nmap(target, open_ports):
        print("\n[+] Identificando serviços ativos com nmap...")
        nm = nmap.PortScanner()
        results = []
        try:
            # Convert the list of open ports to a comma-separated string
            ports_to_scan = ','.join(map(str, open_ports))
            print(f"Escaneando as portas: {ports_to_scan}")

            # Determine if the target is IPv6
            is_ipv6 = ':' in target
            arguments = f'-sV --version-intensity 9 -p {ports_to_scan}'
            if is_ipv6:
                arguments += ' -6'  # Add the IPv6 flag if the target is IPv6
          
            # Use the open ports for the Nmap scan
            print(f"Comando Nmap: {arguments}")
            nm.scan(hosts=target, arguments=arguments)
          
            # Debug: Print raw Nmap scan info
            print(f"Nmap scan info: {nm.scaninfo()}")
            print(f"Nmap all hosts: {nm.all_hosts()}")
          
            for host in nm.all_hosts():
                print(f"Host: {host}")
                for proto in nm[host].all_protocols():
                    print(f"Protocolo: {proto}")
                    ports = nm[host][proto].keys()
                    for port in ports:
                        service = nm[host][proto][port]
                        results.append({
                            "port": port,
                            "state": service['state'],
                            "service": service['name']
                        })
                        print(f"Porta: {port}, Estado: {service['state']}, Serviço: {service['name']}")
        except Exception as e:
            print(f"Erro ao usar nmap: {e}")
        return results

    def save_results_to_json(filename, results):
        try:
            with open(filename, mode='w', encoding='utf-8') as file:
                json.dump(results, file, indent=4)
            print(f"Resultados salvos em {filename}")
        except Exception as e:
            print(f"Erro ao salvar resultados no JSON: {e}")

    def main():
        while True:
            # Display the menu
            print("\n=== Menu de Escaneamento ===")
            print("1. Escaneamento rápido")
            print("2. Escaneamento detalhado")
            print("3. Sair")

            # Get the user's choice
            choice = input("Escolha uma opção: ").strip()
          
            if choice == "1":
                print("\n[+] Você escolheu: Escaneamento rápido")
                quick_scan()
            elif choice == "2":
                print("\n[+] Você escolheu: Escaneamento detalhado")
                detailed_scan()
            elif choice == "3":
                print("\n[+] Saindo do programa. Até logo!")
                break
            else:
                print("\n[!] Opção inválida. Tente novamente.")

    def quick_scan():
        # Perform a quick scan
        target = input("\nDigite o endereço IP para escanear (rápido): ").strip()
        if not validate_ip(target):
            return
        open_ports = scan_ports_with_socket(target)
        print(f"Portas abertas detectadas (rápido): {open_ports}")

        # Save the results to a JSON file
        if open_ports:
            results = [{"port": port, "state": "open"} for port in open_ports]
            save_results_to_json("scan_results.json", results)
        else:
            print("Nenhuma porta aberta encontrada para salvar no arquivo JSON.")

    def detailed_scan():
        # Perform a detailed scan
        target = input("\nDigite o endereço IP para escanear (detalhado): ").strip()
        if not validate_ip(target):
            return
        open_ports = scan_ports_with_socket(target)
        print(f"Portas abertas detectadas (detalhado): {open_ports}")
        if open_ports:
            results = scan_services_with_nmap(target, open_ports)
            print(f"Serviços identificados (detalhado): {results}")
            save_results_to_json("scan_results_detailed.json", results)
        else:
            print("Nenhuma porta aberta encontrada para escanear serviços.")

    def validate_ip(target):
        # Validate the IP address
        try:
            ipaddress.ip_address(target)
            print(f"Alvo recebido: {target}")
            return True
        except ValueError:
            print("Erro: O endereço fornecido não é um IP válido.")
            return False

    if _name_ == "_main_":
        main()
```
