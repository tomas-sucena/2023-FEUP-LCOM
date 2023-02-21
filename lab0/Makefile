CC = cc
CFLAGS = -Wextra -Werror -Wno-unused-parameter
TARGET = hello

all: $(TARGET)

$(TARGET): $(TARGET).c
	@ $(CC) $(CFLAGS) $(TARGET).c -o $(TARGET)

clean:
	@ $(RM) $(TARGET)