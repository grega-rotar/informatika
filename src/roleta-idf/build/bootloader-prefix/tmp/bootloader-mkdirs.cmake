# Distributed under the OSI-approved BSD 3-Clause License.  See accompanying
# file Copyright.txt or https://cmake.org/licensing for details.

cmake_minimum_required(VERSION 3.5)

file(MAKE_DIRECTORY
  "/home/grega/esp/esp-idf/components/bootloader/subproject"
  "/home/grega/Documents/school/informatika/src/roleta-idf/build/bootloader"
  "/home/grega/Documents/school/informatika/src/roleta-idf/build/bootloader-prefix"
  "/home/grega/Documents/school/informatika/src/roleta-idf/build/bootloader-prefix/tmp"
  "/home/grega/Documents/school/informatika/src/roleta-idf/build/bootloader-prefix/src/bootloader-stamp"
  "/home/grega/Documents/school/informatika/src/roleta-idf/build/bootloader-prefix/src"
  "/home/grega/Documents/school/informatika/src/roleta-idf/build/bootloader-prefix/src/bootloader-stamp"
)

set(configSubDirs )
foreach(subDir IN LISTS configSubDirs)
    file(MAKE_DIRECTORY "/home/grega/Documents/school/informatika/src/roleta-idf/build/bootloader-prefix/src/bootloader-stamp/${subDir}")
endforeach()
if(cfgdir)
  file(MAKE_DIRECTORY "/home/grega/Documents/school/informatika/src/roleta-idf/build/bootloader-prefix/src/bootloader-stamp${cfgdir}") # cfgdir has leading slash
endif()
