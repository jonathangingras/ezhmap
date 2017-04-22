include(ExternalProject)

if(NOT EXTERNAL_PROJECT_INSTALL_PREFIX)
  set(EXTERNAL_PROJECT_INSTALL_PREFIX "${CMAKE_CURRENT_BINARY_DIR}/extern")
endif()

function(ExternalProject_add_local)
  ExternalProject_add(
    ${ARGV}
    CMAKE_ARGS "-DCMAKE_INSTALL_PREFIX=${EXTERNAL_PROJECT_INSTALL_PREFIX}"
    UPDATE_COMMAND ""
    )

  include_directories(${EXTERNAL_PROJECT_INSTALL_PREFIX}/include)
  link_directories(${EXTERNAL_PROJECT_INSTALL_PREFIX}/lib)
endfunction()
