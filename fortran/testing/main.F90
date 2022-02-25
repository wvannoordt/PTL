program PTLTest
    
    use PTLf

    implicit none
    integer :: ptl_id
    real*8, allocatable :: data(:)
    integer :: someIntValue
    real*8  :: someRealValue
    
    print*, 'Testing PTL for Fortran...'
    call PTLf_Create_Tree(ptl_id)
    call PTLf_Tree_Read(ptl_id, 'input.ptl')
    call PTLf_Tree_Parse_Int(ptl_id,  'Section.someIntValue', someIntValue)
    call PTLf_Tree_Parse_Real(ptl_id, 'Section.someRealValue', someRealValue)
    
    print*, someIntValue, someRealValue
    
end program PTLTest
