CC = gcc
CFLAGS =  -g -ggdb
LIBS = -lm

OBJS_C =  client.o   
OBJS_S =  agenda.o server.o bd.o  

EXEC_C = c
EXEC_S = s

all:
	make c
	make s

c: $(OBJS_C)
	$(CC) $(CFLAGS) $(OBJS_C) $(LIBS) -o $(EXEC_C)

s: $(OBJS_S)
	$(CC) $(CFLAGS) $(OBJS_S) $(LIBS) -o $(EXEC_S)


run: all
	./$(EXEC) $(ATB)

$(OBJS): %.o: %.c
	$(CC) -c $(CFLAGS) $<

clean:
	rm -f $(OBJS_C) $(OBJS_S) $(EXEC_C) $(EXEC_S) *~

reset: clean
	rm -f $(FILES)
