C_FILES= app2_code.c  app3_code.c  app_code.c 

SPGS= h_filter q_filter Va_filter Vz_filter altitude_hold Va_control	\
	Vz_control aircraft_dynamics elevator engine

compile: $(C_FILES)
#	/opt/prise/x86_64/gnatforleon-2.1.0/bin/sparc-elf-gcc *.c
	/opt/cross/bin/powerpc-elf-gcc *.c

boundt: compile
	boundt_sparc -device=v8e -table a.out $(SPGS) |./table.sh > wcet.txt

clean:
	-rm -rf *.o *~ a.out

