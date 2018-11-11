# Operating Systems Project

## Exercise 2
This exercise has both a CircuitRouter-ParSolver and a CircuitRouter-SeqSolver folders

### Version 1.0
- Global mutexes implemented
- Program is working but has no noticeable speedup

### Version 2.0
- One mutex per grid coordinate has been implemented
- Solution: exponential backoff
- Program is working and a good speedup has been achieved

### Directories structure
`
EX2
├── doTest.sh
├── Makefile
├── README.md
├── CircuitRouter-ParSolver
│	├── CircuitRouter-ParSolver.c
│	├── coordinate.c
│	├── coordinate.h
│	├── grid.c
│	├── grid.h
│	├── lock.c
│	├── lock.h
│	├── Makefile
│	├── maze.c
│	├── maze.h
│	├── router.c
│	└── router.h
├── CircuitRouter-SeqSolver
│	├── CircuitRouter-SeqSolver.c
│	├── coordinate.c
│	├── coordinate.h
│	├── grid.c
│	├── grid.h
│	├── Makefile
│	├── maze.c
│	├── maze.h
│	├── router.c
│	└── router.h
├── inputs
│	├── generate.py
│	├── random-x128-y128-z3-n128.txt
│	├── random-x128-y128-z3-n64.txt
│	├── random-x128-y128-z5-n128.txt
│	├── random-x256-y256-z3-n256.txt
│	├── random-x256-y256-z5-n256.txt
│	├── random-x32-y32-z3-n64.txt
│	├── random-x32-y32-z3-n96.txt
│	├── random-x48-y48-z3-n48.txt
│	├── random-x48-y48-z3-n64.txt
│	├── random-x64-y64-z3-n48.txt
│	└── random-x64-y64-z3-n64.txt
├── lib
│	├── commandlinereader.c
│	├── commandlinereader.h
│	├── list.c
│	├── list.h
│	├── pair.c
│	├── pair.h
│	├── queue.c
│	├── queue.h
│	├── timer.h
│	├── types.h
│	├── utility.h
│	├── vector.c
│	└── vector.h
└── results
`

### How to compile
`
- 1: open bash shell (terminal)
- 2: write cd EX2 (root_directory path)
- 3: write "make"
- 4a (command line):
		- write cd CircuitRouter-ParSolver
		- write "./CircuitRouter-ParSolver -t $(numberOfThreads) $inputFileName"
		- write cd ..
- 4b (doTest.sh):
		- write "./doTest.sh $(numberOfThreads) $inputFileName"
		Note: Results output will be created into results folder
- 5: write "make clean"
- 6: close bash shell
`

### Testing
All testing was performed and executed in the machine whose specs are down below.

### Specs
- Operating System Information

Fedora 29 Workstation

Linux Daniel-Fixo 4.18.16-300.fc29.x86_64 #1 SMP Sat Oct 20 23:24:08 UTC 2018 x86_64 x86_64 x86_64 GNU/Linux


- Hardware

Architecture:        x86_64
CPU op-mode(s):      32-bit, 64-bit
Byte Order:          Little Endian
CPU(s):              16
On-line CPU(s) list: 0-15
Thread(s) per core:  2
Core(s) per socket:  8
Socket(s):           1
NUMA node(s):        1
Vendor ID:           AuthenticAMD
CPU family:          23
Model:               1
Model name:          AMD Ryzen 7 1700X Eight-Core Processor
Stepping:            1
CPU MHz:             1925.359
CPU max MHz:         3850.0000
CPU min MHz:         2200.0000
BogoMIPS:            6800.20
Virtualization:      AMD-V
L1d cache:           32K
L1i cache:           64K
L2 cache:            512K
L3 cache:            8192K
NUMA node0 CPU(s):   0-15
