/* Hello Triangle - código adaptado de https://learnopengl.com/#!Getting-started/Hello-Triangle
 *
 * Adaptado por Rossana Baptista Queiroz
 * para a disciplina de Processamento Gráfico - Unisinos
 * Versão inicial: 7/4/2017
 * Última atualização em 14/08/2023
 *
 */

#include <iostream>
#include <string>
#include <assert.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <random>
#include <cmath>
#include <thread>
#include <chrono>

using namespace std;

//Classe para manipulação dos shaders
#include "Shader.h"

// Função para gerar uma cor aleatória evitando tons muito escuros
void initRandomColors(std::vector<glm::vec3>& colors) {
	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_real_distribution<float> dis(0.1f, 1.0f); // Valores maiores que 0.3 para evitar tons muito escuros

	for (int i = 0; i < colors.size(); ++i) {
		colors[i] = glm::vec3(dis(gen), dis(gen), dis(gen));
	}
}

// Protótipo da função de callback da GLFW
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode);
void mouse_button_callback(GLFWwindow* window, int button, int action, int mods);


// Protótipos das funções
int setup();
void pickColor(GLdouble xpos, GLdouble ypos, unsigned char pixel[4]);
void initRandomColors(std::vector<glm::vec3>& colors);
float colorDistance(unsigned char color1[4], glm::vec3 color2);
void gameManager(unsigned char pixel[4], std::vector<glm::vec3>& colors);

// Dimensões da janela (pode ser alterado em tempo de execução)
const GLuint WIDTH = 1024, HEIGHT = 768;
const int COLUMNS = 5, LINES = 13;
static std::vector<glm::vec3> colors(COLUMNS* LINES);


// Lógica para game
std::string player1Name;
std::string player2Name;

int jogadasRestantesPlayer1 = 3;
int jogadasRestantesPlayer2 = 3;

std::string playerAtual;

int pontuacaoPlayer1 = 0;
int pontuacaoPlayer2 = 0;

std::string sorteioInicial() {
	// Inicializa o gerador de números aleatórios
	srand(time(0));

	// Gera um número aleatório entre 1 e 2
	int resultado = rand() % 2 + 1;

	// Retorna o nome do jogador correspondente ao resultado
	if (resultado == 1) {
		playerAtual = player1Name;
		return player1Name;
	}
	else {
		playerAtual = player2Name;
		return player2Name;
	}
}

// Função MAIN
int main()
{
	// Inicialização da GLFW
	glfwInit();

	//Muita atenção aqui: alguns ambientes não aceitam essas configurações
	//Você deve adaptar para a versão do OpenGL suportada por sua placa
	//Sugestão: comente essas linhas de código para desobrir a versão e
	//depois atualize (por exemplo: 4.5 com 4 e 5)
	/*glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);*/

	//Essencial para computadores da Apple
//#ifdef __APPLE__
//	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
//#endif

	// Criação da janela GLFW
	GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "Jogo das cores", nullptr, nullptr);
	glfwSetWindowAttrib(window, GLFW_RESIZABLE, GLFW_FALSE);
	glfwMakeContextCurrent(window);

	// Fazendo o registro da função de callback para a janela GLFW
	glfwSetKeyCallback(window, key_callback);
	glfwSetMouseButtonCallback(window, mouse_button_callback);

	// GLAD: carrega todos os ponteiros d funções da OpenGL
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
	}

	// Obtendo as informações de versão
	const GLubyte* renderer = glGetString(GL_RENDERER);
	const GLubyte* version = glGetString(GL_VERSION);
	// cout << "Renderer: " << renderer << endl;
	// cout << "OpenGL version supported " << version << endl;
	cout << "Bem vindo ao Jogo das Cores" << endl;

	std::cout << "Digite o nome do Player 1: ";
	std::cin >> player1Name;

	std::cout << "Digite o nome do Player 2: ";
	std::cin >> player2Name;

	std::string jogadorInicial = sorteioInicial();

	std::cout << "Jogador que inicia: " << jogadorInicial << std::endl;


	// Definindo as dimensões da viewport com as mesmas dimensões da janela da aplicação
	int width, height;
	glfwGetFramebufferSize(window, &width, &height);
	glViewport(0, 0, width, height);


	// Compilando e buildando o shader
	Shader shader("../shaders/retangulo.vs", "../shaders/retangulo.fs");
	//Shader shaderPalete("../shaders/palete.vs", "../shaders/palete.fs");

	// Gerando um buffer simples
	GLuint VAO = setup();

	glm::mat4 model = glm::mat4(1);

	//std::vector<glm::vec3> colors(COLUMNS * LINES);
	initRandomColors(colors);

	bool endGame = true;

	// Loop da aplicação - "game loop"
	while (!glfwWindowShouldClose(window) && endGame)
	{

		if (jogadasRestantesPlayer1 == 0 && jogadasRestantesPlayer2 == 0) {
			cout << "\nPlayer vencedor: " << ((pontuacaoPlayer1 > pontuacaoPlayer2) ? player1Name : player2Name) << " com " << ((pontuacaoPlayer1 > pontuacaoPlayer2) ? pontuacaoPlayer1 : pontuacaoPlayer2) << " pontos" << endl;
			
			endGame = false;

			// Aguarda 5 segundos antes de fechar a aplicação
			std::this_thread::sleep_for(std::chrono::seconds(5));
		}

		// Checa se houveram eventos de input (key pressed, mouse moved etc.) e chama as funções de callback correspondentes
		glfwPollEvents();

		// Limpa o buffer de cor
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f); //cor de fundo
		glClear(GL_COLOR_BUFFER_BIT);

		glBindVertexArray(VAO); //Conectando ao buffer de geometria

		//glm::vec3 color(0.0f, 1.0f, 1.0f); // sugestão

		shader.Use();
		GLfloat xc = -0.77f,
			xl = 0.90f;
		int colorIndex = 0;
		for (int c = 0; c < COLUMNS; c++)
			for (int l = 0; l < LINES; l++) {

				shader.setVec3("cor", colors[colorIndex].r, colors[colorIndex].g, colors[colorIndex].b);
				colorIndex++;

				model = glm::mat4(1);

				model = glm::translate(model, glm::vec3(xc + c * 0.385, xl - l * 0.125, 0)); // sugestão
				model = glm::scale(model, glm::vec3(0.38, 0.38, 1)); // sugestão

				shader.setMat4("model", glm::value_ptr(model));

				glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
			}

		glBindVertexArray(0); //Desconectando o buffer de geometria

		// Troca os buffers da tela
		glfwSwapBuffers(window);
	}
	// Pede pra OpenGL desalocar os buffers
	glDeleteVertexArrays(1, &VAO);
	// Finaliza a execução da GLFW, limpando os recursos alocados por ela
	glfwTerminate();
	return 0;
}

