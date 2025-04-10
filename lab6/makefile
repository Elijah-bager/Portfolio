
# Your name goes here or you are getting a zero

CFLAGS= -std=c99 -pedantic -Wformat -Wreturn-type
LDFLAGS= 
LIBS= 


all:  tags headers mtest stest lab6


stest: stest.o stack.s
	gcc -g -m64 -o $@ $^
	
mtest: mtest.o ops.s
	gcc -g -m64 -o $@ $^
	
lab6: lab6.o c_loop.o execute.s ops.s stack.s e_shim.o print.o
	gcc -g -m64 -o $@ $^
	
bonus6: lab6.o loop.s execute.s ops.s stack.s e_shim.o
	gcc -g -m64 -o $@ $^
	

headers: *.c tags
	./headers.sh

tags: *.c
	ctags -R .



# put a self-testing zip target here or it's late or possibly a zero
lab6.zip: makefile *.c *.h *.s README_LAB6 e_shim.o print.o c_loop.o mtest.o stest.o
	zip $@ $^
	rm -rf install
	mkdir install
	unzip lab6.zip -d install
	make -C install -r lab6
	sleep 1
	rm -rf install

# this is our master compiler rule to generate .o files.
# It needs all 4 flags

%.o:%.c *.h
	gcc $(CFLAGS) -g -c $< -o $@

