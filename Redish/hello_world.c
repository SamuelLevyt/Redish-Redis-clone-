#include <stdio.h>
#include <stdbool.h>
#include <string.h>

struct data{
    char key[100];
    char value[100];
};

void mainMenu();

int main() {
    struct data database[100];
    int dataIndex = 0;
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

            if (key == NULL || value == NULL){
                printf("\nUsage: SET <key> <value>\n");
                strcpy(mainCommand, "");
                continue;
            }

            if (dataIndex >= (int)(sizeof(database) / sizeof(database[0]))){
                printf("\nDatabase is full; cannot store more keys.\n");
                strcpy(mainCommand, "");
                continue;
            }

            strncpy(database[dataIndex].key, key, sizeof(database[dataIndex].key) - 1);
            database[dataIndex].key[sizeof(database[dataIndex].key) - 1] = '\0';
            strncpy(database[dataIndex].value, value, sizeof(database[dataIndex].value) - 1);
            database[dataIndex].value[sizeof(database[dataIndex].value) - 1] = '\0';
            dataIndex++;

            printf("\nThe \"%s\" key has been stored in memory.\n", key);

            strcpy(mainCommand, "");
        }

        if (!strcmp(command, "GET")){
            token = strtok(NULL, " ");
            key = token;

            if (key == NULL){
                printf("\nUsage: GET <key>\n");
                strcpy(mainCommand, "");
                continue;
            }

            for (int i = 0; i < dataIndex; i++){
                if (strcmp(database[i].key, key) == 0){
                    printf("\nThe value of the \"%s\" key is: %s\n", key, database[i].value);
                }
            }
        }
    }while (end == false);

    return 0;
}

void mainMenu(){
    printf("SET (key) (value) --> sets the key and value in the database.\nGET (key) --> retrieves the value that is set in the database with the same key.\nDEL (key) --> deletes the key and the value in the database.");
}