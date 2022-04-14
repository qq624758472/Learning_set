include ../common.mk
CALLSRC=4.c

.PHONY : clean

include $(CALLSRC:.c=.d)
%.d: %.c
	$(cc) -MM $< > $@


clean:
	-rm $(CALLSRC:.c=.o)
	-rm $(CALLSRC:.c=.d)