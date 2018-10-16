# -----------------------------------------------------------------------------
#
#
# -----------------------------------------------------------------------------
TARGET := exp_opencvImshow_grayscale

# State your source files
SOURCES := main.cpp
SOURCES += ../driver/ps3eye.cpp ../driver/ps3eye_capi.cpp

# State your include files
CXXFLAGS := -I../driver

# State your additional libraries
CXXFLAGS += $(shell pkg-config --cflags opencv)
LIBS := $(shell pkg-config --libs opencv)

# -----------------------------------------------------------------------------
# You shouldn't be needing to change lines below
# -----------------------------------------------------------------------------
OBJECTS := $(patsubst %.cpp,%.o,$(SOURCES))

CXXFLAGS += -I. -std=c++11 -O3

CXXFLAGS += $(shell pkg-config --cflags libusb-1.0)
LIBS += $(shell pkg-config --libs libusb-1.0)

ifeq ($(shell uname), Linux)
	LIBS += -lpthread
endif

$(TARGET): $(OBJECTS)
	$(CXX) -o $@ $^ $(LIBS)

clean:
	rm -f $(TARGET) $(OBJECTS)
