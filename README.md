![C/C++ CI](https://github.com/rjsu26/ECC_cpabe_revocation/workflows/C/C++%20CI/badge.svg)
# ECC_cpabe_revocation
<!-- ---------------------------------------------------- -->

### Steps to Installation(UNIX):
0. Create a branch with your name and switch to it. 
1. Clone the repository.
2. cd into the cloned directory.
3. Run the command: 
      1. `chmod 744 check.sh`
      2. `chmod 744 compile.sh`
4.  Run `./check.sh` in terminal(sudo password will be asked).
5.  Make the desired changes and add-commit-push the changes to your repo.
6.  Make a pull request to the master branch of the main repo

### Steps to Compile and Execute:
1. Run `./compile.sh` 

### Additional Files:
1. **demo.c** : Extra file to try and test code bits before putting them into main files. 
2. **ecc.c** : openSSL way of defining a curve, doing point addition, double and multiplication. For reference. (requires openSSL to be installed in the system) 