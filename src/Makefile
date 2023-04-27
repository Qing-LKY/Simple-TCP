OBJ := main.o args.o client.o server.o io.o
INCLUDE := $(addprefix ../, $(INCLUDE))
TARGET := ../$(TARGET)
CFLAGS += $(patsubst %, -I%, $(INCLUDE))

all: $(OBJ)
	$(CC) $(OBJ) -o $(TARGET)

$(OBJ):%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

.phony: clean

clean:
	$(RM) $(OBJ)