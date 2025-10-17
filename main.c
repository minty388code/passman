#include<stdio.h>
#include<string.h>
#include<conio.h>
#include<time.h>
#include<stdlib.h>

#include"xorcipher.h"

char* getSalt() {
    static char salt[9];
    salt[0] = '\0';
    for (int i = 0; i<8; i++) {
        char bit = (rand() % 2) + '0';
        strncat(salt, &bit, 1);
    }
    return salt;
}

int checkUser(char *username) {
    FILE *fptr;
    fptr = fopen("users.txt", "r");

    char file[2048];
    
    while (fgets(file, 2048, fptr)) {
        char *token = strtok(file, ":");
        if (strcmp(token, username)==0) {
            return 0;
        }
    }
        
    return 1;
}

char *getPass(char *username) {
    FILE *fptr;
    fptr = fopen("users.txt", "r");

    char file[2048];

    while (fgets(file, 2048, fptr)) {
        char *token = strtok(file, ":");
        if (strcmp(token, username)==0) {
            // If the username is found this code will run 
            token = strtok(NULL, ":");
            return token;
        }
    }
    return "Error - username was not found";
}

char *getUserSalt(char *username) {
    FILE *fptr;
    fptr = fopen("users.txt", "r");

    char file[2048];

    while (fgets(file, 2048, fptr)) {
        char *token = strtok(file, ":");
        if (strcmp(token, username)==0) {
            // If the username is found this code will run 
            token = strtok(NULL, ":");
            token = strtok(NULL, ":");
            return token;
        }
    }
    return "Error - username was not found";
}

char *genPass(int size) {
    char chars[] = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ1234567890!@#$%%^&*()[]{}\"'|,./<>?;:";
    int len = strlen(chars);
    char *password = malloc(size+1);
    for (int i=0; i<size; i++) {
        int ind = rand() % len;
        password[i] = chars[ind];
    }
    password[size] = '\0';
    return password;
}

void newUser(char *user, char *pass, char *salt) {
    FILE *fptr;
    fptr = fopen("users.txt", "a");
    
    fprintf(fptr, "\n%s:%s:%s", user, pass, salt);
    
    fclose(fptr);
}

// This is for password input
void getPassword(char *password, size_t size) {
    size_t i = 0;
    int ch;
    while (1) {
        ch = _getch();

        if (ch == 13) {
            break;
        } else if (ch == 8) {
            if (i > 0) {
                i--;
                printf("\b \b");  // erase last *
            }
        // Probably need to fix something here so that cant enter anything other than a literal
        } else if (i < size - 1 && ch != 0 && ch != 224) {
            password[i++] = ch;
            printf("*");
        }
    }

    password[i] = '\0';  // null-terminate string
    printf("\n");
}

int inp;

int main() {
    srand(time(NULL));
    system("cls");

    // Step 1 - login/signup section

    printf("Password Manager Version 1.0\nPress Ctrl+c to Exit.\n\n");
    while (1) {
        printf("Login or setup? (1/2): ");
        scanf("%d", &inp);
        if (inp == 1) {
            // code for login system
            printf("You have selected to login.\n");
            break;
        } else if (inp == 2) {
            // code for setup system
            printf("You have selected to setup.\n");
            break;
        } else {
            printf("Input invalid. Please try again.\n");
        }
    }

    char sessionUser[25];
    char sessionPass[25];
    char sessionSalt[9];

    if (inp==1) {
        // Login system
        printf("");
        char username[25];
        while (1) {
            printf("Enter your username: ");
            scanf("%s", username);
            if (checkUser(username)==0) {
                break;
            } else {
                printf("Username not found, please try again.\n");
            }
        }

        char password[25];
        while (1) {
            printf("Please enter your password: ");
            getPassword(password, sizeof(password));
            if (strcmp(password, getPass(username))==0) {
                printf("Hello, %s! You have successfully loged in!\n", username);
                strcpy(sessionUser, username);
                strcpy(sessionPass, password);
                strcpy(sessionSalt, getUserSalt(username));
                break;
            } else {
                printf("Password is incorrect, please try again.\n");
            }
        }
        
    } else {
        char username[25];
        while (1) {
            printf("Enter a username for your account: ");
            scanf("%s", username);
            if (checkUser(username)) {
                break;
            } else {
                printf("Sorry, that username is taken, please enter a different one.\n");
            }
        }

        char password[25];
        while (1) {
            char i1[25];
            char i2[25];

            printf("Please enter a password: ");
            getPassword(i1, sizeof(i1));

            printf("Please re-enter your password: ");
            getPassword(i2, sizeof(i2));

            if (strcmp(i1, i2) == 0) {
                strcpy(password, i1);
                strcpy(sessionUser, username);
                strcpy(sessionPass, password);
                strcpy(sessionSalt,getSalt());

                newUser(sessionUser, sessionPass, sessionSalt);
                
                char path[35];
                snprintf(path, sizeof(path), "users/%s.txt", username);

                FILE *fptr;
                fptr = fopen(path, "w");
                fclose(fptr);

                printf("User succesfully created!\n");
                break;
            } else {
                printf("Passwords do not match. Please try again.\n");
            }
        }
    }
    
    // ? Now need to start creating the actual password manager 
    // printf("\nFor current session:\nUsername: %s, Password: %s, Salt: %s.\n", sessionUser, sessionPass, sessionSalt);

    char sessionPath[35];
    snprintf(sessionPath, sizeof(sessionPath), "users/%s.txt", sessionUser);
            

    while (1) {
        char i[7];
        printf("%s: ", sessionUser);
        scanf("%s", i);
        if (strcmp(i, "exit")==0) {
            break;
        } else if (strcmp(i, "help")==0) {
            printf("\n\
        List of Commands\n\
        help - get help to use the program\n\
        exit - terminate the program\n\
        centry - create a password entry\n\
        eentry - edit a password entry\n\
        dentry - delete a password entry\n\n");
        } else if (strcmp(i, "eentry")==0) {
            // ! Have to add checking whether the entry already exists or not
            char entryname[25];
            printf("Enter entry name: ");
            scanf("%s", entryname);

            char passChoice;
            printf("Should your password be generated? (Y/n): ");
            scanf(" %c", &passChoice);
            if (passChoice == 'n') {
                while (1) {
                    char i1[25];
                    char i2[25];

                    printf("Enter password: ");
                    getPassword(i1, sizeof(i1));
                    
                    printf("Please confirm password: ");
                    getPassword(i2, sizeof(i2));
                    
                    if (strcmp(i1, i2) == 0) {
                        FILE *fptr;
                        fptr = fopen(sessionPath, "a");
                        fprintf(fptr, "\n%s:%s", entryname, i1);
                        
                        fclose(fptr);
                        break;
                    } else {
                        printf("Passwords do not match, please try again.\n");
                    }
                }
            } else {
                int len;
                printf("Enter password length: ");
                scanf("%i", &len);

                FILE *fptr;
                fptr = fopen(sessionPath, "a");
                fprintf(fptr, "\n%s:%s", entryname, genPass(len));
                
                fclose(fptr);
            }
            printf("Entry succesfully created.\n\n");

        } else if (strcmp(i, "dentry")==0) {
            // Next step.
            printf("Not created yet.");
        } else if (strcmp(i, "eentry")==0) {
            printf("Not created yet.");
        } else {
            printf("Command not found, for help enter command \"help\"\n\n");
        } 

        // help
        // centry
        // dentry
        // eentry
        // exit
    }

    return 0;
}