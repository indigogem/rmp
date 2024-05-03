include(GNUInstallDirs)

find_library(
    EASTL_LIBRARY
    NAMES eastl
    HINTS ${CMAKE_PREFIX_PATH}/lib/
    #PATH_SUFFIXES LibImagePipeline/native-linux-x64/
    )

# find_path(EASTL_INCLUDE_DIR
  # NAMES Pipeline.hpp
  # HINTS ${CMAKE_PREFIX_PATH}/prebuilt/ ${CMAKE_INSTALL_INCLUDEDIR}
  # PATH_SUFFIXES LibImagePipeline/native-linux-api/imagepipeline/ LibImagePipeline/imagepipeline)
list(APPEND EASTL_INCLUDE_DIR "${CMAKE_PREFIX_PATH}/include")
list(APPEND EASTL_INCLUDE_DIR "g:/projects/rmp/thirdparty/build/eastl/eastl/src/eastl/test/packages/EABase/include/Common")

include(FindPackageHandleStandardArgs)

find_package_handle_standard_args(EASTL DEFAULT_MSG
                                  EASTL_LIBRARY
                                  EASTL_INCLUDE_DIR)

mark_as_advanced(EASTL_LIBRARY EASTL_INCLUDE_DIR)

if(EASTL_FOUND AND NOT TARGET EASTL::EASTL)
  add_library(EASTL::EASTL STATIC IMPORTED)
  set_target_properties(
    EASTL::EASTL
    PROPERTIES
      INTERFACE_INCLUDE_DIRECTORIES "${EASTL_INCLUDE_DIR}"
      IMPORTED_LOCATION ${EASTL_LIBRARY})
endif()