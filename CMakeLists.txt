cmake_minimum_required(VERSION 3.21) # 3.21+ PROJECT_IS_TOP_LEVEL
project(saburou-platform CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include(GNUInstallDirs)

# 1. Definición de la Librería
# ------------------------------------------------------------------------------
add_library(platform INTERFACE)
add_library(saburou::platform ALIAS platform)

target_include_directories(
    platform INTERFACE
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/saburou_platform/include>
    $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>
)

# El "Exorcismo" de macros
target_compile_options(
    platform INTERFACE
    $<$<OR:$<CXX_COMPILER_ID:GNU>,$<CXX_COMPILER_ID:Clang>>:
        -Ulinux -Uunix -Uapple -Ubsd
    >
    $<$<CXX_COMPILER_ID:MSVC>:
        /Ulinux /Uunix /Uapple /Ubsd
    >
)

# 2. Lógica de Tests / Ejecutable Local
# ------------------------------------------------------------------------------
if(PROJECT_IS_TOP_LEVEL)
    message(STATUS "saburou-platform: Modo desarrollo (Tests incluidos)")

    set(SABUROU_OUTPUT_DIR "${CMAKE_CURRENT_SOURCE_DIR}/bin")
    
    file(GLOB_RECURSE TEST_SOURCES CONFIGURE_DEPENDS
        "${CMAKE_CURRENT_SOURCE_DIR}/saburou_platform/src/*.cpp"
    )

    if(TEST_SOURCES)
        add_executable(platform_test ${TEST_SOURCES})
        target_link_libraries(platform_test PRIVATE platform)
        
        set_target_properties(
            platform_test PROPERTIES
            RUNTIME_OUTPUT_DIRECTORY "${SABUROU_OUTPUT_DIR}"
            OUTPUT_NAME "saburou_test"
        )
    endif()
endif()

# 3. Reglas de Exportación e Instalación
# ------------------------------------------------------------------------------
install(TARGETS platform EXPORT saburou-platform-targets)

install(
    DIRECTORY saburou_platform/include/
    DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
)

install(
    EXPORT saburou-platform-targets
    FILE saburou-platformConfig.cmake
    NAMESPACE saburou::
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/saburou-platform
)
