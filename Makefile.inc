APPL_COBJS += 

APPL_CXXOBJS += Util.o Robot.o Field.o Path.o PathGen.o Application.o

SRCLANG := c++


ifdef CONFIG_EV3RT_APPLICATION

include $(EV3RT_SDK_LIB_DIR)/ev3lib/Makefile

endif
