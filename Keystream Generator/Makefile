# Keystream Generator/Makefile
TARGET = keystream_generator
SRC_DIR = .
OBJ_DIR = obj

# Archivos locales
LOCAL_SRC = $(SRC_DIR)/keystream_generator.c
LOCAL_OBJ = $(OBJ_DIR)/keystream_generator.o

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

# Regla principal
$(TARGET): $(OBJ_FILES)
	$(CC) -o $@ $^ $(LDFLAGS) $(LIBS)

# Regla para objeto local
$(OBJ_DIR)/keystream_generator.o: $(LOCAL_SRC) | $(OBJ_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

# Regla para objeto común
$(OBJ_DIR)/kaos.o: $(COMMON_SRC) | $(OBJ_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

# Crear directorio obj si no existe
$(OBJ_DIR):
	mkdir -p $(OBJ_DIR)

# Regla para generar keystream de prueba
generate: $(TARGET)
	@echo "  KAOS-256 Keystream Generator"
	@echo "================================"
	@echo "Generating test keystream..."
	./$(TARGET)
	@echo ""
	@echo "Keystream generation completed"

# Limpieza
clean:
	rm -rf $(OBJ_DIR) $(TARGET)

.PHONY: clean generate