// Função de callback de teclado
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode)
{
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GL_TRUE);
}

// Função de callback de clique do botão
void mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
{
	static unsigned char pixel[4];
	GLdouble xpos, ypos;
	int w, h;

	if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS) {
		glfwGetCursorPos(window, &xpos, &ypos);
		// cout << "\nX: " << xpos << " Y: " << ypos << endl; // posição do pixel
		glfwGetWindowSize(window, &w, &h);
		pickColor(xpos, h - ypos, pixel); // cor do pixel

		if (pixel[0] || pixel[1] || pixel[2]) {
			cout << "\nCor Escolhida: R: " << (int)pixel[0]  << " G: " << (int)pixel[1]  << " B: " << (int)pixel[2]  << endl;

			gameManager(pixel, colors);

		}
		else
			cout << "A cor preta nao pode ser selecionada, favor, " + playerAtual + ", escolha novamente!" << endl;
	}
}

// Esta função está bastante harcoded 
int setup()
{
	GLfloat vertices[] = {
		-0.5f, -0.15f, 0.0f, 0.0f, 0.0f, 1.0f,  // Vértice retângulo
		-0.5f, 0.15f, 0.0f, 0.0f, 0.0f, 1.0f,   // Vértice retângulo
		0.5f, -0.15f, 0.0f, 0.0f, 0.0f, 1.0f,   // Vértice retângulo
		0.5f, 0.15f, 0.0f, 0.0f, 0.0f, 1.0f,    // Vértice retângulo 

	};

	GLuint VBO, VAO;

	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	//Atributo 0 - posição
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);

	//Atributo 1 - cor
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);

	glBindBuffer(GL_ARRAY_BUFFER, 0);

	// Desvincula o VAO (é uma boa prática desvincular qualquer buffer ou array para evitar bugs medonhos)
	glBindVertexArray(0);

	return VAO;
}

void pickColor(GLdouble xpos, GLdouble ypos, unsigned char pixelOut[4]) {
	unsigned char pixel[4];
	glReadPixels(xpos, ypos, 1, 1, GL_RGBA, GL_UNSIGNED_BYTE, &pixel);
	if (pixel[0] || pixel[1] || pixel[2]) { // se a cor for diferente de preto
		for (int i = 0; i < 4; ++i) {
			pixelOut[i] = pixel[i];
		}
	}
}
//Calcula distância euclidiana entre os pontos
float colorDistance(unsigned char color1[4], glm::vec3 color2) {


	float dR = (color1[0] / 255.0f - color2[0]);
	float dG = (color1[1] / 255.0f - color2[1]);
	float dB = (color1[2] / 255.0f - color2[2]);

	float sumOfSquares = dR * dR + dG * dG + dB * dB;

	return sqrt(sumOfSquares);
}

void gameManager(unsigned char pixel[4], std::vector<glm::vec3>& colors) {
	int similarColors = 0;
	float distanceThereshold = 0.3;

	for (int i = 0; i < colors.size(); ++i) {
		float distance = colorDistance(pixel, colors[i]);
		if (distance < distanceThereshold) {
			similarColors++;
			colors[i] = glm::vec3(0, 0, 0);
		}
	}

	if (playerAtual == player1Name) {
		pontuacaoPlayer1 += similarColors;
		jogadasRestantesPlayer1--;
	}
	else {
		pontuacaoPlayer2 += similarColors;
		jogadasRestantesPlayer2--;
	} 

	std::cout << "Quantidade de cores similares escolhida por " << playerAtual + ": " + std::to_string(similarColors) << endl;
	std::cout << "Pontuacao atual de " + playerAtual + ": " + std::to_string((playerAtual == player1Name) ? pontuacaoPlayer1 : pontuacaoPlayer2) << endl;

	(playerAtual == player1Name) ? playerAtual = player2Name : playerAtual = player1Name;
}