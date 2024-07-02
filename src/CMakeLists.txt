cmake_minimum_required(VERSION 3.10)

# Установите имя проекта и его версию
project(ConsoleMediaPlayer VERSION 1.0)

# Установите стандарт C++
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Найдите необходимые пакеты
find_package(PkgConfig REQUIRED)
pkg_check_modules(LIBVLC REQUIRED libvlc)
find_package(X11 REQUIRED)
find_package(Curses REQUIRED)
find_package(Python3 REQUIRED COMPONENTS Interpreter Development)
# Включите заголовочные файлы
include_directories(${LIBVLC_INCLUDE_DIRS})
include_directories(${X11_INCLUDE_DIR})
include_directories(${CURSES_INCLUDE_DIR})
include_directories(${Python3_INCLUDE_DIRS})

# Установите исходные файлы проекта
set(SOURCES main.cpp control.cpp CLI_menu.cpp playback_time.cpp)

# Добавьте исполняемый файл
add_executable(ConsoleMediaPlayer ${SOURCES})

# Свяжите необходимые библиотеки
target_link_libraries(ConsoleMediaPlayer 
${LIBVLC_LIBRARIES} 
${X11_LIBRARIES} 
${CURSES_LIBRARIES}
${Python2_LIBRARIES}
-lpython${Python3_VERSION_MAJOR}.${Python3_VERSION_MINOR})
