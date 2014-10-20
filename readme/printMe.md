# Basic Nginx Module
### Step 1: 
There is a minimum of two files required for writing an nginx module, the first should be called config and looks something like this:

Config: The config file tells NGINX how to compile the module and what it is called.

```bash
ngx_addon_name=ngx_http_print_me_module
HTTP_MODULES="$HTTP_MODULES ngx_http_print_me_module"
NGX_ADDON_SRCS="$NGX_ADDON_SRCS $ngx_addon_dir/ngx_http_print_me_module.c"
```

Module file: (ngx_http_print_me_module.c) The module file contains the actual code.

```c
#include <ngx_config.h>
#include <ngx_core.h>
#include <ngx_http.h>

static char *ngx_http_print_me(ngx_conf_t *cf, ngx_command_t *cmd, void *conf);

static ngx_command_t  ngx_http_print_me_commands[] = {

  { ngx_string("print_me"),
    NGX_HTTP_LOC_CONF|NGX_CONF_NOARGS,
    ngx_http_print_me,
    0,
    0,
    NULL },

    ngx_null_command
};


static u_char  ngx_print_me[] = "hello world";

static ngx_http_module_t  ngx_http_print_me_module_ctx = {
  NULL,                          /* preconfiguration */
  NULL,                          /* postconfiguration */

  NULL,                          /* create main configuration */
  NULL,                          /* init main configuration */

  NULL,                          /* create server configuration */
  NULL,                          /* merge server configuration */

  NULL,                          /* create location configuration */
  NULL                           /* merge location configuration */
};


ngx_module_t ngx_http_print_me_module = {
  NGX_MODULE_V1,
  &ngx_http_print_me_module_ctx, /* module context */
  ngx_http_print_me_commands,   /* module directives */
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


static ngx_int_t ngx_http_print_me_handler(ngx_http_request_t *r)
{
  ngx_buf_t    *b;
  ngx_chain_t   out;

  r->headers_out.content_type.len = sizeof("text/plain") - 1;
  r->headers_out.content_type.data = (u_char *) "text/plain";

  b = ngx_pcalloc(r->pool, sizeof(ngx_buf_t));

  out.buf = b;
  out.next = NULL;

  b->pos = ngx_print_me;
  b->last = ngx_print_me + sizeof(ngx_print_me);
  b->memory = 1;
  b->last_buf = 1;

  r->headers_out.status = NGX_HTTP_OK;
  r->headers_out.content_length_n = sizeof(ngx_print_me);
  ngx_http_send_header(r);

  return ngx_http_output_filter(r, &out);
}


static char *ngx_http_print_me(ngx_conf_t *cf, ngx_command_t *cmd, void *conf)
{
  ngx_http_core_loc_conf_t  *clcf;

  clcf = ngx_http_conf_get_module_loc_conf(cf, ngx_http_core_module);
  clcf->handler = ngx_http_print_me_handler;

  return NGX_CONF_OK;
}

```

```bash
location = /print_me {
  hello_world;
}
```


### References 
1. [nutrun.com](http://nutrun.com/weblog/2009/08/15/hello-world-nginx-module.html)
1. [www.airpair.com](http://www.airpair.com/nginx/extending-nginx-tutorial)


http://blog.zhuzhaoyuan.com/2009/08/creating-a-hello-world-nginx-module/
http://usamadar.com/2012/09/02/writing-a-custom-nginx-module/