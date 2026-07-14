#include <stdio.h>
#include <stdbool.h>
#include <string.h>

#include "store.h"

void mainMenu();

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
                if (strstr(mainCommand, "SET") == NULL && strstr(mainCommand, "GET") == NULL && strstr(mainCommand, "DEL") == NULL && strcmp(mainCommand, "end") != 0 && strcmp(mainCommand, "help") != 0){
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
        }while (strstr(mainCommand, "SET") == 0 && strstr(mainCommand, "GET") == 0 && strstr(mainCommand, "DEL") == 0);

        token = strtok(mainCommand, " ");
        command = token;

        if (!strcmp(command, "SET")){
            token = strtok(NULL, " ");
            key = token;
            token = strtok(NULL, " ");
            value = token;

            if (store_set(&database, key, value)){
                printf("\nThe \"%s\" key has been stored in memory.\n", key);
            }

            strcpy(mainCommand, "");
        }

        if (!strcmp(command, "GET")){
            token = strtok(NULL, " ");
            key = token;

            const char *stored = store_get(&database, key);
            if (stored != NULL){
                printf("\nThe value of the \"%s\" key is: %s\n", key, stored);
            }
        }

        if (!strcmp(command, "DEL")){
            token = strtok(NULL, " ");
            key = token;

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
