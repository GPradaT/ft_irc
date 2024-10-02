NAME	=	ircserv

FILES	=	IRCMessage main

SRCS	=	$(addsuffix .cpp, $(FILES))
OBJS	=	$(SRCS:.cpp=.o)

vpath %.cpp srcs/

CXX		=	c++ -std=c++98 #-Wall -Wextra -Werror -g -fsanitize=address

all: $(NAME)

$(NAME): $(OBJS)
	$(CXX) -o $(NAME) $(OBJS)

%o: %cpp
	$(CXX) -c $< -o $@

clean:
	rm -rf $(OBJS)

fclean: clean
	rm -rf $(NAME)

re: fclean all

.PHONY: all clean fclean re
