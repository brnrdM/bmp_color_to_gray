
CC=gcc
APP=bmp_clr_to_gs

SOURCEDIR = src/
BUILDDIR = build/

bmp_clr_to_gray: $(SOURCEDIR)bmp_clr_to_gray_scale.c
	$(CC) -o $(BUILDDIR)$(APP) $(SOURCEDIR)bmp_clr_to_gray_scale.c

.PHONY: clean

clean:
	rm -fv $(BUILDDIR)$(APP)
