program PTLTest
    
    use PTLf

    implicit none
    integer :: ptl_id
    real*8, allocatable :: someRealData(:)
    integer, allocatable :: someIntData(:)
    integer :: someIntValue
    real*8  :: someRealValue
    
    print*, 'Testing PTL for Fortran...'
    call PTLf_Create_Tree(ptl_id)
    call PTLf_Tree_Read(ptl_id, 'input.ptl')
    call PTLf_Tree_Parse_Int(ptl_id,  'Section1.someIntValue', someIntValue)
    call PTLf_Tree_Parse_Real(ptl_id, 'Section1.someRealValue', someRealValue)
    call PTLf_Tree_Parse_RealVec(ptl_id, 'Section2.somRealValues', someRealData)
    call PTLf_Tree_Parse_IntVec(ptl_id, 'Section2.somIntValues', someIntData)
    
    print*, 'someIntValue = ', someIntValue
    print*, 'someRealValue = ', someRealValue
    print*, 'someRealData = ', someRealData
    print*, 'someIntData = ', someIntData
    
end program PTLTest
