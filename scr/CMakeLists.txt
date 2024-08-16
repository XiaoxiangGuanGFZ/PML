# CMakeLists.txt
cmake_minimum_required(VERSION 3.0)
project(PML)  # Set your project name here
enable_language(C)

# Add your source files here
set(SOURCE_FILES
    main.c
    Func_ConductAir.c
    Func_ConductCanopy.c
    Func_E_canopy.c
    Func_E_interception.c
    Func_E_soil.c
    Func_IO.c
    Func_radiation.c
    Func_PML.c
)


# Add the executable target
add_executable(PML ${SOURCE_FILES})
# Link against the math library
target_link_libraries(PML m)


## cmake -G "MinGW Makefiles" ..
## mingw32-make
