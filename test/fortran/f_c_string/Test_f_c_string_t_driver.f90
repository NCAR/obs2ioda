program Test_f_c_string_t_driver
    use Test_f_c_string_t_mod, only : Test_f_c_string_t
    use Test_f_c_string_1D_mod, only : Test_f_c_string_t_1D
    implicit none

    call Test_f_c_string_t()
    call Test_f_c_string_t_1D()

end program Test_f_c_string_t_driver
