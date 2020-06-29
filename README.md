# ECC_cpabe_revocation
----------------------------------------------------

### Steps to Installation(UNIX):
0. fork the repository to your account
1. Clone the repository.
2. cd into the cloned directory.
3. Run the command: 
      1. `chmod 744 check.sh`
      2. `chmod 744 compile.sh`
4.  Run `./check.sh` in terminal(sudo password will be asked).

### Steps to Compile and Execute:
1. Run `./compile.sh` 

### Additional Files:
1. **demo.c** : Extra file to try and test code bits before putting them into main files. 
2. **ecc.c** : openSSL way of defining a curve, doing point addition, double and multiplication. For reference. (requires openSSL to be installed in the system) 