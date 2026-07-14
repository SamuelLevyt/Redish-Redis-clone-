#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "store.h"

enum { COMMAND_CAPACITY = 256 };

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
    Store database;
    char mainCommand[COMMAND_CAPACITY];
    char *command;
    char *key;
    char *value;

    store_init(&database);

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

        command = strtok(mainCommand, TOKEN_DELIMITERS);
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
            key = strtok(NULL, TOKEN_DELIMITERS);
            value = strtok(NULL, TOKEN_DELIMITERS);

            if (key == NULL || value == NULL ||
                strtok(NULL, TOKEN_DELIMITERS) != NULL) {
                fprintf(stderr, "Usage: SET <key> <value>\n");
                continue;
            }
            if (strlen(key) >= STORE_KEY_SIZE) {
                fprintf(stderr, "Key is too long; the limit is %d characters.\n",
                        STORE_KEY_SIZE - 1);
                continue;
            }
            if (strlen(value) >= STORE_VALUE_SIZE) {
                fprintf(stderr, "Value is too long; the limit is %d characters.\n",
                        STORE_VALUE_SIZE - 1);
                continue;
            }
            if (!store_set(&database, key, value)) {
                fprintf(stderr, "Failed to store key \"%s\"; the database is full.\n",
                        key);
                continue;
            }

            printf("\nThe \"%s\" key has been stored in memory.\n", key);
            continue;
        }

        if (strcmp(command, "GET") == 0) {
            const char *stored;

            key = strtok(NULL, TOKEN_DELIMITERS);
            if (key == NULL || strtok(NULL, TOKEN_DELIMITERS) != NULL) {
                fprintf(stderr, "Usage: GET <key>\n");
                continue;
            }

            stored = store_get(&database, key);
            if (stored == NULL) {
                fprintf(stderr, "Key \"%s\" was not found.\n", key);
                continue;
            }
            printf("\nThe value of the \"%s\" key is: %s\n", key, stored);
            continue;
        }

        if (strcmp(command, "DEL") == 0) {
            key = strtok(NULL, TOKEN_DELIMITERS);
            if (key == NULL || strtok(NULL, TOKEN_DELIMITERS) != NULL) {
                fprintf(stderr, "Usage: DEL <key>\n");
                continue;
            }

            if (!store_del(&database, key)) {
                fprintf(stderr, "Key \"%s\" was not found.\n", key);
                continue;
            }
            printf("\nThe \"%s\" key has been deleted.\n", key);
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
           "DEL <key> --> deletes the key from the database.\n"
           "help --> displays this menu.\n"
           "end --> exits Redish.\n");
}
