macro(sc_target_dependencies)
    include(CTest)

    if(${UNIX})
        sc_linux_target_dependencies()
    elseif(${WIN32})
        message(SEND_ERROR "ostis-metasystem isn't supported on Windows OS.")
    endif(${UNIX})
endmacro()

macro(sc_linux_target_dependencies)
    find_package(nlohmann_json 3.2.0 REQUIRED)

    # for std::thread support
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pthread")
endmacro()
