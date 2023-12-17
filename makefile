.PHONY: x clean tar

EJECUTABLE= practica_objetos_B3

MODULOS:= practica_objetos_B3.o\
         objetos_B3.o\
         file_ply_stl.o\
      
LIBS:= -lglut -lGLU -lGL -lsupc++ -lm -lc -lstdc++ -lpthread -L/usr/lib/nvidia-331
CPPFLAGS:= -Wall -fmax-errors=2 -g

CC= g++

x: $(EJECUTABLE)
	./$(EJECUTABLE) beethoven peon

$(EJECUTABLE): $(MODULOS)	
	$(CC) -o $(EJECUTABLE) $(LDFLAGS) $(MODULOS) $(LIBS) 

clean:
	rm -rf *.o $(EJECUTABLE)

