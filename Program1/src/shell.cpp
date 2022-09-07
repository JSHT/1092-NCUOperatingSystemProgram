#include <iostream>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <vector>
#include <sys/types.h>
#include <stdlib.h>
#include <string.h>
#include <string>
#include <signal.h>
#include <ctime>

#define RESET   "\033[0m"
#define BLACK   "\033[30m"      /* Black */
#define RED     "\033[31m"      /* Red */
#define GREEN   "\033[32m"      /* Green */
#define YELLOW  "\033[33m"      /* Yellow */
#define BLUE    "\033[34m"      /* Blue */
#define MAGENTA "\033[35m"      /* Magenta */
#define CYAN    "\033[36m"      /* Cyan */
#define WHITE   "\033[37m"      /* White */

#define BOLDBLACK   "\033[1m\033[30m"       /* Bold Black */
#define BOLDRED     "\033[1m\033[31m"       /* Bold Red */
#define BOLDGREEN   "\033[1m\033[32m"       /* Bold Green */
#define BOLDYELLOW  "\033[1m\033[33m"       /* Bold Yellow */
#define BOLDBLUE    "\033[1m\033[34m"       /* Bold Blue */
#define BOLDMAGENTA "\033[1m\033[35m"       /* Bold Magenta */
#define BOLDCYAN    "\033[1m\033[36m"       /* Bold Cyan */
#define BOLDWHITE   "\033[1m\033[37m"       /* Bold White */

#define BACKBLACK   "\033[40m"
#define BACKRED     "\033[41m"
#define BACKGREEN   "\033[42m"
#define BACKYELLOW  "\033[43m"
#define BACKBLUE    "\033[44m"
#define BACKMAGENTA "\033[45m"
#define BACKCYAN    "\033[46m"
#define BACKWHITE   "\033[47m"

using namespace std;
int fd[2], in, out, pid;
vector<string> partials;
vector<string> path;

string trim(string input)
{
    int i = 0;
    while (i < input.size() && input[i] == ' ')
        i++;
    if (i < input.size())
        input = input.substr(i);
    else
    {
        return "";
    }

    i = input.size() - 1;
    while (i >= 0 && input[i] == ' ')
        i--;
    if (i >= 0)
        input = input.substr(0, i + 1);
    else
        return "";

    return input;
}

vector<string> split(string line, string separator)
{
    vector<string> result;
    while (line.size())
    {
        size_t found = line.find(separator);
        if (found == string::npos)
        {
            string lastpart = trim(line);
            if (lastpart.size() > 0)
            {
                result.push_back(lastpart);
            }
            break;
        }
        string segment = trim(line.substr(0, found));
        line = line.substr(found + 1);
        if (segment.size() != 0)
            result.push_back(segment);
    }
    return result;
}

char **char_array(vector<string> parts)
{
    char **result = new char *[parts.size() + 1];  // the last one for the NULL
    for (int i = 0; i < parts.size(); i++)
    {
        result[i] = new char[parts[i].size() + 1]; // the last one for the NULL
        strcpy(result[i], parts[i].c_str());
    }
    result[parts.size()] = NULL;
    return result;
}

void changeDirectory(vector<string> args)
{

    char buf[80];
    getcwd(buf, sizeof(buf));
    if (chdir(args[1].c_str()) == -1)
    {
        if(args[1] != ""){
            cout << "No such Directory" << endl;
        }else{
            chdir(getenv("HOME"));
            path[2] = "~";
        }
    }
    else
    {
        getcwd(buf, sizeof(buf));
        path[2] = buf;
    }
}

void execute(string cmdIn)
{
    string cmd = cmdIn;
    vector<string> inString = split(cmd, "<");
    if (inString.size() > 1)
    {
        vector<string> fileInVec = split(inString[1], " ");
        int fdIn = open(fileInVec[0].c_str(), O_RDONLY);
        if (fdIn == -1)
        {
            perror("");
        }
        else
        {
            dup2(fdIn, 0);
            close(fdIn);
        }
        cmd = inString[0];
        for (int i = 1; i < fileInVec.size(); i++)
        {
            cmd += " " + fileInVec[i];
        }
    }

    vector<string> outString = split(cmd, ">");
    if (outString.size() > 1)
    {
        vector<string> fileOutVec = split(outString[1], " ");
        remove(fileOutVec[0].c_str());
        int fdOut = open(fileOutVec[0].c_str(), O_CREAT | O_WRONLY);
        dup2(fdOut, 1);
        close(fdOut);
        cmd = outString[0];
        for (int i = 1; i < fileOutVec.size(); i++)
        {
            cmd += " " + fileOutVec[i];
        }
    }

    vector<string> argstr = split(cmd, " ");

    char **args = char_array(argstr);       // convert vector<string> into an array of char*
    if (argstr[0] == "cd")
    {
        changeDirectory(argstr);
    }
    else
    {
        if (execvp(args[0], args) == -1)
        {
            perror("");
        }
    }
}

