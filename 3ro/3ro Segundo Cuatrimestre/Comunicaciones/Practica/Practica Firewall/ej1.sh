#! /bin/bash

LAN=10.23.0.0/16
DMZ=10.1.1.0/29
WwW=10.1.1.2
MREL=10.1.1.3
SERVERS=10.10.0.0/23
DBSER=10.10.1.2
MAIL=10.10.1.3
IP=200.123.131.112
I=/sbin/iptables

### TABLA FILTER

# INPUT Y OUTPUT
$I -P INPUT DROP
$I -P OUTPUT DROP

$I -A OUTPUT -m state --state INVALID -j DROP
$I -A OUTPUT -m state --states ESTABLISHED,RELATED -j ACCEPT


$I -A INPUT -m state --state INVALID -j DROP
$I -A INPUT -m state --states ESTABLISHED,RELATED -j ACCEPT

$I -A INPUT -s 10.23.23.5 -i eth2 -p tcp --dport 22 -j ACCEPT

$I -A OUTPUT -d $MREL -p tcp,udp --dport 53 -j ACCEPT
## FORWARD
$I -A FORWARD -m state --state INVALID -j DROP
$I -A FORWARD -m state --states ESTABLISHED,RELATED -j ACCEPT


$I -A FORWARD -s $LAN -d $MAIL -p tcp --dports 25,465,110,995,993,143 -j ACCEPT

# y a la aplicacion web de negocios en el dbserver por https.

$I -A FORWARD -s $LAN -d $DBSER -p tcp --dport 443 -j ACCEPT
# Lam accede a DNS en DMZ (mail relay)
$I -A FORWARD -s $LAN -d $MREL -p udp --dport 53 -j ACCEPT 

# EL mail tiene acceso al dns
$I -A FORWARD -s $MAIL -i eth1 -d $MREL -p udp --dport 53 -j ACCEPT


$I -A FORWARD -s $MAIL -i eth1 -d $MREL -p tcp --dport 465 -j ACCEPT


$I -A FORWARD -s $MREL -i eth3 -d $MAIL -p tcp --dport 465 -j ACCEPT


$I -A FORWARD -s $WWW -i eth3 -d $DBSER -p tcp --dport 3306 -j ACCEPT


$I -A FORWARD -s $LAN -i eth2 -o $eth0 -j ACCEPT


$I -A FORWARD -d $MREL -i eth0 -p udp --dport 53 -j ACCEPT


$I -A FORWARD -d $MREL -i eth0 -p tdp --dport 25 -j ACCEPT


$I -A FORWARD -d $WWW -i eth0 -m multiport -p tcp --dports 80,443 -j ACCEPT
# Los servidores de la DMZ pueden consultar DNS a internet y el RELAY puede enviar correo a Internet a traves del puerto 25/tcp
$I -A FORWARD -s $DMZ -o eth0 -p udp --dport 53 -j ACCEPT

$I -A FORWARD -s $MREL -i eth3 -o eth0 -p tcp --dport 25 -j ACCEPT