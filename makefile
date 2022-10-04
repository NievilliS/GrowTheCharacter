SRC_DIR := 60_TESTING
SRCS := main.cpp room.cpp

BUILD_DIR := 90_BUILD
OBJS := $(SRCS:%=$(BUILD_DIR)/%.o)

OUTPUT := out

default: $(OBJS)
	$(CXX) -o $(BUILD_DIR)/$(OUTPUT) $(OBJS)

$(BUILD_DIR)/%.cpp.o: $(SRC_DIR)/%.cpp
	$(CXX) -o $@ -c $<

clean:
	rm ./$(BUILD_DIR)/* -r
	touch ./$(BUILD_DIR)/.ignorethis
