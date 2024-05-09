!/bin/bash
ADM=168.96.249.11
VM1=45.10.234.66
VM2=45.10.234.67
HOST1=10.233.0.2
RINT=45.10.234.70
REXT=168.96.210.22
I=/sbin/iptables

# Carga reglas
# Descartamos paquetes invalidos.
$I -A FORWARD -m state --state INVALID -j DROP
$I -A FORWARD -m state --state RELATED,ESTABLISHED -j ACCEPT


# Permito conectar a los servidores VM1 desde cualquier lugar
$I -A FORWARD -d $VM1 -p tcp -m multiport --dports 80,443,53 -j ACCEPT

$I -A FORWARD -d $VM1 -p udp --dport 53 -j ACCEPT


# Permito conectar a los servidores VM2 desde cualquier lugar
$I -A FORWARD -d $VM2 -p tcp -m multiport --dports 80,81,443,53 -j ACCEPT

$I -A FORWARD -d $VM2 -p udp --dport 53 -j ACCEPT

# Permito al admin acceder mediante puerto tcp 22(ssh) a las VM
$I -A FORWARD -d $VM1 -p tcp -s $ADM --dport 22 -j ACCEPT
$I -A FORWARD -d $VM2 -p tcp -s $ADM --dport 22 -j ACCEPT


# Permito a las VM acceder a cualquier lugar desde los puertos especificados
$I -A FORWARD -s $VM1 -p tcp -m multiport --dports 53,80,443 -j ACCEPT
$I -A FORWARD -d $VM1 -p udp --dport 53 -j ACCEPT

$I -A FORWARD -s $VM2 -p tcp -m multiport --dports 53,80,443 -j ACCEPT
$I -A FORWARD -d $VM2 -p udp --dport 53 -j ACCEPT


# Configuración para el host
$I -A FORWARD -p tcp -d $HOST1 -s $ADM -m multiport --dports 22,8006 -j ACCEPT

$I -A FORWARD -j DROP

# NAT

# No usamos nombres de interfaz para mostrar otra forma

$I -t nat -A PREROUTING -s $HOST1 -d 45.10.234.64/29 -j ACCEPT

$I -t nat -A PREROUTING -s $HOST1 -j SNAT --to IP_EXT

$I -t nat -A PREROUTING -p tcp -d 45.10.234.70 -m multiport --dports 22,8006\
        -j DNAT -to $HOST1
