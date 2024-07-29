﻿
#include <iostream>
#include <string>
#include <assert.h>
#include "Sprite.h"
#include "MovingSprite.h"

//GLM
#include <glm/glm.hpp> 
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

//STB IMAGE
#include<stb_image.h>


using namespace std;

//Classe para manipulação dos shaders
#include "Shader.h"

// Variável global para controlar a movimentação dos sprites
bool walkRight = false;
bool walkLeft = false;
bool walkUp = false;
bool walkDown = false;

// Protótipo da função de callback de teclado
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode);

// Protótipos das funções
int setupGeometry();
void drawScene(int VAO, int texID);
GLuint loadTexture(string texturePath);

// Dimensões da janela (pode ser alterado em tempo de execução)
const GLuint WIDTH = 800, HEIGHT = 600;

// Função MAIN
int main()
{
	// Inicialização da GLFW
	if (!glfwInit()) {
		// Initialization failed
		return -1;
	}

	// Criação da janela GLFW
	GLFWwindow* window = glfwCreateWindow(640, 480, "Hello World", NULL, NULL);
	if (!window) {
		// Window or OpenGL context creation failed
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);

	// Fazendo o registro da função de callback para a janela GLFW
	glfwSetKeyCallback(window, key_callback);

	// GLAD: carrega todos os ponteiros d funções da OpenGL
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		cout << "Failed to initialize GLAD" << endl;

	}

	// Obtendo as informações de versão
	const GLubyte* renderer = glGetString(GL_RENDERER); /* get renderer string */
	const GLubyte* version = glGetString(GL_VERSION); /* version as a string */
	cout << "Renderer: " << renderer << endl;
	cout << "OpenGL version supported " << version << endl;
	cout << "Pressione 'ASDW' para movimentar o personagem " << endl;

	// Compilando e buildando o programa de shader
	//Shader shader("../shaders/helloTriangle.vs", "../shaders/helloTriangle.fs");
	Shader shader("../shaders/tex.vs", "../shaders/tex.fs");

	// Gerando um buffer simples, com a geometria de um triângulo
	GLuint VAO = setupGeometry();
	
	GLuint texID = loadTexture("../../Textures/Battleground1.png");
	GLuint texID6 = loadTexture("../../Textures/sully.png");
	
	//Ativando o buffer de textura 0 da opengl
	glActiveTexture(GL_TEXTURE0);

	shader.Use();
	// habilitando a transparencia do PNG
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	// Criando uma instância da classe MovingSprite
	MovingSprite character(texID6, &shader, 4, 4, 100);
	character.position = glm::vec3(400.0f, 300.0f, 0.5f);
	character.scale = glm::vec3(100.0f, 100.0f, 0.0f);

	//Matriz de projeção paralela ortográfica
	glm::mat4 projection = glm::ortho(0.0,800.0,0.0,600.0,-1.0,1.0);
	//Enviando para o shader a matriz como uma var uniform
	shader.setMat4("projection", glm::value_ptr(projection));
	
	//Matriz de transformação do objeto (matriz de modelo)
	glm::mat4 model = glm::mat4(1); //matriz identidade
	model = glm::translate(model, glm::vec3(400.0, 300.0, 0.0));
	model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0, 0.0, 1.0));
	model = glm::scale(model, glm::vec3(300.0, 300.0, 0.0));
	shader.setMat4("model", glm::value_ptr(model));

	shader.setInt("texBuffer", 0);

	// Loop da aplicação - "game loop"
	while (!glfwWindowShouldClose(window))
	{
		// Checa se houveram eventos de input (key pressed, mouse moved etc.) e chama as funções de callback correspondentes
		glfwPollEvents();
		
		// Definindo as dimensões da viewport com as mesmas dimensões da janela da aplicação
		int width, height;
		glfwGetFramebufferSize(window, &width, &height);
		glViewport(0, 0, width, height); //unidades de tela: pixel
		
		// Limpa o buffer de cor
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f); //cor de fundo
		glClear(GL_COLOR_BUFFER_BIT);

		glLineWidth(10);
		glPointSize(20);
		
		//Modificar a rotação para animação 
		float angle = (float) glfwGetTime();
		model = glm::mat4(1); //matriz identidade
		model = glm::translate(model, glm::vec3(400.0, 300.0, 0.0));
		//model = glm::rotate(model, angle, glm::vec3(0.0, 0.0, 1.0));	
		model = glm::scale(model, glm::vec3(300.0, 300.0, 0.0));
		shader.setMat4("model", glm::value_ptr(model));

		//Chamadas de desenho da cena
		drawScene(VAO, texID);
		
		character.draw(walkRight, walkLeft, walkUp, walkDown);


		// Troca os buffers da tela
		glfwSwapBuffers(window);
	}
	// Pede pra OpenGL desalocar os buffers
	glDeleteVertexArrays(1, &VAO);
	// Finaliza a execução da GLFW, limpando os recursos alocados por ela
	glfwTerminate();
	return 0;
}

