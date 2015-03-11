
CTG		= ctypesgen.py
WLZDIR		= $$HOME/MouseAtlas/Build
#WLZDIR		= $$HOME/MouseAtlas/Build/debug
CFLAGS		= -shared -g

Wlz.py:		libPyWlz.so
		$(CTG) -o Wlz.py \
		       -I$(WLZDIR)/include \
		       -L. -L$(WLZDIR)/lib \
		       -l libPyWlz.so \
		       $(WLZDIR)/include/Wlz.h \
		       $(WLZDIR)/include/WlzType.h \
		       $(WLZDIR)/include/WlzProto.h \
		       $(WLZDIR)/include/WlzError.h

libPyWlz.so:
		$(CC) $(CFLAGS) -o $@ \
		      -Wl,--whole-archive \
		      -L$(WLZDIR)/lib -lWlz -lAlg -lAlc \
		      -Wl,--no-whole-archive \
		      -lgomp


clean:
		$(RM) Wlz.py Wlz.pyc libPyWlz.so
