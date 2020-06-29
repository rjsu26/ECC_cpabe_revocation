#!/bin/bash
# File to compile all the programs in the necessary order.
# To be run only from the repository main folder i.e. at ECC_cpabe_revocation/
BASEDIR="$( pwd )"
cd "${BASEDIR}"/libbswabe-0.9
make new
sudo make install
cd ..
cd cpabe-0.11
rm don.txt
make new 
sudo make install
cd ..

