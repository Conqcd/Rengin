#include "application.hpp"
#include "Event/ApplicationEvent.hpp"
#include "log.hpp"
namespace Rengin
{

void Application::Run()
{
    WindowResizeEvent e(1200,900);
    RE_CORE_TRACE(e);
    

    while(true);
}

}