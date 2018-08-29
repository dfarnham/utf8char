RM           = rm -f
CC           = gcc
LIBS         =
CFLAGS       = -I.
EXTRA_CFLAGS = -O2 -Wall
SRCS         = utf8char.c
OBJS         = $(SRCS:.c=.o)
EXE          = utf8char

all: $(EXE)

$(EXE): $(OBJS)
	$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(OBJS) $(LIBS) -o $(EXE)

$(OBJS): $(SRCS)
	$(CC) -c $(CFLAGS) $(EXTRA_CFLAGS) $(SRCS)

test: spotless all
	@ECHO "-~-~-~-~-~-~-~-~TEST~-~-~-~-~-~-~-~-~-~-"
	@./$(EXE) [ ] < beer+pizza

clean:
	$(RM) $(OBJS)

spotless: clean
	$(RM) $(EXE)

