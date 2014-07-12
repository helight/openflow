// Copyright (c) 2014, HelightXu
// Author: Zhwen Xu<HelightXu@gmail.com>
// Created: 2014-06-23
// Description:
//

//http_server.c

#include <stdio.h>
#include <stdlib.h>

#include <event2/buffer.h>
#include <evhttp.h>
#include <event.h>

#define PORT 8080
#define SERVER_INFO "http server test 0.1"

void http_handle(struct evhttp_request *req, void *arg);

int main()
{
    struct event_base *base;
    struct evhttp *http;
    struct evhttp_bound_socket *handle;

    //创建事件处理机制
    base = event_base_new();
    http = evhttp_new(base);
    evhttp_set_gencb(http, http_handle, NULL);
    evhttp_set_timeout(http, 120);
    handle = evhttp_bind_socket_with_handle(http, "0.0.0.0", PORT);
    if( NULL == handle )
    {
        fprintf(stderr, "Server http handle error\n");
        return -1;
    }

    //开始事件循环
    event_base_dispatch(base);
    evhttp_free(http);
    event_base_free(base);

    return 0;
}

void http_handle(struct evhttp_request *req, void *arg)
{
    struct evbuffer *buffer = evbuffer_new();

    //获取客户端请求的URI
    const char *uri;
    uri = evhttp_request_uri(req);
    evbuffer_add_printf(buffer, "uri = %s\n", uri);

    //解析URI的参数(即GET方法的参数)
    struct evkeyvalq params;
    evhttp_parse_query(uri, &params);
    evbuffer_add_printf(buffer, "a = %s\n", evhttp_find_header(&params, "a"));
    evbuffer_add_printf(buffer, "b = %s\n", evhttp_find_header(&params, "b"));


    //设置HTTP表头
    evhttp_add_header(req->output_headers, "Server", SERVER_INFO);
    evhttp_add_header(req->output_headers, "Content-Type", "text/plain; charset=UTF-8");
    evhttp_add_header(req->output_headers, "Connection", "close");

    //返回数据
    evhttp_send_reply(req, HTTP_OK, "OK", buffer);
    evbuffer_free(buffer);
}
