#!/bin/sh
cd /usr/local/nginx/sbin/
./nginx -s stop
mv  /usr/local/nginx/conf/nginx.conf /tmp/
cd /root/nginx
rm -rf nginx-1.6.2
tar -xzf nginx-1.6.2.tar.gz
cd /root/nginx/nginx-1.6.2
./configure  --add-module=/root/nginx/nginx/module/Print_Me_By_Input
make
rm -rf /usr/local/nginx/*
make install
mv /tmp/nginx.conf /usr/local/nginx/conf/
cd /usr/local/nginx/sbin/
./nginx





