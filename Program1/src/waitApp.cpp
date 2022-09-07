#include<unistd.h>
#include<fcntl.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<sys/wait.h>
#include<iostream>
using namespace std;

bool again = true;
char check;

int main(){
    pid_t q;
    while(again){
        q = fork();
        if(q<0){
            cout << "error" << endl;
        }
        else if(q == 0){
            char *argv[] = {(char*)"sl", NULL};
            cout << "I'm a child. Let's play a train." << endl;
            sleep(5);
            execvp("sl", argv);
        }
        else{
            waitpid(q, NULL, 0);
            cout << "again? " << "(y/n)" << endl;
            cin >> check;
            if(check == 'y'){
                again = true;
            }else{
                again = false;
            }
        }
    }
}