NAME				:=	minishell

CC					:=	gcc

CFLAGS				:=	-Wall -Werror -Wextra
CFLAGS				+=	-g
# CFLAGS				+=	-fsanitize=address

LDFLAGS				:=	-g
# LDFLAGS				+=	-fsanitize=address

MINISHELL_SRCS		=	./main2.c \
						# ./main1.c

DATA_INIT_SRCS		=	./00_data_init/envp_ll_init.c \
						./00_data_init/envp_ll_utils.c \
						./00_data_init/envp_local_init.c \
						./00_data_init/envp_local_utils.c \
						./00_data_init/pipe_data.c

LEXER_SRCS			=	./01_lexer/lexer0.c

PARSER_SRCS			=	./02_parser/parser0.c

EXPANDER_SRCS		=	./03_expander/expander0.c

EXECUTOR_SRCS		=	./04_executor/execute_ast0.c \
						./04_executor/execute_ast1.c \
						./04_executor/child_process.c \
						./04_executor/child_process_utils0.c \
						./04_executor/child_process_utils1.c \
						./04_executor/child_process_utils2.c \
						./04_executor/exit_code_handler0.c \
						./04_executor/signals0.c \
						./04_executor/signals1.c \
						./04_executor/signals2.c

UTILS_SRCS			=	./05_utils/create_ast.c \
						./05_utils/print_ast.c \
						./05_utils/free_ast.c \
						./05_utils/exit_utils0.c \
						./05_utils/exit_utils1.c \
						./05_utils/get_next_line.c

ALL_SRCS			:=	$(MINISHELL_SRCS) \
						$(DATA_INIT_SRCS) \
						$(LEXER_SRCS) \
						$(PARSER_SRCS) \
						$(EXECUTOR_SRCS) \
						$(UTILS_SRCS)

HEADERS				:=	-I ./include
HEADERS				+=	-I ./05_utils
HEADERS				+=	-I $(shell brew --prefix readline)/include
# HEADERS				+=	-I ../LeakSanitizer

LDFLAGS				+=	-L ./lib/ft_printf -lft_ft_printf
LDFLAGS				+=	-lreadline -L $(shell brew --prefix readline)/lib
# LDFLAGS				+=	-L ../LeakSanitizer -llsan -lstdc++ -Wno-gnu-include-next

LIBFT_FT_PRINTF		:=	lib/ft_printf/libft_ft_printf.a
LIBFT_FT_PRINTF_DIR	:=	./lib/ft_printf

OBJS				=	$(ALL_SRCS:.c=.o)

all: $(NAME)

$(NAME): $(LIBFT_FT_PRINTF) $(OBJS)
	$(CC) $(OBJS) -o $(NAME) $(LDFLAGS) \
	&& echo "Compilation of $(NAME) successful"

%.o: %.c
	$(CC) $(CFLAGS) $(HEADERS) -c $< -o $@

$(LIBFT_FT_PRINTF):
	${MAKE} -C $(LIBFT_FT_PRINTF_DIR) -j

re: fclean all

fclean: clean
	rm -f $(NAME)

clean:
	${MAKE} -C $(LIBFT_FT_PRINTF_DIR) fclean
	rm -f $(OBJS)

leaks:
	@valgrind	--track-fds=yes \
				--track-origins=yes \
				--tool=memcheck \
				--trace-children=yes \
				--leak-check=full \
				--show-leak-kinds=all \
				-s \
				./minishell < infile cat | grep PATH | grep /usr > outfile

.PHONY: all clean fclean re leaks
