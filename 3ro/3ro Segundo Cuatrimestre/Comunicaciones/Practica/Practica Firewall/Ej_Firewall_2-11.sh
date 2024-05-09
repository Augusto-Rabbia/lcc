#! /bin/bash
SERVERS=10.0.2.0/24
WWw=10.0.2.3
LAN=10.0.1.0/24
VISITA=192.168.1.0/24
ADM=10.0.1.22/24
I=/sbin/iptables
NLAN=200.3.1.2
NVISITA=200.3.200.2
ILAN=eth0
ISERV=eth1
IINT=eth2
IVIS=eth3

# Carga reglas
# Descartamos paquetes invalidos.
$I -A FORWARD -m state --state INVALID -j DROP
$I -A FORWARD -m state --state RELATED,ESTABLISHED -j ACCEPT

# PC de LAN puede acceder a ciertos servicios de red de servidores.
# PC del admin.

# Servidores no pueden acceder a nada                               |
#                                                                   |
# Nadie más puede acceder a red de servidores.                      |= # Nadie más puede acceder a nada   
#                                                                   |
# Nadie más puede acceder a la LAN (ni servidores ni visitas)       |

# PC de la LAN puede navegar a Internet.

# Visitas pueden navegar a Internet y nada más.

# -------------------------Tabla Filter----------------------------

# PC lan acceso a servicios (solo web)
$I -A FORWARD -i $ILAN -s $LAN -d $WWW -p tcp -m multiport --dports 53,80,443\
            -j ACCEPT # misma linea
# DNS por udp
$I -A FORWARD -i $ILAN -s $LAN -d $WWW -p udp --dport 53 -j ACCEPT

# PC del admin
$I -A FORWARD -i $ILAN -s $ADM -d $SERVERS -p tcp --dport 22 -j ACCEPT

# Acceso PC LAN a Internet
$I -A FORWARD -i $ILAN -s $LAN -o $IINT -j ACCEPT


# Acceso visitas a Internet
$I -A FORWARD -i $IVIS -s $VISITA -o $IINT -j ACCEPT

# Prohibo el resto
# Alternativa 1
$I -P FORWARD DROP

# Alternativa 2
$I -A FORWARD -j DROP

# -------------------------Tabla NAT----------------------------


# PC de la LAN
$I -t nat -A POSTROUTING -s $LAN -o $IINT -j SNAT --to $NLAN

# Visitas
$I -t nat -A POSTROUTING -s $VISITA -o $IINT -j SNAT --to $NVISITA

# FIN
