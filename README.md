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
1. Make a new branch with your name as _firstname_lastname_ using the command : `git branch <your_name>`
2. Change to that branch using the command: `git checkout <your_name>`
3.  Alternatively to above 2 commands, use : `git checkout -b <your_name>`. This command will bring you to this branch after creating it. 
4. Make the desired changes and add + commit + push the changes.  
5. Make a pull request to the master branch of the repo.
6. Do not merge the pull request. Wait for all others to check your code.
7. Once your code is merged, you can delete your branch using the command: `git branch -d <your_name>`.
8. Check your current branch using : `git branch`. The branch with * mark is the current directory. It should be "master" by now. If not, use : `git checkout master`.
9. `git pull` to bring all the new changes in the master branch to your local.
10. To make more changes, goto step 1.

[Reference: working with git branches](https://thenewstack.io/dont-mess-with-the-master-working-with-branches-in-git-and-github/)

### Steps to Compile and Execute:
1. Run `./compile.sh` 

### Additional Files:
1. **demo.c** : Extra file to try and test code bits before putting them into main files. 
2. **ecc.c** : openSSL way of defining a curve, doing point addition, double and multiplication. For reference. (requires openSSL to be installed in the system) 