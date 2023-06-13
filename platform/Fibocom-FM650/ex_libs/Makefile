-include $(CFG)/config.mk

LDFLAGS += -lumdp -lumdpcommon
LDFLAGS += -L./libs -lboatengine
LDFLAGS += -L./libs -lboatvendor
LDFLAGS += -lpthread  -lrt 
LDFLAGS += ./libs/ex_shared_libs/libcurl.so.4.5.0
LDFLAGS += ./libs/ex_shared_libs/libssl.so.1.0.2
LDFLAGS += ./libs/ex_shared_libs/libcrypto.so.1.0.2


CFLAGS += -I/home/alex/works/BoAT-ProjectTemplate/BoAT-Engine/include
CFLAGS += -I/home/alex/works/BoAT-ProjectTemplate/BoAT-SupportLayer/include
CFLAGS += -I/home/alex/works/BoAT-ProjectTemplate/BoAT-SupportLayer/platform/include
CFLAGS += -I/home/alex/works/BoAT-ProjectTemplate/BoAT-SupportLayer/platform/Fibocom-FM650/src/log
CFLAGS += -I/home/alex/works/BoAT-ProjectTemplate/BoAT-SupportLayer/platform/Fibocom-FM650/src/inet
CFLAGS += -I/home/alex/works/BoAT-ProjectTemplate/BoAT-SupportLayer/platform/Fibocom-FM650/src/dal
CFLAGS += -I/home/alex/works/BoAT-ProjectTemplate/BoAT-SupportLayer/platform/Fibocom-FM650/src/osal
CFLAGS += -I/home/alex/works/BoAT-ProjectTemplate/BoAT-SupportLayer/common/storage
CFLAGS += -I/home/alex/works/BoAT-ProjectTemplate/BoAT-SupportLayer/keystore

   
OBJDIR = Object
SRCS_DIR := $(wildcard ./*.c)

OBJS := $(addsuffix .o,$(addprefix $(OBJDIR)/,$(basename $(notdir $(SRCS_DIR)))))
DEPENDS := $(addsuffix .d,$(OBJS))
  
TARGET = demo_boat_call_venachain
TARGET_EXE = $(OBJDIR)/$(TARGET)
all: distclean $(TARGET_EXE)
 
$(TARGET_EXE) : $(OBJS)
	@echo "linking $@"
	$(CC) -o $@ $(OBJS) -Wl,-start-group $(LDFLAGS) -Wl,-end-group $(CFLAGS)
	$(STRIP) -g $(TARGET_EXE)
	$(INSTALL) -c -m 777 $(TARGET_EXE) $(APP_BIN)
define make-cmd-cc
$2 : $1
	@mkdir -p $(OBJDIR)
	$$(CC) $$(CFLAGS) -MMD -MT $$@ -MF $$@.d -c -o $$@ $$<   
endef
 
$(foreach afile,$(SRCS_DIR),\
	$(eval $(call make-cmd-cc,$(afile),\
        $(addsuffix .o,$(addprefix $(OBJDIR)/,$(basename $(notdir $(afile))))))))

clean:
	$(RM) -rf $(OBJS) $(TARGET_EXE) $(OBJDIR)

distclean: clean
	$(RM) $(APP_BIN)/$(TARGET) -f
 
-include $(DEPENDS)
