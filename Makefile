CXX				= c++
CXXFLAGS		= -Iincludes/ -std=c++98 -Wall -Wextra -Werror
RM				= rm -fr

SRCS_DIR		= srcs
OBJS_DIR		= objs

NAME			= ircserv
SOURCES			= $(wildcard srcs/*.cpp) $(wildcard srcs/*/*.cpp)
SOURCES_O		= $(SOURCES:$(SRCS_DIR)/%.cpp=$(OBJS_DIR)/%.o)

all:	$(NAME)

$(NAME):		$(SOURCES_O)
	$(CXX) -o $@ $^

$(OBJS_DIR):
	mkdir -p objs
	mkdir -p objs/commands
	mkdir -p objs/server
	mkdir -p objs/classes

$(OBJS_DIR)/%.o: $(SRCS_DIR)/%.cpp | $(OBJS_DIR)
	$(CXX) $(CXXFLAGS) -c $^ -o $@

clean:
	$(RM) $(OBJS_DIR)

fclean:			clean
	$(RM) $(NAME)

re:				fclean $(NAME)

.PHONY:			all clean fclean re