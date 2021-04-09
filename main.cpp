#include "common.h"
#include "Image.h"
#include "Player.h"
#include "Field.h"

#define GLFW_DLL
#include <GLFW/glfw3.h>
#include <iostream>

constexpr GLsizei WINDOW_WIDTH = 1600, WINDOW_HEIGHT = 1024;

struct InputState
{
  bool keys[1024]{}; //массив состояний кнопок - нажата/не нажата
  GLfloat lastX = 400, lastY = 300; //исходное положение мыши
  bool firstMouse = true;
  bool captureMouse         = true;  // Мышка захвачена нашим приложением или нет?
  bool capturedMouseJustNow = false;
} static Input;


GLfloat deltaTime = 0.0f;
GLfloat lastFrame = 0.0f;

void OnKeyboardPressed(GLFWwindow* window, int key, int scancode, int action, int mode)
{
	switch (key)
	{
	case GLFW_KEY_ESCAPE:
		if (action == GLFW_PRESS)
			glfwSetWindowShouldClose(window, GL_TRUE);
		break;
  case GLFW_KEY_1:
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
    break;
  case GLFW_KEY_2:
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    break;
	default:
		if (action == GLFW_PRESS)
      Input.keys[key] = true;
		else if (action == GLFW_RELEASE)
      Input.keys[key] = false;
	}
}

int processPlayerMovement(Player &player, Field &field) {
    if (Input.keys[GLFW_KEY_W]) {
        player.ProcessInput(MovementDir::UP, field);
        return 1;
    } else if (Input.keys[GLFW_KEY_S]) {
        player.ProcessInput(MovementDir::DOWN, field);
        return 2;
    } else if (Input.keys[GLFW_KEY_A]) {
        player.ProcessInput(MovementDir::LEFT, field);
        return 3;
    } else if (Input.keys[GLFW_KEY_D]) {
        player.ProcessInput(MovementDir::RIGHT, field);
        return 4;
    }
    return 0;
}

void processPlayerMovement_step(Player &player) {
    if (Input.keys[GLFW_KEY_W]) {
        player.ProcessInput_step(MovementDir::UP); 
    } else if (Input.keys[GLFW_KEY_S]) {
        player.ProcessInput_step(MovementDir::DOWN); 
    } else if (Input.keys[GLFW_KEY_A]) {
        player.ProcessInput_step(MovementDir::LEFT); 
    } else if (Input.keys[GLFW_KEY_D]) {
        player.ProcessInput_step(MovementDir::RIGHT); 
    } 
}

void OnMouseButtonClicked(GLFWwindow* window, int button, int action, int mods)
{
  if (button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_RELEASE)
    Input.captureMouse = !Input.captureMouse;

  if (Input.captureMouse)
  {
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    Input.capturedMouseJustNow = true;
  }
  else
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);

}

void OnMouseMove(GLFWwindow* window, double xpos, double ypos)
{
  if (Input.firstMouse)
  {
    Input.lastX = float(xpos);
    Input.lastY = float(ypos);
    Input.firstMouse = false;
  }

  GLfloat xoffset = float(xpos) - Input.lastX;
  GLfloat yoffset = Input.lastY - float(ypos);

  Input.lastX = float(xpos);
  Input.lastY = float(ypos);
}


void OnMouseScroll(GLFWwindow* window, double xoffset, double yoffset)
{
  // ...
}

int initGL()
{
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize OpenGL context" << std::endl;
		return -1;
	}

	std::cout << "Vendor: " << glGetString(GL_VENDOR) << std::endl;
	std::cout << "Renderer: " << glGetString(GL_RENDERER) << std::endl;
	std::cout << "Version: " << glGetString(GL_VERSION) << std::endl;
	std::cout << "GLSL: " << glGetString(GL_SHADING_LANGUAGE_VERSION) << std::endl;

  std::cout << "Controls: "<< std::endl;
  std::cout << "press right mouse button to capture/release mouse cursor  "<< std::endl;
  std::cout << "W, A, S, D - movement  "<< std::endl;
  std::cout << "press ESC to exit" << std::endl;

	return 0;
}

