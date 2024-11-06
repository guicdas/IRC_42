CC				= c++
CFLAGS			= -Wall -Wextra -Werror -std=c++98 -fsanitize=address
RM				= rm -fr

SRCS_DIR		= srcs
OBJS_DIR		= objs
INCLUDES_DIR	= includes

NAME			= ircserv
HEADER			= -I $(INCLUDES_DIR)

SOURCES			= $(SRCS_DIR)/main.cpp

SOURCES_O		= $(SOURCES:$(SRCS_DIR)/%.cpp=$(OBJS_DIR)/%.o)

all:			$(NAME)

$(NAME):		$(SOURCES_O)
	$(CC) -o $@ $^

$(OBJS_DIR)/%.o: $(SRCS_DIR)/%.cpp
	-mkdir -p $(OBJS_DIR)
	$(CC) $(CFLAGS) -c $^ -o $@

clean:
	$(RM) $(OBJS_DIR)/*.o	

fclean:			clean
	$(RM) $(NAME)

re:				fclean $(NAME)

.PHONY:			all clean fclean re