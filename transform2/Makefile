CC=gcc
rectangle: shader.so
	$(CC) ./transform.c -L./lib/ -lopengl32 -l:libglad.so -lglfw3dll -l:libshader.so

shader.so: libglad.so
	$(CC) -o ./lib/loadShader.o -c ./lib/loadShader.c
	$(CC) -shared -fPIC -o ./lib/libshader.so -L. -l:libglad.so ./lib/loadShader.o
	cp ./lib/libshader.so ./libshader.so

libglad.so: ./lib/glad.c
	$(CC) -c ./lib/glad.c -o ./lib/glad.o
	$(CC) -shared -fPIC -o ./lib/libglad.so ./lib/glad.o
	cp ./lib/libglad.so ./libglad.so

clean:
	rm -rf */*.exe
	rm -rf */*.o
	rm -rf */*.so
	rm -rf *.exe
	rm -rf *.o
	rm -rf *.so