! set_datetime_test:
!   Unit test for the `set_datetime` subroutine in goes_abi_converter_mod.
!
!   Description:
!     This program verifies that the `set_datetime` subroutine correctly computes
!     64-bit integer datetime values corresponding to known Gregorian date-times.
!     It checks two test cases:
!       - 2025-01-01 00:00:00 → 1735689600
!       - 2025-01-01 00:10:00 → 1735690200
!
!   Notes:
!     - Fails with a non-zero exit code if any computed datetime differs from the
!       expected reference value.
!     - Output messages indicate the failing index and the expected vs. actual values.
!     - This test assumes the datetime output represents seconds since
!       1970-01-01T00:00:00Z (Unix epoch).
program set_datetime_test
    use kinds, only : i_kind, i_llong
    use goes_abi_converter_mod, only : set_datetime
    implicit none
    integer(i_llong), dimension(2) :: datetime
    integer(i_llong), dimension(2) :: ref_datetime
    integer(i_kind), dimension(2) :: year, month, day, hour, minute, second
    integer :: i
    ref_datetime = [1735689600_i_llong, 1735690200_i_llong]  ! Reference epochs for 2025-01-01 00:00:00 and 2025-01-01 00:10:00
    year = [2025_i_kind, 2025_i_kind]
    month = [1_i_kind, 1_i_kind]
    day = [1_i_kind, 1_i_kind]
    hour = [0_i_kind, 0_i_kind]
    minute = [0_i_kind, 10_i_kind]
    second = [0_i_kind, 0_i_kind]
    call set_datetime(year(1), month(1), day(1), &
            hour(1), minute(1), second(1), datetime(1))
    call set_datetime(year(2), month(2), day(2), &
            hour(2), minute(2), second(2), datetime(2))
    do i = 1, 2
        if (datetime(i) /= ref_datetime(i)) then
            print *, "Test failed for datetime ", i
            print *, "Expected:", ref_datetime(i), "Got:", datetime(i)
            stop 1
        end if
    end do
end program
