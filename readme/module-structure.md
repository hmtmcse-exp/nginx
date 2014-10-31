#### Step-1 : Include Header Files

```C
#include <ngx_config.h>
#include <ngx_core.h>
#include <ngx_http.h>
```

#### Step-2 : Adding Module Directives

```C
static ngx_command_t  ngx_http_<module_name>_commands[] = {

  { ngx_string("<identifier_name_here>"),
    NGX_HTTP_LOC_CONF|NGX_CONF_NOARGS,
    ngx_http_<module_name>,
    0,
    0,
    NULL },
    ngx_null_command
};
```


#### Step-3 : Adding  Module Context

```C
static ngx_http_module_t  ngx_http_<module_name>_module_ctx = {
  NULL,                          /* preconfiguration */
  NULL,                          /* postconfiguration */

  NULL,                          /* create main configuration */
  NULL,                          /* init main configuration */

  NULL,                          /* create server configuration */
  NULL,                          /* merge server configuration */

  NULL,                          /* create location configuration */
  NULL                           /* merge location configuration */
};
```

#### Step-4 : Adding Module Definition

```C
ngx_module_t ngx_http_<module_name>_module = {
  NGX_MODULE_V1,
  &ngx_http_<module_name>_module_ctx, /* module context */
  ngx_http_<module_name>_commands,   /* module directives */
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
```

#### Step-5 : Adding Handler

```C
static ngx_int_t ngx_http_<module_name>_handler(ngx_http_request_t *r)
{
  ngx_buf_t    *b;
  ngx_chain_t   out;

  r->headers_out.content_type.len = sizeof("text/plain") - 1;
  r->headers_out.content_type.data = (u_char *) "text/plain";

  b = ngx_pcalloc(r->pool, sizeof(ngx_buf_t));

  out.buf = b;
  out.next = NULL;

  b->pos = <output_content>;
  b->last = <output_content> + sizeof(<output_content>);
  b->memory = 1;
  b->last_buf = 1;

  r->headers_out.status = NGX_HTTP_OK;
  r->headers_out.content_length_n = sizeof(<output_content>);
  ngx_http_send_header(r);

  return ngx_http_output_filter(r, &out);
}
```

#### Step-6 : Adding Handler in Main Module

```C
static char *ngx_http_<module_name>(ngx_conf_t *cf, ngx_command_t *cmd, void *conf)
{
  ngx_http_core_loc_conf_t  *clcf;
  clcf = ngx_http_conf_get_module_loc_conf(cf, ngx_http_core_module);
  clcf->handler = ngx_http_<module_name>_handler;
  return NGX_CONF_OK;
}
```

