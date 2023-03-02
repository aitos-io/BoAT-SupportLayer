# Select one of the hardware

LIBNAMEBAK = $(BOAT_LIB_DIR)/libboatvendorbak.a

all:
	make -C osal/ all
	make -C dal/ all



clean:	
	make -C osal/ clean
	make -C dal/ clean
	-$(BOAT_RM) $(LIBNAMEBAK)
