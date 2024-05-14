include(GNUInstallDirs)

find_library(
    GLM_LIBRARY
    NAMES glm
    HINTS ${CMAKE_PREFIX_PATH}/lib/
    )


list(APPEND GLM_INCLUDE_DIR "${CMAKE_PREFIX_PATH}/include")

include(FindPackageHandleStandardArgs)

find_package_handle_standard_args(GLM DEFAULT_MSG
                                  GLM_LIBRARY
                                  GLM_INCLUDE_DIR)

mark_as_advanced(GLM_LIBRARY GLM_INCLUDE_DIR)

if(GLM_FOUND AND NOT TARGET GLM::GLM)
  add_library(GLM::GLM STATIC IMPORTED)
  set_target_properties(
    GLM::GLM
    PROPERTIES
      INTERFACE_INCLUDE_DIRECTORIES "${GLM_INCLUDE_DIR}"
      IMPORTED_LOCATION ${GLM_LIBRARY})
endif()