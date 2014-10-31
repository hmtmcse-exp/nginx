### Include header Files
```C
#include <ngx_config.h>
#include <ngx_core.h>
#include <ngx_http.h>
```



### Nginx Variable (Module Configuration Struct(s))

Most modules just need a location configuration. The naming convention for these is 
ngx_http_<module name>_(main|srv|loc)_conf_t. Here's an example, taken from the dav module:

```C

typedef struct {
    ngx_uint_t  methods;
    ngx_flag_t  create_full_put_path;
    ngx_uint_t  access;
} ngx_http_dav_loc_conf_t;

```

Practical Example
```C

typedef struct {
    ngx_str_t   message;
} ngx_http_hello_loc_conf_t;

```

### Module Directives

Identifier of module config file

```C
static ngx_command_t ngx_http_<module name>_commands[] = {
    { ngx_string("indetifier"),
      NGX_HTTP_LOC_CONF|NGX_CONF_TAKE1,
      ngx_conf_set_str_slot,
      NGX_HTTP_LOC_CONF_OFFSET,
      offsetof(ngx_http_<module name>_loc_conf_t, store_variable_name),
      &ngx_http_<module name>_p },
    ngx_null_command
};
```


### The Module Context

The module context has hooks , here we have able to create hook.

Naming Convention: ngx_http_<module name>_module_ctx

In order, the function references are:

1. preconfiguration
2. postconfiguration
3. creating the main conf (i.e., do a malloc and set defaults)
4. initializing the main conf (i.e., override the defaults with what's in nginx.conf)
5. creating the server conf
6. merging it with the main conf
7. creating the location conf
8. merging it with the server conf

```C

static ngx_http_module_t ngx_http_<module name>_module_ctx = {
    NULL,                          /* preconfiguration */
    NULL,                          /* postconfiguration */
 
    NULL,                          /* create main configuration */
    NULL,                          /* init main configuration */
 
    NULL,                          /* create server configuration */
    NULL,                          /* merge server configuration */
 
    ngx_http_<module name>_create_loc_conf, /* create location configuration */
    NULL                           /* merge location configuration */
};

```

### The Module Definition

The module definition is sometimes used as a key to look up data associated with a particular module. The module which binds the context and commands 

Naming Convention: ngx_http_<module name>_module

In order, the function references are:
```C

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

```

### Getting the location configuration 

Naming Convention: ngx_http_<module name>_handler

In order, the function references are:
```C

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

```