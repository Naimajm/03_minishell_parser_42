
DEF_COLOR           = \033[0;39m
ORANGE              = \033[38;5;209m
MID_GRAY            = \033[38;5;245m
DARK_GREEN          = \033[38;2;75;179;82m

NAME := minishell

INCLUDES_DIR 	:= ./include
LIBFT_DIR 		:= ./libft
LIBFT_ARCHIVE 	:= libft.a

CC 				:= cc
CFLAGS 			:= -Wall -Wextra -Werror

INCLUDES		:= -I$(LIBFT_DIR) -I$(INCLUDES_DIR)
EXT_LIBRARYS 	:= $(LIBFT_DIR)/$(LIBFT_ARCHIVE) -lreadline

SRC_DIR			:= ./src
SRC_FILES 		:= $(addprefix $(SRC_DIR)/, 00_main.c \
				01_run_shell.c 01.1_check_syntax.c \
				02_command_generate.c  02.2_command_builder.c \
				03_lexical_analyzer.c 03.1_word_builder.c \
				04_tokenizer.c 04.1_token_builder.c \
				05_variable_expander.c \
				05.1_expand_list.c 05.2_expand_extractor.c 05.3_expand_builder.c 05.4_dequotizer.c \
				06_word_processor.c \
				07_execution_builder.c \
				10_utils_core.c 10.1_utils_strings.c 10.2_free_manager.c 10.3_utils_debug.c \
				12_test_parser.c )

OBJ_FILES 		:= $(SRC_FILES:%.c=%.o)

all: $(NAME)

$(NAME):  $(OBJ_FILES)
	@make library
	@echo "$(ORANGE)🚀​ Compiling $(NAME)... $(DEF_COLOR)"
	@$(CC) $(CFLAGS) $(OBJ_FILES) $(INCLUDES) $(EXT_LIBRARYS) -o $(NAME)
	@echo "$(DARK_GREEN)📚 $(NAME) has been created			OK$(DEF_COLOR)"

library:
	@echo "$(ORANGE)📦 Packing external libraries...$(DEF_COLOR)"
	@make -s -C $(LIBFT_DIR)
	@echo "$(DARK_GREEN)📦 external libraries created			OK$(DEF_COLOR)"

clean:
	@rm -f $(OBJ_FILES)	
	@make clean -s -C $(LIBFT_DIR)
	@echo "${MID_GRAY}Cleaning objects $(NAME)			OK$(DEF_COLOR)"

fclean: clean
	@rm -f $(NAME)
	@make fclean -s -C $(LIBFT_DIR)
	@echo "$(MID_GRAY)Cleaning $(NAME)				OK$(DEF_COLOR)"

debug: CFLAGS += -g -fsanitize=address
debug: EXT_LIBRARYS += -fsanitize=address
debug: re
# ignore external leaks: ASAN_OPTIONS=detect_leaks=0 make debug

re: fclean all
	@echo "$(DARK_GREEN)🔁 Cleaning and recompiled -> $(NAME) 	OK$(DEF_COLOR)"	
	@make -s clean

.PHONY: all clean fclean re debug