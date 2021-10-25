CC=gcc
CFLAGS=-I.
#FOR DEBUG, ADD -g TO CFLAGS
APP = bmp_clr_to_gs

ODIR = build
LDIR = lib
SDIR = src

_DEPS = lib_bmp.h
DEPS = $(patsubst %, $(LDIR)/%,$(_DEPS))

_OBJ = bmp_clr_to_gray_scale.o lib_bmp.o 
OBJ = $(patsubst %, $(ODIR)/%,$(_OBJ))

$(ODIR)/%.o: $(SDIR)/%.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

bmp_clr_to_gray: $(OBJ)
	$(CC) -o $(ODIR)/$(APP) $^ $(CFLAGS)

.PHONY: clean

clean:
	rm -fv $(ODIR)/*.o bmp_clr_to_gray
	rm -fv $(ODIR)/$(APP)
