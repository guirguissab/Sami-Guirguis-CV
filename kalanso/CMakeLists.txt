cmake_minimum_required(VERSION 3.16)
project(Kalanso)

set(SRC_DIR ./src)
set(SRCS_ABSENCE        ${SRC_DIR}/Absence.cpp)
set(SRCS_ACTUALITE      ${SRC_DIR}/Actualite.cpp)
set(SRCS_ADMINISTRATEUR ${SRC_DIR}/Administrateur.cpp)
set(SRCS_CLASSETU       ${SRC_DIR}/ClassEtu.cpp)
set(SRCS_COUR           ${SRC_DIR}/cour.cpp)
set(SRCS_DATABASE       ${SRC_DIR}/Database.cpp)
set(SRCS_DATE           ${SRC_DIR}/Date.cpp)
set(SRCS_EDT            ${SRC_DIR}/EDT.cpp)
set(SRCS_ETUDIANT       ${SRC_DIR}/Etudiant.cpp)
set(SRCS_HEURE          ${SRC_DIR}/Heure.cpp)
set(SRCS_MATIERE        ${SRC_DIR}/Matiere.cpp)
set(SRCS_NOTE           ${SRC_DIR}/Note.cpp)
set(SRCS_PERSONNE       ${SRC_DIR}/Personne.cpp)
set(SRCS_PROFESSEUR     ${SRC_DIR}/Professeur.cpp)
set(SRCS_TXTAPP         ${SRC_DIR}/txtApp.cpp)
set(SRCS_WINTXT         ${SRC_DIR}/WinTXT.cpp)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -ggdb")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin)
set(EXECUTABLE_OUTPUT_PATH ${CMAKE_SOURCE_DIR}/bin)

find_package(SQLite3 REQUIRED)

set(SRCS_COMMON
    ${SRCS_ABSENCE} ${SRCS_ACTUALITE} ${SRCS_ADMINISTRATEUR}
    ${SRCS_CLASSETU} ${SRCS_COUR} ${SRCS_DATABASE} ${SRCS_DATE}
    ${SRCS_EDT} ${SRCS_ETUDIANT} ${SRCS_HEURE} ${SRCS_MATIERE}
    ${SRCS_NOTE} ${SRCS_PERSONNE} ${SRCS_PROFESSEUR}
)

add_executable(testRegression ${SRC_DIR}/main_testRegression.cpp ${SRCS_COMMON})
add_executable(version_TXT    ${SRC_DIR}/main_txt.cpp ${SRCS_COMMON} ${SRCS_TXTAPP} ${SRCS_WINTXT})

target_include_directories(testRegression PUBLIC ${SRC_DIR} ${SQLite3_INCLUDE_DIRS})
target_include_directories(version_TXT    PUBLIC ${SRC_DIR} ${SQLite3_INCLUDE_DIRS})

target_link_libraries(testRegression PRIVATE SQLite::SQLite3)
target_link_libraries(version_TXT    PRIVATE SQLite::SQLite3)

option(BUILD_KALANSO_QT "Build the Qt Quick interface" ON)
if(BUILD_KALANSO_QT)
    find_package(Qt6 QUIET COMPONENTS Quick)
    if(Qt6_FOUND)
        qt_standard_project_setup(REQUIRES 6.2)

        qt_add_executable(version_QT
            qt/main.cpp
            qt/SchoolFacade.cpp
            ${SRCS_COMMON}
        )

        qt_add_qml_module(version_QT
            URI Kalanso
            VERSION 1.0
            QML_FILES
                qt/Main.qml
        )

        target_include_directories(version_QT PRIVATE ${SRC_DIR} qt)
        target_link_libraries(version_QT PRIVATE Qt6::Quick SQLite::SQLite3)
    else()
        message(STATUS "Qt6 not found: only kalanso_txt target will be built.")
    endif()
endif()