int main()
{
    char buf[80];
    time_t timep = time(NULL);
    getcwd(buf, sizeof(buf));

    path.push_back(ctime(&timep));      //get current time
    path.push_back(getenv("USER"));     //get current user
    path.push_back(buf);
    path[0].pop_back();

    cout << BOLDRED << "    ___  ________  ___  ___  _______   ___       ___           " << endl;
    cout << BOLDGREEN << "   |\\  \\|\\   ____\\|\\  \\|\\  \\|\\  ___ \\ |\\  \\     |\\  \\         " << endl;
    cout << BOLDBLUE << "   \\ \\  \\ \\  \\___|\\ \\  \\\\\\  \\ \\   __/|\\ \\  \\    \\ \\  \\        " << endl;
    cout << BOLDCYAN << " __ \\ \\  \\ \\_____  \\ \\   __  \\ \\  \\_|/_\\ \\  \\    \\ \\  \\       " << endl;
    cout << BOLDMAGENTA << "|\\  \\\\_\\  \\|____|\\  \\ \\  \\ \\  \\ \\  \\_|\\ \\ \\  \\____\\ \\  \\____  " << endl;
    cout << BOLDWHITE << "\\ \\________\\____\\_\\  \\ \\__\\ \\__\\ \\_______\\ \\_______\\ \\_______\\" << endl;
    cout << BOLDYELLOW << " \\|________|\\_________\\|__|\\|__|\\|_______|\\|_______|\\|_______|" << endl;
    cout << BOLDBLACK << "           \\|_________|                                       " << endl;

    while (true)
    {
        cout << BACKWHITE << BOLDBLACK << path[0] << RESET << " " << BOLDBLUE << path[1] << BOLDCYAN << "@" << BOLDGREEN << path[2] << " "
             << ">>" << RESET;
        string cmd;
        getline(cin, cmd);
        if (cmd == "exit")
        {
            cout << BACKYELLOW << BOLDBLACK << " _____ ______   ________  _________  ________          ________   _______   ________   _______      " << RESET << endl;
            cout << BACKYELLOW << BOLDMAGENTA << "|\\   _ \\  _   \\|\\   __  \\|\\___   ___\\\\   __  \\        |\\   ___  \\|\\  ___ \\ |\\   ___  \\|\\  ___ \\     " << RESET << endl;
            cout << BACKYELLOW << BOLDBLUE << "\\ \\  \\\\\\__\\ \\  \\ \\  \\|\\  \\|___ \\  \\_\\ \\  \\|\\  \\       \\ \\  \\\\ \\  \\ \\   __/|\\ \\  \\\\ \\  \\ \\   __/|    " << RESET << endl;
            cout << BACKYELLOW << BOLDCYAN << " \\ \\  \\\\|__| \\  \\ \\   __  \\   \\ \\  \\ \\ \\   __  \\       \\ \\  \\\\ \\  \\ \\  \\_|/_\\ \\  \\\\ \\  \\ \\  \\_|/__  " << RESET << endl;
            cout << BACKYELLOW << BOLDGREEN << "  \\ \\  \\    \\ \\  \\ \\  \\ \\  \\   \\ \\  \\ \\ \\  \\ \\  \\       \\ \\  \\\\ \\  \\ \\  \\_|\\ \\ \\  \\\\ \\  \\ \\  \\_|\\ \\ " << RESET << endl;
            cout << BACKYELLOW << BOLDRED << "   \\ \\__\\    \\ \\__\\ \\__\\ \\__\\   \\ \\__\\ \\ \\__\\ \\__\\       \\ \\__\\\\ \\__\\ \\_______\\ \\__\\\\ \\__\\ \\_______\\" << RESET << endl;
            cout << BACKYELLOW << BOLDBLACK << "    \\|__|     \\|__|\\|__|\\|__|    \\|__|  \\|__|\\|__|        \\|__| \\|__|\\|_______|\\|__| \\|__|\\|_______|" << RESET << endl;
            cout << BACKYELLOW << "                                                                                                    " << RESET << endl;
            kill(0, SIGINT);
        }

        partials = split(cmd, "|");

        in = 0; 
        for (int i = 0; i < partials.size(); i++)
        {
            pipe(fd);                       // make pipe
            if ((pid = fork() == 0))
            {
                out = fd[1];                // redirect output to the next level
                if (in != 0)
                {
                    dup2(in, 0);
                    close(in);
                }
                if (i < partials.size() - 1)
                {                           // unless this is the last level
                    dup2(out, 1);
                    close(out);
                }
                execute(partials[i]);       // execute all the arguments
            }
            else
            {
                wait(NULL);                 // wait for the child process
                close(fd[1]);               // do other redirects
                in = fd[0];
            }
        }
        cmd = "";
    }
    return 0;
}