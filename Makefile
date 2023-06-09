CC		=		g++
FLAGS		=		-std=c++98 -Wall -Wextra -Werror -g
INCL		=		-I./includes

NAME		=		webserv

SRC		=		main.cpp \
				src/Socket.cpp \
				src/Server.cpp \
				src/Location.cpp \
				src/Config.cpp \
				src/utils.cpp \
				src/log.cpp \
				src/IOMultiplexing.cpp \
				src/Request.cpp \
				src/Response.cpp \
				src/Client.cpp \
				src/AutoIndex.cpp \


OBJDIR		=		obj
OBJS		=		$(addprefix $(OBJDIR)/, $(SRC:.cpp=.o))



all: $(NAME)


$(NAME): $(OBJS)
	$(CC) $(OBJS) -o $@

$(OBJS): $(OBJDIR)/%.o: %.cpp
	mkdir -p $(@D)
	$(CC) $(FLAGS) $(INCL) -c $< -o $(addprefix $(OBJDIR)/, $(<:.cpp=.o))

clean:
	rm -rf $(OBJDIR)

fclean: clean
	rm $(NAME)

kill: clean fclean

re: fclean all

r:
	make
	./$(NAME)

.PHONY: all clean fclean kill re
