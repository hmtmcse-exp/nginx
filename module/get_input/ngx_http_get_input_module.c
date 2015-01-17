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
ngx_http_get_input_create_loc_conf(ngx_conf_t *cf) {
    get_input_main_conf_t *conf;
    conf = ngx_pcalloc(cf->pool, sizeof (get_input_main_conf_t));
    if (conf == NULL) {
        return NGX_CONF_ERROR;
    }
    return conf;
}

static char *
ngx_http_get_input_merge_loc_conf(ngx_conf_t *cf, void *parent, void *child) {
    ngx_http_get_input_init(cf);
    get_input_main_conf_t *prev = parent;
    get_input_main_conf_t *conf = child;
    ngx_conf_merge_str_value(conf->server_type, prev->server_type, "");
    return NGX_CONF_OK;
}


static ngx_command_t ngx_http_get_input_commands[] = {
    {
        ngx_string("server_type"),
        NGX_HTTP_SRV_CONF | NGX_HTTP_LOC_CONF | NGX_CONF_1MORE,
        ngx_conf_set_str_slot,
        NGX_HTTP_LOC_CONF_OFFSET,
        offsetof(get_input_main_conf_t, server_type),
        NULL
    },
    ngx_null_command
};


static ngx_http_module_t ngx_http_get_input_module_ctx = {
    NULL, /* preconfiguration */
    ngx_http_get_input_init, /* postconfiguration */

    NULL, /* create main configuration */
    NULL, /* init main configuration */

    NULL, /* create server configuration */
    NULL, /* merge server configuration */
    ngx_http_get_input_create_loc_conf, /* create location configuration */
    ngx_http_get_input_merge_loc_conf /* merge location configuration */
};


ngx_module_t ngx_http_get_input_module = {
    NGX_MODULE_V1,
    &ngx_http_get_input_module_ctx, /* module context */
    ngx_http_get_input_commands, /* module directives */
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


static ngx_int_t
ngx_http_get_inpur_variable_print_me(ngx_http_request_t *r, ngx_http_variable_value_t *v, uintptr_t data) {    
    get_input_main_conf_t *conf;    
//    ngx_str_t value;
    conf = ngx_http_get_module_loc_conf(r, ngx_http_get_input_module);
    
    if (strcmp((char*) conf->server_type.data, "API")) {
        ngx_log_error(NGX_LOG_ERR, r->connection->log, 0, "API SERVER %s", (char *) conf->server_type.data);
//        value = ngx_string("API SERVER")
    } else {
        ngx_log_error(NGX_LOG_ERR, r->connection->log, 0, "WEB SERVER %s", (char *) conf->server_type.data);
//         value = ngx_string("WEB SERVER")
    }

    ngx_http_variable_value_t *vv = v;
        vv->valid = 0;
        vv->not_found = 1;
    return NGX_OK;
}

static ngx_http_variable_t ngx_http_get_input_vars[] = {
    { ngx_string("print_me"), NULL,
        ngx_http_get_inpur_variable_print_me, 0, NGX_HTTP_VAR_CHANGEABLE, 0},
    { ngx_null_string, NULL, NULL, 0, 0, 0}
};


static ngx_int_t
ngx_http_get_input_init(ngx_conf_t *cf) {
    ngx_http_variable_t *var, *v;
    for (v = ngx_http_get_input_vars; v->name.len; v++) {
        var = ngx_http_add_variable(cf, &v->name, v->flags);
        if (var == NULL) {
            return NGX_ERROR;
        }
        var->get_handler = v->get_handler;
        var->data = v->data;
    }
    return NGX_OK;
}
