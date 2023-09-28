NAME = ircserv

CXX = g++
CXXFLAGS = -Wall -Wextra -Werror -std=c++98

RM = rm -f

OUTDIR = out/

DIR_SRCS = src
DIR_CMD = command
DIR_CHAN = channel
DIR_USER = user
DIR_SERV = server

CMD_SRCS = $(wildcard $(DIR_SRCS)/$(DIR_CMD)/*.cpp)
CHAN_SRCS = $(wildcard $(DIR_SRCS)/$(DIR_CHAN)/*.cpp)
USER_SRCS = $(wildcard $(DIR_SRCS)/$(DIR_USER)/*.cpp)
SERV_SRCS = $(wildcard $(DIR_SRCS)/$(DIR_SERV)/*.cpp)

SRCS = $(DIR_SRCS)/main.cpp $(CMD_SRCS) $(CHAN_SRCS) $(USER_SRCS) $(SERV_SRCS)
OBJS = $(SRCS:%.cpp=$(OUTDIR)%.o)

all: $(NAME)

$(NAME): $(OBJS)
	$(CXX) $(CXXFLAGS) $(OBJS) -o $@

clean:
	$(RM) -r $(OUTDIR)

fclean: clean
	$(RM) -r $(NAME)

re: fclean all

$(OBJS): $(OUTDIR)%.o: %.cpp
	@mkdir -p $(@D)
	$(CXX) $(CXXFLAGS) -c $< -o $@

.PHONY: all clean fclean re
