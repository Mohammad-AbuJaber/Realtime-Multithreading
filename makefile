G = gcc -g
O = -o
OPENGL = -lglut -lGLU -lGL -lm
PTHREAD = -pthread
names = ant

files:
	$(G) $(O) ant ant.c $(OPENGL) $(PTHREAD)
clean:
	rm -f $(names)

run:
	./ant

