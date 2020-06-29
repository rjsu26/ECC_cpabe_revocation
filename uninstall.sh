#!/bin/bash
# To be run only from the repository main folder i.e. at ECC_cpabe_revocation/

BASEDIR="$( pwd )"
cd "${BASEDIR}"/libbswabe-0.9
sudo make uninstall
cd ..
cd cpabe-0.11/
sudo make uninstall
cd ..