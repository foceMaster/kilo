CC = clang
CFLAGS = -Wall -Wextra -std=c99
SRC = src/main.c src/terminal/terminal.c src/input/input.c src/output/output.c src/abuf/abuf.c src/files/files.c
TARGET = target/exec

$(TARGET): $(SRC)
	$(CC) $(SRC) -o $@ $(CFLAGS)

run: $(TARGET)
	./$(TARGET)

test: $(TARGET)
	./$(TARGET) test.file

clean:
	rm $(TARGET)
