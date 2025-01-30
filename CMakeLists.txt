cmake_minimum_required(VERSION 3.16)

project(NMP3 VERSION 0.1 LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets)
find_package(Qt6 REQUIRED COMPONENTS Sql)
find_package(Qt6 REQUIRED COMPONENTS Multimedia)
find_package(Qt6 REQUIRED COMPONENTS MultimediaWidgets)

# include_directories(
#     ${CMAKE_CURRENT_SOURCE_DIR}/pythonqt-3.5.7/include
#     ${CMAKE_CURRENT_SOURCE_DIR}/python312/include
# )

# link_directories(
#     ${CMAKE_CURRENT_SOURCE_DIR}/pythonqt-3.5.7/lib
#     ${CMAKE_CURRENT_SOURCE_DIR}/python312/libs
# )

# link_libraries(
#     PythonQt_QtAll-Qt6-Python3.12
#     PythonQt-Qt6-Python3.12
#     python312
# )

set(PROJECT_SOURCES
        main.cpp
        homepage.cpp
        homepage.h
        homepage.ui
)

if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(NMP3
        MANUAL_FINALIZATION
        ${PROJECT_SOURCES}
        RootPage/rootpage.h RootPage/rootpage.cpp RootPage/rootpage.ui
        MP3PlayerColumn/mp3playercolumn.cpp MP3PlayerColumn/mp3playercolumn.ui MP3PlayerColumn/volumnslider.cpp MP3PlayerColumn/volumnslider.h MP3PlayerColumn/volumnslider.ui
        MP3PlayerColumn/include/mp3playercolumn.h
        PlayMP4Page/displaymp4nodelist.cpp PlayMP4Page/displaymp4nodelist.h PlayMP4Page/displaymp4nodelist.ui PlayMP4Page/mp4node.cpp PlayMP4Page/mp4node.h PlayMP4Page/mp4node.ui PlayMP4Page/playmp4page.cpp PlayMP4Page/playmp4page.ui
        PlayMP4Page/include/playmp4page.h
        DownloadBiliBiliVideoPage/downloadbilibilivideopage.h DownloadBiliBiliVideoPage/downloadbilibilivideopage.cpp DownloadBiliBiliVideoPage/downloadbilibilivideopage.ui
        DownloadBilibiliVideo.py
        DownloadBiliBiliVideoPage/downloadbilibilivideonode.h DownloadBiliBiliVideoPage/downloadbilibilivideonode.cpp DownloadBiliBiliVideoPage/downloadbilibilivideonode.ui
        images.qrc
        MP3PlayerColumn/LyricsPlayer/lyricsnode.cpp MP3PlayerColumn/LyricsPlayer/lyricsnode.h MP3PlayerColumn/LyricsPlayer/lyricsnode.ui MP3PlayerColumn/LyricsPlayer/lyricsplayer.cpp MP3PlayerColumn/LyricsPlayer/lyricsplayer.h MP3PlayerColumn/LyricsPlayer/lyricsplayer.ui
        RenameSongPage/renamesongpage.h RenameSongPage/renamesongpage.cpp RenameSongPage/renamesongpage.ui
        NVideoWidget/nvideowidget.h NVideoWidget/nvideowidget.cpp
    )

# Define target properties for Android with Qt 6 as:
#    set_property(TARGET NMP3 APPEND PROPERTY QT_ANDROID_PACKAGE_SOURCE_DIR
#                 ${CMAKE_CURRENT_SOURCE_DIR}/android)
# For more information, see https://doc.qt.io/qt-6/qt-add-executable.html#target-creation
else()
    if(ANDROID)
        add_library(NMP3 SHARED
            ${PROJECT_SOURCES}
        )
# Define properties for Android with Qt 5 after find_package() calls as:
#    set(ANDROID_PACKAGE_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/android")
    else()
        add_executable(NMP3
            ${PROJECT_SOURCES}
        )
    endif()
endif()

add_subdirectory(QHotkey)

target_include_directories(NMP3 PUBLIC
${CMAKE_CURRENT_SOURCE_DIR}/MP3PlayerColumn/include
${CMAKE_CURRENT_SOURCE_DIR}/PlayMP4Page/include
)

target_link_libraries(NMP3 PRIVATE Qt${QT_VERSION_MAJOR}::Widgets QHotkey::QHotkey)
target_link_libraries(NMP3 PUBLIC Qt6::Sql)
target_link_libraries(NMP3 PRIVATE Qt6::Multimedia)
target_link_libraries(NMP3 PRIVATE Qt6::MultimediaWidgets)

# Qt for iOS sets MACOSX_BUNDLE_GUI_IDENTIFIER automatically since Qt 6.1.
# If you are developing for iOS or macOS you should consider setting an
# explicit, fixed bundle identifier manually though.
if(${QT_VERSION} VERSION_LESS 6.1.0)
  set(BUNDLE_ID_OPTION MACOSX_BUNDLE_GUI_IDENTIFIER com.example.NMP3)
endif()
set_target_properties(NMP3 PROPERTIES
    ${BUNDLE_ID_OPTION}
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

include(GNUInstallDirs)
install(TARGETS NMP3
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

set(COPY_FILE_DESTINATION_DIR_RELEASE ${CMAKE_CURRENT_SOURCE_DIR}/build/Desktop_Qt_6_7_3_MSVC2019_64bit-Release )
set(COPY_FILE_DESTINATION_DIR_DEBUG ${CMAKE_CURRENT_SOURCE_DIR}/build/Desktop_Qt_6_7_3_MSVC2019_64bit-Debug )
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/DownloadBilibiliVideo.py ${COPY_FILE_DESTINATION_DIR_DELEASE} COPYONLY)
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/DownloadBilibiliVideo.py ${COPY_FILE_DESTINATION_DIR_DEBUG} COPYONLY)

if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(NMP3)
endif()
