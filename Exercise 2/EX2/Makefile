#global makefile that calls all other makefiles

SUBDIRS = CircuitRouter-ParSolver CircuitRouter-SeqSolver

all: $(SUBDIRS)

$(SUBDIRS):
	$(MAKE) -C $@

.PHONY: $(SUBDIRS)

clean:
	cd CircuitRouter-ParSolver && $(MAKE) clean
	cd CircuitRouter-SeqSolver && $(MAKE) clean
