CXX = gcc
SRC = main.c prompt.c cd.c list.c execute.c pwd.c echo.c background.c options.c pinfo.c nightswatch.c history.c jobs.c parse.c overkill.c redirect.c piping.c environment.c kjob.c foreground.c chaining.c
OBJ = $(SRC:.c=.o)
EXEC = ash

.SUFFIXES: .c

.c.o:
	$(CXX) -c $<

all: $(EXEC)

$(EXEC): $(OBJ)
	$(CXX) -o $@ $(OBJ)

clean:
	rm -rf $(OBJ) $(EXEC)