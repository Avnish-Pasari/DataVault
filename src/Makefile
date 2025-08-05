CXX = g++ 
CXXFLAGS = -std=c++17 -Wall -g

SRC_DIR_MAIN = kv-store
SRC_DIR_TEST = test
SRC_DIR_EXPERIMENTS = experiments

MAIN_SRC = $(SRC_DIR_MAIN)/main.cpp
MAIN_TEST_SRC = $(SRC_DIR_TEST)/main.cpp

SRC_MAIN = $(filter-out $(MAIN_SRC), $(shell find $(SRC_DIR_MAIN) -name "*.cpp"))
SRC_TEST = $(filter-out $(MAIN_SRC), $(shell find $(SRC_DIR_TEST) -name "*.cpp") $(shell find $(SRC_DIR_MAIN) -name "*.cpp"))

MAIN_OBJ = $(MAIN_SRC:.cpp=.o)

OBJ_MAIN = $(filter-out $(SRC_DIR_MAIN)/main.o, $(SRC_MAIN:.cpp=.o))
OBJ_TEST = $(filter-out src/kv-store/main.o, $(SRC_TEST:.cpp=.o))

DEPS_MAIN = $(OBJ_MAIN:.o)
DEPS_TEST = $(OBJ_TEST:.o)

APP_EXEC = run_app
TEST_EXEC = run_tests

EXPERIMENT_SRC_PART2 = $(SRC_DIR_EXPERIMENTS)/part2/main.cpp
EXPERIMENT_SRC_PART3 = $(SRC_DIR_EXPERIMENTS)/part3/main.cpp
EXPERIMENTS_EXEC = part2_experiments part3_experiments

all: $(APP_EXEC) $(TEST_EXEC) $(EXPERIMENTS_EXEC)

$(APP_EXEC): $(MAIN_OBJ) $(SRC_MAIN:.cpp=.o)
	$(CXX) $(CXXFLAGS) -o $@ $^
$(TEST_EXEC): $(OBJ_TEST)
	$(CXX) $(CXXFLAGS) -o $@ $^

part2_experiments: $(EXPERIMENT_SRC_PART2) $(OBJ_MAIN)
	$(CXX) $(CXXFLAGS) -o $@ $^

part3_experiments: $(EXPERIMENT_SRC_PART3) $(OBJ_MAIN)
	$(CXX) $(CXXFLAGS) -o $@ $^

.PHONY: clean
clean:
	rm -f $(OBJ_MAIN) $(OBJ_TEST) $(DEPS_MAIN) $(DEPS_TEST) $(APP_EXEC) $(TEST_EXEC) $(EXPERIMENTS_EXEC)
	rm -f $(SRC_DIR_MAIN)/main.o
	rm -f $(SRC_DIR_TEST)/main.o

-include $(DEPS_MAIN)
-include $(DEPS_TEST)
-include $(DEPS_EXPERIMENTS)