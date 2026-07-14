#include <stdio.h>
#include <stdbool.h>
#include <string.h>

struct data{
    char key[100];
    char value[100];
};

void mainMenu();
bool hasDataCommand(const char *input);
char *nextToken();

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

            strcpy(database[dataIndex].key, key);
            strcpy(database[dataIndex].value, value);
            dataIndex++;

            printf("\nThe \"%s\" key has been stored in memory.\n", key);

            strcpy(mainCommand, "");
        }

        if (!strcmp(command, "GET")){
            key = nextToken();

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

bool hasDataCommand(const char *input){
    return strstr(input, "SET") != NULL || strstr(input, "GET") != NULL || strstr(input, "DEL") != NULL;
}

char *nextToken(){
    return strtok(NULL, " ");
}
