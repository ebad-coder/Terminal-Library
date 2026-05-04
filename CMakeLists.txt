cmake_minimum_required(VERSION 3.14)

# Name of your application
project(LibrarySystem VERSION 1.0)

# FTXUI strictly requires at least C++17
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# 1. Fetch FTXUI directly from GitHub
include(FetchContent)
FetchContent_Declare(
  ftxui
  GIT_REPOSITORY https://github.com/ArthurSonzogni/ftxui
  GIT_TAG        main # Or use a specific release tag like v5.0.0 for stability
)
FetchContent_MakeAvailable(ftxui)

# 2. Gather all your source files
# This grabs every .cpp file in your src/ directory
file(GLOB SOURCES "src/*.cpp")

# 3. Create the executable
add_executable(LibrarySystem ${SOURCES})

# 4. Tell CMake where to find your header files
target_include_directories(LibrarySystem PRIVATE include)

# 5. Link FTXUI's core modules to your executable
# - ftxui::screen: Manages terminal pixels and rendering
# - ftxui::dom: The declarative UI elements (vbox, hbox, text, border)
# - ftxui::component: The interactive stateful elements (Menu, Input, Button)
target_link_libraries(LibrarySystem
  PRIVATE
    ftxui::screen
    ftxui::dom
    ftxui::component
)