CC		=		g++
FLAGS		=		-Wall -Wextra -Werror -std=c++98 -g
INCL		=		-I./includes

NAME		=		test

SRC		=		main.cpp \
				src/Socket.cpp \
				src/Server.cpp \
				src/Location.cpp \
				src/Config.cpp \
				src/utils.cpp

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


re: kill
	make

r:
	make
	./$(NAME)

.PHONY: all clean fclean kill re
