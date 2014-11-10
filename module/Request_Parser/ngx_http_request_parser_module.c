#include <ngx_config.h>
#include <ngx_core.h>
#include <ngx_http.h>
#include "ngx_http_request_parser_service.h"


//static u_char  ngx_printable_message[] = "Yes I was print.... ha ha ha";
static char *ngx_http_request_parser(ngx_conf_t *cf, ngx_command_t *cmd, void *conf);
static ngx_int_t
ngx_http_request_parser_init(ngx_conf_t *cf);
void
ngx_http_request_parser_post_read(ngx_http_request_t *r);

typedef struct {
    unsigned          done:1;
    unsigned          waiting_more_body:1;
} ngx_http_form_input_ctx_t;




ngx_int_t
ngx_http_request_parser_post_read_handler(ngx_http_request_t *r){
    ngx_http_read_client_request_body(r, ngx_http_request_parser_post_read);
    return NGX_DONE; 
}

static ngx_command_t ngx_http_request_parser_commands[] = {

    { ngx_string("request_parser"),
        NGX_HTTP_LOC_CONF | NGX_CONF_NOARGS,
        ngx_http_request_parser,
        0,
        0,
        NULL},
    ngx_null_command
};

static ngx_http_module_t ngx_http_request_parser_module_ctx = {
    NULL, /* preconfiguration */
    ngx_http_request_parser_init, /* postconfiguration */

    NULL, /* create main configuration */
    NULL, /* init main configuration */

    NULL, /* create server configuration */
    NULL, /* merge server configuration */

    NULL, /* create location configuration */
    NULL /* merge location configuration */
};


ngx_module_t ngx_http_request_parser_module = {
    NGX_MODULE_V1,
    &ngx_http_request_parser_module_ctx, /* module context */
    ngx_http_request_parser_commands, /* module directives */
    NGX_HTTP_MODULE, /* module type */
    NULL, /* init master */
    NULL, /* init module */
    NULL, /* init process */
    NULL, /* init thread */
    NULL, /* exit thread */
    NULL, /* exit process */
    NULL, /* exit master */
    NGX_MODULE_V1_PADDING
};

//static void ngx_http_form_input_post_read(ngx_http_request_t *r) {
//    ngx_http_request_parser_module_ctx_t *ctx;
//
//    ngx_log_debug0(NGX_LOG_DEBUG_HTTP, r->connection->log, 0,
//            "http form_input post read request body");
//
//    r->read_event_handler = ngx_http_request_empty_handler;
//
//    ctx = ngx_http_get_module_ctx(r, ngx_http_request_parser_module);
//    ctx->done = 1;
//
//#if defined(nginx_version) && nginx_version >= 8011
//    r->main->count--;
//#endif
//    if (ctx->waiting_more_body) {
//        ctx->waiting_more_body = 0;
//
//        ngx_http_core_run_phases(r);
//    }
//}

static ngx_int_t ngx_http_request_parser_handler(ngx_http_request_t *r) {
    ngx_buf_t *b;
    ngx_chain_t out;

    ngx_str_t args = r->args;

    r->headers_out.content_type_len = sizeof ("text/html") - 1;
    r->headers_out.content_type.len = sizeof ("text/html") - 1;
    r->headers_out.content_type.data = (u_char *) "text/html";

    b = ngx_pcalloc(r->pool, sizeof (ngx_buf_t));

    out.buf = b;
    out.next = NULL;

    ngx_str_t arg_val;
    char *params_args = "myArgs";

    //    if (r->method == NGX_HTTP_POST) {
    //        ngx_int_t rc = ngx_http_read_client_request_body(r, ngx_http_form_input_post_read);
    //    }

     ngx_str_t headerValue;    
    if (ngx_http_request_parser_headers_value(r, (u_char *) params_args, ngx_strlen((u_char *) params_args), &headerValue) == NGX_OK) {
        b->pos = headerValue.data;
        b->last = headerValue.data + headerValue.len;
        r->headers_out.content_length_n = headerValue.len;
    }else  if (ngx_http_request_parser_post_arg(r, (u_char *) params_args, ngx_strlen((u_char *) params_args), &headerValue) == NGX_OK) {
        b->pos = headerValue.data;
        b->last = headerValue.data + headerValue.len;
        r->headers_out.content_length_n = headerValue.len;
    }else if (ngx_http_request_parser_get_method_params(r, (u_char *) params_args, ngx_strlen((u_char *) params_args), &arg_val) == NGX_OK) {
        b->pos = arg_val.data;
        b->last = arg_val.data + arg_val.len;
        r->headers_out.content_length_n = arg_val.len;
    } else{
        b->pos = args.data;
        b->last = args.data + args.len;
        r->headers_out.content_length_n = args.len;
    }



    b->memory = 1;
    b->last_buf = 1;

    r->headers_out.status = NGX_HTTP_OK;
    ngx_http_send_header(r);

    return ngx_http_output_filter(r, &out);
}

static char *ngx_http_request_parser(ngx_conf_t *cf, ngx_command_t *cmd, void *conf) {
    ngx_http_core_loc_conf_t *clcf;
    clcf = ngx_http_conf_get_module_loc_conf(cf, ngx_http_core_module);
    clcf->handler = ngx_http_request_parser_handler;
    return NGX_CONF_OK;
}

static ngx_int_t
ngx_http_request_parser_init(ngx_conf_t *cf){
    ngx_http_handler_pt             *h;
    ngx_http_core_main_conf_t       *cmcf;
    
    cmcf = ngx_http_conf_get_module_main_conf(cf, ngx_http_core_module);

    h = ngx_array_push(&cmcf->phases[NGX_HTTP_REWRITE_PHASE].handlers);

    if (h == NULL) {
        return NGX_ERROR;
    }

    *h = ngx_http_request_parser_post_read_handler;
    return NGX_OK;
}

void
ngx_http_request_parser_post_read(ngx_http_request_t *r){
    ngx_http_form_input_ctx_t     *ctx;

    ctx = ngx_http_get_module_ctx(r, ngx_http_request_parser_module);

    ctx->done = 1;

#if defined(nginx_version) && nginx_version >= 8011
    r->main->count--;
#endif
    /* waiting_more_body my rewrite phase handler */
    if (ctx->waiting_more_body) {
        ctx->waiting_more_body = 0;

        ngx_http_core_run_phases(r);
    }
}

