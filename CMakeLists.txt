# CMakeList.txt : CMake project for fbx_tracing_visualization, include source and define
# project specific logic here.
#
project(qtopengl)
cmake_minimum_required (VERSION 3.8)
set(CMAKE_CXX_STANDARD 17)

set(QT_PATH C:/Qt/5.12.10/msvc2017_64)
set(CMAKE_PREFIX_PATH ${QT_PATH})
set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOMOC ON)

if(CMAKE_VERSION VERSION_LESS "3.7.0")
    set(CMAKE_INCLUDE_CURRENT_DIR ON)
endif()

link_directories(${QT_PATH}/lib)
link_directories(${QT_PATH}/bin)

#add_custom_command(OUTPUT moc_mainwindow.cpp PRE_BUILD
#                  COMMAND C:/Qt/5.12.10/msvc2017_64/bin/moc.exe mainwindow.h -o moc_mainwindow.cpp
#                 WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
#)

set(HEADERS_RPGEDITOR 
    mainwindow.h
)

set(SOURCES_RPGEDITOR
    main.cpp
    mainwindow.cpp
)

set(MOC_RPGEDITOR
    moc_files/moc_mainwindow.cpp
)

add_executable (${PROJECT_NAME}
    ${MOC_RPGEDITOR}
    ${HEADERS_RPGEDITOR}
    ${SOURCES_RPGEDITOR}
)

target_link_libraries(${PROJECT_NAME}
  PUBLIC
  Qt5Cored
  Qt5Guid
  Qt5Widgetsd
  Qt5OpenGLd
  Qt5OpenGLExtensionsd
  libEGLd
  libGLESv2d
)

target_include_directories(${PROJECT_NAME}
    PUBLIC ${QT_PATH}/include
    PUBLIC ${QT_PATH}/include/QtCore
    PUBLIC ${QT_PATH}/include/QtWidgets
    PUBLIC ${QT_PATH}/include/QtGui
    PUBLIC ${QT_PATH}/include/QtOpenGL
    PUBLIC ${QT_PATH}/include/Qt5OpenGLExtensions
    PUBLIC ${QT_PATH}/include/QtANGLE/EGL
    PUBLIC ${QT_PATH}/include/QtANGLE/GLES2
    PUBLIC ${QT_PATH}/include/QtANGLE/GLES3
    PUBLIC ${QT_PATH}/include/QtANGLE/KHR
    PUBLIC ${QT_PATH}/include/QtCore/5.12.10/QtCore/private
    PUBLIC ${QT_PATH}/include/QtGui/5.12.10/QtGui/private
    PUBLIC ${QT_PATH}/include/QtWidgets/5.12.10/QtWidgets/private
    PUBLIC ${QT_PATH}/include/QtOpenGL/5.12.10/QtOpenGL/private
)

add_custom_command(
        TARGET ${PROJECT_NAME} PRE_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy
                ${QT_PATH}/bin/Qt5Cored.dll
                ${CMAKE_CURRENT_BINARY_DIR} )

add_custom_command(
        TARGET ${PROJECT_NAME} PRE_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy
                ${QT_PATH}/bin/Qt5Guid.dll
                ${CMAKE_CURRENT_BINARY_DIR} )

add_custom_command(
        TARGET ${PROJECT_NAME} PRE_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy
                ${QT_PATH}/bin/Qt5Widgetsd.dll
                ${CMAKE_CURRENT_BINARY_DIR} )

add_custom_command(
        TARGET ${PROJECT_NAME} PRE_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy
                ${QT_PATH}/bin/Qt5OpenGLd.dll
                ${CMAKE_CURRENT_BINARY_DIR} )

add_custom_command(
        TARGET ${PROJECT_NAME} PRE_BUILD
        COMMAND ${CMAKE_COMMAND} -E make_directory ${CMAKE_CURRENT_BINARY_DIR}/platforms )

 add_custom_command(
        TARGET ${PROJECT_NAME} PRE_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy
                ${QT_PATH}/plugins/platforms/qwindowsd.dll
                ${CMAKE_CURRENT_BINARY_DIR}/platforms )

 add_custom_command(
        TARGET ${PROJECT_NAME} PRE_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
                ${CMAKE_CURRENT_SOURCE_DIR}/shaders
                ${CMAKE_CURRENT_BINARY_DIR}/shaders )