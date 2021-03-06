
if(NOT MSVC)
    string(APPEND STDGPU_HOST_FLAGS " -Wall")
    string(APPEND STDGPU_HOST_FLAGS " -pedantic")
    string(APPEND STDGPU_HOST_FLAGS " -Wextra")
    if(${CMAKE_BUILD_TYPE} MATCHES "Release" OR ${CMAKE_BUILD_TYPE} MATCHES "MinSizeRel")
        string(APPEND STDGPU_HOST_FLAGS " -O3")
    endif()
    string(APPEND STDGPU_HOST_FLAGS " -Wno-format")
else()
    #string(APPEND STDGPU_HOST_FLAGS " /W3") # or /W4 depending on how useful this is
    #string(APPEND STDGPU_HOST_FLAGS " /O2")
endif()

# Apply compiler flags
string(APPEND CMAKE_CXX_FLAGS ${STDGPU_HOST_FLAGS})

message(STATUS "Created host flags : ${STDGPU_HOST_FLAGS}")
message(STATUS "Building with CXX flags : ${CMAKE_CXX_FLAGS}")

# Auxiliary compiler flags for tests to be used with target_compile_options
if(NOT MSVC)
    set(STDGPU_TEST_HOST_FLAGS "$<$<COMPILE_LANGUAGE:CXX>:-Wno-deprecated-declarations>")
else()
    set(STDGPU_TEST_HOST_FLAGS "$<$<COMPILE_LANGUAGE:CXX>:/wd4996>")
endif()
