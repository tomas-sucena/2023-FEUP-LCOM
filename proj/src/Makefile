# name of the program (Minix service)
PROG = proj 

# source code files to be compiled
SRCS = main.c list.c

# header files to be included
#INC_DIR = ../headers

# additional compilation flags
# "-Wall -Wextra -Werror -I . -std=c11 -Wno-unused-parameter" are already set
CFLAGS += -pedantic -I$(INC_DIR)
CPPFLAGS += -D PROJ

# list of library dependencies
DPADD += ${LIBLCF}
LDADD += -llcf

# include LCOM's makefile that does all the "heavy lifting"
.include <minix.lcom.mk>
