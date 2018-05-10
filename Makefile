# TARGET = prog
# LIBS = -lm
# CC = gcc
# CFLAGS = -g -Wall

# .PHONY: default all clean

# default: $(TARGET)
# all: default

# OBJECTS = $(patsubst %.c, %.o, $(wildcard *.c))
# HEADERS = $(wildcard *.h)

# %.o: %.c $(HEADERS)
# 	$(CC) $(CFLAGS) -c $< -o $@

# .PRECIOUS: $(TARGET) $(OBJECTS)

# $(TARGET): $(OBJECTS)
# 	$(CC) $(OBJECTS) -Wall $(LIBS) -o $@

# clean:
# 	-rm -f *.o
# 	-rm -f $(TARGET)
#-----------------------------------------------------------------------------------------------
# Makefile for set
#****************************************************************

CPPOPTIONS = -Wall -g

# ***************************************************************
# Entry to bring the package up to date
#    The "make all" entry should be the first real entry

all: client server

client: client.h client.c 
	g++ $(CPPOPTIONS) -o client client.c

server: server.h server.c
	g++ $(CPPOPTIONS) -o server server.c


# ***************************************************************
# Standard entries to remove files from the directories
#    tidy  -- eliminate unwanted files
#    clean -- delete derived files in preparation for rebuild

clean: tidy
	rm -f *o server client