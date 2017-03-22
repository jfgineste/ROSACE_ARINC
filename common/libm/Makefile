C_FILES= cos.c 

SPGS= cos

compile: $(C_FILES)
#	/opt/prise/x86_64/gnatforleon-2.1.0/bin/sparc-elf-gcc *.c
	/opt/cross/bin/powerpc-elf-gcc cos.c

boundt: compile
	boundt_sparc -device=v8e -table a.out $(SPGS) |./table.sh > wcet.txt

clean:
	-rm -rf *.o *~ a.out

