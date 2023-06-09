# Source and Objects
SOURCES = $(wildcard *.c)
OBJECTS_DIR = $(BOAT_BUILD_DIR)/BoAT-SupportLayer/keystore
OBJECTS = $(patsubst %.c,$(OBJECTS_DIR)/%.o,$(SOURCES))
		  
.PHONY: all   keystore_SE keystore_soft

all:   keystore_SE keystore_soft $(OBJECTS_DIR) $(OBJECTS)

keystore_soft:
	make -C  soft all

keystore_SE:
#	make -C SE/NXP all

$(OBJECTS_DIR):
	$(BOAT_MKDIR) -p $(OBJECTS_DIR)

$(OBJECTS_DIR)/%.o:%.c
	$(CC) -c $(BOAT_CFLAGS) $< -o $@ -DUSE_KECCAK=1
	$(BOAT_COPY) $(SOURCES) $(OBJECTS_DIR)


clean:
	-$(BOAT_RM) $(OBJECTS)
	make -C SE/NXP  clean



