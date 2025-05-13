CC = g++
CFLAGS = -std=c++20 -Wall -Wextra -Iinclude -Winvalid-pch
LDFLAGS = -Lbuild/lib -lsqlite3  # Указываем путь к локальной библиотеке

# Все объектные файлы будут в build/
OBJS = \
	build/pch.o \
	build/Auth.o \
	build/AutoSalonDatabaseManager.o \
	build/main.o

all: bin/autosalon

# Правило для сборки исполняемого файла
bin/autosalon: $(OBJS)
	@mkdir -p bin
	$(CC) $^ $(LDFLAGS) -o $@

# Правило для создания объектных файлов в build/
build/%.o: src/%.cpp
	@mkdir -p build
	$(CC) $(CFLAGS) -c $< -o $@

# Чистка проекта
clean:
	rm -rf bin/* build/*

.PHONY: all clean

# Автоматические зависимости
-include $(OBJS:.o=.d)

# Генерация зависимостей
build/%.d: src/%.cpp
	@mkdir -p build
	@$(CC) $(CFLAGS) -MM -MT '$*.o' -MF $@ $< > /dev/null
