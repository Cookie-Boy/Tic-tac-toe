CXX := g++
CXXFLAGS := -g -Wall -Isrc
LDFLAGS := -lsfml-graphics -lsfml-window -lsfml-system

SRC_DIR := src
OBJ_DIR := obj
BIN_DIR := bin

CLIENT_SRCS := $(SRC_DIR)/interface.cpp $(SRC_DIR)/network.cpp $(SRC_DIR)/name_window.cpp $(SRC_DIR)/chat_window.cpp $(SRC_DIR)/main.cpp
SERVER_SRCS := $(SRC_DIR)/chat_server.cpp

CLIENT_OBJS := $(patsubst $(SRC_DIR)/%.cpp, $(OBJ_DIR)/%.o, $(CLIENT_SRCS))
SERVER_OBJS := $(patsubst $(SRC_DIR)/%.cpp, $(OBJ_DIR)/%.o, $(SERVER_SRCS))

CLIENT_BIN := $(BIN_DIR)/main.exe
SERVER_BIN := $(BIN_DIR)/chat_server.exe

all: $(CLIENT_BIN) $(SERVER_BIN)

# Компиляция объектных файлов
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp
	@mkdir -p $(OBJ_DIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Сборка клиента
$(CLIENT_BIN): $(CLIENT_OBJS)
	@mkdir -p $(BIN_DIR)
	$(CXX) $^ -o $@ $(LDFLAGS)

# Сборка сервера
$(SERVER_BIN): $(SERVER_OBJS)
	@mkdir -p $(BIN_DIR)
	$(CXX) $^ -o $@

clean:
	rm -rf $(OBJ_DIR)/*.o $(BIN_DIR)/*.exe

run: $(CLIENT_BIN)
	$<

run_server: $(SERVER_BIN)
	$<
