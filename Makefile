###############################################################################
# Colors
###############################################################################
GREEN  := \033[0;32m
YELLOW := \033[1;33m
BLUE   := \033[0;34m
NC     := \033[0m

###############################################################################
# Compiler & Flags
###############################################################################
CC       = gcc
CFLAGS   = -Wall -Wextra -Werror -O2 -MMD -MP #-fsanitize=address -g
LDFLAGS  = -lm 
# -MMD and -MP tell the compiler to generate .d (dependency) files for each .c

DEBUG_FLAGS   := -g -fsanitize=address
BUILD_MODE_FILE = $(OBJ_DIR)/.build_mode

###############################################################################
# Project Settings
###############################################################################
TARGET   = ft_ls
SRC_DIR  = srcs
INC_DIR  = inc
OBJ_DIR  = build

# Libft info
LIBFT_DIR = libft
LIBFT_LIB = lib
LIBFT     = $(LIBFT_DIR)/$(LIBFT_LIB)/libft.a

###############################################################################
# Sources / Objects
###############################################################################
SRC_FILES := $(shell find $(SRC_DIR) -type f -name '*.c')
			
SRCS      := $(SRC_FILES)

OBJS      := $(patsubst $(SRC_DIR)/%.c,$(OBJ_DIR)/%.o,$(SRCS))

DEPS      = $(OBJS:.o=.d)

###############################################################################
# Default Rule
###############################################################################
all: $(TARGET)

###############################################################################
# Linking
###############################################################################
$(TARGET): $(LIBFT) $(OBJS)
	@echo "$(BLUE)Linking $(TARGET)...$(NC)"
	@$(CC) $(CFLAGS) -I$(INC_DIR) -o $@ $(OBJS) $(LIBFT) $(LDFLAGS)
	@echo "$(GREEN)Build complete!$(NC)"

###############################################################################
# Object File Compilation
###############################################################################
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	@echo "$(YELLOW)Compiling $<...$(NC)"
	@mkdir -p $(dir $@)
	@$(CC) $(CFLAGS) -I$(INC_DIR) -c $< -o $@

###############################################################################
# Libft
###############################################################################
$(LIBFT):
	@$(MAKE) -C $(LIBFT_DIR)

###############################################################################
# Cleanup
###############################################################################
clean:
	rm -f $(OBJS) $(DEPS) $(BUILD_MODE_FILE)
	$(MAKE) -C $(LIBFT_DIR) clean

fclean: clean
	rm -f $(TARGET)
	$(MAKE) -C $(LIBFT_DIR) fclean
	rm -rf $(OBJ_DIR)

re: fclean all

###############################################################################
# Others Rules
###############################################################################
debug: CFLAGS := $(DEBUG_FLAGS)
debug:
	@if [ ! -f $(BUILD_MODE_FILE) ] || [ "$$(cat $(BUILD_MODE_FILE))" != "debug" ]; then \
		$(MAKE) fclean; \
		mkdir -p $(OBJ_DIR); \
		echo "debug" > $(BUILD_MODE_FILE); \
	fi
	@$(MAKE) all

help:
	@echo "Available targets:"
	@echo "  all      : Build $(TARGET)"
	@echo "  debug    : Build with debug flags"
	@echo "  release  : Build with optimization"
	@echo "  debug    : Build with debug flags"
	@echo "  release  : Build with optimization"
	@echo "  test     : Run tests with arguments"
	@echo "  tests    : Run all test scripts"
	@echo "  clean    : Remove object files"
	@echo "  fclean   : Remove all generated files"
	@echo "  re       : Rebuild everything"

setup:
	chmod +x setup.sh
	./setup.sh

###############################################################################
# Tests Rules
###############################################################################
test: all
	@echo "Running tests with args: $(ARGS)"
	@ASAN_OPTIONS=detect_leaks=0 ./$(TARGET) $(ARGS)

tests: all
	@cd tests && bash run_tests.sh

###############################################################################
# Dependency Handling
###############################################################################
-include $(DEPS)

###############################################################################
# Phony Targets
###############################################################################
.PHONY: all clean fclean re test tests debug help
