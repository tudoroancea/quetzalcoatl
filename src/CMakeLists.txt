project(${PROJECT_NAME}-src)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

add_executable(${PROJECT_NAME}
        main.cpp
        global.h
        global.cpp
        Snake.cpp
        Snake.h
        Game.cpp
        Game.h
        MainWindow.cpp
        MainWindow.hpp
        resources.qrc
        )
target_link_libraries(${PROJECT_NAME} ${REQUIRED_LIBS_QUALIFIED})