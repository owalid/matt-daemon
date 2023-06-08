CPP = c++

SRC = matt_daemon.cpp event.cpp tintin_reporter.cpp utils.cpp
CPPFLAGS = -std=c++11 -pedantic -Wall -Wextra -Werror -O2
NAME = Matt_daemon

SRCDIR = ./src
OBJDIR = ./obj

OBJS = $(addprefix $(OBJDIR)/,$(SRC:.cpp=.o))
DEPS = $(SRC:.cpp=.d)

all: $(NAME)

$(NAME): $(OBJS)
	$(CPP) $(CPPFLAGS) $(OBJS) -o $(NAME) -Iincludes

$(OBJDIR)/%.o: $(SRCDIR)/%.cpp
	@mkdir -p $(OBJDIR)
	$(CPP) -MP -MMD -c $(FLAGS) $< -o $@ -Iincludes

-include $(DEPS)

clean:
	rm -rf $(OBJDIR)

fclean: clean
	rm $(NAME)

re: fclean all

.PHONY: clean re all fclean
