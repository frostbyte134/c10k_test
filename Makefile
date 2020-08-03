CC=gcc
CFLAGS=-W
SERVER=c10k_serv.out
CLIENT=c10k_cli.out
SRCS=$(wildcard src/*.c)
#OBJS=$(SRCS:.c=.o)
IDIRS=include




all: clean $(SERVER) $(CLIENT)

server: $(SERVER)

client: $(CLIENT)

$(SERVER) :
	$(CC) $(CFLAGS) -o $@ $^ -I $(IDIRS) $(SRCS) #acually we dont need $^ here since we dont have any dependencies

$(CLIENT) :
	$(CC) $(CFLAGS) -o $@ $^ client.c

clean:
	rm -f *.out
	find . | grep "\.o$$" | xargs rm -f
