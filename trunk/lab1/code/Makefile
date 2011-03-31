CC = gcc
CFLAGS =  -g -ggdb
LIBS = -lm

OBJS_C =  client.o   
OBJS_S =  agenda.o server.o  

EXEC = run

all:
	make c
	make s

c: $(OBJS_C)
	$(CC) $(CFLAGS) $(OBJS_C) $(LIBS) -o $(EXEC)

s: $(OBJS_S)
	$(CC) $(CFLAGS) $(OBJS_S) $(LIBS) -o $(EXEC)


run: all
	./$(EXEC) $(ATB)

$(OBJS): %.o: %.c
	$(CC) -c $(CFLAGS) $<

clean:
	rm -f $(OBJS_C) $(OBJS_S) $(EXEC) *~

reset: clean
	rm -f $(FILES)
