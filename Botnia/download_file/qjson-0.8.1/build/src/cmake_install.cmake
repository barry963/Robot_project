# Install script for directory: /home/barry963/Botnia/download_file/qjson-0.8.1/src

# Set the install prefix
IF(NOT DEFINED CMAKE_INSTALL_PREFIX)
  SET(CMAKE_INSTALL_PREFIX "/home/barry963/Botnia/download_file/qjson-0.8.1/build/_preferred_path_")
ENDIF(NOT DEFINED CMAKE_INSTALL_PREFIX)
STRING(REGEX REPLACE "/$" "" CMAKE_INSTALL_PREFIX "${CMAKE_INSTALL_PREFIX}")

# Set the install configuration name.
IF(NOT DEFINED CMAKE_INSTALL_CONFIG_NAME)
  IF(BUILD_TYPE)
    STRING(REGEX REPLACE "^[^A-Za-z0-9_]+" ""
           CMAKE_INSTALL_CONFIG_NAME "${BUILD_TYPE}")
  ELSE(BUILD_TYPE)
    SET(CMAKE_INSTALL_CONFIG_NAME "")
  ENDIF(BUILD_TYPE)
  MESSAGE(STATUS "Install configuration: \"${CMAKE_INSTALL_CONFIG_NAME}\"")
ENDIF(NOT DEFINED CMAKE_INSTALL_CONFIG_NAME)

# Set the component getting installed.
IF(NOT CMAKE_INSTALL_COMPONENT)
  IF(COMPONENT)
    MESSAGE(STATUS "Install component: \"${COMPONENT}\"")
    SET(CMAKE_INSTALL_COMPONENT "${COMPONENT}")
  ELSE(COMPONENT)
    SET(CMAKE_INSTALL_COMPONENT)
  ENDIF(COMPONENT)
ENDIF(NOT CMAKE_INSTALL_COMPONENT)

# Install shared libraries without execute permission?
IF(NOT DEFINED CMAKE_INSTALL_SO_NO_EXE)
  SET(CMAKE_INSTALL_SO_NO_EXE "1")
ENDIF(NOT DEFINED CMAKE_INSTALL_SO_NO_EXE)

IF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  FOREACH(file
      "$ENV{DESTDIR}/home/barry963/Botnia/download_file/qjson-0.8.1/build/_preferred_path_/lib/libqjson.so.0.8.1"
      "$ENV{DESTDIR}/home/barry963/Botnia/download_file/qjson-0.8.1/build/_preferred_path_/lib/libqjson.so.0"
      "$ENV{DESTDIR}/home/barry963/Botnia/download_file/qjson-0.8.1/build/_preferred_path_/lib/libqjson.so"
      )
    IF(EXISTS "${file}" AND
       NOT IS_SYMLINK "${file}")
      FILE(RPATH_CHECK
           FILE "${file}"
           RPATH "")
    ENDIF()
  ENDFOREACH()
  list(APPEND CPACK_ABSOLUTE_DESTINATION_FILES
   "/home/barry963/Botnia/download_file/qjson-0.8.1/build/_preferred_path_/lib/libqjson.so.0.8.1;/home/barry963/Botnia/download_file/qjson-0.8.1/build/_preferred_path_/lib/libqjson.so.0;/home/barry963/Botnia/download_file/qjson-0.8.1/build/_preferred_path_/lib/libqjson.so")
FILE(INSTALL DESTINATION "/home/barry963/Botnia/download_file/qjson-0.8.1/build/_preferred_path_/lib" TYPE SHARED_LIBRARY FILES
    "/home/barry963/Botnia/download_file/qjson-0.8.1/build/lib/libqjson.so.0.8.1"
    "/home/barry963/Botnia/download_file/qjson-0.8.1/build/lib/libqjson.so.0"
    "/home/barry963/Botnia/download_file/qjson-0.8.1/build/lib/libqjson.so"
    )
  FOREACH(file
      "$ENV{DESTDIR}/home/barry963/Botnia/download_file/qjson-0.8.1/build/_preferred_path_/lib/libqjson.so.0.8.1"
      "$ENV{DESTDIR}/home/barry963/Botnia/download_file/qjson-0.8.1/build/_preferred_path_/lib/libqjson.so.0"
      "$ENV{DESTDIR}/home/barry963/Botnia/download_file/qjson-0.8.1/build/_preferred_path_/lib/libqjson.so"
      )
    IF(EXISTS "${file}" AND
       NOT IS_SYMLINK "${file}")
      IF(CMAKE_INSTALL_DO_STRIP)
        EXECUTE_PROCESS(COMMAND "/usr/bin/strip" "${file}")
      ENDIF(CMAKE_INSTALL_DO_STRIP)
    ENDIF()
  ENDFOREACH()
ENDIF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")

IF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  list(APPEND CPACK_ABSOLUTE_DESTINATION_FILES
   "/home/barry963/Botnia/download_file/qjson-0.8.1/build/_preferred_path_/include/qjson/parser.h;/home/barry963/Botnia/download_file/qjson-0.8.1/build/_preferred_path_/include/qjson/parserrunnable.h;/home/barry963/Botnia/download_file/qjson-0.8.1/build/_preferred_path_/include/qjson/qobjecthelper.h;/home/barry963/Botnia/download_file/qjson-0.8.1/build/_preferred_path_/include/qjson/serializer.h;/home/barry963/Botnia/download_file/qjson-0.8.1/build/_preferred_path_/include/qjson/serializerrunnable.h;/home/barry963/Botnia/download_file/qjson-0.8.1/build/_preferred_path_/include/qjson/qjson_export.h")
FILE(INSTALL DESTINATION "/home/barry963/Botnia/download_file/qjson-0.8.1/build/_preferred_path_/include/qjson" TYPE FILE FILES
    "/home/barry963/Botnia/download_file/qjson-0.8.1/src/parser.h"
    "/home/barry963/Botnia/download_file/qjson-0.8.1/src/parserrunnable.h"
    "/home/barry963/Botnia/download_file/qjson-0.8.1/src/qobjecthelper.h"
    "/home/barry963/Botnia/download_file/qjson-0.8.1/src/serializer.h"
    "/home/barry963/Botnia/download_file/qjson-0.8.1/src/serializerrunnable.h"
    "/home/barry963/Botnia/download_file/qjson-0.8.1/src/qjson_export.h"
    )
ENDIF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")

