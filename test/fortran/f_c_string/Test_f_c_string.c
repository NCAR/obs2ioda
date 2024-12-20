#include "Test_f_c_string.h"
#include <ctype.h>
#include <stdio.h>

void to_upper(char *c_string) {
    char *str = c_string;
    for (int i = 0; str[i] != '\0'; i++) {
        str[i] = toupper(str[i]);
    }
}

void to_upper_array(char **c_string, int n) {
    for (int i = 0; i < n; i++) {
        to_upper(c_string[i]);
    }
}
