NAME	=	ircserv

FILES	=	IRCMessage Client Server Channel main
HEADERS	=	IRCMessage Server Channel Client

SRCS	=	$(addsuffix .cpp, $(FILES))
OBJS	=	$(addprefix objs/, $(SRCS:.cpp=.o))

vpath %.cpp srcs/

CXX		=	c++ -std=c++98 #-Wall -Wextra -Werror -g -fsanitize=address

all: $(NAME)

objs:
	mkdir -p objs

$(NAME): $(OBJS)
	$(CXX) -o $(NAME) $(OBJS)

objs/%.o: srcs/%.cpp | objs
	$(CXX) -c $< -o $@

clean:
	rm -rf $(OBJS)

fclean: clean
	rm -rf $(NAME) objs

re: fclean all

.PHONY: all clean fclean re
