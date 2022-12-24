#include <iostream>
#include <string>

#include <unistd.h>
#include <string.h>
#include <sys/wait.h>

int main() {

  char username[1024];
  char hostname[1024];

  getlogin_r(username, sizeof(username));
  gethostname(hostname, sizeof(hostname));

  std::string username_s(username);
  std::string hostname_s(hostname);

  while (true) {

    char cwd[1024];
    getcwd(cwd, sizeof(cwd));
    std::string cwd_s(cwd);

    std::cout << username_s << "@" << hostname_s << " " << cwd_s << " $ " << "\0";
    std::string input;
    std::getline(std::cin, input);

    char* cmd = strtok(&input[0], " ");
    char* args[20];
    args[0] = cmd;
    for (int i = 1; i < 20; i++) {
      args[i] = strtok(NULL, " ");
      if (args[i] == NULL) break;
    }

    if (strcmp(cmd, "cd") == 0) {
      if (args[1] == NULL) {
        std::cerr << "cd: missing operand" << std::endl;
      } else {
        if (chdir(args[1]) != 0) {
          perror("chdir");
        }
      }
    } else {
      int child_pid = fork();
      if (child_pid == 0) {
        execvp(cmd, args);
        std::cout << "nut: invalid command" << std::endl;
        exit(1);
      } else {
        wait(NULL);
      }
    }
  }

  return 0;
}

