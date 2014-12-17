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
    ngx_list_part_t *part;
    ngx_table_elt_t *h;
    ngx_uint_t i;
    part = &r->headers_in.headers.part;
    h = part->elts;
    for (i = 0; /* void */; i++) {
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
        value->data = h[i].value.data;
        value->len = h[i].value.len;
        return NGX_OK;
    }
    return NGX_DECLINED;
}

ngx_int_t
ngx_http_request_parser_post_arg(ngx_http_request_t *r, u_char *arg_name, size_t arg_len, ngx_str_t *value) {

    u_char *p, *v, *last, *buf;
    ngx_chain_t *cl;
    size_t len = 0;
    ngx_buf_t *b;


    ngx_str_set(value, "");

    /* we read data from r->request_body->bufs */
    if (r->request_body == NULL || r->request_body->bufs == NULL) {
        return NGX_OK;
    }

    if (r->request_body->bufs->next != NULL) {
        /* more than one buffer...we should copy the data out... */
        len = 0;
        for (cl = r->request_body->bufs; cl; cl = cl->next) {
            b = cl->buf;

            if (b->in_file) {
                return NGX_OK;
            }

            len += b->last - b->pos;
        }

        if (len == 0) {
            return NGX_OK;
        }

        buf = ngx_palloc(r->pool, len);
        if (buf == NULL) {
            return NGX_ERROR;
        }

        p = buf;
        last = p + len;

        for (cl = r->request_body->bufs; cl; cl = cl->next) {
            p = ngx_copy(p, cl->buf->pos, cl->buf->last - cl->buf->pos);
        }

    } else {
        b = r->request_body->bufs->buf;
        if (ngx_buf_size(b) == 0) {
            return NGX_OK;
        }

        buf = b->pos;
        last = b->last;
    }

    for (p = buf; p < last; p++) {
        /* we need '=' after name, so drop one char from last */

        p = ngx_strlcasestrn(p, last - 1, arg_name, arg_len - 1);
        if (p == NULL) {
            return NGX_OK;
        }

        if ((p == buf || *(p - 1) == '&') && *(p + arg_len) == '=') {
            v = p + arg_len + 1;
            p = ngx_strlchr(v, last, '&');
            if (p == NULL) {

                p = last;

            } else {
            }
            value->data = v;
            value->len = p - v;

            return NGX_OK;
        }
    }
    return NGX_OK;
}


//ngx_http_request_parser_request_handler(ngx_http_request_t *r) {
//    
//}





