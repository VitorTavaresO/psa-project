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

if __name__ == "__main__":
    main()