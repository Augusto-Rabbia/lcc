WWW=181.16.1.18
PROXY=181.16.1.19
DMZ=181.16.1.16/28
LAN=10.0.1.0/24
ADM=10.0.1.22
IF_DMZ=eth1
IF_LAN=eth0
IF_INT=eth2
IP=200.3.1.2
I=/sbin/iptables


### TABLA FILTER
### CADENAS INPUT Y OUTPUT
$I -A OUTPUT -m state --state INVALID -j DROP
$I -A OUTPUT -m state --state ESTABLISHED,RELATED -j ACCEPT

$I -A OUTPUT -d $PROXY -p tcp --dport 3128 -j ACCEPT

$I -A INPUT -m state --state INVALID -j DROP
$I -A INPUT -m state --state ESTABLISHED,RELATED -j ACCEPT

$I -A INPUT -s $ADM -i $IF_LAN -p tcp --dport ssh -j ACCEPT
#### CADENA FORWARD
$I -A FORWARD -i $IF_LAN -d $WWW -p tcp --dports 80,443,ssh -j ACCEPT
$I -A FORWARD -i $IF_LAN -d $WWW -p udp --dports 53 -j ACCEPT


$I -A FORWARD -i $IF_LAN -d $PROXY -p tcp --dports 3128,ssh -j ACCEPT
$I -A FORWARD -i $IF_LAN -d $PROXY -p udp --dports 53 -j ACCEPT

$I -A FORWARD -i $IF_LAN -o $IF_INT -p tcp -m multiport --dports 80,443 -j DROP
$I -A FORWARD -i $IF_LAN -o $IF_INT -j ACCEPT

$I -A FORWARD -i $IF_DMZ -o $IF_INT -p tcp -m multiport --dports 80,443 -j ACCEPT
$I -A FORWARD -i $IF_DMZ -o $IF_INT -p udp --dport 53 -j ACCEPT


$I -A FORWARD -i $IF_INT -p $IF_DMZ -p udp --dport 53 -j ACCEPT
$I -A FORWARD -i $IF_INT -p $IF_DMZ -p tcp -m multiport --dports 80,443 -j ACCEPT


$I -t nat -A POSTROUTING -s $LAN -o $IF_INT -j SNAT --to $IP