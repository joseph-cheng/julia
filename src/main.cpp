#include "glad/glad.h"
#include "GLFW/glfw3.h"
#include <cstdio>
#include <cmath>
#include <iostream>
#include <string>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "shader.h"

unsigned int screen_width = 1920;
unsigned int screen_height = 1080;

float left_bound = -2.0 * (float)screen_width/(float)screen_height;
float right_bound = 2.0 * (float)screen_width/(float)screen_height;
float top_bound = 2.0;
float bottom_bound = -2.0;

float target_left_bound = left_bound;
float target_right_bound = right_bound;
float target_top_bound = top_bound;
float target_bottom_bound = bottom_bound;

bool paused = false;
bool rotating = false;

glm::vec3 hsl2rgb(glm::vec3 c) {
    glm::vec4 K = glm::vec4(1.0f, 2.0f/3.0f, 1.0f/3.0f, 3.0f);
    glm::vec3 p = glm::abs(glm::fract(glm::vec3(c.x+K.x, c.x+K.y, c.x+K.z)) * 6.0f - glm::vec3(K.w));
    return c.z * glm::mix(glm::vec3(K.x), glm::clamp(p-glm::vec3(K.x), 0.0f, 1.0f), c.y);
}

void process_input(GLFWwindow* window) {
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, true);
    }
}

void rotate_colours(glm::vec3 colours[], int length) {
    glm::vec3 temp = colours[0];
    int i;

    for (i = 0; i < length-1; i++) {
        colours[i] = colours[i+1];
    }
    colours[i] = temp;
}

void zoom(float target_x, float target_y, float zoom_factor) {
    float dx = target_x - left_bound;
    float dy = target_y - bottom_bound;
    target_left_bound = left_bound + dx - zoom_factor * dx;
    target_right_bound = left_bound + dx - zoom_factor * dx + zoom_factor * (right_bound-left_bound);

    target_bottom_bound = bottom_bound + dy - zoom_factor * dy;
    target_top_bound = bottom_bound + dy - zoom_factor * dy + zoom_factor * (top_bound-bottom_bound);

}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset) {

    double xpos, ypos;
    glfwGetCursorPos(window, &xpos, &ypos);
    xpos = (float)xpos * (right_bound-left_bound) / (float)screen_width + left_bound;
    ypos = ((float)screen_height - (float)ypos) * (top_bound - bottom_bound) / (float)screen_height + bottom_bound;

    if (yoffset == 1) {
        zoom(xpos, ypos, 0.70);
    }
    else if (yoffset == -1) {
        zoom(xpos, ypos, 1.30);
    }
    
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {
    if (key == GLFW_KEY_SPACE && action == GLFW_PRESS) {
        paused = not(paused);
    }
    if (key == GLFW_KEY_C && action == GLFW_PRESS) {
        rotating = not(rotating);
    }
}


int main () {
    float a;
    float b;
    int n;
    int itcap;
    std::cout << std::endl << "Oscillating Julia Set Renderer - z^n + c" << std::endl << "c = a + bi" << std::endl << "Enter a: ";
    std::cin >> a;
    std::cout << std::endl << "Enter b: ";
    std::cin >> b;
    std::cout << std::endl << "Enter n: ";
    std::cin >> n;
    std::cout << std::endl << "Enter iteration cap (caps greater than 300 may result in lag): ";
    std::cin >> itcap;

    glm::vec3 colours[itcap];
    for (int i=0; i < itcap; i++) {
        float fraction = (float)i/(float)itcap;
        colours[i] = hsl2rgb(glm::vec3(exp(fraction)-1.0f, 1.0f, fraction));
        //colours[i] = glm::vec3(fraction, 0.0f, 0.0f);
    }
    colours[itcap] = glm::vec3(0.0f, 0.0f, 0.0f);


    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_SAMPLES, 16);
    
    GLFWwindow* window = glfwCreateWindow(screen_width, screen_height, "julia", NULL, NULL);
    if (window == NULL) {
        std::cout << "Window creation failed" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);


    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cout << "Failed to initialise GLAD" << std::endl;
        return -1;
    }

    glViewport(0,0, screen_width, screen_height);
    glfwSetScrollCallback(window, scroll_callback);
    glfwSetKeyCallback(window, key_callback);
    glEnable(GL_MULTISAMPLE);

    Shader shader("vert.glsl", "frag.glsl", itcap);

    float vertices[] = {
        -1.0f, -1.0f, 0.0f,
        1.0f, -1.0f, 0.0f,
        -1.0f, 1.0f, 0.0f,

        1.0f, -1.0f, 0.0f,
        -1.0f, 1.0f, 0.0f,
        1.0f, 1.0f, 0.0f
    };

    unsigned int VBO;
    unsigned int VAO;
    glGenBuffers(1, &VBO);
    glGenVertexArrays(1, &VAO);

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3*sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);



    glm::vec2 julia_c = glm::vec2(a, b);
    float time = 0;
    float frame_time;
    float start_time;
    float end_time;
    while (!glfwWindowShouldClose(window)) {
        start_time = glfwGetTime();
        process_input(window);
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        shader.use();

        shader.setFloat("left_bound", left_bound);
        shader.setFloat("right_bound", right_bound);
        shader.setFloat("top_bound", top_bound);
        shader.setFloat("bottom_bound", bottom_bound);

        shader.setFloat("screen_width", screen_width);
        shader.setFloat("screen_height", screen_height);
        shader.setInt("itcap", itcap);
        glUniform3fv(glGetUniformLocation(shader.ID, "colours"), itcap, glm::value_ptr(colours[0]));
        shader.setInt("z_power", n);
        shader.setVec2("julia_c", julia_c);

        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLES, 0, 6);

        glfwSwapBuffers(window);
        glfwPollEvents();

        left_bound += (target_left_bound-left_bound)*0.1;
        right_bound += (target_right_bound-right_bound)*0.1;
        top_bound += (target_top_bound-top_bound)*0.1;
        bottom_bound += (target_bottom_bound-bottom_bound)*0.1;
        end_time = glfwGetTime();

        frame_time = end_time - start_time;

        if (not(paused)) {
            julia_c = glm::vec2(a+sin(time)*0.01, b+cos(time)*0.01);
            time += frame_time;
        }
        char title[100];
        std::sprintf(title, "Julia | FPS = %.0f | c = %f + %fi", 1.0/frame_time, julia_c.x, julia_c.y);
        glfwSetWindowTitle(window, title);
        if (rotating) {
            rotate_colours(colours, itcap);
        }


    }

    glfwTerminate();


}




