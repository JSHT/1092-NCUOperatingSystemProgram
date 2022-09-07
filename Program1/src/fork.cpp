#include<unistd.h>
#include<fcntl.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<sys/wait.h>
#include<iostream>
using namespace std;

int main(){
    pid_t q;
    cout << "before fork" << endl;
    q = fork();
    if(q<0){
        cout << "error" << endl;
    }
    else if(q == 0){
        
        cout << "I am child having pid " << getpid() << endl;
        cout << "My parent's pid is " << getppid() << endl;

    }
    else{
        wait(NULL);
        // sleep(1);

        cout << "I am parent having pid " << getpid() << endl;
        cout << "My Child's pid is " << q << endl;  
        
    }
    cout << "common" << endl;
}