#include "Test_f_c_string.h"
#include <ctype.h>
#include <stdlib.h>
#include <stdio.h>

void to_upper(char *c_string) {
    char *str = c_string;
    for (int i = 0; str[i] != '\0'; i++) {
        str[i] = toupper(str[i]);
    }
}

void to_upper_array(char **c_string_array, int m) {
    for (int i = 0; i < m; i++) {
        to_upper(c_string_array[i]);
    }
}

void variable_length_string_array(char ***c_string_array, int m) {
    *c_string_array = (char **) malloc(m * sizeof(char *));
    for (int i = 0; i < m; i++) {
        (*(c_string_array))[i] = (char *) malloc(i + 1 * sizeof(char) + 1);
    }
    for (int i = 0; i < m; i++) {
        for (int j = 0; j < i+1; j++) {
            (*(c_string_array))[i][j] = 'a';
        }
        (*(c_string_array))[i][i+1] = '\0';

    }
}

void free_variable_length_string_array(char ***c_string_array, int m) {
    for (int i = 0; i < m; i++) {
        free((*(c_string_array))[i]);
    }
    free(*c_string_array);
}