// Função de callback de teclado - É chamada sempre que uma tecla for pressionada
// ou solta via GLFW
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode)
{
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GL_TRUE);
	
	if (key == GLFW_KEY_D)
	{
		if (action == GLFW_PRESS)
		{
			walkRight = true;
		}
		else if (action == GLFW_RELEASE)
		{
			walkRight = false;
		}
	}
	
	if (key == GLFW_KEY_A)
	{
		if (action == GLFW_PRESS)
		{
			walkLeft = true;
		}
		else if (action == GLFW_RELEASE)
		{
			walkLeft = false;
		}
	}

	if (key == GLFW_KEY_W)
	{
		if (action == GLFW_PRESS)
		{
			walkUp = true;
		}
		else if (action == GLFW_RELEASE)
		{
			walkUp = false;
		}
	}

	if (key == GLFW_KEY_S)
	{
		if (action == GLFW_PRESS)
		{
			walkDown = true;
		}
		else if (action == GLFW_RELEASE)
		{
			walkDown = false;
		}
	}
}

int setupGeometry()
{
	// Aqui setamos as coordenadas x, y e z do triângulo e as armazenamos de forma
	// sequencial, já visando mandar para o VBO (Vertex Buffer Objects)
	// Cada atributo do vértice (coordenada, cores, coordenadas de textura, normal, etc)
	// Pode ser arazenado em um VBO único ou em VBOs separados
	GLfloat vertices[] = {
		//x     y     z    r    g    b    s    t
		//Triangulo 0
		-1.0 , -1.0, 0.0, 1.0, 0.0, 0.0, 0.0, 1.0,  //v0
		 1.0 , -1.0, 0.0, 0.0, 1.0, 0.0, 1.0, 1.0,  //v1
		-1.0 ,  1.0, 0.0, 0.0, 0.0, 1.0, 0.0, 0.0,   //v2
		 1.0 , -1.0, 0.0, 0.0, 1.0, 0.0, 1.0, 1.0,  //v1
		-1.0 ,  1.0, 0.0, 0.0, 0.0, 1.0, 0.0, 0.0,   //v2
		 1.0 ,  1.0, 0.0, 0.0, 0.0, 1.0, 1.0, 0.0,   //v2
	};

	GLuint VBO, VAO;
	//Geração do identificador do VBO
	glGenBuffers(1, &VBO);
	//Faz a conexão (vincula) do buffer como um buffer de array
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	//Envia os dados do array de floats para o buffer da OpenGl
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	//                                              vertices.data()  
	//Geração do identificador do VAO (Vertex Array Object)
	glGenVertexArrays(1, &VAO);
	// Vincula (bind) o VAO primeiro, e em seguida  conecta e seta o(s) buffer(s) de vértices
	// e os ponteiros para os atributos 
	glBindVertexArray(VAO);
	//Para cada atributo do vertice, criamos um "AttribPointer" (ponteiro para o atributo), indicando: 
	// Localização no shader * (a localização dos atributos devem ser correspondentes no layout especificado no vertex shader)
	// Numero de valores que o atributo tem (por ex, 3 coordenadas xyz) 
	// Tipo do dado
	// Se está normalizado (entre zero e um)
	// Tamanho em bytes 
	// Deslocamento a partir do byte zero 

	//Atributo 0 - posição
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);

	//Atributo 1 - cor
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);

	//Atributo 2 - coordenadas de textura
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(6 * sizeof(GLfloat)));
	glEnableVertexAttribArray(2);


	// Observe que isso é permitido, a chamada para glVertexAttribPointer registrou o VBO como o objeto de buffer de vértice 
	// atualmente vinculado - para que depois possamos desvincular com segurança
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	// Desvincula o VAO (é uma boa prática desvincular qualquer buffer ou array para evitar bugs medonhos)
	glBindVertexArray(0);

	return VAO;
}

void drawScene(int VAO, int texID)
{
	glBindTexture(GL_TEXTURE_2D, texID);
	glBindVertexArray(VAO); //Conectando ao buffer de geometria

	glDrawArrays(GL_TRIANGLES, 0, 6);

	glBindVertexArray(0); //Desconectando o buffer de geometria
}

GLuint loadTexture(string texturePath)
{
	GLuint texID;

	// Gera o identificador da textura na memória 
	glGenTextures(1, &texID);
	glBindTexture(GL_TEXTURE_2D, texID);

	//Configuração do parâmetro WRAPPING nas coords s e t
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	//Confirugação do parâmetro FILTERING na minificação e magnificação da textura
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	int width, height, nrChannels;
	unsigned char* data = stbi_load(texturePath.c_str(), &width, &height, &nrChannels, 0);

	if (data)
	{
		if (nrChannels == 3) //jpg, bmp
		{
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
		}
		else //png
		{
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
		}
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		std::cout << "Failed to load texture" << std::endl;
	}

	stbi_image_free(data);
	glBindTexture(GL_TEXTURE_2D, 0);

	return texID;
}

