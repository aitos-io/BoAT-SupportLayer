# BoAT SDK Library Objects

# Generate sub-directory list
ALLSUBDIRS := $(shell $(BOAT_FIND) . -maxdepth 1 -type d)
ALLSUBDIRS := $(basename $(patsubst ./%,%,$(ALLSUBDIRS)))

EXCLUDE_DIRS := include
SUBDIRS := $(filter-out $(EXCLUDE_DIRS),$(ALLSUBDIRS))

# Add _clean_ prefix to avoid clean subdir target names being confused with compile subdir targets
CLEAN_SUBDIRS := $(addprefix _clean_,$(SUBDIRS) )

SRC_PATH = $(BOAT_BUILD_DIR)/BoAT-SupportLayer/tests/$(PLATFORM_TARGET)/ \
	   $(BOAT_BUILD_DIR)/BoAT-SupportLayer/tests/keystore/ \
	   $(BOAT_BUILD_DIR)/BoAT-SupportLayer/tests/keypair/
	   
define getdir
DIRS += $(shell $(BOAT_FIND) $(SRC_PATH) -maxdepth 3 -type d)
endef

define geto
SRCS_o += $(foreach dir, $(DIRS), $(wildcard $(dir)/*.o))
endef

define testapp
ifeq ($(PLATFORM_TARGET),linux-default)
	#dependence libs
	TESTLIBS = $(BOAT_LIB_DIR)/libboatvendor.a \
					-lrt \
					-lpthread \
					-lcheck \
					-lm\
					-lsubunit\
					$(LINK_LIBS)

	TESTMAKE = $(CC) $(BOAT_CFLAGS) $(BOAT_LFLAGS) $(BOAT_BASE_DIR)/BoAT-SupportLayer/tests/platform/linux-default/testapp.c
	TESTO = -o 
	
	TESTAPP = /testapp
	TESTDIR = $(BOAT_BUILD_DIR)/BoAT-SupportLayer/tests/
	TESTMKDIR =  $(BOAT_MKDIR) -p 
else
	TESTMKDIR = $(AR) r $(LIBNAME) $(BOAT_BASE_DIR)/boatcheck/*.o

endif
endef

LIBNAME = $(BOAT_LIB_DIR)/libboatvendor.a

.PHONY: all $(SUBDIRS) clean $(CLEAN_SUBDIRS)


all: $(SUBDIRS)

	$(eval $(call getdir))
	$(eval $(call geto))
	$(AR) r $(LIBNAME) $(SRCS_o)
#	$(AR) r $(LIBNAME) $(BOAT_BASE_DIR)/boatcheck/*.o
	$(eval $(call testapp))
	$(TESTMKDIR) $(TESTDIR)
	$(TESTMAKE) $(TESTLIBS) $(TESTO) $(TESTDIR)$(TESTAPP)

$(SUBDIRS):
	make -C $@/ all	
	
$(CLEAN_SUBDIRS):
	make -C $(patsubst _clean_%,%,$@)/ clean

clean: $(CLEAN_SUBDIRS)
	-$(BOAT_RM) `$(BOAT_FIND) -name \*.gcda`
	-$(BOAT_RM) `$(BOAT_FIND) -name \*.gcno`




