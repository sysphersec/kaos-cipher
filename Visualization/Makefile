# KAOS Visualization/Makefile
TARGET = visualize
SRC_DIR = .
OBJ_DIR = obj

# Archivos locales
LOCAL_SRC = $(SRC_DIR)/visualize_chaos.c
LOCAL_OBJ = $(OBJ_DIR)/visualize_chaos.o

# Archivos comunes desde src/
COMMON_DIR = ../src
COMMON_SRC = $(COMMON_DIR)/kaos.c
COMMON_OBJ = $(OBJ_DIR)/kaos.o

# Todos los objetos
OBJ_FILES = $(LOCAL_OBJ) $(COMMON_OBJ)

# Compilador y flags
CC = gcc
CFLAGS = -Wall -O3 -I$(COMMON_DIR)
LDFLAGS = 
LIBS = -lm

# Regla principal - compila el ejecutable
$(TARGET): $(OBJ_FILES)
	$(CC) -o $@ $^ $(LDFLAGS) $(LIBS)

# Regla para objeto local
$(OBJ_DIR)/visualize_chaos.o: $(LOCAL_SRC) | $(OBJ_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

# Regla para objeto común
$(OBJ_DIR)/kaos.o: $(COMMON_SRC) | $(OBJ_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

# Crear directorio obj si no existe
$(OBJ_DIR):
	mkdir -p $(OBJ_DIR)

# Regla para generar la visualización
generate: $(TARGET)
	@echo "  KAOS-256 Visualization Generator"
	@echo "==================================="
	@echo "Generating high-density visualization..."
	./$(TARGET)
	@echo ""
	@echo "  VISUALIZATION READY!"
	@echo "================================"
	@echo ""
	@echo "  Open 'animation.html' in your browser"
	@echo ""
	@echo "  FEATURES:"
	@echo "   * 1000+ points"
	@echo "   * Smooth speed control 0.1x - 5x"
	@echo "   * Proper Lorenz butterfly pattern"
	@echo "   * Real KAOS-256 execution data"
	@echo "   * No CORS - double-click to open"
	@echo ""
	@echo "  Verification:"
	@echo "   Check console for: 'DATA LOADED: 1028 points'"

# Limpieza
clean:
	rm -rf $(OBJ_DIR) $(TARGET) animation.html

.PHONY: clean generate