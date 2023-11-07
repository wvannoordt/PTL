program PTLTest
    
    use PTLf

    implicit none
    integer :: ptl_id
    real*8, allocatable :: someRealData(:)
    integer, allocatable :: someIntData(:)
    integer :: someIntValue
    real*8  :: someRealValue, missingValue
    character*128 :: someStringValue !note that if this is too small, then PTL will throw an error
    
    print*, 'Testing PTL for Fortran...'
    call PTLf_Except_On_Missing(.false.) ! if set to true, we will crash when an option is missing
    call PTLf_Create_Tree(ptl_id)
    call PTLf_Tree_Read(ptl_id, 'input.ptl')
    call PTLf_Tree_Parse_Int(ptl_id,  'Section1.someIntValue', someIntValue)
    call PTLf_Tree_Parse_Real(ptl_id, 'Section1.someRealValue', someRealValue)
    call PTLf_Tree_Parse_RealVec(ptl_id, 'Section2.someRealValues', someRealData)
    call PTLf_Tree_Parse_IntVec(ptl_id, 'Section2.someIntValues', someIntData)
    call PTLf_Tree_Parse_String(ptl_id, 'Section2.someStringValue', someStringValue)
    
    ! We need to default values so that if they aren't found, they still have valid data
    missingValue = -1.2398
    call PTLf_Tree_Parse_Real(ptl_id, 'Section1.missingValue', missingValue)
    
    print*, 'someIntValue    = ', someIntValue
    print*, 'someRealValue   = ', someRealValue
    print*, 'someRealData    = ', someRealData
    print*, 'someIntData     = ', someIntData
    print*, 'someStringValue = ', trim(someStringValue)
    print*, 'missingValue    = ', missingValue
    
end program PTLTest
