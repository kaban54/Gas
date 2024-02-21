CC = g++
CFLAGS += -DNDEBUG
SFMLFLAGS += -lsfml-graphics -lsfml-window -lsfml-system
OBJDIR = obj/
SRCDIR = src/

all: reactor

prepare:
	mkdir obj

reactor: obj/main.o obj/reactor.o obj/vec.o obj/buttons.o obj/reactorbtns.o obj/plot.o obj/molecules.o
	$(CC) -o reactor obj/main.o obj/reactor.o obj/vec.o obj/buttons.o obj/reactorbtns.o obj/plot.o obj/molecules.o $(SFMLFLAGS)

$(OBJDIR)%.o: $(SRCDIR)%.cpp
	$(CC) -c $(CFLAGS) $< -o $@

clean:
	rm obj/*.o -f
	clear
	
.PHONY: clean