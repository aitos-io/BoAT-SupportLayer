# Source and Objects

# Generate sub-directory list
OBJECTS_DIRSUB :=  $(BOAT_BUILD_DIR)/BoAT-SupportLayer/third-party/protos/common \
			       $(BOAT_BUILD_DIR)/BoAT-SupportLayer/third-party/protos/discovery \
			 		$(BOAT_BUILD_DIR)/BoAT-SupportLayer/third-party/protos/google/protobuf \
					$(BOAT_BUILD_DIR)/BoAT-SupportLayer/third-party/protos/gossip \
					$(BOAT_BUILD_DIR)/BoAT-SupportLayer/third-party/protos/idemix \
					$(BOAT_BUILD_DIR)/BoAT-SupportLayer/third-party/protos/ledger/queryresult \
					$(BOAT_BUILD_DIR)/BoAT-SupportLayer/third-party/protos/ledger/rwset \
					$(BOAT_BUILD_DIR)/BoAT-SupportLayer/third-party/protos/msp \
					$(BOAT_BUILD_DIR)/BoAT-SupportLayer/third-party/protos/orderer \
					$(BOAT_BUILD_DIR)/BoAT-SupportLayer/third-party/protos/peer \
					$(BOAT_BUILD_DIR)/BoAT-SupportLayer/third-party/protos/peer/lifecycle \
					$(BOAT_BUILD_DIR)/BoAT-SupportLayer/third-party/protos/token \
					$(BOAT_BUILD_DIR)/BoAT-SupportLayer/third-party/protos/transientstore

