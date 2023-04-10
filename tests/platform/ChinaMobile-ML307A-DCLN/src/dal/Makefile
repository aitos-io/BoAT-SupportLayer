# Source and Objects

SOURCES = $(wildcard *.c)
OBJECTS_DIR = $(BOAT_BUILD_DIR)/BoAT-SupportLayer/tests/$(PLATFORM_TARGET)/src/dal
OBJECTS = $(patsubst %.c,$(OBJECTS_DIR)/%.o,$(SOURCES))


all: $(OBJECTS_DIR) $(OBJECTS)

$(OBJECTS_DIR):
	$(BOAT_MKDIR) -p $(OBJECTS_DIR)

$(OBJECTS_DIR)/%.o:%.c
	$(CC) -c $(BOAT_CFLAGS) $< -o $@
	$(BOAT_COPY) $(SOURCES) $(OBJECTS_DIR)


clean:
	-$(BOAT_RM) $(OBJECTS)
