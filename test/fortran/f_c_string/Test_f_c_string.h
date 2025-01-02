#ifndef OBS2IODA_TEST_F_C_STRING_H
#define OBS2IODA_TEST_F_C_STRING_H

void to_upper(char *c_string);

void to_upper_array(char **c_string_array, int m);

void variable_length_string_array(char ***c_string_array, int m);

void free_variable_length_string_array(char ***c_string_array, int m);

#endif //OBS2IODA_TEST_F_C_STRING_H
