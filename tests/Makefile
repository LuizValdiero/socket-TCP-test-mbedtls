CC := gcc

SRCS =
SRCS += ../src/my_post.c
SRCS += ../src/data_handler.c
SRCS += ../src/http_handler.c
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
