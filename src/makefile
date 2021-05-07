
IDIR =../include
BINDIR=../bin
CC=gcc
CFLAGS=-I$(IDIR) -w


ODIR=obj

LDIR =../lib

LIBS=-pthread


_DEPS = util.h network_util.h menu_control.h global.h thread_controller.h
DEPS = $(patsubst %,$(IDIR)/%,$(_DEPS))


_OBJ = util.o network_util.o menu_control.o thread_controller.o
OBJ = $(patsubst %,$(ODIR)/%,$(_OBJ))

$(ODIR)/%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)
	
$(BINDIR)/main.out: $(OBJ) main.c
	$(CC) -o $@ $^ $(CFLAGS) $(LIBS)
	
.PHONY: clean

clean: 
	rm -f $(ODIR)/*.o
	rm -f $(BINDIR)/main.out
	rm -f $(BINDIR)/*.txt $(BINDIR)/*.html