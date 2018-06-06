# Install script for directory: /home/don_vanchos/CAPSYS/libs/MathGL/mathgl-2.4.1

# Set the install prefix
if(NOT DEFINED CMAKE_INSTALL_PREFIX)
  set(CMAKE_INSTALL_PREFIX "/usr/local")
endif()
string(REGEX REPLACE "/$" "" CMAKE_INSTALL_PREFIX "${CMAKE_INSTALL_PREFIX}")

# Set the install configuration name.
if(NOT DEFINED CMAKE_INSTALL_CONFIG_NAME)
  if(BUILD_TYPE)
    string(REGEX REPLACE "^[^A-Za-z0-9_]+" ""
           CMAKE_INSTALL_CONFIG_NAME "${BUILD_TYPE}")
  else()
    set(CMAKE_INSTALL_CONFIG_NAME "Release")
  endif()
  message(STATUS "Install configuration: \"${CMAKE_INSTALL_CONFIG_NAME}\"")
endif()

# Set the component getting installed.
if(NOT CMAKE_INSTALL_COMPONENT)
  if(COMPONENT)
    message(STATUS "Install component: \"${COMPONENT}\"")
    set(CMAKE_INSTALL_COMPONENT "${COMPONENT}")
  else()
    set(CMAKE_INSTALL_COMPONENT)
  endif()
endif()

# Install shared libraries without execute permission?
if(NOT DEFINED CMAKE_INSTALL_SO_NO_EXE)
  set(CMAKE_INSTALL_SO_NO_EXE "1")
endif()

if("${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  list(APPEND CMAKE_ABSOLUTE_DESTINATION_FILES
   "/usr/local/share/locale/ru/LC_MESSAGES/mathgl.mo")
  if(CMAKE_WARN_ON_ABSOLUTE_INSTALL_DESTINATION)
    message(WARNING "ABSOLUTE path INSTALL DESTINATION : ${CMAKE_ABSOLUTE_DESTINATION_FILES}")
  endif()
  if(CMAKE_ERROR_ON_ABSOLUTE_INSTALL_DESTINATION)
    message(FATAL_ERROR "ABSOLUTE path INSTALL DESTINATION forbidden (by caller): ${CMAKE_ABSOLUTE_DESTINATION_FILES}")
  endif()
file(INSTALL DESTINATION "/usr/local/share/locale/ru/LC_MESSAGES" TYPE FILE RENAME "mathgl.mo" FILES "/home/don_vanchos/CAPSYS/libs/MathGL/mathgl-2.4.1-linux-build/mathgl_ru.mo")
endif()

if("${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  list(APPEND CMAKE_ABSOLUTE_DESTINATION_FILES
   "/usr/local/share/locale/es/LC_MESSAGES/mathgl.mo")
  if(CMAKE_WARN_ON_ABSOLUTE_INSTALL_DESTINATION)
    message(WARNING "ABSOLUTE path INSTALL DESTINATION : ${CMAKE_ABSOLUTE_DESTINATION_FILES}")
  endif()
  if(CMAKE_ERROR_ON_ABSOLUTE_INSTALL_DESTINATION)
    message(FATAL_ERROR "ABSOLUTE path INSTALL DESTINATION forbidden (by caller): ${CMAKE_ABSOLUTE_DESTINATION_FILES}")
  endif()
file(INSTALL DESTINATION "/usr/local/share/locale/es/LC_MESSAGES" TYPE FILE RENAME "mathgl.mo" FILES "/home/don_vanchos/CAPSYS/libs/MathGL/mathgl-2.4.1-linux-build/mathgl_es.mo")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for each subdirectory.
  include("/home/don_vanchos/CAPSYS/libs/MathGL/mathgl-2.4.1-linux-build/src/cmake_install.cmake")
  include("/home/don_vanchos/CAPSYS/libs/MathGL/mathgl-2.4.1-linux-build/widgets/cmake_install.cmake")
  include("/home/don_vanchos/CAPSYS/libs/MathGL/mathgl-2.4.1-linux-build/include/cmake_install.cmake")
  include("/home/don_vanchos/CAPSYS/libs/MathGL/mathgl-2.4.1-linux-build/udav/cmake_install.cmake")
  include("/home/don_vanchos/CAPSYS/libs/MathGL/mathgl-2.4.1-linux-build/json/cmake_install.cmake")
  include("/home/don_vanchos/CAPSYS/libs/MathGL/mathgl-2.4.1-linux-build/utils/cmake_install.cmake")
  include("/home/don_vanchos/CAPSYS/libs/MathGL/mathgl-2.4.1-linux-build/examples/cmake_install.cmake")
  include("/home/don_vanchos/CAPSYS/libs/MathGL/mathgl-2.4.1-linux-build/fonts/cmake_install.cmake")
  include("/home/don_vanchos/CAPSYS/libs/MathGL/mathgl-2.4.1-linux-build/mgllab/cmake_install.cmake")
  include("/home/don_vanchos/CAPSYS/libs/MathGL/mathgl-2.4.1-linux-build/scripts/cmake_install.cmake")

endif()

if(CMAKE_INSTALL_COMPONENT)
  set(CMAKE_INSTALL_MANIFEST "install_manifest_${CMAKE_INSTALL_COMPONENT}.txt")
else()
  set(CMAKE_INSTALL_MANIFEST "install_manifest.txt")
endif()

string(REPLACE ";" "\n" CMAKE_INSTALL_MANIFEST_CONTENT
       "${CMAKE_INSTALL_MANIFEST_FILES}")
file(WRITE "/home/don_vanchos/CAPSYS/libs/MathGL/mathgl-2.4.1-linux-build/${CMAKE_INSTALL_MANIFEST}"
     "${CMAKE_INSTALL_MANIFEST_CONTENT}")
