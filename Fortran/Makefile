MPIFC=mpifort
OBJ = eeprobe.o eetest.o 
FCFLAGS = -g

%.o: %.f $(DEPS)
	$(MPIFC) -c -o $@ $< $(FCFLAGS)

eetest: $(OBJ)
	$(MPIFC) -o $@ $^

clean:
	rm eeprobe.o eetest.o eetest

