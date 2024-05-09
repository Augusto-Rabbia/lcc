#!/bin/sh
F=/sbin/iptables

LAN=10.23.0.0/16

DMZ=10.23.0.0/16
WEBSV=10.1.1.2
MAILRELAY=10.1.1.3

SERVERS=10.23.0.0/16
DBSV=10.10.1.2
MAILSV=10.10.1.3

$IP_EXTERNA=200.123.131.112

# Por omision se asume -t filter
# 8) Solo se puede acceder al firewall desde PC del admin(10.23.23.5) con ssh(22/tcp)
# y el firewal solo puede consultar dns(53/tcp-udp) en MAILRELAY
$F -P INPUT DROP
$F -P OUTPUT DROP
$F -A INPUT -i eth2 -src 10.23.23.5 --dport 22 -p tcp -j ACCEPT
$F -A OUTPUT -o eth3 -dst $MAILRELAY -p udp --dport 53 -j ACCEPT
$F -A OUTPUT -o eth3 -dst $MAILRELAY -p tcp --dport 53 -j ACCEPT
# 9) Nada mas puede pasar por el weon ql
$F -P FORWARD DROP

# 1) LAN tiene acceso a MAILSV desde esos puertos tcp, a DBSV con puerto https(443), y a MAILRELAY en DMZ para DNS(53/udp)
$F -A FORWARD -m multiport -i eth2 -src $LAN -dst $MAILSV -p tcp -dports imap,imaps,pop,pop3s,smtp,smtp-ssl -j ACCEPT
$F -A FORWARD -i eth2 -src $LAN -dst $DBSV -p tcp --dport 443 -j ACCEPT
$F -A FORWARD -i eth2 -src $LAN -dst $MAILRELAY -p udp --dport 53 -j ACCEPT

# 2) MAILSV tiene acceso a DNS(53/udp) y al puerto 465/tcp de MAILRELAY
# (no se puede multiport pq es uno udp el otro tcp)
$F -A FORWARD -i eth1 -src $MAILSV -p tcp --dport 465 -j ACCEPT
$F -A FORWARD -i eth1 -src $MAILSV -p udp --dport 53 -j ACCEPT

# 3) MAILRELAY acceso a 465/tcp de MAILSV
$F -A FORWARD -i eth3 -src $MAILRELAY -dst $MAILSV -p tcp --dport 465 -j ACCEPT

# 4) WEBSV acceso a 3306/tcp de DBSV
$F -A FORWARD -i eth3 -src $WEBSV -p tcp --dport 3306 -j ACCEPT

# 5) Las PCs de LAN pueden navegar internec
$F -A FORWARD -i eth2 -src $LAN -o eth0 -j ACCEPT
# Natteo para que puedan acceder
$F -t nat -A POSTROUTING -src $LAN -o eth0 -j SNAT -to $IP_EXTERNA

# 6) Internet puede acceder MAILRELAY (53,25/tcp-udp), WEBSV (80,443/tcp)
# MAILRELAY
$F -A FORWARD -m multiport -i eth0 -dst $MAILRELAY -p tcp -dports 53,25 -j ACCEPT
$F -A FORWARD -m multiport -i eth0 -dst $MAILRELAY -p udp -dports 53,25 -j ACCEPT
# WEBSV
$F -A FORWARD -i eth0 -p tcp -m multiport -dst $WEBSV -dports 80,443 -j ACCEPT

# 7) DMZ puede consultar DNS(53/tcp-udp) a Internet y MAILRELAY puede enviar correo a Internet (25/tcp)
$F -A FORWARD -i eth3 -p udp --dport 53 -o eth0 -j ACCEPT
$F -A FORWARD -i eth3 -p tcp --dport 53 -o eth0 -j ACCEPT
$F -A FORWARD -i eth3 -src $MAILRELAY -p tcp --dport 25 -o eth0 -j ACCEPT
# Natteo para que puedan acceder
$F -t nat -A POSTROUTING -src $DMZ -o eth0 -j SNAT -to $IP_EXTERNA














