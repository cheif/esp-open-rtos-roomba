
INC_DIRS += $(roomba_ROOT)/include

roomba_INC_DIR = $(roomba_ROOT)/include
roomba_SRC_DIR = $(roomba_ROOT)/src

$(eval $(call component_compile_rules,roomba))

