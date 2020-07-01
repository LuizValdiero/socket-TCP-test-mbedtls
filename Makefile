CC := gcc

LIBS_DIR = ./libs/
FLAGS = -Wall -Wextra  -O2
MBEDTLS_FLAGS = -I$(LIBS_DIR) -D_FILE_OFFSET_BITS=64 -L$(LIBS_DIR) -lmbedtls -lmbedx509 -lmbedcrypto

SRCS = my_test.c
SRCS += data_handler/data_handler.c
SRCS += http_handler/http_handler.c

.PHONY: all my_test

all: my_test

my_test: $(SRCS)
	@echo "compile $@"
	@$(CC) $(FLAGS) $^ $(MBEDTLS_FLAGS) -o $@


.PHONY: clean
clean:
	@echo "clean"
	@rm -rf *.o *~ my_test
