#include "Engine.h"
#include <thread>
#include <chrono>
#include "../../main.h"

int Engine::windowWidth = 800;
int Engine::windowHeight = 600;

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
    Engine::windowWidth = width;
    Engine::windowHeight = height;
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
    windowWidth = width;
    windowHeight = height;
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

    //Windows creation
    GLFWwindow* window = glfwCreateWindow(width, height, "Prender", NULL, NULL);
    if (window == NULL)
    {
#ifdef CONSOLE
        std::cout << "Failed to create GLFW window" << std::endl;
#endif
        glfwTerminate();
        return nullptr;
    }
    glfwMakeContextCurrent(window);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
#ifdef CONSOLE
        std::cout << "Failed to initialize GLAD" << std::endl;
#endif
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
#ifdef DEBUG
    debug::NB_OPENGL_PTR++;
#endif
#ifdef IMGUI
    ImGuiManager::initializeContext(window);
    imGuiManager = new ImGuiManager();
#endif

    fps = 60;
    tps = 250;

    maxMissedFrames = static_cast<int>(0.1*fps);
    maxMissedTicks = static_cast<int>(2*tps);

    glfwSwapInterval(0);//no vsync (not working)
#ifdef DEBUG
    debug::NB_MAIN_INSTANCES++;
#endif
}

Engine::~Engine() {
    for (size_t i = 0; i < scenes.size(); i++)
        delete scenes[i];
#ifdef IMGUI
    ImGuiManager::destroyContext();
    delete imGuiManager;
#endif

    glfwDestroyWindow(window);
#ifdef DEBUG
    debug::NB_OPENGL_PTR--;
#endif

    delete(bufferKeyState);
    delete(currentKeyState);
    delete(previousKeyState);
    delete(toggleKeyState);

    glfwTerminate();
#ifdef DEBUG
    debug::NB_MAIN_INSTANCES--;
#endif
}

void Engine::update() {
#ifdef IMGUI
    imGuiManager->setAttributes();
#endif

    glfwPollEvents();
    updateInput();

    processInput(window);
    scenes[activeScene]->update(this);

#ifdef IMGUI
    imGuiManager->updateAttributes();
#endif
}

void Engine::render() {

#ifdef IMGUI
    ImGuiManager::startFrame();
    ImGui::ShowDemoWindow();
    imGuiManager->beginConsole();

    ImGui::Text("FPS : %d, TPS : %d", timeData.fps, timeData.tps);
    ImGui::Text("Time (ms) | idle : %3.0lf, render : %3.0lf, update : %3.0lf", timeData.sleepedTimeLastSecond * 1000, timeData.renderTimeLastSecond * 1000, timeData.updateTimeLastSecond * 1000);
    ImGui::Text("Time per Tick : %3.2lfms, time per Frame : %3.2lfms", timeData.updateTimeLastSecond * 1000 / static_cast<double>(timeData.tps), timeData.renderTimeLastSecond * 1000 / static_cast<double>(timeData.fps));
    
    imGuiManager->renderInstancesTree();
#endif

    glfwSetWindowTitle(window, "My Window");
    scenes[activeScene]->render();

#ifdef IMGUI
    
    imGuiManager->endConsole();
    imGuiManager->renderInstances();

    ImGuiManager::endFrame();
#endif

    glfwSwapBuffers(window);
};

void Engine::run() {
    start = glfwGetTime();
    nextRender = start + 1 / fps;
    nextUpdate = start + 1 / tps;

    timeData = TimeData(start);

    while (!glfwWindowShouldClose(window))
    {
        start = glfwGetTime();
        if (start - timeData.lastSecond >= 1.0) {
            Time dt = start - timeData.lastSecond;
            timeData.fps = static_cast<int>(timeData.framesLastSecond / dt);
            timeData.tps = static_cast<int>(timeData.ticksLastSecond / dt);
            timeData.sleepedTimeLastSecond = timeData.sleepedTime / dt;
            timeData.renderTimeLastSecond = timeData.renderTime / dt;
            timeData.updateTimeLastSecond = timeData.updateTime / dt;
            timeData.framesLastSecond = 0;
            timeData.ticksLastSecond = 0;
            timeData.sleepedTime = 0;
            timeData.renderTime = 0;
            timeData.updateTime = 0;
            timeData.lastSecond = start;
        }

        if (timeData.lastLoopAction == TimeData::UPDATE)
            timeData.updateTime += start - timeData.lastStart;
        else if (timeData.lastLoopAction == TimeData::RENDER)
            timeData.renderTime += start - timeData.lastStart;
        else if (timeData.lastLoopAction == TimeData::IDLE)
            timeData.sleepedTime += start - timeData.lastStart;

        if (start >= nextUpdate) {
            update();
            nextUpdate = std::max(nextUpdate, start-maxMissedTicks/tps) + 1 / tps;
            timeData.ticksLastSecond++;
            //timeData.updateTime += glfwGetTime() - start;
            timeData.lastLoopAction = TimeData::RENDER;
            //next update does not take into acount the start time. This ensure stability
            //WARNING The computer MUST be able to perform the updates quick enough, otherwise delay will accumulate and there will never be any render
        }
        else if (start >= nextRender) {
            render();
            nextRender = std::max(nextRender, start-maxMissedFrames/fps) + 1 / fps;
            timeData.framesLastSecond++;
            //timeData.renderTime += glfwGetTime() - start;
            timeData.lastLoopAction = TimeData::UPDATE;
            //next render take into acount the start time. This avoid accumulating delay 
        }
        else {
            Time wait = std::min(nextRender, nextUpdate) - start;
            std::this_thread::sleep_for(std::chrono::microseconds(static_cast<long long>(wait*1000000)));//wait is in second
            timeData.lastLoopAction = TimeData::IDLE;
        }
        timeData.lastStart = start;
    }
}

void Engine::addScene(Scene* scene) {
    scenes.push_back(scene);
}

void Engine::setActiveScene(int sceneID) {
    activeScene = sceneID;
}

double Engine::get_fps() const {
    return fps;
}

double Engine::get_tps()const  {
    return tps;
}