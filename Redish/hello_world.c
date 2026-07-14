#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

enum {
    DATABASE_CAPACITY = 100,
    KEY_CAPACITY = 100,
    VALUE_CAPACITY = 100,
    COMMAND_CAPACITY = 256
};

struct data {
    char key[KEY_CAPACITY];
    char value[VALUE_CAPACITY];
};

static const char TOKEN_DELIMITERS[] = " \t";

enum read_result {
    READ_OK,
    READ_EOF,
    READ_TOO_LONG,
    READ_ERROR
};

static void main_menu(void);
static enum read_result read_command(char command[COMMAND_CAPACITY]);

int main(void) {
    struct data database[DATABASE_CAPACITY];
    int dataIndex = 0;
    char mainCommand[COMMAND_CAPACITY];
    char *token;
    char *command;
    char *key;
    char *value;

    while (true) {
        enum read_result result;

        printf("\nType \"help\" to see all the commands you can use.\n");
        printf("Enter your command: ");
        if (fflush(stdout) == EOF) {
            perror("Failed to display the command prompt");
            return EXIT_FAILURE;
        }

        result = read_command(mainCommand);
        if (result == READ_EOF) {
            printf("\nExiting...\n");
            return EXIT_SUCCESS;
        }
        if (result == READ_ERROR) {
            perror("Failed to read command");
            return EXIT_FAILURE;
        }
        if (result == READ_TOO_LONG) {
            fprintf(stderr, "Command is too long; the limit is %d characters.\n",
                    COMMAND_CAPACITY - 1);
            continue;
        }

        token = strtok(mainCommand, TOKEN_DELIMITERS);
        command = token;
        if (command == NULL) {
            continue;
        }

        if (strcmp(command, "help") == 0) {
            if (strtok(NULL, TOKEN_DELIMITERS) != NULL) {
                fprintf(stderr, "Usage: help\n");
                continue;
            }
            main_menu();
            continue;
        }

        if (strcmp(command, "end") == 0) {
            if (strtok(NULL, TOKEN_DELIMITERS) != NULL) {
                fprintf(stderr, "Usage: end\n");
                continue;
            }
            printf("Exiting...\n");
            return EXIT_SUCCESS;
        }

        if (strcmp(command, "SET") == 0) {
            token = strtok(NULL, TOKEN_DELIMITERS);
            key = token;
            token = strtok(NULL, TOKEN_DELIMITERS);
            value = token;

            if (key == NULL || value == NULL ||
                strtok(NULL, TOKEN_DELIMITERS) != NULL) {
                fprintf(stderr, "Usage: SET <key> <value>\n");
                continue;
            }
            if (strlen(key) >= KEY_CAPACITY) {
                fprintf(stderr, "Key is too long; the limit is %d characters.\n",
                        KEY_CAPACITY - 1);
                continue;
            }
            if (strlen(value) >= VALUE_CAPACITY) {
                fprintf(stderr, "Value is too long; the limit is %d characters.\n",
                        VALUE_CAPACITY - 1);
                continue;
            }
            if (dataIndex >= DATABASE_CAPACITY) {
                fprintf(stderr, "Database is full; cannot store another key.\n");
                continue;
            }

            memcpy(database[dataIndex].key, key, strlen(key) + 1);
            memcpy(database[dataIndex].value, value, strlen(value) + 1);
            dataIndex++;

            printf("\nThe \"%s\" key has been stored in memory.\n", key);
            continue;
        }

        if (strcmp(command, "GET") == 0) {
            bool found = false;

            token = strtok(NULL, TOKEN_DELIMITERS);
            key = token;

            if (key == NULL || strtok(NULL, TOKEN_DELIMITERS) != NULL) {
                fprintf(stderr, "Usage: GET <key>\n");
                continue;
            }

            for (int i = 0; i < dataIndex; i++) {
                if (strcmp(database[i].key, key) == 0) {
                    printf("\nThe value of the \"%s\" key is: %s\n", key, database[i].value);
                    found = true;
                }
            }
            if (!found) {
                fprintf(stderr, "Key \"%s\" was not found.\n", key);
            }
            continue;
        }

        if (strcmp(command, "DEL") == 0) {
            key = strtok(NULL, TOKEN_DELIMITERS);
            if (key == NULL || strtok(NULL, TOKEN_DELIMITERS) != NULL) {
                fprintf(stderr, "Usage: DEL <key>\n");
                continue;
            }
            fprintf(stderr, "DEL is not implemented.\n");
            continue;
        }

        fprintf(stderr, "Invalid command: %s\n", command);
    }
}

static enum read_result read_command(char command[COMMAND_CAPACITY]) {
    int next_character;

    if (fgets(command, COMMAND_CAPACITY, stdin) == NULL) {
        return ferror(stdin) ? READ_ERROR : READ_EOF;
    }

    if (strchr(command, '\n') == NULL && !feof(stdin)) {
        do {
            next_character = fgetc(stdin);
        } while (next_character != '\n' && next_character != EOF);

        if (ferror(stdin)) {
            return READ_ERROR;
        }
        return READ_TOO_LONG;
    }

    command[strcspn(command, "\r\n")] = '\0';
    return READ_OK;
}

static void main_menu(void) {
    printf("SET <key> <value> --> sets the key and value in the database.\n"
           "GET <key> --> retrieves the value set for the key.\n"
           "DEL <key> --> not implemented.\n"
           "help --> displays this menu.\n"
           "end --> exits Redish.\n");
}
