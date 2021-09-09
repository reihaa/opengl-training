CC = g++
CFLAGS = -lGLEW -lglfw3 -lGL -lX11 -lpthread -lXrandr -lXi -ldl -lassimp -std=c++17

TARGET = all

all: mesh shader window camera texture light directionalLight pointLight spotLight material model
	$(CC) main.cpp Mesh.o Light.o PointLight.o SpotLight.o DirectionalLight.o Texture.o Shader.o Window.o Camera.o Material.o Model.o -o a.out $(CFLAGS)
	./a.out

camera: Camera.cpp Camera.h
	$(CC) -Wall -g -c Camera.cpp $(CFLAGS)

light: Light.cpp Light.h
	$(CC) -Wall -g -c Light.cpp $(CFLAGS)

directionalLight: DirectionalLight.cpp DirectionalLight.h Light.o
	$(CC) -Wall -g -c DirectionalLight.cpp $(CFLAGS)

pointLight: PointLight.cpp PointLight.h Light.o
	$(CC) -Wall -g -c PointLight.cpp $(CFLAGS)

spotLight: SpotLight.cpp SpotLight.h PointLight.o
	$(CC) -Wall -g -c SpotLight.cpp $(CFLAGS)
	
mesh: Mesh.cpp Mesh.h
	$(CC) -Wall -g -c Mesh.cpp $(CFLAGS)

model: Model.cpp Model.h Mesh.o Texture.o
	$(CC) -Wall -g -c Model.cpp $(CFLAGS)

material: Material.cpp Material.h
	$(CC) -Wall -g -c Material.cpp $(CFLAGS)

shader: Shader.cpp Shader.h
	$(CC) -Wall -g -c Shader.cpp $(CFLAGS)

window: Window.cpp Window.h
	$(CC) -Wall -g -c Window.cpp $(CFLAGS)

texture: Texture.cpp Texture.h
	$(CC) -Wall -g -c Texture.cpp $(CFLAGS)

clean: 
	$(RM) *.o
