# Operating Systems Project

## Exercise 2
This exercise has both a CircuitRouter-ParSolver and a CircuitRouter-SeqSolver folders

### Version 1.0
- Global mutexes implemented
- Program is working but has no noticeable speedup

### Version 2.0
- One mutex per grid coordinate has been implemented
- Program is working and a good speedup has been achieved


### Directories structure
+---EX2\n
|		|		doTest.sh\n
|		|		Makefile\n
|		|		README.md\n
|		|		\n
|		+---CircuitRouter-ParSolver
|		|				CircuitRouter-ParSolver.c
|		|				coordinate.c
|		|				coordinate.h
|		|				grid.c
|		|				grid.h
|		|				lock.c
|		|				lock.h
|		|				Makefile
|		|				maze.c
|		|				maze.h
|		|				router.c
|		|				router.h
|		|
|		+---CircuitRouter-SeqSolver
|		|				CircuitRouter-SeqSolver.c
|		|				coordinate.c
|		|				coordinate.h
|		|				grid.c
|		|				grid.h
|		|				Makefile
|		|				maze.c
|		|				maze.h
|		|				router.c
|		|				router.h
|		|				
|		+---inputs
|		|				generate.py
|		|				random-x128-y128-z3-n128.txt
|		|				random-x128-y128-z3-n64.txt
|		|				random-x128-y128-z5-n128.txt
|		|				random-x256-y256-z3-n256.txt
|		|				random-x256-y256-z5-n256.txt
|		|				random-x32-y32-z3-n64.txt
|		|				random-x32-y32-z3-n96.txt
|		|				random-x48-y48-z3-n48.txt
|		|				random-x48-y48-z3-n64.txt
|		|				random-x64-y64-z3-n48.txt
|		|				random-x64-y64-z3-n64.txt
|		|				
|		+---lib
|		|				commandlinereader.c
|		|				commandlinereader.h
|		|				list.c
|		|				list.h
|		|				pair.c
|		|				pair.h
|		|				queue.c
|		|				queue.h
|		|				timer.h
|		|				types.h
|		|				utility.h
|		|				vector.c
|		|				vector.h
|		|				
|		\---results


ii) os passos para compilar e executar o projecto por linha de comando ou pelo doTest.sh, iii)
uma descri¸c˜ao das carater´ısticas do processador e sistema operativo usado pelo testes (i.e., n´umero
de cores, clock rate e modelo), que poder˜ao ser obtidas, respectivamente, usando os comandos i)
cat /proc/cpuinfo e ii) uname -a
