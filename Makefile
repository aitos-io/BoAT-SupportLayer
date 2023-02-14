# Select one of the crypto-dep and platform

.PHONY:all mbedTLS crypto_default keccak storage keystore platform keypair utilities tests

ifeq ($(SOFT_CRYPTO), CRYPTO_DEFAULT)

    BUILD_DEP := third-party keypair utilities storage crypto_default keystore tests platform 

else ifeq ($(SOFT_CRYPTO), CRYPTO_MBEDTLS)

    BUILD_DEP := third-party keypair utilities keccak  storage mbedTLS keystore tests platform 

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

utilities:
	make -C $(BOAT_BASE_DIR)/BoAT-SupportLayer/utilities all

mbedTLS:
	make -C $(BOAT_BASE_DIR)/BoAT-SupportLayer/crypto/mbedTLS all

crypto_default:
	make -C $(BOAT_BASE_DIR)/BoAT-SupportLayer/crypto/crypto_default all

keccak:	
	make -C $(BOAT_BASE_DIR)/BoAT-SupportLayer/crypto/keccak all

storage:
	make -C $(BOAT_BASE_DIR)/BoAT-SupportLayer/storage all

platform:
	make -C $(BOAT_BASE_DIR)/BoAT-SupportLayer/platform all

keystore:
	make -C $(BOAT_BASE_DIR)/BoAT-SupportLayer/keystore all
	
clean:
	make -C $(BOAT_BASE_DIR)/BoAT-SupportLayer/keypair clean
	make -C $(BOAT_BASE_DIR)/BoAT-SupportLayer/utilities clean
	make -C $(BOAT_BASE_DIR)/BoAT-SupportLayer/crypto/crypto_default clean
	make -C $(BOAT_BASE_DIR)/BoAT-SupportLayer/storage clean
	make -C $(BOAT_BASE_DIR)/BoAT-SupportLayer/platform clean 
	make -C $(BOAT_BASE_DIR)/BoAT-SupportLayer/keystore clean
	make -C $(BOAT_BASE_DIR)/BoAT-SupportLayer/tests clean


