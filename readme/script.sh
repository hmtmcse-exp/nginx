#!/bin/sh

cd /root/touhid-nginx/nginx-1.6.0
configure --add-module=/root/touhid-nginx/nginx/module/Print_Me
make
make install

/usr/local/nginx/sbin/nginx -s stop
/usr/local/nginx/sbin/nginx
