EXE  := program
MAIN := $(wildcard *.c)
main:
	${C_HOST} ${ICONFIG} ${MAIN} -o ${EXE} ${LCONFIG}

clean:
	-rm ${EXE}

test: clean main
	./${EXE}
