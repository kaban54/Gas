CC = g++
CFLAGS += -DNDEBUG
SFMLFLAGS += -lsfml-graphics -lsfml-window -lsfml-system
OBJDIR = obj/
SRCDIR = src/

all: gas

gas: obj/main.o obj/gas.o obj/vec.o obj/buttons.o obj/reactorbtns.o obj/plot.o
	$(CC) -o gas obj/main.o obj/gas.o obj/vec.o obj/buttons.o obj/reactorbtns.o obj/plot.o $(SFMLFLAGS)

$(OBJDIR)%.o: $(SRCDIR)%.cpp
	$(CC) -c $(CFLAGS) $< -o $@

clean:
	rm obj/*.o -f
	clear
	
.PHONY: clean