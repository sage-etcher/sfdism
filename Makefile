
.SUFFIXES:
.SUFFIXES: .txt .h .c .o

FLAVOR ?= i8080
# FLAVOR ?= i8085
# FLAVOR ?= z80
# FLAVOR ?= z80intel

EMBEDS = ${FLAVOR}_primary.h

#if FLAVOR = z80intel || FLAVOR = z80
#EMBEDS += ${FLAVOR}_cb.h ${FLAVOR}_dd.h ${FLAVOR}_ed.h ${FLAVOR}_fd.h
#endif

build: dism${FLAVOR}

clean:
	rm -f *.tmp
	rm -f *.o
	rm -f ${EMBEDS}
	rm -f dism${FLAVOR}

.c.o:
	cc -c -o $@ $< -DDISM_FLAVOR=${FLAVOR}

.txt.h:
	sed -i.tmp 's/.*/"&",/' $<
	xxd -i -C $<.tmp $@.tmp
	rm -f $<.tmp
	sed -i 's/_TMP//' $@.tmp
	mv $@.tmp $@

opcodes.o: opcodes.c opcodes.h ${EMBEDS}

dismi8080: dism.o opcodes.o
	cc -o $@ $^

# end of file
# vim: noet
