# Source files
list(APPEND SRCS Application.cpp)
list(APPEND SRCS DatabaseManager.cpp)
list(APPEND SRCS DatabaseManager_delete.cpp)
list(APPEND SRCS DatabaseManager_getters.cpp)
list(APPEND SRCS DatabaseManager_insert.cpp)
list(APPEND SRCS DatabaseManager_update.cpp)
list(APPEND SRCS MainWindow.cpp)
list(APPEND SRCS MetadataFetcher.cpp)
list(APPEND SRCS MovieDialog.cpp)
list(APPEND SRCS MoviesDebug.cpp)
list(APPEND SRCS PeopleDialog.cpp)
list(APPEND SRCS SettingsWindow.cpp)
# main.cpp included in DatabaseManager.cpp 
# list(APPEND SRCS main.cpp)
list(APPEND SRCS Entities/Movie.cpp)
list(APPEND SRCS Entities/People.cpp)
list(APPEND SRCS Entities/Playlist.cpp)
list(APPEND SRCS Entities/Tag.cpp)


# Forms
list(APPEND FORMS MainWindow.ui)
list(APPEND FORMS MovieDialog.ui)
list(APPEND FORMS PeopleDialog.ui)



# Resources
list(APPEND RSRCS images.qrc)



include_directories(".")
qt5_add_resources(RSRCS_RCC ${RSRCS})
qt5_wrap_ui(FORMS_MOC ${FORMS})
add_executable(${EXECUTABLE_NAME} ${SRCS} ${RSRCS_RCC} ${FORMS_MOC})
qt5_use_modules(${EXECUTABLE_NAME} Widgets Network Xml Sql)

