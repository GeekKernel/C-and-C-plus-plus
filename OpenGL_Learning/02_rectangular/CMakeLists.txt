set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin)
set(LIBRARY_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/lib)

# =========== thirdparty library ===========
include_directories("${PROJECT_SOURCE_DIR}/thirdparty/GLAD/include/")
include_directories("${PROJECT_SOURCE_DIR}/thirdparty/GLFW/include/")

# cmake . -B build -G "MinGW Makefiles"
# cmake . -B build -G "Visual Studio 17 2022" -DIS_MinGW=OFF
# cmake --build build
option(IS_MinGW "Is using the MinGW Compiler on Windows" ON)
if(IS_MinGW)
    link_directories("${PROJECT_SOURCE_DIR}/thirdparty/GLFW/lib-mingw-w64/")
else()
    link_directories("${PROJECT_SOURCE_DIR}/thirdparty/GLFW/lib-vc2022/")
endif()

add_executable(Rectangular ${source_files})
target_sources(Rectangular
    PRIVATE
        "./rectangular.cpp"
        "${PROJECT_SOURCE_DIR}/thirdparty/GLAD/src/glad.c"
)
target_link_libraries(Rectangular
    glfw3.lib
    OpenGL32.Lib
)
