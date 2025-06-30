#======================================================================================================================
target_precompile_headers(${PROJECT_NAME} PRIVATE private.hpp)
    target_link_libraries(${PROJECT_NAME} PRIVATE glfw glad stb glm graphics-module resources-module)
            target_sources(${PROJECT_NAME} PRIVATE main.cpp)
#======================================================================================================================
