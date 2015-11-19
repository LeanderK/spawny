#include "main.h"
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>

#define PATH_MAX 4096

int prompted = 0;
Spawny__Server__Data *data;

#define PROMPT(...) printf(__VA_ARGS__);

static int prompt_run(void);

static void
login(void) {
    char username[PATH_MAX], template[PATH_MAX], *password;
    char *templates = NULL;

    PROMPT("Spawny login! (Inputs are without echo)\n");

    PROMPT("\nUsername : ");
    scanf("%s", username);

    password = getpass("Password :");
    PROMPT("");

    do {
        PROMPT("Session Template: (l to list templates):");
        scanf("%s", template);
        printf("\n");
        if (template[0] == 'l') {
            //list templates
            for (int i = 0; i < data->n_templates; i++){
                printf("%s\t - \t%s\n", data->templates[i]->id, data->templates[i]->name);
            }
        } else if (template[0] == 't') {
            //TODO test if template is real
            templates = template;
        }
    }while(!templates);

    client_login(username, password, templates);
    PROMPT("Waiting for feedback\n");
}

static void
listsessions(void) {
    for(int i = 0; i < data->n_sessions; i++){
        printf("Session %s - %s\n", data->sessions[i]->id, data->sessions[i]->name);
    }
    prompt_run();
}

static int
prompt_run(void) {
    char c = '\0';

    do {
        if (c != '\0') {
          printf("\nInvalid input \"%c\"\n", c);
          printf("Iteration\n");
        }

        printf("Choose action [login(l)/listSessions(s)] default is login:");

        c = getchar();
        if (c == EOF) {
            perror("Error while read ");
            return 0;
        }
        printf("\n");

        if (c == '\n') {
            c = 'l';
        } else {
            //we need to eat the \n
            getchar();
        }

    } while (c != 'l' && c != 's');

    switch(c){
        case 'l':
            login();
        break;
        case 's':
            listsessions();
        break;
    }
    return 1;
}

static void
_data_cb(Spawny__Server__Data *msg) {
    data = msg;

    if (!prompted) {
        if (!prompt_run()) {
            printf("Failed to run prompt!!\n");
        }
    }
    prompted = 1;
}

static void
_login_cb(int success, char *msg) {
    if (!success) {
        printf("Login failed, reason %s\n", msg);
        //relogin
        prompt_run();
    } else {
        printf("Login Successfull, waiting for leave msg\n");
    }
}


int main(int argc, char **argv) {
    //eat all the stuff which is in stdin
    printf("#########################################\n");
    printf("# Spawny Command line greeter interface #\n");
    printf("#########################################\n");
    client_init(_data_cb, _login_cb);
    client_run();
    client_shutdown();
}