# Source and Objects
SRC_OBJECTS = $(wildcard $(BOAT_BUILD_DIR)/BoAT-SupportLayer/common/rpc/*.o) \
				$(wildcard $(BOAT_BUILD_DIR)/BoAT-SupportLayer/common/utilities/*.o) \
				$(wildcard $(BOAT_BUILD_DIR)/BoAT-SupportLayer/keypair/*.o) \
				$(wildcard $(BOAT_BUILD_DIR)/BoAT-SupportLayer/third-party/nghttp2/*.o) \
				$(wildcard $(BOAT_BUILD_DIR)/BoAT-SupportLayer/third-party/protos/*/*.o) \
				$(wildcard $(BOAT_BUILD_DIR)/BoAT-SupportLayer/third-party/protos/*/*/*.o) \
				$(wildcard $(BOAT_BUILD_DIR)/BoAT-SupportLayer/third-party/protobuf-c/*.o) \
				$(wildcard $(BOAT_BUILD_DIR)/BoAT-SupportLayer/tests/DAL/*.o) \
				$(wildcard $(BOAT_BUILD_DIR)/BoAT-SupportLayer/tests/OSAL/*.o)
	

PORT_CRYPTO_OBJECTS = $(wildcard $(BOAT_BUILD_DIR)/BoAT-SupportLayer/common/storage/*.o) \
					  $(wildcard $(BOAT_BUILD_DIR)/BoAT-SupportLayer/keystore/*.o) \
					  $(wildcard $(BOAT_BUILD_DIR)/BoAT-SupportLayer/keystore/SE/NXP/sscom/se_process/*.o) \
		  			  $(wildcard $(BOAT_BUILD_DIR)/BoAT-SupportLayer/keystore/SE/NXP/sscom/smCom/*.o) \
		  			  $(wildcard $(BOAT_BUILD_DIR)/BoAT-SupportLayer/keystore/SE/NXP/sscom/smCom/T1oI2C/*.o) \

PORT_CRYPTO_OBJECTS += $(wildcard $(BOAT_BUILD_DIR)/BoAT-SupportLayer/third-party/crypto/*.o) \
			$(wildcard $(BOAT_BUILD_DIR)/BoAT-SupportLayer/third-party/crypto/aes/*.o) \
			$(wildcard $(BOAT_BUILD_DIR)/BoAT-SupportLayer/platform/$(PLATFORM_TARGET)/src/port_crypto_default/*.o) 

PORT_CRYPTO_OBJECTS += $(wildcard $(BOAT_BUILD_DIR)/BoAT-SupportLayer/platform/$(PLATFORM_TARGET)/src/DAL/*.o) \
						$(wildcard $(BOAT_BUILD_DIR)/BoAT-SupportLayer/platform/$(PLATFORM_TARGET)/src/OSAL/*.o) \


LIBNAME = $(BOAT_LIB_DIR)/libboatvendor.a

.PHONY: all $(CLEAN_SUBDIRS) boatvendor_objs clean

all: boatvendor_objs $(LIBNAME)

$(LIBNAME): boatvendor_objs
	$(AR) r $(LIBNAME) $(PORT_CRYPTO_OBJECTS) $(SRC_OBJECTS)


boatvendor_objs:
	make -C port_crypto_default all
	make -C DAL all
	make -C OSAL all


clean: $(CLEAN_SUBDIRS)
	-$(BOAT_RM) $(LIBNAME)
