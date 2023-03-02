CPP=c++
CFLAGS=-Wall -Wextra -Werror -std=c++98
NAME=server
SRC=time_server.cpp
OBJ=$(SRC:.cpp=.o)

all: $(NAME)

%.o: %.cpp
	$(CPP) $(CFLAGS) -c $< -o $@

$(NAME): $(OBJ)
	$(CPP) $^ -o $@

clean:
	@rm -rf *.o
	@echo cleaning

test: all
	@clear
	@./$(NAME)

fclean: clean
	@rm -rf $(NAME)
	@echo "froced cleaning"

