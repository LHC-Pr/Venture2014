CXX = g++
LDLIBS  = -g -lglut -framework GLUT -framework OpenGL
OBJS    = my_main.o my_assist.o my_stage1.o 
PROGRAM = game

all: $(PROGRAM)

$(PROGRAM): $(OBJS)
	$(CXX) -W -Wall $(OBJS) $(LDLIBS) -o $(PROGRAM)

clean: rm -f $(OBJS) $(PROGRAM)

