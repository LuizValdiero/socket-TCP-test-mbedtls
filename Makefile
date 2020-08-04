CC := gcc

LIBS_DIR = ./libs/
FLAGS = -Wall -Wextra  -O2
MBEDTLS_FLAGS = -I$(LIBS_DIR) -D_FILE_OFFSET_BITS=64 -L$(LIBS_DIR) -lmbedtls -lmbedx509 -lmbedcrypto

SRCS = src/main.c
SRCS += src/connections_handler.c
SRCS += src/tls_handler.c
SRCS += src/data_handler.c
SRCS += src/http_handler.c
SRCS += src/my_post.c

.PHONY: all my_test

all: my_test

my_test: $(SRCS)
	@echo "compile $@"
	@$(CC) $(FLAGS) $^ $(MBEDTLS_FLAGS) -o $@


.PHONY: clean
clean:
	@echo "clean"
	@rm -rf *.o *~ my_test
