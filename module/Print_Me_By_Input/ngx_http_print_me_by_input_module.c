#include <ngx_config.h>
#include <ngx_core.h>
#include <ngx_http.h>

static ngx_int_t ngx_http_print_me_by_input(ngx_conf_t *cf);

typedef struct {
    ngx_str_t   message;
} ngx_http_print_me_by_input_loc_conf_t;



static ngx_command_t  ngx_http_print_me_by_input_commands[] = {

  { ngx_string("print_me_by_input"),
    NGX_HTTP_LOC_CONF|NGX_CONF_TAKE1,
    ngx_conf_set_str_slot,
    NGX_HTTP_LOC_CONF_OFFSET,
    offsetof(ngx_http_print_me_by_input_loc_conf_t, message),
    NULL },
    ngx_null_command
};


static void *
ngx_http_print_me_by_input_create_loc_conf(ngx_conf_t *cf){
    ngx_http_print_me_by_input_loc_conf_t  *conf;

    conf = ngx_pcalloc(cf->pool, sizeof(ngx_http_print_me_by_input_loc_conf_t));
    if (conf == NULL) {
        return NULL;
    }
    return conf;
}


static ngx_http_module_t  ngx_http_print_me_by_input_module_ctx = {
  NULL,                                         /* preconfiguration */
  ngx_http_print_me_by_input,                   /* postconfiguration */

  NULL,                                         /* create main configuration */
  NULL,                                         /* init main configuration */

  NULL,                                         /* create server configuration */
  NULL,                                         /* merge server configuration */
  ngx_http_print_me_by_input_create_loc_conf,   /* create location configuration */
  NULL                                          /* merge location configuration */
};


ngx_module_t ngx_http_print_me_by_input_module = {
  NGX_MODULE_V1,
  &ngx_http_print_me_by_input_module_ctx, /* module context */
  ngx_http_print_me_by_input_commands,   /* module directives */
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

static ngx_int_t ngx_http_print_me_by_input_handler(ngx_http_request_t *r){
    
  ngx_http_print_me_by_input_loc_conf_t *loc = ngx_http_get_module_loc_conf(r, ngx_http_print_me_by_input_module);
    
  ngx_buf_t    *b;
  ngx_chain_t   out;

    r->headers_out.content_type_len = sizeof("text/html") - 1;
    r->headers_out.content_type.len = sizeof("text/html") - 1;
    r->headers_out.content_type.data = (u_char *) "text/html";

  b = ngx_pcalloc(r->pool, sizeof(ngx_buf_t));

  out.buf = b;
  out.next = NULL;

  b->pos = loc->message.data;
  b->last = loc->message.data + loc->message.len;
  b->memory = 1;
  b->last_buf = 1;

  r->headers_out.status = NGX_HTTP_OK;
  r->headers_out.content_length_n = loc->message.len;
  ngx_http_send_header(r);

  return ngx_http_output_filter(r, &out);
}



static ngx_int_t ngx_http_print_me_by_input(ngx_conf_t *cf){ 
    ngx_http_handler_pt        *h;
    ngx_http_core_main_conf_t  *cmcf;
    cmcf = ngx_http_conf_get_module_main_conf(cf, ngx_http_core_module);
    h = ngx_array_push(&cmcf->phases[NGX_HTTP_ACCESS_PHASE].handlers);
    if (h == NULL) {
        return NGX_ERROR;
    }

    *h = ngx_http_print_me_by_input_handler;
    return NGX_OK;
}
