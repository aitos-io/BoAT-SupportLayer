# Select one of the crypto-dep and platform

.PHONY:all mbedTLS crypto_default keccak storage keystore platform keypair utilities rpc tests third-party
    #cryptointf http2

ifeq ($(SOFT_CRYPTO), CRYPTO_DEFAULT)

    BUILD_DEP := third-party rpc keypair utilities storage crypto_default keystore tests platform 
    #cryptointf http2

else ifeq ($(SOFT_CRYPTO), CRYPTO_MBEDTLS)

    BUILD_DEP := third-party rpc keypair utilities keccak  storage mbedTLS keystore tests platform 
    #cryptointf http2

else

    BUILD_DEP :=

endif

all: $(BUILD_DEP) 

third-party:
	make -C $(BOAT_BASE_DIR)/BoAT-SupportLayer/third-party all

tests:
	make -C $(BOAT_BASE_DIR)/BoAT-SupportLayer/tests all

keypair:
	make -C $(BOAT_BASE_DIR)/BoAT-SupportLayer/keypair all

mbedTLS:
	make -C $(BOAT_BASE_DIR)/BoAT-SupportLayer/third-party/crypto/mbedTLS all

crypto_default:
	make -C $(BOAT_BASE_DIR)/BoAT-SupportLayer/third-party/crypto all

keccak:	
	make -C $(BOAT_BASE_DIR)/BoAT-SupportLayer/third-party/crypto/keccak all

utilities:
	make -C $(BOAT_BASE_DIR)/BoAT-SupportLayer/common/utilities all

storage:
	make -C $(BOAT_BASE_DIR)/BoAT-SupportLayer/common/storage all

# add
rpc:
	make -C $(BOAT_BASE_DIR)/BoAT-SupportLayer/common/rpc all

#http2:
#	make -C $(BOAT_BASE_DIR)/BoAT-SupportLayer/common/http2intf all

#cryptointf:
#	make -C $(BOAT_BASE_DIR)/BoAT-SupportLayer/common/cryptointf all
	
platform:
	make -C $(BOAT_BASE_DIR)/BoAT-SupportLayer/platform all

keystore:
	make -C $(BOAT_BASE_DIR)/BoAT-SupportLayer/keystore all
	
clean:
	make -C $(BOAT_BASE_DIR)/BoAT-SupportLayer/keypair clean
	make -C $(BOAT_BASE_DIR)/BoAT-SupportLayer/third-party/crypto clean
	make -C $(BOAT_BASE_DIR)/BoAT-SupportLayer/common/utilities clean
	make -C $(BOAT_BASE_DIR)/BoAT-SupportLayer/common/rpc clean
	make -C $(BOAT_BASE_DIR)/BoAT-SupportLayer/common/storage clean
	#make -C $(BOAT_BASE_DIR)/BoAT-SupportLayer/common/cryptointf clean
	#make -C $(BOAT_BASE_DIR)/BoAT-SupportLayer/common/http2intf clean	
	make -C $(BOAT_BASE_DIR)/BoAT-SupportLayer/platform clean 
	make -C $(BOAT_BASE_DIR)/BoAT-SupportLayer/keystore clean
	make -C $(BOAT_BASE_DIR)/BoAT-SupportLayer/tests clean


