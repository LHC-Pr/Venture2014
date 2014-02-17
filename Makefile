CXX = g++
LDLIBS  = -g -lglut -framework GLUT -framework OpenGL
OBJS    = Main.o Assist.o Stage1.o 
PROGRAM = game

all: $(PROGRAM)

$(PROGRAM): $(OBJS)
	$(CXX) -W -Wall $(OBJS) $(LDLIBS) -o $(PROGRAM)

clean: rm -f $(OBJS) $(PROGRAM)

