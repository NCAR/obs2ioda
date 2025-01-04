program Test_f_c_string_t_driver
    use Test_f_c_string_1D_mod, only : Test_f_c_string_1D_t
    implicit none

    type(Test_f_c_string_1D_t) :: test_f_c_string_1D
    call test_f_c_string_1D%f_c_string_t_1D_single_string_single_instance()

end program Test_f_c_string_t_driver
