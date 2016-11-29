#ifndef  __CONN_H__
#define  __CONN_H__

#include <sys/time.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/socket.h>

#include <netdb.h>
#include <arpa/inet.h>
#include <netinet/in.h>

#ifndef NO_TLS
#include <openssl/err.h>
#include <openssl/dh.h>
#include <openssl/ssl.h>
#endif

#include <errno.h>
#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <syslog.h>

#ifdef __cplusplus
extern "C" {
#endif

#define NETBUF_RECV		0
#define NETBUF_SEND		1
#define NETBUF_SEND_PAYLOAD_MAX	8192

#define NETBUF_LAST_CHAIN	0
#define NETBUF_BEFORE_CHAIN	1

#define NETBUF_CALL_CB_ALWAYS	0x01
#define NETBUF_FORCE_REMOVE	0x02
#define NETBUF_MUST_RESEND	0x04
#define NETBUF_IS_STREAM	0x10

enum PROTO{
	PROTO_UNKNOWN,
	PROTO_HTTP,
	PROTO_WEBSOCKET,
	PROTO_MSG,
};

#ifndef NO_TLS
enum TLS_TYPE{
	TLS_NO_USE,
	TLS_USE
};
#endif

struct connection{
	u_int8_t	type;
	u_int8_t	state;
	u_int8_t	proto;
	u_int8_t	flags;
#ifndef	NO_TLS
	x509		*cert;
	SSL		*ssl;
	int		tls_reneg;
#endif
	union{
		struct sockaddr_in	ipv4;
		struct sockaddr_in6	ipv6;
	} addr;
	void		ws;
	int		fd;
};

int server_bind(unsigned int
#ifndef NO_TLS
		, enum TLS_TYPE
#endif
		);

#ifdef __cplusplus
}
#endif

#endif  /*CONN_H*/
