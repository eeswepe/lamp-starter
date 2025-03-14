#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>

int is_service_running(const char *service) {
  char command[256];
  snprintf(command, sizeof(command), "pgrep %s > /dev/null", service);
  int status = system(command);
  return WIFEXITED(status) && WEXITSTATUS(status) == 0;
}

void manage_service(const char *service, const char *action) {
  char command[256];
  snprintf(command, sizeof(command), "sudo systemctl %s %s", action, service);
  int status = system(command);
  if (WIFEXITED(status) && WEXITSTATUS(status) == 0) {
    printf("%s %sed successfully\n", service, action);
  } else {
    printf("Failed to %s %s\n", action, service);
  }
}

void editConfigFile() {
  char command[256];
  printf("Choose the config file you want to edit: \n");
  printf("1. Httpd\n2. MariaDB\n");
  int choice;
  scanf("%d", &choice);
  switch (choice) {
  case 1:
    snprintf(command, sizeof(command), "nano /etc/httpd/conf/httpd.conf");
    system(command);
    break;
  case 2:
    snprintf(command, sizeof(command), "nano /etc/my.cnf");
    system(command);
    break;
  default:
    printf("Invalid choice\n");
    break;
  }
}

int main(int argc, char *argv[]) {
  if (argc != 2) {
    printf("Usage: %s <start|stop|restart|status>\n", argv[0]);
    return 1;
  }

  char *action = argv[1];

  if (strcmp(action, "start") == 0) {
    if (!is_service_running("httpd"))
      manage_service("httpd", "start");
    if (!is_service_running("mariadb"))
      manage_service("mariadb", "start");
    printf("Httpd and/or MariaDB were started\n");
  } else if (strcmp(action, "stop") == 0) {
    if (is_service_running("httpd"))
      manage_service("httpd", "stop");
    if (is_service_running("mariadb"))
      manage_service("mariadb", "stop");
    printf("Httpd and MariaDB were stopped\n");
  } else if (strcmp(action, "status") == 0) {
    if (is_service_running("httpd"))
      printf("Httpd is running\n");
    else
      printf("Httpd is not running\n");
    if (is_service_running("mariadb"))
      printf("MariaDB is running\n");
    else
      printf("MariaDB is not running\n");
  } else if (strcmp(action, "restart") == 0) {
    manage_service("httpd", "restart");
    manage_service("mariadb", "restart");
    printf("Httpd and MariaDB were restarted\n");
  } else if (strcmp(action, "config") == 0) {
    editConfigFile();
  } else {
    printf("Invalid action: %s\n", action);
    printf("Usage: %s <start|stop|restart|status>\n", argv[0]);
    return 1;
  }

  return 0;
}
