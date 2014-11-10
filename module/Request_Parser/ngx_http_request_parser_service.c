#include "ngx_http_request_parser_service.h"
#include <ngx_config.h>
#include <ngx_core.h>
#include <ngx_http.h>

/*GET Method Params Parse By Name*/

ngx_int_t
ngx_http_request_parser_get_method_params(ngx_http_request_t *r, u_char *name, size_t len, ngx_str_t *value) {
    u_char *p, *last;
    if (r->args.len == 0) {
        return NGX_DECLINED;
    }
    p = r->args.data;
    last = p + r->args.len;
    for (/* void */; p < last; p++) {
        if (p + len > last) {
            return NGX_DECLINED;
        }
        if (ngx_strncasecmp(p, name, len) != 0) {
            continue;
        }
        if (p == r->args.data || *(p - 1) == '&' || (p + len) == last || *(p + len) == '&' || *(p + len) == '=') {
            if ((p + len) < last && *(p + len) == '=') {
                value->data = p + len + 1;
                p = ngx_strlchr(p, last, '&');
                if (p == NULL) {
                    p = r->args.data + r->args.len;
                }
                value->len = p - value->data;
            } else {
                value->len = 0;
            }
            return NGX_OK;
        }
    }
    return NGX_DECLINED;
}


/*Parse Header Value*/

ngx_int_t
ngx_http_request_parser_headers_value(ngx_http_request_t *r, u_char *name, size_t len, ngx_str_t* value) {
    ngx_list_part_t            *part;
    ngx_table_elt_t            *h;
    ngx_uint_t                  i;
    part = &r->headers_in.headers.part;
    h = part->elts;
    for (i = 0; /* void */ ; i++) {
        if (i >= part->nelts) {
            if (part->next == NULL) {
                /* The last part, search is done. */
                break;
            }
 
            part = part->next;
            h = part->elts;
            i = 0;
        }
        if (len != h[i].key.len || ngx_strcasecmp(name, h[i].key.data) != 0) {
            continue;
        }
        value->data =  h[i].value.data;
        value->len = h[i].value.len;
        return NGX_OK;
    }
    return NGX_DECLINED;
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


ngx_int_t
ngx_http_request_parser_post_read_handler(ngx_http_request_t *r){
    ngx_http_read_client_request_body(r, ngx_http_request_parser_post_read);
    return NGX_DONE; 
}

//ngx_http_request_parser_request_handler(ngx_http_request_t *r) {
//    
//}