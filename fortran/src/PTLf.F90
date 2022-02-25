module PTLf
	contains
	subroutine PTLf_Create_Tree(ptl_id)

		use, intrinsic :: iso_c_binding
		implicit none
		integer(c_int), intent(out) :: ptl_id
		interface
			subroutine PTLC_Create_Tree(ptl_id_f) bind (c,name="PTLC_Create_Tree")
				use iso_c_binding
				integer (c_int), intent (out) :: ptl_id_f
			end subroutine PTLC_Create_Tree
		end interface
		call PTLC_Create_Tree(ptl_id)

	end subroutine PTLf_Create_Tree

	subroutine PTLf_Tree_Read(ptl_id, filename)
		
		use, intrinsic :: iso_c_binding
		implicit none
		integer(c_int), intent(in) :: ptl_id
		character*(*),  intent(in) :: filename
		interface
			subroutine PTLC_Tree_Read(ptl_id_f, chars_f, len_f) bind (c,name="PTLC_Tree_Read")
				use iso_c_binding
				integer   (c_int),  intent(in) :: ptl_id_f
				character (c_char), intent(in) :: chars_f
				integer   (c_int),  intent(in) :: len_f
			end subroutine PTLC_Tree_Read
		end interface
		call PTLC_Tree_Read(ptl_id, filename, len(filename))
		
	end subroutine PTLf_Tree_Read
	
	subroutine PTLf_Tree_Parse_Int(ptl_id, var_string, value_Int)
	
		use, intrinsic :: iso_c_binding
		implicit none
		integer(c_int), intent(in)  :: ptl_id
		character*(*),  intent(in)  :: var_string
		integer(c_int), intent(out) :: value_Int
		interface
			subroutine PTLC_Tree_Parse_Int(ptl_id_f, chars_f, len_f, val_out) bind(c, name="PTLC_Tree_Parse_Int")
				use iso_c_binding
				integer   (c_int),  intent(in) :: ptl_id_f
				character (c_char), intent(in) :: chars_f
				integer   (c_int),  intent(in) :: len_f
				integer   (c_int),  intent(in) :: val_out
			end subroutine PTLC_Tree_Parse_Int
		end interface
		call PTLC_Tree_Parse_Int(ptl_id, var_string, len(var_string), value_Int)
		
	end subroutine PTLf_Tree_Parse_Int
	
	subroutine PTLf_Tree_Parse_Real(ptl_id, var_string, value_Real)
	
		use, intrinsic :: iso_c_binding
		implicit none
		integer(c_int),    intent(in)  :: ptl_id
		character*(*),     intent(in)  :: var_string
		real(c_double),    intent(out) :: value_Real
		interface
			subroutine PTLC_Tree_Parse_Real(ptl_id_f, chars_f, len_f, val_out) bind(c, name="PTLC_Tree_Parse_Real")
				use iso_c_binding
				integer   (c_int),    intent(in) :: ptl_id_f
				character (c_char),   intent(in) :: chars_f
				integer   (c_int),    intent(in) :: len_f
				real      (c_double), intent(in) :: val_out
			end subroutine PTLC_Tree_Parse_Real
		end interface
		call PTLC_Tree_Parse_Real(ptl_id, var_string, len(var_string), value_Real)
		
	end subroutine PTLf_Tree_Parse_Real
	
end module PTLf