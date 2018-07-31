XEN_ROOT = $(CURDIR)/../..

ifeq (,$(findstring clean,$(MAKECMDGOALS)))
include $(XEN_ROOT)/Config.mk
endif

all: main.a

main.a: main.o daytime.o chargen.o
	$(AR) cr $@ $^

clean:
	rm -f *.a *.o
