
.SUFFIXES:
.SUFFIXES: .c .o

FLAVOR ?= i8080
# FLAVOR ?= i8085
# FLAVOR ?= z80
# FLAVOR ?= z80intel

EMBEDS = ${FLAVOR}_primary.txt

ifeq (FLAVOR, z80intel)
EMBEDS += ${FLAVOR}_cb.h ${FLAVOR}_dd.h ${FLAVOR}_ed.h ${FLAVOR}_fd.h
endif

ifeq (FLAVOR, z80)
EMBEDS += ${FLAVOR}_cb.h ${FLAVOR}_dd.h ${FLAVOR}_ed.h ${FLAVOR}_fd.h
endif

build: dism${FLAVOR}

clean:
	rm -f *.tmp
	rm -f *.o
	rm -f opcodes.c
	rm -f dism${FLAVOR}

.c.o:
	cc -c -o $@ $< -DDISM_FLAVOR=${FLAVOR}

opcodes.c: ${EMBEDS}
	./gen_c_embeds $@ $^

dismi8080: dism.o opcodes.o
	cc -o $@ $^

# end of file
# vim: noet
