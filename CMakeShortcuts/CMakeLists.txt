project(CMakeShortcuts)
cmake_minimum_required(VERSION 3.0.0)

function(install_modules)
  install(FILES ${ARGV} DESTINATION share/cmake/Modules)
endfunction()

install_modules(
  ExternalProjectLocal.cmake
  CTestShortcuts.cmake
  )
