EXE = connect4
SRC = $(wildcard src/*.c)

.PHONY: all clean

all: $(EXE)

$(EXE): $(SRC)
	$(CC) -o $@ $^

clean:
	$(RM) $(EXE)
