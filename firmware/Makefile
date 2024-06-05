# Project Name
TARGET = firmware

LDFLAGS += -u _printf_float

CPP_SOURCES = firmware.cpp

# Sources
CPP_SOURCES += src/Discomfort.cpp
CPP_SOURCES += src/Folder.cpp
CPP_SOURCES += src/Clipper.cpp
CPP_SOURCES += src/DryWet.cpp
CPP_SOURCES += src/Follower.cpp
CPP_SOURCES += src/Crush.cpp
CPP_SOURCES += src/FilterBank.cpp
CPP_SOURCES += src/util.cpp
CPP_SOURCES += src/hardware/DiscomfortHwInputs.cpp
CPP_SOURCES += src/hardware/mux.cpp
CPP_SOURCES += src/hardware/pins.cpp

## UNCOMMENT TO TEST
# testDiscomfort:
# 	g++ test/test.cpp src/Folder.cpp
# 	./a.out
# 	rm a.out

# CPP_SOURCES += $(DISCOMFORT_CPP_SOURCES)
# Library Locations
LIBDAISY_DIR = ../../DaisyExamples/libDaisy/
DAISYSP_DIR = ../../DaisyExamples/DaisySP/

# Core location, and generic Makefile.
SYSTEM_FILES_DIR = $(LIBDAISY_DIR)/core
include $(SYSTEM_FILES_DIR)/Makefile