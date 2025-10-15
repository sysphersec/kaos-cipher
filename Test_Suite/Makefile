# Test Suite/Makefile
TARGET = test_suite
SRC_DIR = .
OBJ_DIR = obj

# Archivos locales
LOCAL_SRC = $(SRC_DIR)/test_suite.c
LOCAL_OBJ = $(OBJ_DIR)/test_suite.o

# Archivos comunes desde src/
COMMON_DIR = ../src
COMMON_SRC = $(COMMON_DIR)/kaos.c
COMMON_OBJ = $(OBJ_DIR)/kaos.o

# Todos los objetos
OBJ_FILES = $(LOCAL_OBJ) $(COMMON_OBJ)

# Compilador y flags
CC = gcc
CFLAGS = -Wall -Wextra -pedantic -O3 -I$(COMMON_DIR)
LDFLAGS = 
LIBS = -lm

$(TARGET): $(OBJ_FILES)
	$(CC) -o $@ $^ $(LDFLAGS) $(LIBS)

$(OBJ_DIR)/test_suite.o: $(LOCAL_SRC) | $(OBJ_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

$(OBJ_DIR)/kaos.o: $(COMMON_SRC) | $(OBJ_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

$(OBJ_DIR):
	mkdir -p $(OBJ_DIR)

# Ejecutar tests completos
test: $(TARGET)
	@echo "  KAOS-256 Cryptographic Test Suite"
	@echo "===================================="
	@echo "Running comprehensive tests..."
	./$(TARGET)
	@echo ""
	@echo "Test suite execution completed"

# Solo compilar tests
build: $(TARGET)
	@echo "Test suite compiled successfully"

# Limpieza
clean:
	rm -rf $(OBJ_DIR) $(TARGET)

.PHONY: clean test build