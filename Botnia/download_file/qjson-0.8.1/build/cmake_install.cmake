# Install script for directory: /home/barry963/Botnia/download_file/qjson-0.8.1

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
  list(APPEND CPACK_ABSOLUTE_DESTINATION_FILES
   "/home/barry963/Botnia/download_file/qjson-0.8.1/build/_preferred_path_/lib/pkgconfig/QJson.pc")
FILE(INSTALL DESTINATION "/home/barry963/Botnia/download_file/qjson-0.8.1/build/_preferred_path_/lib/pkgconfig" TYPE FILE FILES "/home/barry963/Botnia/download_file/qjson-0.8.1/build/QJson.pc")
ENDIF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")

IF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  IF(EXISTS "$ENV{DESTDIR}/home/barry963/Botnia/download_file/qjson-0.8.1/build/_preferred_path_/lib/cmake/qjson/QJSONTargets.cmake")
    FILE(DIFFERENT EXPORT_FILE_CHANGED FILES
         "$ENV{DESTDIR}/home/barry963/Botnia/download_file/qjson-0.8.1/build/_preferred_path_/lib/cmake/qjson/QJSONTargets.cmake"
         "/home/barry963/Botnia/download_file/qjson-0.8.1/build/CMakeFiles/Export/_home/barry963/Botnia/download_file/qjson-0.8.1/build/_preferred_path_/lib/cmake/qjson/QJSONTargets.cmake")
    IF(EXPORT_FILE_CHANGED)
      FILE(GLOB OLD_CONFIG_FILES "$ENV{DESTDIR}/home/barry963/Botnia/download_file/qjson-0.8.1/build/_preferred_path_/lib/cmake/qjson/QJSONTargets-*.cmake")
      IF(OLD_CONFIG_FILES)
        MESSAGE(STATUS "Old export file \"$ENV{DESTDIR}/home/barry963/Botnia/download_file/qjson-0.8.1/build/_preferred_path_/lib/cmake/qjson/QJSONTargets.cmake\" will be replaced.  Removing files [${OLD_CONFIG_FILES}].")
        FILE(REMOVE ${OLD_CONFIG_FILES})
      ENDIF(OLD_CONFIG_FILES)
    ENDIF(EXPORT_FILE_CHANGED)
  ENDIF()
  list(APPEND CPACK_ABSOLUTE_DESTINATION_FILES
   "/home/barry963/Botnia/download_file/qjson-0.8.1/build/_preferred_path_/lib/cmake/qjson/QJSONTargets.cmake")
FILE(INSTALL DESTINATION "/home/barry963/Botnia/download_file/qjson-0.8.1/build/_preferred_path_/lib/cmake/qjson" TYPE FILE FILES "/home/barry963/Botnia/download_file/qjson-0.8.1/build/CMakeFiles/Export/_home/barry963/Botnia/download_file/qjson-0.8.1/build/_preferred_path_/lib/cmake/qjson/QJSONTargets.cmake")
  IF("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^()$")
    list(APPEND CPACK_ABSOLUTE_DESTINATION_FILES
     "/home/barry963/Botnia/download_file/qjson-0.8.1/build/_preferred_path_/lib/cmake/qjson/QJSONTargets-noconfig.cmake")
FILE(INSTALL DESTINATION "/home/barry963/Botnia/download_file/qjson-0.8.1/build/_preferred_path_/lib/cmake/qjson" TYPE FILE FILES "/home/barry963/Botnia/download_file/qjson-0.8.1/build/CMakeFiles/Export/_home/barry963/Botnia/download_file/qjson-0.8.1/build/_preferred_path_/lib/cmake/qjson/QJSONTargets-noconfig.cmake")
  ENDIF("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^()$")
ENDIF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")

IF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  list(APPEND CPACK_ABSOLUTE_DESTINATION_FILES
   "/home/barry963/Botnia/download_file/qjson-0.8.1/build/_preferred_path_/lib/cmake/qjson/QJSONConfig.cmake;/home/barry963/Botnia/download_file/qjson-0.8.1/build/_preferred_path_/lib/cmake/qjson/QJSONConfigVersion.cmake")
FILE(INSTALL DESTINATION "/home/barry963/Botnia/download_file/qjson-0.8.1/build/_preferred_path_/lib/cmake/qjson" TYPE FILE FILES
    "/home/barry963/Botnia/download_file/qjson-0.8.1/build/QJSONConfig.cmake"
    "/home/barry963/Botnia/download_file/qjson-0.8.1/build/QJSONConfigVersion.cmake"
    )
ENDIF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")

IF(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for each subdirectory.
  INCLUDE("/home/barry963/Botnia/download_file/qjson-0.8.1/build/src/cmake_install.cmake")

ENDIF(NOT CMAKE_INSTALL_LOCAL_ONLY)

IF(CMAKE_INSTALL_COMPONENT)
  SET(CMAKE_INSTALL_MANIFEST "install_manifest_${CMAKE_INSTALL_COMPONENT}.txt")
ELSE(CMAKE_INSTALL_COMPONENT)
  SET(CMAKE_INSTALL_MANIFEST "install_manifest.txt")
ENDIF(CMAKE_INSTALL_COMPONENT)

FILE(WRITE "/home/barry963/Botnia/download_file/qjson-0.8.1/build/${CMAKE_INSTALL_MANIFEST}" "")
FOREACH(file ${CMAKE_INSTALL_MANIFEST_FILES})
  FILE(APPEND "/home/barry963/Botnia/download_file/qjson-0.8.1/build/${CMAKE_INSTALL_MANIFEST}" "${file}\n")
ENDFOREACH(file)
