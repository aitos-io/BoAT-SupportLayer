# Source and Objects

SOURCES = $(wildcard *.c)


OBJECTS_DIR = $(BOAT_BUILD_DIR)/BoAT-SupportLayer/keystore
OBJECTS = $(patsubst %.c,$(OBJECTS_DIR)/%.o,$(SOURCES))


all: $(OBJECTS_DIR) $(OBJECTS) Boat_sscom

$(OBJECTS_DIR):
	$(BOAT_MKDIR) -p $(OBJECTS_DIR)

$(OBJECTS_DIR)/%.o:%.c
	$(CC) -c $(BOAT_CFLAGS) $< -o $@
	$(BOAT_COPY) $(SOURCES) $(OBJECTS_DIR)

Boat_sscom:
	make -C sscom all

clean:
	-$(BOAT_RM) $(OBJECTS)
	make -C sscom clean

