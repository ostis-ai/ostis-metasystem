install(TARGETS
    identifiers-module sections-module
    EXPORT privateExport
    LIBRARY DESTINATION "${CMAKE_INSTALL_LIBDIR}/extensions"
)

install(EXPORT privateExport
    FILE privateTargets.cmake
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/ostis-metasystem
)

include(CMakePackageConfigHelpers)

write_basic_package_version_file(
    "${CMAKE_CURRENT_BINARY_DIR}/ostis-metasystem-config-version.cmake"
    VERSION ${PROJECT_VERSION}
    COMPATIBILITY AnyNewerVersion
)

install(FILES
    "${CMAKE_CURRENT_BINARY_DIR}/ostis-metasystem-config-version.cmake"
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/ostis-metasystem
)

set(CPACK_PACKAGE_NAME                  ostis-metasystem)
set(CPACK_PACKAGE_VENDOR                "OSTIS AI")
set(CPACK_PACKAGE_DESCRIPTION_SUMMARY   "Software implementation of semantic network program interpreter")
set(CPACK_PACKAGE_INSTALL_DIRECTORY     ${CPACK_PACKAGE_NAME})
set(CPACK_PACKAGE_VERSION_MAJOR         ${PROJECT_VERSION_MAJOR})
set(CPACK_PACKAGE_VERSION_MINOR         ${PROJECT_VERSION_MINOR})
set(CPACK_PACKAGE_VERSION_PATCH         ${PROJECT_VERSION_PATCH})
set(CPACK_VERBATIM_VARIABLES            TRUE)

if (WIN32)
    set(CPACK_GENERATOR     ZIP)
else()
    set(CPACK_GENERATOR     TGZ)
endif()
include(CPack)
