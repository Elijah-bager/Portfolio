
# Your name goes here or you are getting a zero

CFLAGS= -std=c99 -pedantic -Wformat -Wreturn-type
LDFLAGS= -L. 
LIBS=-llinkedlist -lmc -lncursesw  -lm

all:  tags headers lab4


headers: *.c tags
	./headers.sh

tags: *.c
	ctags -R .
	
p1: p1.o
	gcc -g -o $@ $^  $(LDFLAGS) $(LIBS)

lab4u: lab4.o bits.o sim.o input.o aimpoints.o missiles.o output.o text.o graphics.o memory.o n2.o unreliable.o
	gcc -g -o $@ $^ $(LDFLAGS) $(LIBS)

lab4: lab4.o bits.o sim.o input.o aimpoints.o missiles.o output.o text.o graphics.o memory.o n2.o reliable.o
	gcc -g -o $@ $^ $(LDFLAGS) $(LIBS)


# you need a zip target
l4.zip: *.c *.h *.a n2.o makefile unreliable.o reliable.o README_LAB4
	zip $@ $^
	rm -rf install
	mkdir install
	unzip $@ -d install 
	make -C install -r lab4
	sleep 1
	rm -rf install



# this is our master compiler rule to generate .o files.
# It needs all 4 flags

%.o:%.c *.h
	gcc $(CFLAGS) -g -c $< -o $@







