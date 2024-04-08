#include "main.h"

#include "Engine.h"
#include "src/scenes/BasicScene.h"

#ifdef DEBUG
int debug::NB_MAIN_INSTANCES = 0;
int debug::NB_INSTANCES = 0;
int debug::NB_ATTR = 0;
int debug::NB_OPENGL_PTR = 0;
#endif

#ifdef CONSOLE
int main()
#else
int WinMain()
#endif
{
    {
        Engine engine = Engine();

        engine.addScene(new BasicScene());
        engine.setActiveScene(0); //optional, 0 is set by default

        engine.run();
    }

#ifdef DEBUG
    std::cout << "alive object : " << debug::NB_MAIN_INSTANCES + debug::NB_INSTANCES + debug::NB_ATTR + debug::NB_OPENGL_PTR << std::endl;
    if (debug::NB_MAIN_INSTANCES != 0)
        std::cout << "[WARNING] There is still " << debug::NB_MAIN_INSTANCES << " major instances alive." << std::endl;
    if (debug::NB_INSTANCES != 0)
        std::cout << "[WARNING] There is still " << debug::NB_INSTANCES << " instances alive." << std::endl;
    if (debug::NB_ATTR != 0)
        std::cout << "[WARNING] There is still " << debug::NB_ATTR << " attributes alive." << std::endl;
    if (debug::NB_OPENGL_PTR != 0)
        std::cout << "[WARNING] There is still " << debug::NB_OPENGL_PTR << " opengl pointer alive." << std::endl;
#endif

#ifdef CONSOLE
    system("pause");
#endif

    return 0;
}