void fading(Image &screenBuffer, GLFWwindow*  window) {
    double p1 = 1;
    for(int i = 0; i < 20; i++) {
        for(int y = 0; y < WINDOW_HEIGHT; ++y) {
            for(int x = 0; x < WINDOW_WIDTH; ++x) {
                Pixel pix = screenBuffer.GetPixel(x, y);
                pix.r *= p1;
                pix.g *= p1;
                pix.b *= p1;
                pix.a *= p1;
                screenBuffer.PutPixel(x, y, pix);
            }
        }
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); GL_CHECK_ERRORS; 
        glDrawPixels (WINDOW_WIDTH, WINDOW_HEIGHT, GL_RGBA, GL_UNSIGNED_BYTE, screenBuffer.Data()); GL_CHECK_ERRORS;
        glfwSwapBuffers(window); 
        p1 -= 0.05;
    }
}

int main(int argc, char** argv)
{
	if(!glfwInit())
    return -1;
 
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

    GLFWwindow*  window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "Reginushka's project", nullptr, nullptr);
	if (window == nullptr) {
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	
	glfwMakeContextCurrent(window); 

	glfwSetKeyCallback        (window, OnKeyboardPressed);  
	glfwSetCursorPosCallback  (window, OnMouseMove); 
    glfwSetMouseButtonCallback(window, OnMouseButtonClicked);
	glfwSetScrollCallback     (window, OnMouseScroll);

	if(initGL() != 0) 
		return -1;
	
  //Reset any OpenGL errors which could be present for some reason
	GLenum gl_error = glGetError();
	while (gl_error != GL_NO_ERROR)
		gl_error = glGetError();
	 
    Field field[2]; 
    int i_f = 0;
    field[i_f].coords.x = 0;
    field[i_f].coords.y = 0;
	Image screen(WINDOW_WIDTH, WINDOW_HEIGHT, 4);  
    string p[4];
    p[0] = "./field/test.txt";
    p[1] = "./field/test_state.txt";
    p[2] = "./field/level1.txt";
    p[3] = "./field/level1_state.txt";
    string path;
    field[i_f].InitField(p[0], p[1]); 
    field[i_f].DrawBackground(screen);
    Point starting_pos = field[i_f].DrawResources(screen); 
    Player player{starting_pos};
    Image screenBuffer(screen); 
    Image screenAnimations(screen);
    glViewport(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);  GL_CHECK_ERRORS;
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f); GL_CHECK_ERRORS;
    

  //game loop
    int i_up = 0, i_down = 0, i_left = 0, i_right = 0, i_last = 0;
    int k = 0, k_last = 2;
    unsigned i_st = 0; 
    int fl = 0;
	while (!glfwWindowShouldClose(window)) { 
		GLfloat currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;
        glfwPollEvents();
        
        if(fl == 3) {
            for(int i = 0; i < field[i_f].static_animations.size(); i++) {
                field[i_f].DrawStaticAnimations(screen, screenAnimations, field[i_f].static_animations[i].first, i_st % field[i_f].static_animations[i].second.second+ field[i_f].static_animations[i].second.first);
                field[i_f].DrawStaticAnimations(screen, screenBuffer, field[i_f].static_animations[i].first, i_st % field[i_f].static_animations[i].second.second+ field[i_f].static_animations[i].second.first);
            }
            __builtin_uadd_overflow(i_st, 1, &i_st); 
        } 
        k = processPlayerMovement(player, field[i_f]); 
        
        if(field[i_f].win == true){
            fading(screenBuffer, window);
            path = "./resources/Letter/YouWon.png";
            field[i_f].DrawGO(screenBuffer, path); 
            for(int i = 0; i < 50; i++) { 
                glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); GL_CHECK_ERRORS;  
                glDrawPixels (WINDOW_WIDTH, WINDOW_HEIGHT, GL_RGBA, GL_UNSIGNED_BYTE, screenBuffer.Data()); GL_CHECK_ERRORS;
                glfwSwapBuffers(window); 
            }
            break;
        }
        
        if(field[i_f].level) { 
            for(int i = 0; i < 8; i++) {
                processPlayerMovement_step(player);
                player.Draw(screenAnimations, screenBuffer, k_last, i_last);
                glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); GL_CHECK_ERRORS; 
                glDrawPixels (WINDOW_WIDTH, WINDOW_HEIGHT, GL_RGBA, GL_UNSIGNED_BYTE, screenBuffer.Data()); GL_CHECK_ERRORS;
                glfwSwapBuffers(window); 
            }
            player.Draw(screenAnimations, screenBuffer, k_last, i_last);
            fading(screenBuffer, window);
            path = "./resources/Letter/NextLevel.png";
            field[i_f].DrawGO(screenBuffer, path); 
            for(int i = 0; i < 50; i++) { 
                glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); GL_CHECK_ERRORS;  
                glDrawPixels (WINDOW_WIDTH, WINDOW_HEIGHT, GL_RGBA, GL_UNSIGNED_BYTE, screenBuffer.Data()); GL_CHECK_ERRORS;
                glfwSwapBuffers(window); 
            }
            i_f++;
            field[i_f].InitField(p[2], p[3]); 
            field[i_f].level = true;
            field[i_f].DrawBackground(screen);
            field[i_f].level = false;
            Point starting_pos_1 = field[i_f].DrawResources(screen); 
            for(int i = 0; i < field[i_f].light.size(); i++) {
                field[i_f].DrawLight(screen, field[i_f].light[i]); 
            }
            screenBuffer = screen;
            screenAnimations = screen;
            player.coords.x = starting_pos_1.x;
            player.coords.y = starting_pos_1.y; 
        }
        if(field[i_f].die) {
            for(int i = 0; i < 30; i++) {
                player.Draw(screenAnimations, screenBuffer, 5, i);
                glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); GL_CHECK_ERRORS; 
                glDrawPixels (WINDOW_WIDTH, WINDOW_HEIGHT, GL_RGBA, GL_UNSIGNED_BYTE, screenBuffer.Data()); GL_CHECK_ERRORS;
                glfwSwapBuffers(window); 
            } 
            path = "./resources/Letter/GameOver.png";
            field[i_f].DrawGO(screenBuffer, path); 
            for(int i = 0; i < 50; i++) { 
                glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); GL_CHECK_ERRORS;  
                glDrawPixels (WINDOW_WIDTH, WINDOW_HEIGHT, GL_RGBA, GL_UNSIGNED_BYTE, screenBuffer.Data()); GL_CHECK_ERRORS;
                glfwSwapBuffers(window); 
            }
            break;
        }
        
        if(k == 0) { 
            player.Draw(screenAnimations, screenBuffer, k_last, i_last);
        } else if(k == 1) {
            player.Draw(screenAnimations, screenBuffer, k, i_up); 
            i_up = (i_up + 1) % 15;
        } else if(k == 2) {
            player.Draw(screenAnimations, screenBuffer, k, i_down);
            i_last = i_down; 
            i_down = (i_down + 1) % 15; 
        } else if(k == 3) {
            player.Draw(screenAnimations, screenBuffer, k, i_left); 
            i_last = i_left;
            i_left = (i_left + 1) % 15; 
        } else if(k == 4) {
            player.Draw(screenAnimations, screenBuffer, k, i_right);
            i_last = i_right;
            i_right = (i_right + 1) % 15; 
        }
        
        if(field[i_f].floor == true) { 
            for(int y = 0; y < tileSize; ++y) {
                    for(int x = 0; x < tileSize; ++x) {
                        Pixel pix = field[i_f].resources[field[i_f].floors_w]->GetPixel(x, tileSize - y - 1);
                        screenBuffer.PutPixel(x + field[i_f].floors.x, y + field[i_f].floors.y, blend(screenBuffer.GetPixel(x + field[i_f].floors.x, y + field[i_f].floors.y), pix)); 
                        screenAnimations.PutPixel(x + field[i_f].floors.x, y + field[i_f].floors.y, blend(screen.GetPixel(x + field[i_f].floors.x, y + field[i_f].floors.y), pix));
                    }
            }
            field[i_f].floor = false;
        }
        
        k_last = k;  
        fl = (fl + 1) % 4;
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); GL_CHECK_ERRORS;  
        glDrawPixels (WINDOW_WIDTH, WINDOW_HEIGHT, GL_RGBA, GL_UNSIGNED_BYTE, screenBuffer.Data()); GL_CHECK_ERRORS;
		glfwSwapBuffers(window); 
	}

	glfwTerminate();
	return 0;
}
