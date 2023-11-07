LIB_NAME := PTL

ifndef OPTLEVEL
OPTLEVEL := 0
endif

ifndef TESTS
TESTS := testing/*
endif


BASEIDIR  := $(shell pwd)
SRC_DIR   := ${BASEIDIR}/src
LIB_DIR   := ${BASEIDIR}/lib
OBJ_DIR   := ${BASEIDIR}/obj
BIN_DIR   := ${BASEIDIR}/bin
UTL_DIR   := ${BASEIDIR}/util
HDR_DIR   := ${BASEIDIR}/include
export BASEIDIR
export LIB_DIR
export HDR_DIR
export BIN_DIR

IFLAGS := -I${HDR_DIR}

SRC_FILES   := $(wildcard ${SRC_DIR}/*.cpp)
SRC_FILES_C := $(wildcard ${SRC_DIR}/*.c)

HEADER_FILES := $(wildcard ${SRC_DIR}/*.h)

OBJ_FILES   := $(patsubst ${SRC_DIR}/%.cpp,$(OBJ_DIR)/%.o,$(SRC_FILES))
OBJ_FILES_C := $(patsubst ${SRC_DIR}/%.c,$(OBJ_DIR)/%.o,$(SRC_FILES_C))

TARGET := ${LIB_DIR}/lib${LIB_NAME}.a

ifndef CC_HOST
CC_HOST := $(shell which g++)
endif

ifndef C_HOST
C_HOST := $(shell which gcc)
endif

ifndef FC_HOST
FC_HOST := $(shell which gfortran)
endif
export FC_HOST

LINKS :=

PTL_FORTRAN :=
OBJ_FILES_F := 

ifndef PTLF
PTLF := 1
endif

ifeq (${PTLF},1)
PTL_FORTRAN := fort
OBJ_FILES_F := ${OBJ_DIR}/PTLf.o
LINKS += -lgfortran
endif

C11F := -fpermissive -std=c++11 -fPIC
export C11F

HOST_FLAGS   := -O${OPTLEVEL} -Wno-unknown-pragmas -g -DGLIBCXX_FORCE_NEW=1 ${C11F}
HOST_FLAGS_C := -O${OPTLEVEL} -g -fpermissive

export CC_HOST
export C_HOST
export ICONFIG=-I${HDR_DIR}
export LCONFIG=-L${LIB_DIR} -l${LIB_NAME}

.PHONY: executables
executables: final
	@for fldr in executables/* ; do \
				${MAKE} -C $${fldr} -f makefile || exit 1; \
		done

fort:
	${FC_HOST} -fPIC ${COMPILE_TIME_OPT} ${IFLAGS} ${BASEIDIR}/fortran/src/PTLf.F90 -c -o ${OBJ_DIR}/PTLf.o
	mv ${BASEIDIR}/ptlf.mod ${HDR_DIR}

forttest: final
	${MAKE} -C ${BASEIDIR}/fortran/testing -f makefile run

.PHONY: final
final: setup ${OBJ_FILES} ${PTL_FORTRAN} ${OBJ_FILES_C}
	#${CC_HOST} ${COMPILE_TIME_OPT} ${OBJ_FILES} ${OBJ_FILES_C} ${OBJ_FILES_F} ${IFLAGS} -o ${TARGET} ${LINKS}
	ar rcs ${TARGET} ${OBJ_FILES} ${OBJ_FILES_C} ${OBJ_FILES_F}

${OBJ_FILES}: ${OBJ_DIR}/%.o : ${SRC_DIR}/%.cpp
	${CC_HOST} ${HOST_FLAGS} ${COMPILE_TIME_OPT} ${IFLAGS} -c $< -o $@

${OBJ_FILES_C}: ${OBJ_DIR}/%.o : ${SRC_DIR}/%.c
	${C_HOST} ${HOST_FLAGS_C} ${COMPILE_TIME_OPT} ${IFLAGS} -c $< -o $@

setup:
	-rm -r ${HDR_DIR}
	mkdir -p ${LIB_DIR}
	mkdir -p ${OBJ_DIR}
	mkdir -p ${HDR_DIR}
	mkdir -p ${BIN_DIR}
	@for hdr in ${HEADER_FILES} ; do \
		ln -sf $${hdr} -t ${HDR_DIR};\
	done

test: executables
	@for fldr in ${TESTS} ; do \
				echo $${fldr} ; \
				ln -sf ${UTL_DIR}/makefile.test $${fldr}/makefile; \
                ${MAKE} -C $${fldr} -f makefile -s test || exit 1; \
        done
	@for fldr in examples/* ; do \
                ${MAKE} -C $${fldr} -f makefile || exit 1; \
        done
	@echo "${LIB_NAME} passed all tests."

clean:
	${MAKE} -C fortran/testing -f makefile clean
	for fldr in testing/* ; do \
	            ${MAKE} -C $${fldr} -f makefile clean ; \
				rm -f $${fldr}/makefile ; \
	    done
	for fldr in examples/* ; do \
                ${MAKE} -C $${fldr} -f makefile clean || exit 1; \
        done
	-rm -r ${LIB_DIR}
	-rm -r ${OBJ_DIR}
	-rm -r ${HDR_DIR}
	-rm -r ${BIN_DIR}
