
.SUFFIXES:
.SUFFIXES: .c .o

FLAVOR ?= i8080
# FLAVOR ?= i8085
# FLAVOR ?= z80
# FLAVOR ?= z80intel

EMBEDS = ${FLAVOR}/primary.txt

EXTENDED_OPCODES=	n

ifeq (${FLAVOR},z80)
EXTENDED_OPCODES=	y
endif

ifeq (${FLAVOR},z80intel)
EXTENDED_OPCODES=	y
endif

ifeq (${EXTENDED_OPCODES},y)
EMBEDS += ${FLAVOR}/bit.txt ${FLAVOR}/misc.txt \
		  ${FLAVOR}/ix_bit.txt ${FLAVOR}/ix.txt \
		  ${FLAVOR}/iy_bit.txt ${FLAVOR}/iy.txt
endif

buildall:
	${MAKE} build FLAVOR=i8080
	#${MAKE} build FLAVOR=i8085
	${MAKE} build FLAVOR=z80
	${MAKE} build FLAVOR=z80intel

build: dism${FLAVOR}

clean:
	rm -f *.tmp
	rm -f *.o
	rm -f i8080_opcodes.c
	rm -f i8085_opcodes.c
	rm -f z80_opcodes.c
	rm -f z80intel_opcodes.c
	rm -f dismi8080
	rm -f dismi8085
	rm -f dismz80
	rm -f dismz80intel

.c.o:
	cc -c -o $@ $< -DFLAVOR=${FLAVOR}

${FLAVOR}_opcodes.c: ${EMBEDS}
	./gen_c_embeds $@ ${EMBEDS}

dism${FLAVOR}: dism.o ${FLAVOR}_opcodes.o
	cc -o $@ $^ -DFLAVOR=${FLAVOR}

# end of file
# vim: noet
