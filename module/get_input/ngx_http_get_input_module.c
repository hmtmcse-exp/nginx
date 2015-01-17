#include <ngx_config.h>
#include <ngx_core.h>
#include <ngx_http.h>
#include <string.h>


static ngx_int_t
ngx_http_get_input_init(ngx_conf_t *cf);


typedef struct {
  ngx_str_t server_type; //API or SITE
} get_input_main_conf_t;

static void *
ngx_http_get_input_create_loc_conf(ngx_conf_t *cf){
    get_input_main_conf_t  *conf;
    conf = ngx_pcalloc(cf->pool, sizeof(get_input_main_conf_t));
    if (conf == NULL) {
        return NGX_CONF_ERROR;
    }
    return conf;    
}

static char *
ngx_http_get_input_merge_loc_conf(ngx_conf_t *cf, void *parent, void *child){
//    register_variable(cf);

    get_input_main_conf_t *prev = parent;
    get_input_main_conf_t *conf = child;

    ngx_conf_merge_str_value(conf->server_type, prev->server_type, "");
    return NGX_CONF_OK;
}


static ngx_command_t  ngx_http_get_input_commands[] = {
  {
    ngx_string("server_type"),
    NGX_HTTP_SRV_CONF|NGX_HTTP_LOC_CONF|NGX_CONF_1MORE,
    ngx_conf_set_str_slot,
    NGX_HTTP_LOC_CONF_OFFSET,
    offsetof(get_input_main_conf_t, server_type),
    NULL
  },
    ngx_null_command
};


static ngx_http_module_t  ngx_http_get_input_module_ctx = {
  NULL,                          /* preconfiguration */
  ngx_http_get_input_init,                          /* postconfiguration */

  NULL,                          /* create main configuration */
  NULL,                          /* init main configuration */

  NULL,                          /* create server configuration */
  NULL,                          /* merge server configuration */

  ngx_http_get_input_create_loc_conf,/* create location configuration */
  ngx_http_get_input_merge_loc_conf /* merge location configuration */
};


ngx_module_t ngx_http_get_input_module = {
  NGX_MODULE_V1,
  &ngx_http_get_input_module_ctx, /* module context */
  ngx_http_get_input_commands,   /* module directives */
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

static ngx_int_t ngx_http_get_input_handler(ngx_http_request_t *r){
  ngx_buf_t    *b;
  ngx_chain_t   out;

    r->headers_out.content_type_len = sizeof("text/html") - 1;
    r->headers_out.content_type.len = sizeof("text/html") - 1;
    r->headers_out.content_type.data = (u_char *) "text/html";

  b = ngx_pcalloc(r->pool, sizeof(ngx_buf_t));

  out.buf = b;
  out.next = NULL;
  
  get_input_main_conf_t *conf = ngx_http_get_module_main_conf(r, ngx_http_get_input_module);
     
   u_char  ngx_printable_message[] = "GOT";        
  if(strcmp ((char*)conf->server_type.data,"API")){
      ngx_log_error(NGX_LOG_ERR, r->connection->log, 0, "API SERVER");
  }else{
       ngx_log_error(NGX_LOG_ERR, r->connection->log, 0, "WEB SERVER");
  }
      
  b->pos = ngx_printable_message;
  b->last = ngx_printable_message + sizeof(ngx_printable_message);
  b->memory = 1;
  b->last_buf = 1;

  r->headers_out.status = NGX_HTTP_OK;
  r->headers_out.content_length_n = sizeof(ngx_printable_message);
  ngx_http_send_header(r);

  return ngx_http_output_filter(r, &out);
}

static ngx_int_t
ngx_http_get_input_init(ngx_conf_t *cf) {
    ngx_http_handler_pt *h;
    ngx_http_core_main_conf_t *cmcf;

    cmcf = ngx_http_conf_get_module_main_conf(cf, ngx_http_get_input_module);

    h = ngx_array_push(&cmcf->phases[NGX_HTTP_REWRITE_PHASE].handlers);

    if (h == NULL) {
        return NGX_ERROR;
    }

    *h = ngx_http_get_input_handler;
    return NGX_OK;
}