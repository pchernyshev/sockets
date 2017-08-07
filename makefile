CFLAGS = -Wall -Werror
LDFLAGS = -lpthread

all: tcp udp

tcp: tcp_server_echo tcp_client_echo # tcp_server_mult
udp: udp_server_echo udp_client_echo

%: %.o
