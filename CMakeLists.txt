cmake_minimum_required(VERSION 3.26)
project(myproject VERSION 0.1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

option(MYPROJECT_ENABLE_LOGGING "Keep diagnostic logging enabled in Release builds" OFF)

# compile_commands.json which clangd reads (see .clangd)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin")

# Applied to our targets only, not dependencies.
if(MSVC)
    set(PROJECT_WARNINGS /W4 /permissive-)
else()
    set(PROJECT_WARNINGS -Wall -Wextra -Wpedantic)
endif()

# Everything except main() lives in the library so tests and extra executables can link against it.
add_library(${PROJECT_NAME}_lib STATIC
    src/Logger.cpp
)
target_include_directories(${PROJECT_NAME}_lib PUBLIC include)
target_compile_options(${PROJECT_NAME}_lib PRIVATE ${PROJECT_WARNINGS})
target_compile_definitions(${PROJECT_NAME}_lib PUBLIC
    $<$<BOOL:${MYPROJECT_ENABLE_LOGGING}>:MYPROJECT_ENABLE_LOGGING>
)

add_executable(${PROJECT_NAME} src/main.cpp)
target_link_libraries(${PROJECT_NAME} PRIVATE ${PROJECT_NAME}_lib)
target_compile_options(${PROJECT_NAME} PRIVATE ${PROJECT_WARNINGS})

# Keep the executable and its resources together, including with multi-config generators.
set_target_properties(${PROJECT_NAME} PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/$<0:>"
)

# Run on every build so resource-only changes do not require relinking.
if(IS_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/res")
    add_custom_target(${PROJECT_NAME}_resources
        COMMAND ${CMAKE_COMMAND} -E copy_directory_if_different
                "${CMAKE_CURRENT_SOURCE_DIR}/res"
                "$<TARGET_FILE_DIR:${PROJECT_NAME}>/res"
        COMMENT "Updating resources for ${PROJECT_NAME}"
        VERBATIM
    )
    add_dependencies(${PROJECT_NAME} ${PROJECT_NAME}_resources)
endif()

# Vendored dependencies (configure their options before add_subdirectory):
# add_subdirectory(vendor/foo EXCLUDE_FROM_ALL)
# target_link_libraries(${PROJECT_NAME}_lib PUBLIC foo::foo)
#
# For header-only dependencies, mark their includes as SYSTEM to suppress vendor warnings:
# target_include_directories(${PROJECT_NAME}_lib SYSTEM PUBLIC vendor)
