

## Application to the NAS Parallel Benchmarks (NPB)

Download and extract the latest NPB source code form https://www.nas.nasa.gov/software/npb.html

```bash
tar xvzf NPB3.4.3.tar.gz
```

Copy `eeprobe/C/eeprobe.c` and `eeprobe/C/eeprobe.h` files into the `NPB3.4.3/NPB3.4-MPI/common/` directory.

Add the following lines to the `NPB3.4.3/NPB3.4-MPI/sys/make.common` file (next to similar rules) :

```Makefile
${COMMON}/eeprobe.o: ${COMMON}/eeprobe.c
	cd ${COMMON}; ${CCOMPILE} eeprobe.c
```

Add the `${COMMON}/eeprobe.o` object file to the `OBJS` list in specific benchmark makefiles (eg `NPB3.4.3/NPB3.4-MPI/IS/Makefile`).

Instantiate the `NPB3.4.3/NPB3.4-MPI/config/make.def` file:
```bash
cp NPB3.4.3/NPB3.4-MPI/config/make.def.template NPB3.4.3/NPB3.4-MPI/config/make.def
```

Convert the benchmark source code to eeprobe (replace MPI collective operations to equivalent eeprobe operations):
```bash
python3 eeprobe/script/mpi2eep.py --source NPB3.4.3/NPB3.4-MPI/IS/ --includepath "../common/"
```

Compile benchmark:
```bash
make IS CLASS=C
```

Execute benchmark:
```bash
mpirun -n 2 bin/is.C.x
```