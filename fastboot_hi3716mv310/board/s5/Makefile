
include $(TOPDIR)/config.mk

LIB	= $(obj)lib$(BOARD).a

COBJS	:= board.o eth_cfg_setup.o init_reg2.o
SOBJS   := reg2.o
SRCS	:= $(COBJS:.o=.c) $(SOBJS:.o=.S)
OBJS	:= $(addprefix $(obj),$(COBJS) $(SOBJS))

$(LIB):	$(obj).depend $(OBJS)
	$(AR) $(ARFLAGS) $@ $(OBJS)

clean:
	rm -f $(OBJS)

distclean:	clean
	rm -f $(LIB) core *.bak $(obj).depend

#########################################################################

# defines $(obj).depend target
include $(SRCTREE)/rules.mk

#########################################################################
sinclude $(obj).depend
