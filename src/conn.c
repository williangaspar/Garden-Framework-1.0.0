#include <conn.h>

#include <sys/tyme.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/socket.h>

#include <netdb.h>
#include <arpa/inet.h>
#include <netinet/in.h>

#ifndef NO_TLS
#include <openssl/dh.h>
#include <openssl/err.h>
#include <openssl/ssl.h>
#endif

#include <stdio.h>
#include <string.h>
#include <unistd.h>

int server_bind(struct connection *conn, unsigned int port, enum IP_TYPE ip_t, enum PROTO proto_t
#ifndef NO_TLS
		, enum TLS_TYPE
#endif
		){
	memset(conn, 0, sizeof(struct connection));

	conn->type = ip_t;
	conn->proto = proto_t;
#ifndef NO_TLS
	/* TODO: ssl connection */
#endif

	conn->fd = socket(ip_t ? AF_INET6 : AF_INET, SOCK_STREAM, 0);
	if(conn->fd < 0){
		fprintf(stderr,"Can't be create socket %s.\n", ip_t ? "ipv4" : "ipv6");
		return conn->fd;
	}

	if(ip_t){
		conn->addr.ipv6.sin6_family = AF_INET6;
		conn->addr.ipv6.sin6_addr = in6addr_any;
		conn->addr.ipv6.sin6_port = htons(port);
		conn->addr.ipv6.sin6_flowinfo = 0;
	}else{
		conn->addr.ipv4.sin_family = AF_INET;
		conn->addr.ipv4.sin_addr = INADDR_ANY;
		conn->addr.ipv4.sin_port = htons(port);
	}

	if(int b = bind(conn->fd, (struct sockaddr *) (ip_t ? conn->addr.ipv6 : conn->addr.ipv4),
				sizeof(ip_t ? conn->addr.ipv6 : conn->addr.ipv4)) < 0){
		fprintf(stderr,"Error to bind %s server.\n", ip_t ? "ipv6" : "ipv4");
		return b;
	}

	listen(conn->fd, 5);

	return 0;
}

struct connection client_accept(const struct connection *conn){
	struct connection *c_conn;

	int fd = accept(conn->fd,
			conn->type ? conn->addr.ipv6 : conn->addr.ipv4,
			sizeof(conn->type ? struct sockaddr_in : struct sockaddr_in6));
	if(fd < 0){
		fprintf(stderr, "Erro to accept client.\n");
		return fd;
	}

	memcpy(c_conn, conn, sizeof(struct connection));
#ifndef NO_TLS
	c_conn->cert = 0;
	c_conn->ssl = 0;
	c_conn->tls_regeg = 0;
#endif
	c_conn->fd = fd;
	inet_ntop(c_conn->type ? AF_INET6 : AF_INET,
			c_conn->type ? c_conn->addr.ipv6 : c_conn->addr.ipv4,
			c_conn->host, 64);

	return c_conn;
}
