#include "Engine.h"
#include <thread>
#include <chrono>

#ifdef IMGUI
#include "src/imgui/imgui_impl_opengl3.h"
#include "src/imgui/imgui_impl_glfw.h"
#endif


void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}

void processInputCallback(GLFWwindow* window, int key, int scancode, int action, int mods) {
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        Engine::bufferKeyState->forward = true;
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_RELEASE)
        Engine::bufferKeyState->forward = false;

    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        Engine::bufferKeyState->backward = true;
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_RELEASE)
        Engine::bufferKeyState->backward = false;

    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        Engine::bufferKeyState->right = true;
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_RELEASE)
        Engine::bufferKeyState->right = false;

    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        Engine::bufferKeyState->left = true;
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_RELEASE)
        Engine::bufferKeyState->left = false;
}

GLFWwindow* Engine::initWindows(int width, int height)
{
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

    //Windows creation
    GLFWwindow* window = glfwCreateWindow(width, height, "Prender", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return nullptr;
    }
    glfwMakeContextCurrent(window);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return nullptr;
    }
    glViewport(0, 0, width, height);

    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetKeyCallback(window, processInputCallback);

    return window;
}

void processInput(GLFWwindow* window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}



void Engine::updateInput() {
    //store the current key state in the previous key state
    std::swap(currentKeyState, previousKeyState); 

    //update the current key state
    currentKeyState->forward = bufferKeyState->forward;
    currentKeyState->backward = bufferKeyState->backward;
    currentKeyState->left = bufferKeyState->left;
    currentKeyState->right = bufferKeyState->right;
    
    //update the toggle key state
    toggleKeyState->forward = currentKeyState->forward && !previousKeyState->forward;
    toggleKeyState->backward = currentKeyState->backward && !previousKeyState->backward;
    toggleKeyState->left = currentKeyState->left && !previousKeyState->left;
    toggleKeyState->right = currentKeyState->right && !previousKeyState->right;
}

keyStates* Engine::bufferKeyState = new keyStates();

Engine::Engine() : activeScene(0), start(), nextRender(), nextUpdate() {
    
    currentKeyState = new keyStates();
    previousKeyState = new keyStates();
    toggleKeyState = new keyStates();

    window = initWindows(800, 600);
    if (window == nullptr)
        exit(-1);
#ifdef IMGUI
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGui::StyleColorsDark();
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 130");
#endif

    fps = 60;
    tps = 250;

    glfwSwapInterval(0);//no vsync (not working)
}

Engine::~Engine() {
#ifdef IMGUI
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
#endif

    glfwDestroyWindow(window);

    delete(bufferKeyState);
    delete(currentKeyState);
    delete(previousKeyState);
    delete(toggleKeyState);

    glfwTerminate();
}

void Engine::update() {
    //std::cout << "update" << std::endl;

    glfwPollEvents();
    updateInput();

    processInput(window);
    scenes[activeScene]->update(this);
}

void Engine::render() {
    //std::cout << "render" << std::endl;

#ifdef IMGUI
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();
#endif

    glfwSetWindowTitle(window, "My Window");
    scenes[activeScene]->render();

#ifdef IMGUI
    //ImGui::ShowDemoWindow();
    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
#endif

    glfwSwapBuffers(window);
};

void Engine::run() {
    start = glfwGetTime();
    nextRender = start + 1 / fps;
    nextUpdate = start + 1 / tps;

    TimeData timeData = TimeData(start);

    while (!glfwWindowShouldClose(window))
    {
        start = glfwGetTime();
        if (start - timeData.lastSecond >= 1.0) {
            Time dt = start - timeData.lastSecond;
            timeData.fps = static_cast<int>(timeData.framesLastSecond / dt);
            timeData.tps = static_cast<int>(timeData.ticksLastSecond / dt);
            timeData.sleepedTimeLastSecond = timeData.sleepedTime / dt;
            timeData.framesLastSecond = 0;
            timeData.ticksLastSecond = 0;
            timeData.sleepedTime = 0;
            timeData.lastSecond = start;
            //std::cout << "FPS : " << timeData.fps << ", TPS : " << timeData.tps << ", sleeped seconds : " << timeData.sleepedTimeLastSecond << ", dt : " << dt << std::endl;
        }

        if (start >= nextUpdate) {
            update();
            nextUpdate += 1 / tps;
            timeData.ticksLastSecond++;
            //next update does not take into acount the start time. This ensure stability
            //WARNING The computer MUST be able to perform the updates quick enough, otherwise delay will accumulate and there will never be any render
        }
        else if (start >= nextRender) {
            render();
            nextRender = start + 1 / fps;
            timeData.framesLastSecond++;
            //next render take into acount the start time. This avoid accumulating delay 
        }
        else {
            Time wait = std::min(nextRender, nextUpdate) - start;
            //std::cout << wait << std::endl;
            timeData.sleepedTime += wait;
            std::this_thread::sleep_for(std::chrono::microseconds(static_cast<long long>(wait*1000000)));//wait is in second
        }
    }
}

void Engine::addScene(Scene* scene) {
    scenes.push_back(scene);
}

void Engine::setActiveScene(int sceneID) {
    activeScene = sceneID;
}

double Engine::get_fps() {
    return fps;
}

double Engine::get_tps() {
    return tps;
}