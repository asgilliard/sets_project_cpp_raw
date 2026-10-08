# Минимальная версия CMake.
# Если CMake старше 3.25, сборка остановится с понятной ошибкой.
cmake_minimum_required(VERSION 3.25)

# Имя проекта, его версия и используемый язык.
# CMake создает служебные переменные с этим именем.
project(sets_project_raw VERSION 0.1.0 LANGUAGES CXX)

# Стандарт C++11 для всех целей (targets) в этом файле.
set(CMAKE_CXX_STANDARD 11)

# C++11 строго обязательный.
# Если он недоступен в компиляторе, то сборка не пойдёт.
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Отключаем нестандартные расширения компиляторов.
# Это гарантирует, что код - чистый C++ и соберется на MSVC.
set(CMAKE_CXX_EXTENSIONS OFF)

# Дебаг-сборка с санитайзерами
# Для выполнения:
# cmake -S . -B build-asan -DCMAKE_BUILD_TYPE=Debug -DENABLE_SANITIZERS=ON
# cmake --build build-asan
# ./build-asan/sets_cli
option(ENABLE_SANITIZERS "Build with ASan/UBSan" OFF)
if(ENABLE_SANITIZERS AND NOT MSVC)
    add_compile_options(-fsanitize=address,undefined -fno-omit-frame-pointer -g)
    add_link_options(-fsanitize=address,undefined)
endif()

# Добавляем статическую библиотеку (.a Mac/Linux, .lib Windows)
# с именем sets_logic.
# В нее компилируется исходный код нашей логики,
# а затем она вшивается в итоговый бинарник.
# Другой вариант - SHARED, такая библиотека компилируется
# отдельным файлом (.dylib Mac, .so Linux, .dll Windows)
add_library(sets_logic STATIC
    src/bitset.cpp
)

# Указываем, где искать заголовочные файлы (.hpp) для библиотеки sets_logic.
# PUBLIC означает: папка include нужна и для самого bitset.cpp,
# и для всех, кто будет подключать эту библиотеку (например, sets_cli).
target_include_directories(sets_logic PUBLIC
    include
)

# Объявляем исполняемый файл (бинарник) с именем sets_cli.
# Точка входа main() находится в src/main.cpp.
add_executable(sets_cli
    src/main.cpp
)

# Линкуем исполняемый файл sets_cli с нашей библиотекой sets_logic.
# PRIVATE означает: зависимость нужна только самому sets_cli
# и никому выше по цепочке передаваться не должна.
target_link_libraries(sets_cli PRIVATE
    sets_logic
)

# Compiler-специфичные флаги компиляции
if(MSVC)
    target_compile_options(sets_cli PRIVATE /W4)
else()
    target_compile_options(sets_cli PRIVATE -Wall -Wextra -Wpedantic )
endif()
