cmake_minimum_required (VERSION 3.8)
project (BMPEditor)
find_package(Qt5 COMPONENTS Widgets REQUIRED)
add_executable(BMPEditor main.cpp editorMainWindow.cpp bmp.cpp imageLabel.cpp histogramPanel.cpp)
target_link_libraries(BMPEditor
Qt5::Widgets
Qt5::Gui
Qt5::QWindowsIntegrationPlugin)

