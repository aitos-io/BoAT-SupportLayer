# BoAT SDK Library Objects

# Generate sub-directory list
ALLSUBDIRS := $(shell $(BOAT_FIND) . -maxdepth 1 -type d)
ALLSUBDIRS := $(basename $(patsubst ./%,%,$(ALLSUBDIRS)))

EXCLUDE_DIRS := include
SUBDIRS := $(filter-out $(EXCLUDE_DIRS),$(ALLSUBDIRS))

# Add _clean_ prefix to avoid clean subdir target names being confused with compile subdir targets
CLEAN_SUBDIRS := $(addprefix _clean_,$(SUBDIRS) )

SRC_PATH = $(BOAT_BUILD_DIR)/BoAT-SupportLayer/$(PLATFORM_TARGET)/tests/
define getdir
DIRS += $(shell find $(SRC_PATH) -maxdepth 3 -type d)
endef

define geto
SRCS_o += $(foreach dir, $(DIRS), $(wildcard $(dir)/*.o))
endef

LIBNAME = $(BOAT_LIB_DIR)/libboatvendor.a

.PHONY: all $(SUBDIRS) clean $(CLEAN_SUBDIRS)


all: $(SUBDIRS)

	$(eval $(call getdir))
	$(eval $(call geto))
	$(AR) r $(LIBNAME) $(SRCS_o)
	$(AR) r $(LIBNAME) $(BOAT_BASE_DIR)/boatcheck/*.o



$(SUBDIRS):
	make -C $@/ all	
	
$(CLEAN_SUBDIRS):
	make -C $(patsubst _clean_%,%,$@)/ clean

clean: $(CLEAN_SUBDIRS)





