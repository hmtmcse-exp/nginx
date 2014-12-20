### Nginx RPM Build in CENTOS
---
1. yum install rpm-build
2. yum install redhat-rpm-config
3. mkdir -p ~/rpmbuild/{BUILD,RPMS,SOURCES,SPECS,SRPMS}
4. Download NGINX RPM Source From http://nginx.org/packages/centos/6/SRPMS/
5. rpm -Uvh nginx-1.6.2-1.el6.ngx.src.rpm
6. rpmbuild -ba /root/rpmbuild/SPECS/nginx.spec