# Select one of the crypto-dep and platform

.PHONY:all common crypto_default keccak  keystore platform keypair   third-party tests
    #cryptointf http2

ifeq ($(SOFT_CRYPTO), CRYPTO_DEFAULT)

    BUILD_DEP := common third-party  keypair   crypto_default keystore  platform 
    #cryptointf http2

else

    BUILD_DEP :=

endif

all: $(BUILD_DEP) 

test: $(BUILD_DEP) tests

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

common:
	make -C $(BOAT_BASE_DIR)/BoAT-SupportLayer/common all

	
platform:
	make -C $(BOAT_BASE_DIR)/BoAT-SupportLayer/platform all

keystore:
	make -C $(BOAT_BASE_DIR)/BoAT-SupportLayer/keystore all
	
clean:
	make -C $(BOAT_BASE_DIR)/BoAT-SupportLayer/keypair clean
	make -C $(BOAT_BASE_DIR)/BoAT-SupportLayer/third-party/crypto clean
	make -C $(BOAT_BASE_DIR)/BoAT-SupportLayer/common clean
	make -C $(BOAT_BASE_DIR)/BoAT-SupportLayer/platform clean 
	make -C $(BOAT_BASE_DIR)/BoAT-SupportLayer/keystore clean
	make -C $(BOAT_BASE_DIR)/BoAT-SupportLayer/tests clean


