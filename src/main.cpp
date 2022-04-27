#include "Application.h"

int main()
{
    ApplicationSettings settings;

    // settings.LoadConfig("config.yaml");

    Application app;

    if (app.Init(settings) != -1)
    {
        app.Run();
    }
    
    return 0;
}
