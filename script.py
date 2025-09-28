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