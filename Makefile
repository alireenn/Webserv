CC		=		g++
FLAGS		=		-Wall -Wextra -Werror -std=c++98

NAME		=		test

SRC		=		temp_main.cpp \
				src/Socket.cpp \
				src/Server.cpp \
				src/Location.cpp

OBJDIR		=		obj
OBJS		=		$(addprefix $(OBJDIR)/, $(SRC:.cpp=.o))



all: $(NAME)


$(NAME): $(OBJS)
	$(CC) $(OBJS) -o $@

$(OBJS): $(OBJDIR)/%.o: %.cpp
	mkdir -p $(@D)
	$(CC) $(FLAGS) -c $< -o $(addprefix $(OBJDIR)/, $(<:.cpp=.o))

clean:
	rm -rf $(OBJDIR)

fclean:
	rm $(NAME)

kill: clean fclean


re: kill
	make

.PHONY: all clean fclean kill re
