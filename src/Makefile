OBJ := main.o args.o client.o server.o io.o
INCLUDE := $(addprefix ../, $(INCLUDE))
TARGET := ../$(TARGET)
CFLAGS += $(patsubst %, -I%, $(INCLUDE))

all: $(OBJ)
	$(CC) $(OBJ) -o $(TARGET)

.phony: clean static

static: $(OBJ)
	$(CC) -static $(OBJ) -o $(TARGET)-static

$(OBJ):%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	$(RM) $(OBJ)