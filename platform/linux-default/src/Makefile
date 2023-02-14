# Source and Objects
#SRC_OBJECTS = $(wildcard $(BOAT_BUILD_DIR)/BoAT-SupportLayer/platform/$(PLATFORM_TARGET)/rpc/*.o) 
OBJECTS_DIR = $(wildcard $(BOAT_BUILD_DIR)/BoAT-SupportLayer/common/rpc/*.o)


PORT_CRYPTO_OBJECTS = $(wildcard $(BOAT_BUILD_DIR)/BoAT-SupportLayer/common/storage/*.o ) \
					 $(wildcard $(BOAT_BUILD_DIR)/BoAT-SupportLayer/keystore/*.o) 

ifeq ($(SOFT_CRYPTO), CRYPTO_DEFAULT)
    PORT_CRYPTO_OBJECTS += $(wildcard $(BOAT_BUILD_DIR)/BoAT-SupportLayer/third-party/crypto/*.o) \
			   $(wildcard $(BOAT_BUILD_DIR)/BoAT-SupportLayer/third-party/crypto/aes/*.o) \
	           $(wildcard $(BOAT_BUILD_DIR)/BoAT-SupportLayer/platform/$(PLATFORM_TARGET)/port_crypto_default/*.o) 
    PORT_CRYPTO_DIRECTORY = port_crypto_default
else
    PORT_CRYPTO_OBJECTS += 
    PORT_CRYPTO_DIRECTORY = 
endif

LIBNAME = $(BOAT_LIB_DIR)/libboatvendor.a

.PHONY: all $(CLEAN_SUBDIRS) boatvendor_objs clean

all: boatvendor_objs $(LIBNAME)

$(LIBNAME): boatvendor_objs
	$(AR) r $(LIBNAME) $(PORT_CRYPTO_OBJECTS) $(SRC_OBJECTS)


boatvendor_objs:
	make -C $(PORT_CRYPTO_DIRECTORY) all


clean: $(CLEAN_SUBDIRS)
	-$(BOAT_RM) $(LIBNAME)