SOURCES :=  $(wildcard $(BOAT_BASE_DIR)/BoAT-SupportLayer/third-party/protos/common/*.c) \
			$(wildcard $(BOAT_BASE_DIR)/BoAT-SupportLayer/third-party/protos/discovery/*.c) \
			$(wildcard $(BOAT_BASE_DIR)/BoAT-SupportLayer/third-party/protos/google/protobuf/*.c) \
			$(wildcard $(BOAT_BASE_DIR)/BoAT-SupportLayer/third-party/protos/gossip/*.c) \
			$(wildcard $(BOAT_BASE_DIR)/BoAT-SupportLayer/third-party/protos/idemix/*.c) \
			$(wildcard $(BOAT_BASE_DIR)/BoAT-SupportLayer/third-party/protos/ledger/queryresult/*.c) \
			$(wildcard $(BOAT_BASE_DIR)/BoAT-SupportLayer/third-party/protos/ledger/rwset/*.c) \
			$(wildcard $(BOAT_BASE_DIR)/BoAT-SupportLayer/third-party/protos/msp/*.c) \
			$(wildcard $(BOAT_BASE_DIR)/BoAT-SupportLayer/third-party/protos/orderer/*.c) \
			$(wildcard $(BOAT_BASE_DIR)/BoAT-SupportLayer/third-party/protos/peer/*.c) \
			$(wildcard $(BOAT_BASE_DIR)/BoAT-SupportLayer/third-party/protos/peer/lifecycle/*.c) \
			$(wildcard $(BOAT_BASE_DIR)/BoAT-SupportLayer/third-party/protos/token/*.c) \
			$(wildcard $(BOAT_BASE_DIR)/BoAT-SupportLayer/third-party/protos/transientstore/*.c)
			

OBJECTS_DIR     := $(BOAT_BUILD_DIR)/BoAT-SupportLayer/third-party/protos

OBJECTS         := $(subst $(BOAT_BASE_DIR),$(BOAT_BUILD_DIR),$(SOURCES))
OBJECTS         := $(patsubst %.c,%.o,$(OBJECTS))

all: $(OBJECTS_DIR) $(OBJECTS) 

$(OBJECTS_DIR):
	$(BOAT_MKDIR) -p $(OBJECTS_DIR)
	$(BOAT_MKDIR) -p $(OBJECTS_DIRSUB)

$(OBJECTS):$(SOURCES)

$(BOAT_BUILD_DIR)/BoAT-SupportLayer/third-party/protos/common/%.o:common/%.c
	$(CC) -c $(BOAT_CFLAGS) $< -o $@
	$(BOAT_COPY) $(wildcard common/*.c) $(BOAT_BUILD_DIR)/BoAT-SupportLayer/third-party/protos/common/
$(BOAT_BUILD_DIR)/BoAT-SupportLayer/third-party/protos/discovery/%.o:discovery/%.c
	$(CC) -c $(BOAT_CFLAGS) $< -o $@
	$(BOAT_COPY) $(wildcard discovery/*.c) $(BOAT_BUILD_DIR)/BoAT-SupportLayer/third-party/protos/discovery/
$(BOAT_BUILD_DIR)/BoAT-SupportLayer/third-party/protos/google/protobuf/%.o:google/protobuf/%.c
	$(CC) -c $(BOAT_CFLAGS) $< -o $@
	$(BOAT_COPY) $(wildcard google/protobuf/*.c) $(BOAT_BUILD_DIR)/BoAT-SupportLayer/third-party/protos/google/protobuf/
$(BOAT_BUILD_DIR)/BoAT-SupportLayer/third-party/protos/gossip/%.o:gossip/%.c
	$(CC) -c $(BOAT_CFLAGS) $< -o $@
	$(BOAT_COPY) $(wildcard gossip/*.c) $(BOAT_BUILD_DIR)/BoAT-SupportLayer/third-party/protos/gossip/
$(BOAT_BUILD_DIR)/BoAT-SupportLayer/third-party/protos/idemix/%.o:idemix/%.c
	$(CC) -c $(BOAT_CFLAGS) $< -o $@
	$(BOAT_COPY) $(wildcard idemix/*.c) $(BOAT_BUILD_DIR)/BoAT-SupportLayer/third-party/protos/idemix/
$(BOAT_BUILD_DIR)/BoAT-SupportLayer/third-party/protos/ledger/queryresult/%.o:ledger/queryresult/%.c
	$(CC) -c $(BOAT_CFLAGS) $< -o $@
	$(BOAT_COPY) $(wildcard ledger/queryresult/*.c) $(BOAT_BUILD_DIR)/BoAT-SupportLayer/third-party/protos/ledger/queryresult/
$(BOAT_BUILD_DIR)/BoAT-SupportLayer/third-party/protos/ledger/rwset/%.o:ledger/rwset/%.c
	$(CC) -c $(BOAT_CFLAGS) $< -o $@
	$(BOAT_COPY) $(wildcard ledger/rwset/*.c) $(BOAT_BUILD_DIR)/BoAT-SupportLayer/third-party/protos/ledger/rwset/
$(BOAT_BUILD_DIR)/BoAT-SupportLayer/third-party/protos/msp/%.o:msp/%.c
	$(CC) -c $(BOAT_CFLAGS) $< -o $@
	$(BOAT_COPY) $(wildcard msp/*.c) $(BOAT_BUILD_DIR)/BoAT-SupportLayer/third-party/protos/msp/
$(BOAT_BUILD_DIR)/BoAT-SupportLayer/third-party/protos/orderer/%.o:orderer/%.c
	$(CC) -c $(BOAT_CFLAGS) $< -o $@
	$(BOAT_COPY) $(wildcard orderer/*.c) $(BOAT_BUILD_DIR)/BoAT-SupportLayer/third-party/protos/orderer/
$(BOAT_BUILD_DIR)/BoAT-SupportLayer/third-party/protos/orderer/etcdraft/%.o:orderer/etcdraft/%.c
	$(CC) -c $(BOAT_CFLAGS) $< -o $@
	$(BOAT_COPY) $(wildcard orderer/etcdraft/*.c) $(BOAT_BUILD_DIR)/BoAT-SupportLayer/third-party/protos/orderer/etcdraft/
$(BOAT_BUILD_DIR)/BoAT-SupportLayer/third-party/protos/peer/%.o:peer/%.c
	$(CC) -c $(BOAT_CFLAGS) $< -o $@
	$(BOAT_COPY) $(wildcard peer/*.c) $(BOAT_BUILD_DIR)/BoAT-SupportLayer/third-party/protos/peer/
$(BOAT_BUILD_DIR)/BoAT-SupportLayer/third-party/protos/peer/lifecycle/%.o:peer/lifecycle/%.c
	$(CC) -c $(BOAT_CFLAGS) $< -o $@
	$(BOAT_COPY) $(wildcard peer/lifecycle/*.c) $(BOAT_BUILD_DIR)/BoAT-SupportLayer/third-party/protos/peer/lifecycle/
$(BOAT_BUILD_DIR)/BoAT-SupportLayer/third-party/protos/token/%.o:token/%.c
	$(CC) -c $(BOAT_CFLAGS) $< -o $@
	$(BOAT_COPY) $(wildcard token/*.c) $(BOAT_BUILD_DIR)/BoAT-SupportLayer/third-party/protos/token/
$(BOAT_BUILD_DIR)/BoAT-SupportLayer/third-party/protos/transientstore/%.o:transientstore/%.c
	$(CC) -c $(BOAT_CFLAGS) $< -o $@
	$(BOAT_COPY) $(wildcard transientstore/*.c) $(BOAT_BUILD_DIR)/BoAT-SupportLayer/third-party/protos/transientstore/

clean:
	-$(BOAT_RM) $(OBJECTS)