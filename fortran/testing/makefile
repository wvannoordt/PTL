basedir = $(realpath ../../)

fc := ${FC_HOST}
ifndef FC_HOST
fc := gfortran
endif

run: main
	./program

main:
	${fc} -I${basedir}/include main.F90 -o program -L${basedir}/lib -lPTL -lstdc++

clean:
	rm -f program
