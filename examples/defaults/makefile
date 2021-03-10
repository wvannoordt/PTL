EXE  := program
MAIN := $(wildcard *.cc)
main:
	${CC_HOST} ${C11F} ${ICONFIG} ${MAIN} -o ${EXE} ${LCONFIG}

clean:
	-rm ${EXE}

test: clean main
	./${EXE}
