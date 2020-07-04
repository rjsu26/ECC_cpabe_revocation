#!/bin/bash
# File for first installation of this repository
# To be run only from the repository main folder i.e. at ECC_cpabe_revocation/
BASEDIR="$( pwd )"
cd "${BASEDIR}"/libbswabe-0.9
chmod +x configure
chmod +x mkinstalldirs
chmod +x install-sh
./configure
make
sudo make install
cd ..
cd cpabe-0.11
chmod +x configure
chmod +x mkinstalldirs
chmod +x install-sh
./configure
make LDFLAGS="-lgmp -lpbc -lcrypto -L/usr/lib/x86_64-linux-gnu -lglib-2.0 -lbswabe -lgmp -fPIC"
sudo make install
cd ..

