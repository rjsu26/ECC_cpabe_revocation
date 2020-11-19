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
5.  Add the following command to the end of Makefile in cpabe and libswabe folder: 
       `new: clean` 
       `$(MAKE)`   (Add this line after a tab)

### To make changes:
1. Make a new branch with your name as _firstname_lastname_ using the command : `git branch <your_name>`
2. Change to that branch using the command: `git checkout <your_name>`
3.  Alternatively to above 2 commands, use : `git checkout -b <your_name>`. This command will bring you to this branch after creating it. 
4. Make the desired changes and add + commit + push the changes.  
5. Make a pull request to the master branch of the repo.
6. Do not merge the pull request. Wait for all others to check your code.
7. Check your current branch using : `git branch`. The branch with * mark is the current directory. Use : `git checkout master` to goto master branch.
8. Once your code is merged, you can delete your branch using the command: `git branch -d <your_name>`.
9.  `git pull` to bring all the new changes in the master branch to your local.
10. To make more changes, goto step 1.

[Reference: working with git branches](https://thenewstack.io/dont-mess-with-the-master-working-with-branches-in-git-and-github/)

### Steps to Compile (all at once):
1. Run `./compile.sh` 

### Steps to Compile(individual files):
1. If any changes are made to files in libswabe folder:
      1. Goto libswabe directory using terminal
      2. Run : `make new`
      3. Run : `sudo make install`

2. #### setup.c
      1. Goto cpabe folder using terminal
      2. Run the command:
            `gcc -o setupdemo setup.c common.c -I/usr/include/glib-2.0 -I/usr/lib/x86_64-linux-gnu/glib-2.0/include -I/usr/include/pbc -I/usr/local/include/pbc -lglib-2.0 -Wl,-rpath /usr/local/lib -lgmp -Wl,-rpath /usr/local/lib -lpbc -lbswabe -lcrypto -lcrypto -lgmp -w`
      3. If no error, `setupdemo` executable file is created.
      4. `./setupdemo`
      5. Enter number of attributes in the attribute universe, defined as 'n' in the research paper.
      6. Files "pub_key" and "master_key" will be created, to be used in next steps.

3. #### keygen.c
      1. Goto cpabe folder using Terminal
      2. Run the command:
            `gcc -o keygendemo keygen.c common.c policy_lang.c -I/usr/include/glib-2.0 -I/usr/lib/x86_64-linux-gnu/glib-2.0/include -I/usr/include/pbc -I/usr/local/include/pbc -lglib-2.0 -Wl,-rpath /usr/local/lib -lgmp -Wl,-rpath /usr/local/lib -lpbc -lbswabe -lcrypto -lcrypto -lgmp -w`
      3.   If no error, `keygendemo` executable file is created.
      4.   Run `./keygendemo pub_key master_key [Attrs..]` where n is number of attributes entered in setup program while [Attrs.. ] represents list of attributes in binary separated by space. Eg. If n=4 then a sample attribute list will be 1 0 0 1 .
      5.   File named "priv_key" will be created unless "-o" option is used in step 4 in which case, the mentioned name will be the output file.

4. #### enc.c    
      1. Run the command:
            `gcc -o encdemo enc.c common.c policy_lang.c -I/usr/include/glib-2.0 -I/usr/lib/x86_64-linux-gnu/glib-2.0/include -I/usr/include/pbc -I/usr/local/include/pbc -O3 -Wall -lglib-2.0 -Wl,-rpath /usr/local/lib -lgmp -Wl,-rpath /usr/local/lib -lpbc -lbswabe -lcrypto -lcrypto -lgmp -w`
      2. If no error, `encdemo` executable file is created.
      3. Before running this file, create/bring a sample file that needs to be encrypted, say `trial.txt`
      4. Run `./encdemo pub_key trial.txt 1 0 1 1` , where pub_key is name of public key file generated in setup phase, and 1 0 1 1 is a sample policy corresponding to n=4 (specified in keygen step), which will be used in encrypting the doc.
      5. File named "trial.txt.cpabe" will replace the originally existing file containing the encrypted unreadable text.

5. #### dec.c
      1.  Run the command:
            `gcc -o decdemo dec.c common.c  -I/usr/include/glib-2.0 -I/usr/lib/x86_64-linux-gnu/glib-2.0/include -I/usr/include/pbc -I/usr/local/include/pbc -O3 -Wall -lglib-2.0 -Wl,-rpath /usr/local/lib -lgmp -Wl,-rpath /usr/local/lib -lpbc -lbswabe -lcrypto -lcrypto -lgmp -w`
      2. If no error, `decdemo` executable file is created.
      3. Ensure that the previously encrypted file is available in the present directory.
      4. Run `./decdemo pub_key priv_key trial.txt.cpabe`
      5. If the user's ID is available on the server and is not revoked from access, then decryption will pass and "trial.txt" will be produced, otherwise decryption will fail. 
 
### Additional Files:
1. **demo.c** : Extra file to try and test code bits before putting them into main files. 
2. **ecc.c** : openSSL way of defining a curve, doing point addition, double and multiplication. For reference. (requires openSSL to be installed in the system) 