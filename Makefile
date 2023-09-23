CC = g++
CFLAGS += -DNDEBUG
SFMLFLAGS += -lsfml-graphics -lsfml-window -lsfml-system
OBJDIR = obj/
SRCDIR = src/

all: reactor

reactor: obj/main.o obj/reactor.o obj/vec.o obj/buttons.o obj/reactorbtns.o obj/plot.o
	$(CC) -o reactor obj/main.o obj/reactor.o obj/vec.o obj/buttons.o obj/reactorbtns.o obj/plot.o $(SFMLFLAGS)

$(OBJDIR)%.o: $(SRCDIR)%.cpp
	$(CC) -c $(CFLAGS) $< -o $@

clean:
	rm obj/*.o -f
	clear
	
.PHONY: clean