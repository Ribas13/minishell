# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: ysantos- <ysantos-@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2022/12/17 19:26:52 by nnuno-ca          #+#    #+#              #
#    Updated: 2024/01/06 19:19:59 by ysantos-         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME		=	minishell
CC			= 	cc
CFLAGS		= 	-Wall -Wextra -Werror -g #-fsanitize=address
LINK		= 	-I/inc/
VPATH 		= 	src src/parser src/exp src/commands \
				src/commands/built_ins src/execs src/utils

RM			= 	rm -rf
LIBFT		= 	./libft/libft.a

UTILS		=	invalid_syntax_utils cd_utils cmd_utils destroy fits_longlong free_matrix 		\
				ft_atoll ft_lltoa init_envp_lst is_all_digits_or_signals	\
				is_onstr is_spaces join_free p_llst_utils streq trim_free 	\
				v_llst_utils_v2 v_llst_utils valid_input close_fds
PARSER		=	invalid_syntax parser parser_utils remove_quotes join_args
EXPANDER	=	expander expander_utils
GENERAL		=	config_signals setup_shell
COMMANDS	=	binaries builtins binaries_utils
BUILTINS	=	echo env exit export pwd unset cd
EXECUTES	=	exec_cmd executables pipe redirects redirects_utils redirects_utils2

SRCS		=	main.c						 	\
				$(addsuffix .c, $(UTILS))		\
				$(addsuffix .c, $(UTILS_LL))	\
				$(addsuffix .c, $(PARSER))		\
				$(addsuffix .c, $(EXPANDER))	\
				$(addsuffix .c, $(GENERAL))		\
				$(addsuffix .c, $(COMMANDS))	\
				$(addsuffix .c, $(BUILTINS))	\
				$(addsuffix .c, $(EXECUTES))	\
		
OBJ_DIR = obj
OBJS = $(SRCS:%.c=$(OBJ_DIR)/%.o)

all: $(NAME)

$(NAME): $(OBJ_DIR) $(OBJS) $(LIBFT)
	$(CC) $(CFLAGS) $(OBJS) $(LIBFT) -lreadline -o minishell

$(LIBFT):
	$(MAKE) -C ./libft

$(OBJ_DIR):
	mkdir -p obj

$(OBJ_DIR)/%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@ $(LINK)

clean:
	$(MAKE) clean -C ./libft
	$(RM) $(OBJ_DIR)
	$(RM) $(OBJS)
	
fclean: clean
	$(MAKE) fclean -C ./libft
	$(RM) $(NAME)
	
re: fclean all

noleaks: re
	valgrind ./minishell

noleaksfull: re
	valgrind --leak-check=full --show-leak-kinds=all -s ./minishell

run: re
	./minishell

run2:
	./minishell

.PHONY: all clean fclean re run
