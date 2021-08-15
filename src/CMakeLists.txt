project(${PROJECT_BASE_NAME}-src)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(SRC
        global.cpp
        Snake.cpp
        Game.cpp
        MainWindow.cpp
        )
set(HDR
        global.h
        Snake.h
        Game.h
        MainWindow.hpp
        )
add_library(${PROJECT_NAME}-lib STATIC ${SRC} ${HDR})
target_link_libraries(${PROJECT_NAME}-lib ${REQUIRED_LIBS_QUALIFIED})

add_executable(${PROJECT_NAME}
        main.cpp
        resources.qrc
        )
target_link_libraries(${PROJECT_NAME} ${REQUIRED_LIBS_QUALIFIED} ${PROJECT_NAME}-lib)