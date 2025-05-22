
DEF_COLOR           = \033[0;39m
ORANGE              = \033[38;5;209m
MID_GRAY            = \033[38;5;245m
DARK_GREEN          = \033[38;2;75;179;82m

NAME := minishell

INCLUDES_DIR 	:= ./include
LIBFT_DIR 		:= ./libft
LIBFT_ARCHIVE 	:= libft.a
FT_PRINTF_DIR 		:= ./ft_printf
FT_PRINTF_ARCHIVE 	:= libftprintf.a

CC 				:= cc
CCFLAGS 		:= -Wall -Wextra -Werror
MAKE_LIBRARY 	:= ar -rcs 

INCLUDES		:= -I$(LIBFT_DIR) -I$(FT_PRINTF_DIR) -I$(INCLUDES_DIR)
EXT_LIBRARYS 	:= $(FT_PRINTF_DIR)/$(FT_PRINTF_ARCHIVE) $(LIBFT_DIR)/$(LIBFT_ARCHIVE) -lreadline

SRC_DIR			:= ./src
SRC_FILES 		:= $(addprefix $(SRC_DIR)/, 00_minishell.c 01_init_shell.c \
				02_parser_tokenize.c 03_parser_tokenize_utils.c \
				04_parser_expand.c 05_parser_expand_utils.c \
				06_parser_expand_option.c \
				08_utils.c 09_free_functions.c \
				15_utils_process.c)

OBJ_FILES 		:= $(SRC_FILES:%.c=%.o)

all: $(NAME)

$(NAME):  $(OBJ_FILES)
	@make library
	@echo "$(ORANGE)🚀​ Compiling $(NAME)... $(DEF_COLOR)"
	@$(CC) $(CCFLAGS) $(OBJ_FILES) $(INCLUDES) $(EXT_LIBRARYS) -o $(NAME)
	@echo "$(DARK_GREEN)📚 $(NAME) has been created			OK$(DEF_COLOR)"

library:
	@echo "$(ORANGE)📦 Packing library...$(DEF_COLOR)"
	@make -s -C $(LIBFT_DIR)
	@make -s -C $(FT_PRINTF_DIR)
	@echo "$(DARK_GREEN)📦 library created				OK$(DEF_COLOR)"

clean:
	@rm -f $(OBJ_FILES)	
	@make clean -s -C $(LIBFT_DIR)
	@make clean -s -C $(FT_PRINTF_DIR)
	@echo "${MID_GRAY}Cleaning objects $(NAME)			OK$(DEF_COLOR)"

fclean: clean
	@rm -f $(NAME)
	@make fclean -s -C $(LIBFT_DIR)
	@make fclean -s -C $(FT_PRINTF_DIR)
	@echo "$(MID_GRAY)Cleaning $(NAME)				OK$(DEF_COLOR)"

debug: CCFLAGS += -g -fsanitize=address
debug: EXT_LIBRARYS += -fsanitize=address
debug: re
# ignore external leaks: ASAN_OPTIONS=detect_leaks=0 make debug

re: fclean all
	@echo "$(DARK_GREEN)🔁 Cleaning and recompiled -> $(NAME) 	OK$(DEF_COLOR)"	
	@make -s clean

.PHONY: all clean fclean re debug