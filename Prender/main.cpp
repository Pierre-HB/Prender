#include "main.h"

#include "Engine.h"
#include "src/scenes/BasicScene.h"


#ifdef CONSOLE
int main()
#else
int WinMain()
#endif
{
    Engine engine = Engine();

    engine.addScene(new BasicScene());
    engine.setActiveScene(0); //optional, 0 is set by default

    engine.run();

    return 0;
}