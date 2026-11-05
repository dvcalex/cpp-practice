#include "myproject/Logger.hpp"

int main()
{
    // These loggers only log in debug mode
    DEBUG_LOG("hello from {}", "myproject");
    DEBUG_LOG("{ literal braces }");
    DEBUG_LOG("The number {{ {} }}", 42);

    // These loggers always log
    myproject::log::log("Application started!");
    myproject::log::warn("This is a warning and the number {}", 10);
    myproject::log::error("Could not open {}", "some.txt");
    return 0;
}
