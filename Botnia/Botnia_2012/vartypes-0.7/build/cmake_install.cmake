# Install script for directory: /home/gao/Desktop/test/vartypes-0.7

# Set the install prefix
IF(NOT DEFINED CMAKE_INSTALL_PREFIX)
  SET(CMAKE_INSTALL_PREFIX "/usr/local")
ENDIF(NOT DEFINED CMAKE_INSTALL_PREFIX)
STRING(REGEX REPLACE "/$" "" CMAKE_INSTALL_PREFIX "${CMAKE_INSTALL_PREFIX}")

# Set the install configuration name.
IF(NOT DEFINED CMAKE_INSTALL_CONFIG_NAME)
  IF(BUILD_TYPE)
    STRING(REGEX REPLACE "^[^A-Za-z0-9_]+" ""
           CMAKE_INSTALL_CONFIG_NAME "${BUILD_TYPE}")
  ELSE(BUILD_TYPE)
    SET(CMAKE_INSTALL_CONFIG_NAME "Release")
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
  FILE(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/vartypes" TYPE FILE FILES
    "/home/gao/Desktop/test/vartypes-0.7/./vartypes/primitives/VarType.h"
    "/home/gao/Desktop/test/vartypes-0.7/./vartypes/primitives/VarBlob.h"
    "/home/gao/Desktop/test/vartypes-0.7/./vartypes/primitives/VarBool.h"
    "/home/gao/Desktop/test/vartypes-0.7/./vartypes/primitives/VarDouble.h"
    "/home/gao/Desktop/test/vartypes-0.7/./vartypes/primitives/VarExternal.h"
    "/home/gao/Desktop/test/vartypes-0.7/./vartypes/primitives/VarInt.h"
    "/home/gao/Desktop/test/vartypes-0.7/./vartypes/primitives/VarList.h"
    "/home/gao/Desktop/test/vartypes-0.7/./vartypes/primitives/VarQWidget.h"
    "/home/gao/Desktop/test/vartypes-0.7/./vartypes/primitives/VarSelection.h"
    "/home/gao/Desktop/test/vartypes-0.7/./vartypes/primitives/VarString.h"
    "/home/gao/Desktop/test/vartypes-0.7/./vartypes/primitives/VarStringEnum.h"
    "/home/gao/Desktop/test/vartypes-0.7/./vartypes/primitives/VarTrigger.h"
    "/home/gao/Desktop/test/vartypes-0.7/./vartypes/gui/VarItem.h"
    "/home/gao/Desktop/test/vartypes-0.7/./vartypes/gui/VarItemDelegate.h"
    "/home/gao/Desktop/test/vartypes-0.7/./vartypes/gui/VarTreeModel.h"
    "/home/gao/Desktop/test/vartypes-0.7/./vartypes/gui/VarTreeView.h"
    "/home/gao/Desktop/test/vartypes-0.7/./vartypes/VarNotifier.h"
    "/home/gao/Desktop/test/vartypes-0.7/./vartypes/VarTypes.h"
    "/home/gao/Desktop/test/vartypes-0.7/./vartypes/VarTypesFactory.h"
    "/home/gao/Desktop/test/vartypes-0.7/./vartypes/gui/VarTreeViewOptions.h"
    "/home/gao/Desktop/test/vartypes-0.7/./vartypes/xml/xmlParser.h"
    "/home/gao/Desktop/test/vartypes-0.7/./vartypes/primitives/VarVal.h"
    "/home/gao/Desktop/test/vartypes-0.7/./vartypes/primitives/VarStringVal.h"
    "/home/gao/Desktop/test/vartypes-0.7/./vartypes/primitives/VarIntVal.h"
    "/home/gao/Desktop/test/vartypes-0.7/./vartypes/primitives/VarDoubleVal.h"
    "/home/gao/Desktop/test/vartypes-0.7/./vartypes/primitives/VarBoolVal.h"
    "/home/gao/Desktop/test/vartypes-0.7/./vartypes/VarXML.h"
    "/home/gao/Desktop/test/vartypes-0.7/./vartypes/VarTypesInstance.h"
    "/home/gao/Desktop/test/vartypes-0.7/./vartypes/VarBase64.h"
    "/home/gao/Desktop/test/vartypes-0.7/./vartypes/primitives/VarProtoBufferVal.h"
    "/home/gao/Desktop/test/vartypes-0.7/./vartypes/primitives/VarProtoBuffer.h"
    )
ENDIF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")

IF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  FILE(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/share/doc/vartypes" TYPE FILE FILES
    "/home/gao/Desktop/test/vartypes-0.7/./README"
    "/home/gao/Desktop/test/vartypes-0.7/./COPYING"
    )
ENDIF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")

IF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  FOREACH(file
      "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libvartypes.so.0.7.0"
      "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libvartypes.so.1"
      "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libvartypes.so"
      )
    IF(EXISTS "${file}" AND
       NOT IS_SYMLINK "${file}")
      FILE(RPATH_CHECK
           FILE "${file}"
           RPATH "")
    ENDIF()
  ENDFOREACH()
  FILE(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib" TYPE SHARED_LIBRARY FILES
    "/home/gao/Desktop/test/vartypes-0.7/build/libvartypes.so.0.7.0"
    "/home/gao/Desktop/test/vartypes-0.7/build/libvartypes.so.1"
    "/home/gao/Desktop/test/vartypes-0.7/build/libvartypes.so"
    )
  FOREACH(file
      "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libvartypes.so.0.7.0"
      "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libvartypes.so.1"
      "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libvartypes.so"
      )
    IF(EXISTS "${file}" AND
       NOT IS_SYMLINK "${file}")
      IF(CMAKE_INSTALL_DO_STRIP)
        EXECUTE_PROCESS(COMMAND "/usr/bin/strip" "${file}")
      ENDIF(CMAKE_INSTALL_DO_STRIP)
    ENDIF()
  ENDFOREACH()
ENDIF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")

IF(CMAKE_INSTALL_COMPONENT)
  SET(CMAKE_INSTALL_MANIFEST "install_manifest_${CMAKE_INSTALL_COMPONENT}.txt")
ELSE(CMAKE_INSTALL_COMPONENT)
  SET(CMAKE_INSTALL_MANIFEST "install_manifest.txt")
ENDIF(CMAKE_INSTALL_COMPONENT)

FILE(WRITE "/home/gao/Desktop/test/vartypes-0.7/build/${CMAKE_INSTALL_MANIFEST}" "")
FOREACH(file ${CMAKE_INSTALL_MANIFEST_FILES})
  FILE(APPEND "/home/gao/Desktop/test/vartypes-0.7/build/${CMAKE_INSTALL_MANIFEST}" "${file}\n")
ENDFOREACH(file)
