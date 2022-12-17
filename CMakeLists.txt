# CMakeList.txt: QFaceIdentify 的 CMake 项目，在此处包括源代码并定义
# 项目特定的逻辑。
#
cmake_minimum_required (VERSION 3.8)

project ("QFaceIdentify" VERSION 0.1 LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Core Quick)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Core Quick)
find_package(Qt5QuickControls2 REQUIRED)
find_package(OpenCV 4 REQUIRED)
find_package(jsoncpp CONFIG REQUIRED)

set(NCNN_INCLUDE_DIRS "F:/ncnn/ncnn-msvc/include")
set(NCNN_LIBS "F:/ncnn/ncnn-msvc/lib/ncnnd.lib")

include_directories(${OpenCV_INCLUDE_DIRS})
include_directories(${JSON_INC_PATH}})
include_directories(${NCNN_INCLUDE_DIRS})

file(GLOB SCREEN_FILES "src/screen/*")
configure_file("src/assets/classifier/haarcascade_frontalface_alt.xml" "assets/classifier/haarcascade_frontalface_alt.xml" COPYONLY)
configure_file("src/assets/classifier/haarcascade_eye.xml" "assets/classifier/haarcascade_eye.xml" COPYONLY)
configure_file("src/assets/classifier/haarcascade_mcs_mouth.xml" "assets/classifier/haarcascade_mcs_mouth.xml" COPYONLY)
configure_file("src/assets/classifier/haarcascade_mcs_nose.xml" "assets/classifier/haarcascade_mcs_nose.xml" COPYONLY)
configure_file("src/assets/classifier/haarcascade_mcs_leftear.xml" "assets/classifier/haarcascade_mcs_leftear.xml" COPYONLY)
configure_file("src/assets/classifier/haarcascade_mcs_rightear.xml" "assets/classifier/haarcascade_mcs_rightear.xml" COPYONLY)

#add_definitions(-DQT_NO_DEBUG_OUTPUT)

# 将源代码添加到此项目的可执行文件。
if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(QFaceIdentify
        MANUAL_FINALIZATION
        ${PROJECT_SOURCES}
    )
# Define target properties for Android with Qt 6 as:
#    set_property(TARGET QFaceIdentify APPEND PROPERTY QT_ANDROID_PACKAGE_SOURCE_DIR
#                 ${CMAKE_CURRENT_SOURCE_DIR}/android)
# For more information, see https://doc.qt.io/qt-6/qt-add-executable.html#target-creation
else()
    if(ANDROID)
        add_library(QFaceIdentify SHARED
            ${PROJECT_SOURCES}
        )
# Define properties for Android with Qt 5 after find_package() calls as:
#    set(ANDROID_PACKAGE_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/android")
    else()
	add_executable(
	    QFaceIdentify
	    src/main.cpp
	    ${SCREEN_FILES}
	    src/qml.qrc
        )
    endif()
endif()

target_link_libraries(QFaceIdentify
  PRIVATE Qt${QT_VERSION_MAJOR}::Core Qt${QT_VERSION_MAJOR}::Quick Qt${QT_VERSION_MAJOR}::QuickControls2)
link_directories(${OpenCV_LIBRARY_DIRS})
target_link_libraries(QFaceIdentify PRIVATE ${OpenCV_LIBS})
target_link_libraries(QFaceIdentify PRIVATE jsoncpp_lib)
target_link_libraries(QFaceIdentify PRIVATE ${NCNN_LIBS})

set_target_properties(QFaceIdentify PROPERTIES
    MACOSX_BUNDLE_GUI_IDENTIFIER my.example.com
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

install(TARGETS QFaceIdentify
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR})

if(QT_VERSION_MAJOR EQUAL 6)
    qt_import_qml_plugins(QFaceIdentify)
    qt_finalize_executable(QFaceIdentify)
endif()

# TODO: 如有需要，请添加测试并安装目标。