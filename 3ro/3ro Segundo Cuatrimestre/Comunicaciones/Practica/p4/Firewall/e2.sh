#! /bin/sh
F=/sbin/iptables

DMZ=181.16.1.16/28
WWW=181.16.1.18/28
PROXY=181.16.1.19/28

LAN=10.0.1.0/24
ADMINPC=10.0.1.22

IP_EXTERNA=200.3.1.2

$F -P FORWARD DROP # generalmente siempre hacer
$F -P INPUT DROP # usado en 1)
$F -P OUTPUT DROP # tambien util para 6)

# 1) Solo ADMIN puede acceder ssh(22/tcp) PROXY, WWW y el firewall
$F -A FORWARD -i eth0 -src $ADMIN -p tcp --dport 22 -dst $WWW -j ACCEPT
$F -A FORWARD -i eth0 -src $ADMIN -p tcp --dport 22 -dst $PROXY -j ACCEPT
$F -A INPUT -i eth0 -src $ADMIN -p tcp --dport 22 -j ACCEPT











# 2) PCs de LAN pueden acceder a los demás servicios de DMZ 
# - WWW: web (http/https: 80,443/tcp) y DNS (53/udp-tcp)
# - PROXY: (3128/tcp) y DNS (53/udp-tcp)
# TCP a WWW
$F -A FORWARD -m multiport -i eth0 -src $LAN -dst $WWW -p tcp -dports 53,80,443 -j ACCEPT
# UDP a WWW
$F -A FORWARD -i eth0 -src $LAN -dst $WWW -p udp --dport 53 -j ACCEPT
# TCP a PROXY
$F -A FORWARD -m multiport -i eth0 -src $LAN -dst $PROXY -p tcp -dports 53,3128 -j ACCEPT
# UDP a PROXY
$F -A FORWARD -i eth0 -src $LAN -dst $PROXY -p udp --dport 53 -j ACCEPT
