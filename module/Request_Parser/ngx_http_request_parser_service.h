#include <ngx_config.h>
#include <ngx_core.h>
#include <ngx_http.h>

ngx_int_t
ngx_http_request_parser_get_method_params(ngx_http_request_t *r, u_char *name, size_t len, ngx_str_t *value);

ngx_int_t
ngx_http_request_parser_headers_value(ngx_http_request_t *r, u_char *name, size_t len, ngx_str_t* value);