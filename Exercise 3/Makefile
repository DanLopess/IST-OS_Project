#global makefile that calls all other makefiles

SUBDIRS = CircuitRouter-AdvShell CircuitRouter-SeqSolver CircuitRouter-Client

all: $(SUBDIRS)

$(SUBDIRS):
	$(MAKE) -C $@

.PHONY: $(SUBDIRS)

clean:
	cd CircuitRouter-AdvShell && $(MAKE) clean
	cd CircuitRouter-SeqSolver && $(MAKE) clean
	cd CircuitRouter-Client && $(MAKE) clean
