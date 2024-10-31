set -e

cd /cygdrive/d/CLionProjects/word_book/cmake-build-debug
/cygdrive/c/Users/Asaka/AppData/Local/JetBrains/CLion2023.2/cygwin_cmake/bin/cmake.exe --regenerate-during-build -S$(CMAKE_SOURCE_DIR) -B$(CMAKE_BINARY_DIR)
