# facilecoin-core
It is strongly recommended to download binaries from facilecoin.org.
Best alternative - compile core yourself.
<br>
The source is not yet clean enough (in regards to code elegance and copyrights), but it will compile.<br>

Tested on: Windows 7 (mingw), Ubuntu 14.04, CentOs 7, Debian 8.


<h3> == Building under CentOs 7 (may apply to other linux distributions) == </h3>
== Build BerkeleyDb 4.8 (source here: https://github.com/bitcoin/bitcoin/issues/2998)<br> ==
Download sources of BerkeleyDb 4.8 (http://www.oracle.com/technetwork/database/berkeleydb/downloads/index-082944.html)<br>
Go to /build_unix/<br>
../dist/configure --prefix=/usr/local --enable-cxx<br>
make<br>
(as root) make install<br>
(as root) nano /usr/local/include/db_cxx.h<br>
Add a line '#define HAVE_CXX_STDHEADERS 1' before '#ifdef HAVE_CXX_STDHEADERS'<br>
<br>
<br>
== Build openssl ==<br>
wget https://www.openssl.org/source/openssl-1.0.1g.tar.gz -O openssl-1.0.1g.tar.gz<br>
tar -z -xvf openssl-1.0.1g.tar.gz<br>
cd openssl-1.0.1g<br>
./config --prefix=/usr --openssldir=/usr/local/openssl shared<br>
make<br>
(as root) make install_sw<br>
openssl version<br>
returns 'OpenSSL 1.0.1g 7 Apr 2014'<br>
<br>
<br>
== libboost==<br>
yum install boost-devel.x86_64
<br>
<br>
== Build facilecoin-core ==<br>
./autogen.sh<br>
export LD_LIBRARY_PATH=/usr/local/lib<br>
./configure<br>
make<br>
(as root) make install<br>
<br>
