CXX				= c++
CXXFLAGS		= -Wall -Wextra -Iincludes/ -std=c++98#-Werror
RM				= rm -fr

SRCS_DIR		= srcs
OBJS_DIR		= objs

NAME			= ircserv
SOURCES			= $(SRCS_DIR)/main.cpp $(SRCS_DIR)/server.cpp $(SRCS_DIR)/client.cpp \
				$(SRCS_DIR)/fd.cpp
SOURCES_O		= $(SOURCES:$(SRCS_DIR)/%.cpp=$(OBJS_DIR)/%.o)

all:	$(NAME)

$(NAME):		$(SOURCES_O)
	$(CXX) -o $@ $^

$(OBJS_DIR):
	mkdir -p objs

$(OBJS_DIR)/%.o: $(SRCS_DIR)/%.cpp | $(OBJS_DIR)
	$(CXX) $(CXXFLAGS) -c $^ -o $@

clean:
	$(RM) $(OBJS_DIR)

fclean:			clean
	$(RM) $(NAME)

re:				fclean $(NAME)

.PHONY:			all clean fclean re