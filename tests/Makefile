CC := gcc

SRCS =
SRCS += ../data_handler/data_handler.c
SRCS += ../http_handler/http_handler.c
SRCS += test_mount_request/test_mount_request.c

.PHONY: all mount_request

all: mount_request

mount_request: $(SRCS)
	@echo "compile $@"
	@$(CC) $(FLAGS) $^ -o $@


.PHONY: clean
clean:
	@echo "clean"
	@rm -rf *.o *~ mount_request
