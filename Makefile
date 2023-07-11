CPP = g++

SRC = matt_daemon.cpp event.cpp tintin_reporter.cpp utils.cpp server.cpp errors.cpp lockfile.cpp \
			daemonize.cpp aes_decrypter.cpp proc_connector.cpp
CPPFLAGS = --std=c++17 -pedantic -Wall -Wextra -Werror -O2
NAME = Matt_daemon

SRCDIR = ./src
OBJDIR = ./obj

OBJS = $(addprefix $(OBJDIR)/,$(SRC:.cpp=.o))
DEPS = $(SRC:.cpp=.d)

all: $(NAME)

$(NAME): $(OBJS)
	$(CPP) $(CPPFLAGS) $(OBJS) -o $(NAME) -Iincludes -lcrypto -lpthread -lstdc++fs


$(OBJDIR)/%.o: $(SRCDIR)/%.cpp
	@mkdir -p $(OBJDIR)
	$(CPP) -MP -MMD -c $(FLAGS) $< -o $@ -Iincludes -rdynamic -lstdc++fs

-include $(DEPS)

clean:
	rm -rf $(OBJDIR)

fclean: clean
	rm $(NAME)

test:
	python3 test/main.py

install:
	python3 -m pip install -r ./scripts/encrypted_command/requirements.txt
	python3 -m pip install -r ./web/api/requirements.txt
	npm i --prefix ./web/client/

api:
	FLASK_APP=./web/api python3 ./web/api/app.py

client:
	npm --prefix ./web/client/ run dev
	

re: fclean all

.PHONY: clean re all fclean test
