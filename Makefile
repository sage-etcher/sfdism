
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

ifeq (${EXTENDED_OPCODES},y)
EMBEDS += ${FLAVOR}/bit.txt ${FLAVOR}/misc.txt \
		  ${FLAVOR}/ix_bit.txt ${FLAVOR}/ix.txt \
		  ${FLAVOR}/iy_bit.txt ${FLAVOR}/iy.txt
endif

build: dism${FLAVOR}

clean:
	rm -f *.tmp
	rm -f *.o
	rm -f opcodes.c
	rm -f dism${FLAVOR}

.c.o:
	cc -c -o $@ $< -DFLAVOR=${FLAVOR}

opcodes.c: ${EMBEDS}
	./gen_c_embeds $@ ${EMBEDS}

dism${FLAVOR}: dism.o opcodes.o
	cc -o $@ $^ -DFLAVOR=${FLAVOR}

# end of file
# vim: noet
