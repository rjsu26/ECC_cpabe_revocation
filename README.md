![C/C++ CI](https://github.com/rjsu26/ECC_cpabe_revocation/workflows/C/C++%20CI/badge.svg)
# ECC_cpabe_revocation
<!-- ---------------------------------------------------- -->

### Steps to Installation(UNIX): 
1. Clone the repository.
2. cd into the cloned directory.
3. Run the command: 
      1. `chmod 744 check.sh`
      2. `chmod 744 compile.sh`
4.  Run `./check.sh` in terminal(sudo password will be asked).

### To make changes:
1. Make a new branch with your name as _firstname_lastname_ using the command : `git checkout -b <your_name>`. This command will also bring you to this branch after creating it. 
2. Make the desired changes and add + commit the changes.  
3. Make a pull request to the master branch of the main repo

### Steps to Compile and Execute:
1. Run `./compile.sh` 

### Additional Files:
1. **demo.c** : Extra file to try and test code bits before putting them into main files. 
2. **ecc.c** : openSSL way of defining a curve, doing point addition, double and multiplication. For reference. (requires openSSL to be installed in the system) 