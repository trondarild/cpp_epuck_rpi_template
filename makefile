SOURCES=main.cpp tat_graphics.cpp tat_math.cpp tat_utils.cpp tat_neurosim.cpp
mac:	#main.cpp tat_math.cpp tat_graphics.cpp
	g++ $(SOURCES) -o test -L/System/Library/Frameworks -framework GLUT -framework OpenGL -Wno-deprecated

macdbg:	# main.cpp tat_math.cpp
	g++ -g $(SOURCES) -o test -L/System/Library/Frameworks -framework GLUT -framework OpenGL -Wno-deprecated
linux:
	g++ $(SOURCES) -o test -lglut -lGL -lGLU -Wno-deprecated
linuxdbg:
	g++ -g $(SOURCES) -o test -lglut -lGL -lGLU -Wno-deprecated
pi:
	g++ -g $(SOURCES) -o test -lglut -lGL -lGLU -Wno-deprecated
clean:
	-rm test test_d
