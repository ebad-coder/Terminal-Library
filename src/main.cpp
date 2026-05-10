#include "../include/LibrarySystem.h"
#include <iostream>
#include <stdexcept>
#include <ctime>
#include <cstdlib>

int main()
{
    std::srand(std::time(0)); // seeding the random generator only once for generating unique id for new resources.

    try 
    {
        LibrarySystem application;
        application.run();
    }
    catch (const std::runtime_error& error) // .txt files failed to open
    {
        std::cerr << '\n' << error.what() << '\n';
        std::cerr << "The application terminated to prevent data loss.\n";
        
        return 1;
    }

    return 0;
}