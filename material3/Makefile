CC=gcc
target: shader.so camera.so
	$(CC) ./material.c -L./lib/ -lopengl32 -l:libglad.so -lglfw3dll -l:libshader.so -l:libcamera.so

camera.so: ./lib/camera.c
	$(CC) -c ./lib/camera.c -o ./lib/camera.o
	$(CC) -shared -fPIC -o ./lib/libcamera.so ./lib/camera.o
	cp ./lib/libcamera.so ./libcamera.so

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