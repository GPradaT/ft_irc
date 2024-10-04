NAME= ircserv
CC= g++
CFLAGS= -std=c++11
SRC = src/Client.cpp src/Channel.cpp src/Server.cpp src/main.cpp
OBJ = $(SRC:.cpp=.o)
INCLUDES= includes/Client.hpp includes/Channel.hpp includes/Server.hpp

all: $(NAME)


$(NAME): $(OBJ)
	$(CC) $(CFLAGS) $(OBJ) -o $(NAME)

%.o: %.cpp $(INCLUDES)
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(NAME)

fclean: clean
	rm -rf $(OBJ)

re: fclean all

.PHONY: all clean fclean re