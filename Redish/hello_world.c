#include <stdio.h>
#include <stdbool.h>
#include <string.h>

#include "store.h"

void mainMenu();
bool hasDataCommand(const char *input);
char *nextToken();

int main() {
    Store database;
    store_init(&database);
    bool end = false;
    char mainCommand[100] = "";
    char *token;
    char *command;
    char *key;
    char *value;

    do{
        do{
            if (strcmp(mainCommand, "") != 0){
                if (!hasDataCommand(mainCommand) && strcmp(mainCommand, "end") != 0 && strcmp(mainCommand, "help") != 0){
                    printf("\nInvalid command.\n");
                }
            }
            if (strcmp(mainCommand, "help") == 0){
                mainMenu();
            }
            if (strcmp(mainCommand, "end") == 0){
                end = true;
                printf("Exiting...");
                break;
            }
            printf("\nType \"help\" to see all the commands you can use.\n");
            printf("Enter your command: ");
            fgets(mainCommand, 50, stdin);
            mainCommand[strcspn(mainCommand, "\n")] = '\0';
        }while (!hasDataCommand(mainCommand));

        token = strtok(mainCommand, " ");
        command = token;

        if (!strcmp(command, "SET")){
            key = nextToken();
            value = nextToken();

            if (store_set(&database, key, value)){
                printf("\nThe \"%s\" key has been stored in memory.\n", key);
            }

            strcpy(mainCommand, "");
        }

        if (!strcmp(command, "GET")){
            key = nextToken();

            const char *stored = store_get(&database, key);
            if (stored != NULL){
                printf("\nThe value of the \"%s\" key is: %s\n", key, stored);
            }
        }

        if (!strcmp(command, "DEL")){
            key = nextToken();

            if (store_del(&database, key)){
                printf("\nThe \"%s\" key has been deleted.\n", key);
            }
        }
    }while (end == false);

    return 0;
}

void mainMenu(){
    printf("SET (key) (value) --> sets the key and value in the database.\nGET (key) --> retrieves the value that is set in the database with the same key.\nDEL (key) --> deletes the key and the value in the database.");
}

bool hasDataCommand(const char *input){
    return strstr(input, "SET") != NULL || strstr(input, "GET") != NULL || strstr(input, "DEL") != NULL;
}

char *nextToken(){
    return strtok(NULL, " ");
}
