#include <ngx_config.h>
#include <ngx_core.h>
#include <ngx_http.h>

/* 
 * Declare variable for hold data
 */
typedef struct {
    ngx_str_t   message;
} ngx_http_print_me_loc_conf_t;



/* 
 * Scan Identefier from configuration file
 */
static ngx_command_t ngx_http_print_me_commands[] = {
    { ngx_string("print_me"),
      NGX_HTTP_LOC_CONF|NGX_CONF_TAKE1,
      ngx_conf_set_str_slot,
      NGX_HTTP_LOC_CONF_OFFSET,
      offsetof(ngx_http_print_me_loc_conf_t, message),
      &ngx_http_print_me_p },
 
    ngx_null_command
};

/* The function which initializes memory for the module configuration structure
 * and Call from Context hooks       
 */
static void *
ngx_http_print_me_create_loc_conf(ngx_conf_t *cf)
{
    ngx_http_print_me_loc_conf_t  *conf;

    conf = ngx_pcalloc(cf->pool, sizeof(ngx_http_print_me_loc_conf_t));
    if (conf == NULL) {
        return NULL;
    }
    
    return conf;
}

/*
 * The module context has hooks , here we have a hook for creating
 * location configuration
 */
static ngx_http_module_t ngx_http_print_me_module_ctx = {
    NULL,                          /* preconfiguration */
    NULL,                          /* postconfiguration */
 
    NULL,                          /* create main configuration */
    NULL,                          /* init main configuration */
 
    NULL,                          /* create server configuration */
    NULL,                          /* merge server configuration */
 
    ngx_http_print_me_create_loc_conf, /* create location configuration */
    NULL                           /* merge location configuration */
};


/*
 * The module which binds the context and commands 
 * 
 */
ngx_module_t ngx_http_print_me_module = {
    NGX_MODULE_V1,
    &ngx_http_print_me_module_ctx,    /* module context */
    ngx_http_print_me_commands,       /* module directives */
    NGX_HTTP_MODULE,               /* module type */
    NULL,                          /* init master */
    NULL,                          /* init module */
    NULL,                          /* init process */
    NULL,                          /* init thread */
    NULL,                          /* exit thread */
    NULL,                          /* exit process */
    NULL,                          /* exit master */
    NGX_MODULE_V1_PADDING
};


/*
 * Main handler function of the module. 
 */
static ngx_int_t
ngx_http_print_me_handler(ngx_http_request_t *r)
{
    ngx_int_t    rc;
    ngx_buf_t   *b;
    ngx_chain_t  out;
 
    /* we response to 'GET' and 'HEAD' requests only */
    if (!(r->method & (NGX_HTTP_GET|NGX_HTTP_HEAD))) {
        return NGX_HTTP_NOT_ALLOWED;
    }
 
    /* discard request body, since we don't need it here */
    rc = ngx_http_discard_request_body(r);
 
    if (rc != NGX_OK) {
        return rc;
    }
 
    /* set the 'Content-type' header */
    r->headers_out.content_type_len = sizeof("text/html") - 1;
    r->headers_out.content_type.len = sizeof("text/html") - 1;
    r->headers_out.content_type.data = (u_char *) "text/html";
 
    /* send the header only, if the request type is http 'HEAD' */
    if (r->method == NGX_HTTP_HEAD) {
        r->headers_out.status = NGX_HTTP_OK;
        r->headers_out.content_length_n = hello_string.len;
 
        return ngx_http_send_header(r);
    }
 
    /* allocate a buffer for your response body */
    b = ngx_pcalloc(r->pool, sizeof(ngx_buf_t));
    if (b == NULL) {
        return NGX_HTTP_INTERNAL_SERVER_ERROR;
    }
 
    /* attach this buffer to the buffer chain */
    out.buf = b;
    out.next = NULL;
 
    /* adjust the pointers of the buffer */
    b->pos = hello_string.data;
    b->last = hello_string.data + hello_string.len;
    b->memory = 1;    /* this buffer is in memory */
    b->last_buf = 1;  /* this is the last buffer in the buffer chain */
 
    /* set the status line */
    r->headers_out.status = NGX_HTTP_OK;
    r->headers_out.content_length_n = hello_string.len;
 
    /* send the headers of your response */
    rc = ngx_http_send_header(r);
 
    if (rc == NGX_ERROR || rc > NGX_OK || r->header_only) {
        return rc;
    }
 
    /* send the buffer chain of your response */
    return ngx_http_output_filter(r, &out);
}