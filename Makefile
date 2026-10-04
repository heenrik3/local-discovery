CC = gcc
CFLAGS = -Wall -Wextra -Wpedantic -std=c11
CPPFLAGS = -Iinclude
TARGET = bin/app
BUILD_DIR = build
SOURCES = $(wildcard src/*.c)
OBJECTS = $(patsubst src/%.c,$(BUILD_DIR)/%.o,$(SOURCES))
C_FILES = $(wildcard src/*.c tests/*.c include/*.h)

.PHONY: all run clean lint format test check help

all: $(TARGET)

$(TARGET): $(OBJECTS) | bin
	$(CC) $(CFLAGS) $(OBJECTS) -o $@

$(BUILD_DIR)/%.o: src/%.c | $(BUILD_DIR)
	$(CC) $(CPPFLAGS) $(CFLAGS) -c $< -o $@

bin $(BUILD_DIR):
	mkdir -p $@

run: $(TARGET)
	./$(TARGET)

clean:
	rm -rf $(BUILD_DIR) $(TARGET)

lint:
	uncrustify -c .uncrustify.cfg --check $(C_FILES)

format:
	uncrustify -c .uncrustify.cfg --replace --no-backup $(C_FILES)

test:
	@echo "Nenhum teste implementado."

check:
	$(MAKE) format
	$(MAKE) all
	$(MAKE) test

help:
	@echo "Comandos disponíveis:"
	@echo "  make        - Compila o projeto"
	@echo "  make run    - Executa o programa"
	@echo "  make clean  - Remove artefatos"
	@echo "  make lint   - Verifica o estilo do código"
	@echo "  make format - Formata o código"
	@echo "  make test   - Executa os testes"
	@echo "  make check  - Formata, compila e executa testes"
	@echo "  make help   - Lista estes comandos"