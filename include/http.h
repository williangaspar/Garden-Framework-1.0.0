#ifndef  __HTTP_H__
#define  __HTTP_H__

#include <sys/types.h>

#include <stdio.h>
#include <errno.h>
#include <string.h>

#include <attrib.h>

#ifdef __cplusplus
extern "C" {
#endif

#define HTTP_KEEPALIVE_TIME	20
#define HTTP_HSTS_ENABLE	31536000
#define HTTP_HEADER_MAX_LEN	(80*1024)
#define HTTP_BODY_MAX_LEN	(1<<30)
#define HTTP_URI_LEN		2048
#define HTTP_USERAGENT_LEN	256
#define HTTP_REQ_HEADER_MAX	25
#define HTTP_MAX_QUERY_ARGS	20
#define HTTP_MAX_COOKIES	10
#define HTTP_REQUEST_LIMIT	1000

enum HTTP_ARG_TYPE{
	HTTP_TYPE_RAW,
	HTTP_TYPE_BYTE,
	HTTP_TYPE_INT16,
	HTTP_TYPE_UINT16,
	HTTP_TYPE_INT32,
	HTTP_TYPE_UINT32,
	HTTP_TYPE_STRING,
	HTTP_TYPE_INT64,
	HTTP_TYPE_UINT64
};

enum HTTP_STATE{
	HTTP_STATE_ERROR,
	HTTP_STATE_CONTINUE,
	HTTP_STATE_COMPLETEN,
	HTTP_STATE_RETRY
};

enum HTTP_METHOD{
	HTTP_METHOD_GET,
	HTTP_METHOD_POST,
	HTTP_METHOD_PUT,
	HTTP_METHOD_DELETE,
	HTTP_METHOD_HEAD
}

enum HTTP_REQUEST{
	HTTP_REQUEST_COMPLETE		M_BYTE(0),
	HTTP_REQUEST_DELETE		M_BYTE(1),
	HTTP_REQUEST_SLEEPING		M_BYTE(2),
	HTTP_REQUEST_EXPECT_BODY	M_BYTE(3),
	HTTP_REQUEST_NO_CONTENT_LENGTH	M_BYTE(4),
	HTTP_REQUEST_AUTHED		M_BYTE(5)
};

enum HTTP_STATUS_CODE{
	HTTP_STATUS_CONTINUE			100,
	HTTP_STATUS_SWITCHING_PROTOCOLS		101,
	HTTP_STATUS_OK				200,
	HTTP_STATUS_CREATED			201,
	HTTP_STATUS_ACCEPTED			202,
	HTTP_STATUS_NON_AUTHORITATIVE		203,
	HTTP_STATUS_NO_CONTENT			204,
	HTTP_STATUS_RESET_CONTENT		205,
	HTTP_STATUS_PARTIAL_CONTENT		206,
	HTTP_STATUS_MULTIPLE_CHOICES		300,
	HTTP_STATUS_MOVED_PERMANENTLY		301,
	HTTP_STATUS_FOUND			302,
	HTTP_STATUS_SEE_OTHER			303,
	HTTP_STATUS_NOT_MODIFIED		304,
	HTTP_STATUS_USE_PROXY			305,
	HTTP_STATUS_TEMPORARY_REDIRECT		307,
	HTTP_STATUS_BAD_REQUEST			400,
	HTTP_STATUS_UNAUTHORIZED		401,
	HTTP_STATUS_PAYMENT_REQUIRED		402,
	HTTP_STATUS_FORBIDDEN			403,
	HTTP_STATUS_NOT_FOUND			404,
	HTTP_STATUS_METHOD_NOT_ALLOWED		405,
	HTTP_STATUS_NOT_ACCEPTABLE		406,
	HTTP_STATUS_PROXY_AUTH_REQUIRED		407,
	HTTP_STATUS_REQUEST_TIMEOUT		408,
	HTTP_STATUS_CONFLICT			409,
	HTTP_STATUS_GONE			410,
	HTTP_STATUS_LENGTH_REQUIRED		411,
	HTTP_STATUS_PRECONDITION_FAILED		412,
	HTTP_STATUS_REQUEST_ENTITY_TOO_LARGE	413,
	HTTP_STATUS_REQUEST_URI_TOO_LARGE	414
	HTTP_STATUS_UNSUPPORTED_MEDI_TYPE	415,
	HTTP_STATUS_REQUEST_RANGE_INVELID	416,
	HTTP_STATUS_EXPECTATION_FAILED		417,
	HTTP_STATUS_INTERNAL_ERROR		500,
	HTTP_STATUS_NOT_IMPLEMENTED		501,
	HTTP_STATUS_BAD_GATEWAY			502,
	HTTP_STATUS_SERVICE_UNAVAILABLE		503,
	HTTP_STATUS_GATEWAY_TIMEOUT		504,
	HTTP_STATUS_BAD_VERSION			505
};

struct http_header{
	char *header;
	char *value;
};

struct http_form{
	char *field;
	char *value;
};

struct http_request{
	u_int8_t		method;
	u_int8_t		state;
	u_int16_t		flags;
	u_int16_t		status;
	const char *		host;
	const char *		agent;
	size_t			body_length;
	size_t			header_length;
	size_t			content_length;
	struct http_header	*header;
	struct http_form	*form_get;
	struct http_form	*form_submit;
	struct connection	*conn;
};

struct http_file{
	char	*name;
	char	*f_name;
	size_t	position;
	size_t	offset;
	size_t	length;
	struct	http_request *request;
};

struct http_response{
	int (*header_stat)(struct http_request *, int, const void *);
	int (*send_content)(struct http_request *, const char *);
	int (*send_file)(struct http_request *, FILE *);
};

void http_request_new(struct http_request *, int);
void http_request_header(struct http_request *req);
void http_request_free(struct http_request *);

int http_response_stat(struct http_request *, int, const void *);
int http_response_send_content(struct http_request *, const char *);
int http_response_send_file(struct http_request *, FILE *);

#ifdef __cplusplus
}
#endif

#endif  /*HTTP_H*/
