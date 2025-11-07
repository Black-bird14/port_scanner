# Compiler
CXX = g++

#Compiler flags
CXXFLAGS = -Wall -g
LDFLAGS = -Wl,-rpath,/usr/lib/x86_64-linux-gnu
INCLUDES = -I$(CURDIR)/include/ -I/usr/local/include -I/usr/include
# Space-separated pkg-config libraries used by this project
LIBS = -lboost_program_options

# path #
SRC_PATH = src
BUILD_PATH = build
BIN_PATH = $(BUILD_PATH)/bin
BIN_NAME = scanner
# extensions #
SRC_EXT = cpp

# Find all source files in the source directory, sorted by
# most recently modified
SRCS = $(shell find $(SRC_PATH) -name '*.$(SRC_EXT)' | sort -k 1nr | cut -f2-)

# Object files
#OBJS = $(SRCS:.cpp=.o)

# Set the object file names, with the source directory stripped
# from the path, and the build path prepended in its place
OBJS = $(SRCS:$(SRC_PATH)/%.$(SRC_EXT)=$(BUILD_PATH)/%.o)

# Set the dependency files that will be used to add header dependencies
DEPS = $(OBJS:.o=.d)

# Target executable
TARGET = scanner

# For deleting the target
TARGET_DEL = scanner.exe

default_target: release
release: dirs
	@$(MAKE) all

dirs:
	@echo "Creating directories"
	@mkdir -p $(dir $(OBJS))
	@mkdir -p $(BIN_PATH)

# Default rule to build and run the executable
# checks the executable and symlinks to the output
all: $(BIN_PATH)/$(TARGET)
	@echo "Making symlink: $(TARGET) -> $<"
	@$(RM) $(TARGET)
	@ln -s $(BIN_PATH)/$(TARGET) $(TARGET)

# Add dependency files, if they exist
-include $(DEPS)

# Rule to link object files into the target executable
#$(TARGET): $(OBJS)
#	$(CXX) $(CXXFLAGS) -o $(TARGET) $(OBJS)
$(BIN_PATH)/$(TARGET): $(OBJS)
	@echo "Linking: $@"
	$(CXX) $(OBJS) -o $@ $(LDFLAGS) ${LIBS}

# Rule to compile .cpp files into .o files
#%.o: %.cpp
#	$(CXX) $(CXXFLAGS) -c $< -o $@

# Source file rules
# After the first compilation they will be joined with the rules from the
# dependency files to provide header dependencies
$(BUILD_PATH)/%.o: $(SRC_PATH)/%.$(SRC_EXT)
	@echo "Compiling: $< -> $@"
	@echo "INCLUDES = $(INCLUDES)"
	$(CXX) $(CXXFLAGS) $(INCLUDES) -MP -MMD -c $< -o $@

# Rule to run the executable
run: $(TARGET)
	$(TARGET)

# Clean rule to remove generated files
clean:
	@echo "Deleting $(BIN_NAME) symlink"
	@$(RM) $(BIN_NAME)
	@echo "Deleting directories"
	@$(RM) -r $(BUILD_PATH)
	@$(RM) -r $(BIN_PATH)

#src: https://gist.github.com/mauriciopoppe/de8908f67923091982c8c8136a063ea6