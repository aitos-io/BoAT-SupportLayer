# Source and Objects
SRC_OBJECTS = $(wildcard $(BOAT_BUILD_DIR)/BoAT-SupportLayer/common/rpc/*.o) \
				$(wildcard $(BOAT_BUILD_DIR)/BoAT-SupportLayer/common/utilities/*.o) \
				$(wildcard $(BOAT_BUILD_DIR)/BoAT-SupportLayer/common/http2intf/*.o) \
				$(wildcard $(BOAT_BUILD_DIR)/BoAT-SupportLayer/keypair/*.o) \
				$(wildcard $(BOAT_BUILD_DIR)/BoAT-SupportLayer/third-party/cJSON/*.o) \
				$(wildcard $(BOAT_BUILD_DIR)/BoAT-SupportLayer/third-party/nghttp2/*.o) \
				$(wildcard $(BOAT_BUILD_DIR)/BoAT-SupportLayer/third-party/rlp/*.o) \
				$(wildcard $(BOAT_BUILD_DIR)/BoAT-SupportLayer/third-party/protos/*/*.o) \
				$(wildcard $(BOAT_BUILD_DIR)/BoAT-SupportLayer/third-party/protos/*/*/*.o) \
				$(wildcard $(BOAT_BUILD_DIR)/BoAT-SupportLayer/third-party/protobuf-c/*.o) \
				$(wildcard $(BOAT_BUILD_DIR)/BoAT-SupportLayer/tests/dal/*.o) \
				$(wildcard $(BOAT_BUILD_DIR)/BoAT-SupportLayer/tests/osal/*.o)
	

PORT_CRYPTO_OBJECTS = $(wildcard $(BOAT_BUILD_DIR)/BoAT-SupportLayer/common/storage/*.o) \
					  $(wildcard $(BOAT_BUILD_DIR)/BoAT-SupportLayer/keystore/*.o) \
					  $(wildcard $(BOAT_BUILD_DIR)/BoAT-SupportLayer/keystore/SE/NXP/sscom/se_process/*.o) \
		  			  $(wildcard $(BOAT_BUILD_DIR)/BoAT-SupportLayer/keystore/SE/NXP/sscom/smCom/*.o) \
		  			  $(wildcard $(BOAT_BUILD_DIR)/BoAT-SupportLayer/keystore/SE/NXP/sscom/smCom/T1oI2C/*.o) \

PORT_CRYPTO_OBJECTS += $(wildcard $(BOAT_BUILD_DIR)/BoAT-SupportLayer/third-party/crypto/*.o) \
			$(wildcard $(BOAT_BUILD_DIR)/BoAT-SupportLayer/third-party/crypto/aes/*.o) 

PORT_CRYPTO_OBJECTS += $(wildcard $(BOAT_BUILD_DIR)/BoAT-SupportLayer/platform/$(PLATFORM_TARGET)/src/dal/*.o) \
						$(wildcard $(BOAT_BUILD_DIR)/BoAT-SupportLayer/platform/$(PLATFORM_TARGET)/src/osal/*.o) \
						$(wildcard $(BOAT_BUILD_DIR)/BoAT-SupportLayer/platform/$(PLATFORM_TARGET)/src/dal/http/*.o) \


LIBNAME = $(BOAT_LIB_DIR)/libboatvendor.a

.PHONY: all $(CLEAN_SUBDIRS) boatvendor_objs clean

all: boatvendor_objs $(LIBNAME)

$(LIBNAME): boatvendor_objs
	$(AR) r $(LIBNAME) $(PORT_CRYPTO_OBJECTS) $(SRC_OBJECTS)


boatvendor_objs:
	make -C dal all
	make -C osal all


clean: $(CLEAN_SUBDIRS)
	-$(BOAT_RM) $(LIBNAME)
