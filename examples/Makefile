# examples/Makefile
CC = gcc
CFLAGS = -Wall -Wextra -pedantic -O2 -I../src
LDFLAGS = 
LIBS = -lm

# Directorios
SRC_DIR = .
OBJ_DIR = obj
COMMON_DIR = ../src

# Targets principales
TARGETS = example_text example_file
OBJ_FILES = $(OBJ_DIR)/example_text.o $(OBJ_DIR)/example_file.o $(OBJ_DIR)/kaos.o

# Regla principal - compila todos los ejemplos
all: $(TARGETS)

# Ejemplo de encriptación de texto
example_text: $(OBJ_DIR)/example_text.o $(OBJ_DIR)/kaos.o
	$(CC) -o $@ $^ $(LDFLAGS) $(LIBS)

# Ejemplo de encriptación de archivos
example_file: $(OBJ_DIR)/example_file.o $(OBJ_DIR)/kaos.o
	$(CC) -o $@ $^ $(LDFLAGS) $(LIBS)

# Reglas para objetos locales
$(OBJ_DIR)/example_text.o: $(SRC_DIR)/example_text.c | $(OBJ_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

$(OBJ_DIR)/example_file.o: $(SRC_DIR)/example_file.c | $(OBJ_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

# Regla para objeto común
$(OBJ_DIR)/kaos.o: $(COMMON_DIR)/kaos.c | $(OBJ_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

# Crear directorio obj si no existe
$(OBJ_DIR):
	mkdir -p $(OBJ_DIR)

# Ejecutar ejemplos
run-text: example_text
	@echo "=== Ejecutando ejemplo de texto ==="
	./example_text

run-file: example_file
	@echo "=== Ejecutando ejemplo de archivo ==="
	./example_file

run-all: run-text run-file

# Limpieza
clean:
	rm -rf $(OBJ_DIR) $(TARGETS)

# Help
help:
	@echo "Targets disponibles:"
	@echo "  all       - Compila todos los ejemplos"
	@echo "  example_text - Compila solo ejemplo de texto"
	@echo "  example_file - Compila solo ejemplo de archivo"
	@echo "  run-text  - Compila y ejecuta ejemplo de texto"
	@echo "  run-file  - Compila y ejecuta ejemplo de archivo"
	@echo "  run-all   - Compila y ejecuta todos los ejemplos"
	@echo "  clean     - Limpia binarios"
	@echo "  help      - Muestra esta ayuda"

.PHONY: all clean run-text run-file run-all help