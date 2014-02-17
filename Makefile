CXX = g++

UNAME = ${shell uname}

LDLIBS = -g -lglut -framework GLUT -framework OpenGL
ifeq (\$(UNAME), Linux)
LDLIBS = -lglut -lGLU -lGL
endif

ifeq (\$(UNAME), Darwin)
LDLIBS  = -g -lglut -framework GLUT -framework OpenGL
endif

OBJS    = Main.o Assist.o Stage1.o 
PROGRAM = game

all: $(PROGRAM)

$(PROGRAM): $(OBJS)
	$(CXX) -W -Wall $(OBJS) $(LDLIBS) -o $(PROGRAM) 

clean: rm -f $(OBJS) $(PROGRAM)

