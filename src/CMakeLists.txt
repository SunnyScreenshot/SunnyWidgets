cmake_minimum_required(VERSION 3.14)

set(project_name sunny-widgets)
set(project_version_major 2)
set(project_version_minor 0)
set(project_version_path 0)
set(project_version ${project_version_major}.${project_version_minor}.${project_version_path})
project(${project_name} VERSION ${project_version} LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(BUILD_SHARED_LIBS ON)                     # ON-SHARED 生成 dll; OFF-STATIC 生成lib

set(qt_moduls Widgets Xml Svg Network)    # Network Multimedia WebSockets Xml Svg Network Charts 是区分大小写
set(qt_version_moduls)
foreach(it ${qt_moduls})
    list(APPEND qt_version_moduls Qt${QT_VERSION_MAJOR}::${it})
endforeach()

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS ${qt_moduls} LinguistTools)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS ${qt_moduls} LinguistTools) # LinguistTools No link


set(src_files
    # include/xtoolbutton.h
    # include/xcolorbutton.h
    # include/colorpicker.h
    # include/licenseui.h
    # include/horspacerline.h
    # include/verspacerline.h
    # include/groupdlg.h
    # include/verlinewithspace.h
    # include/xtextedit.h
    # include/imageview.h
    include/shortcutlineedit.h
    include/switchbutton.h
    # include/aboutinfo.h

    # sources/xtoolbutton.cpp
    # sources/xcolorbutton.cpp
    # sources/colorpicker.cpp
    # sources/licenseui.cpp
    # sources/licenseui.ui
    # sources/horspacerline.cpp
    # sources/verspacerline.cpp
    # sources/groupdlg.cpp
    # sources/verlinewithspace.cpp
    # sources/xtextedit.cpp
    # sources/imageview.cpp
    sources/shortcutlineedit.cpp
    sources/switchbutton.cpp
    # sources/aboutinfo.cpp
    # sources/aboutinfo.ui
)

if(APPLE)
elseif(WIN32)
    # list(APPEND src_files)
else()
    # list(APPEND src_files)
endif()

set(src_root_path ${CMAKE_CURRENT_SOURCE_DIR})                                              # default root path curr path （CMakeList.txt）
file(GLOB_RECURSE source_lists LIST_DIRECTORIES false
    "${src_root_path}/*.cpp"
    "${src_root_path}/*.h")
source_group(TREE ${src_root_path} FILES ${source_lists})

add_library(${project_name} ${src_files})

target_include_directories(${project_name} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include)     # .cpp 文件可以直接使用 include "文件名.h"，去掉相对路径
target_compile_definitions(${project_name} PRIVATE WINDOWSRECT_LIBRARY UNICODE _UNICODE )

target_link_libraries(${project_name} ${qt_version_moduls})

# if(WIN32 AND CMAKE_CXX_COMPILER_ID STREQUAL "GNU")  # 在这里添加针对 Windows 并使用 MinGW 的配置
#     target_link_libraries(${project_name} psapi uiautomationcore oleacc)
# endif()

if(MSVC)
    set_target_properties(${project_name} PROPERTIES RUNTIME_OUTPUT_DIRECTORY_DEBUG "${trage_runtime_output_dir}")
    set_target_properties(${project_name} PROPERTIES RUNTIME_OUTPUT_DIRECTORY_RELEASE "${trage_runtime_output_dir}")
    set_target_properties(${project_name} PROPERTIES RUNTIME_OUTPUT_DIRECTORY_MINSIZEREL "${trage_runtime_output_dir}")
    set_target_properties(${project_name} PROPERTIES RUNTIME_OUTPUT_DIRECTORY_RELWITHDEBINFO "${trage_runtime_output_dir}")

    set_target_properties(${project_name} PROPERTIES LIBRARY_OUTPUT_DIRECTORY_DEBUG "${trage_library_output_dir}")
    set_target_properties(${project_name} PROPERTIES LIBRARY_OUTPUT_DIRECTORY_RELEASE "${trage_library_output_dir}")
    set_target_properties(${project_name} PROPERTIES LIBRARY_OUTPUT_DIRECTORY_MINSIZEREL "${trage_library_output_dir}")
    set_target_properties(${project_name} PROPERTIES LIBRARY_OUTPUT_DIRECTORY_RELWITHDEBINFO "${trage_library_output_dir}")

    set_target_properties(${project_name} PROPERTIES ARCHIVE_OUTPUT_DIRECTORY_DEBUG "${trage_archive_output_dir}")
    set_target_properties(${project_name} PROPERTIES ARCHIVE_OUTPUT_DIRECTORY_RELEASE "${trage_archive_output_dir}")
    set_target_properties(${project_name} PROPERTIES ARCHIVE_OUTPUT_DIRECTORY_MINSIZEREL "${trage_archive_output_dir}")
    set_target_properties(${project_name} PROPERTIES ARCHIVE_OUTPUT_DIRECTORY_RELWITHDEBINFO "${trage_archive_output_dir}")

    # 开启 MSVC 终端上的中文不乱码
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /utf-8")
    add_compile_options(/source-charset:utf-8 /execution-charset:utf-8)
    # 开启 UNICODE 等常规
    target_compile_definitions(${project_name} PRIVATE
        WIN32_LEAN_AND_MEAN                                                                  # Header files containing only the common APIs
        _CRT_SECURE_NO_WARNINGS                                                              # Unsafe functions such as strcpy, scanf, etc. are allowed
        )
endif()
