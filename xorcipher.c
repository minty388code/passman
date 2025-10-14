#include"xorcipher.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

char* xorcipher(const char *input, const char *salt) {
    // input either hello->2?665 or input 2?665->hello but this is reversible.
    if (input == NULL) return 0;
    int length = strlen(input);
    char *binary = malloc(8*length + 1);
    binary[0] = '\0';
    for (int i = 0; i<length; ++i) {
        char ch = input[i];
        for (int j = 7; j>=0; --j) {
            if (ch & (1 << j)) {
                strcat(binary, "1");
            } else {
                strcat(binary, "0");
            }
        }
        strcat(binary, " ");
    }
    printf("%s", binary);

    char result;
    return binary;
}