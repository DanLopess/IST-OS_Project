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
- Operating System
Fedora 29 Workstation
...

- Hardware
Model: AMD Ryzen™ 7 1700X
Core count: 8cores
Clock rate: 3.85Ghz (Overclocked)
...

NOTE: get a more specified information on operating system

usando os comandos i)
cat /proc/cpuinfo e ii) uname -a
