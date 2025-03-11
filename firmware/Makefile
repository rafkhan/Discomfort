# Project Name
TARGET = firmware

LDFLAGS += -u _printf_float

CPP_SOURCES = firmware.cpp
CPP_SOURCES += src/DiscomfortInternal/Discomfort.cpp
CPP_SOURCES += src/DiscomfortInternal/Folder.cpp
CPP_SOURCES += src/DiscomfortInternal/Clipper.cpp
CPP_SOURCES += src/DiscomfortInternal/DryWet.cpp
CPP_SOURCES += src/DiscomfortInternal/Follower.cpp
CPP_SOURCES += src/DiscomfortInternal/Crush.cpp
CPP_SOURCES += src/DiscomfortInternal/FilterBank.cpp
CPP_SOURCES += src/DiscomfortInternal/util.cpp
CPP_SOURCES += src/hardware/discomfortHwInputs.cpp
CPP_SOURCES += src/hardware/mux.cpp
CPP_SOURCES += src/hardware/pins.cpp

## UNCOMMENT TO TEST
# testDiscomfort:
# 	g++ test/test.cpp src/Folder.cpp
# 	./a.out
# 	rm a.out

# CPP_SOURCES += $(DISCOMFORT_CPP_SOURCES)
# Library Locations
LIBDAISY_DIR = ./DaisyExamples/libDaisy
DAISYSP_DIR = DaisyExamples/DaisySP

# Core location, and generic Makefile.
SYSTEM_FILES_DIR = $(LIBDAISY_DIR)/core
include $(SYSTEM_FILES_DIR)/Makefile