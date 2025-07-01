cmake_minimum_required(VERSION 3.16)

# ─── Project Metadata ─────────────────────────────
project(snapshot_publisher
    VERSION 0.1.0
    DESCRIPTION "RTSP snapshot capture and event pipeline"
    LANGUAGES CXX
)

# ─── Options ──────────────────────────────────────
option(ENABLE_TESTING "Enable unit testing" ON)
option(ENABLE_COVERAGE "Enable coverage flags" OFF)
option(ENABLE_OPENCV "Enable OpenCV support" OFF)
option(ENABLE_AWS_SDK "Enable AWS SDK support" OFF)

# ─── Default Build Type ───────────────────────────
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Debug CACHE STRING "Build type" FORCE)
endif()

# ─── Compiler Settings ────────────────────────────
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

if(MSVC)
    add_compile_options(/W4 /WX)
else()
    add_compile_options(-Wall -Wextra -Wpedantic)
    if(ENABLE_COVERAGE)
        add_compile_options(--coverage)
        link_libraries(--coverage)
    endif()
endif()

# ─── Export compile_commands.json ────────────────
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# ─── Source Files ─────────────────────────────────
file(GLOB_RECURSE SRC_FILES
    "${CMAKE_SOURCE_DIR}/src/core/*.cpp"
    "${CMAKE_SOURCE_DIR}/src/service/*.cpp"
    "${CMAKE_SOURCE_DIR}/src/infra/*.cpp"
    "${CMAKE_SOURCE_DIR}/src/utils/*.cpp"
    "${CMAKE_SOURCE_DIR}/src/types/*.cpp"
    "${CMAKE_SOURCE_DIR}/src/main.cpp"
)

add_executable(${PROJECT_NAME} ${SRC_FILES})

# ─── Include Directories ──────────────────────────
target_include_directories(${PROJECT_NAME} PRIVATE ${CMAKE_SOURCE_DIR}/include)

if(POLICY CMP0167)
    cmake_policy(SET CMP0167 OLD)
endif()

# ─── Dependencies ─────────────────────────────────

## ── spdlog Logging ──────────────────────────────
find_package(spdlog REQUIRED)
message(STATUS "Found spdlog")
target_link_libraries(${PROJECT_NAME} PRIVATE spdlog::spdlog)
target_compile_definitions(${PROJECT_NAME} PRIVATE ENABLE_LOGGING)

## ── Boost (UUID) ────────────────────────────────
find_package(Boost REQUIRED COMPONENTS system)
message(STATUS "Found Boost ${Boost_VERSION}")
target_link_libraries(${PROJECT_NAME} PRIVATE Boost::system)
target_compile_definitions(${PROJECT_NAME} PRIVATE ENABLE_BOOST_UUID)

## ── GStreamer ───────────────────────────────────
find_package(PkgConfig REQUIRED)
pkg_check_modules(GSTREAMER REQUIRED 
    gstreamer-1.0>=1.0
    gstreamer-video-1.0>=1.0
    gstreamer-app-1.0>=1.0
)
message(STATUS "Found GStreamer ${GSTREAMER_VERSION}")
target_include_directories(${PROJECT_NAME} PRIVATE ${GSTREAMER_INCLUDE_DIRS})
target_link_directories(${PROJECT_NAME} PRIVATE ${GSTREAMER_LIBRARY_DIRS})
target_link_libraries(${PROJECT_NAME} PRIVATE ${GSTREAMER_LIBRARIES})
target_compile_options(${PROJECT_NAME} PRIVATE ${GSTREAMER_CFLAGS_OTHER})

## ── ZeroMQ ──────────────────────────────────────
pkg_check_modules(ZMQ REQUIRED libzmq>=4.0)
message(STATUS "Found ZeroMQ ${ZMQ_VERSION}")
target_include_directories(${PROJECT_NAME} PRIVATE ${ZMQ_INCLUDE_DIRS})
target_link_directories(${PROJECT_NAME} PRIVATE ${ZMQ_LIBRARY_DIRS})
target_link_libraries(${PROJECT_NAME} PRIVATE ${ZMQ_LIBRARIES})
target_compile_options(${PROJECT_NAME} PRIVATE ${ZMQ_CFLAGS_OTHER})

## ── Optional: OpenCV ────────────────────────────
if(ENABLE_OPENCV)
    find_package(OpenCV REQUIRED)
    message(STATUS "Found OpenCV ${OpenCV_VERSION}")
    target_include_directories(${PROJECT_NAME} PRIVATE ${OpenCV_INCLUDE_DIRS})
    target_link_libraries(${PROJECT_NAME} PRIVATE ${OpenCV_LIBS})
    target_compile_definitions(${PROJECT_NAME} PRIVATE ENABLE_OPENCV)
endif()

## ── Optional: AWS SDK ───────────────────────────
if(ENABLE_AWS_SDK)
    find_package(AWSSDK REQUIRED COMPONENTS s3 core)
    message(STATUS "Found AWS SDK")
    target_link_libraries(${PROJECT_NAME} PRIVATE ${AWSSDK_LINK_LIBRARIES})
    target_compile_definitions(${PROJECT_NAME} PRIVATE ENABLE_AWS_SDK)
endif()

## ── Platform-specific ───────────────────────────
if(APPLE)
    find_library(COREFOUNDATION_LIBRARY CoreFoundation)
    find_library(SECURITY_LIBRARY Security)
    target_link_libraries(${PROJECT_NAME} PRIVATE ${COREFOUNDATION_LIBRARY} ${SECURITY_LIBRARY})
elseif(UNIX)
    target_link_libraries(${PROJECT_NAME} PRIVATE pthread dl)
endif()

# ─── Installation ─────────────────────────────────
install(TARGETS ${PROJECT_NAME} RUNTIME DESTINATION bin)
install(DIRECTORY include/ DESTINATION include FILES_MATCHING PATTERN "*.h" PATTERN "*.hpp")

# ─── Testing ──────────────────────────────────────
if(ENABLE_TESTING)
    enable_testing()
    if(EXISTS "${CMAKE_SOURCE_DIR}/tests/CMakeLists.txt")
        add_subdirectory(tests)
    else()
        message(STATUS "Tests directory not found, skipping test configuration")
    endif()
endif()

# ─── Print Configuration Summary ──────────────────
message(STATUS "")
message(STATUS "=== Configuration Summary ===")
message(STATUS "Project: ${PROJECT_NAME} v${PROJECT_VERSION}")
message(STATUS "Build type: ${CMAKE_BUILD_TYPE}")
message(STATUS "C++ Standard: ${CMAKE_CXX_STANDARD}")
message(STATUS "Testing: ${ENABLE_TESTING}")
message(STATUS "Coverage: ${ENABLE_COVERAGE}")
message(STATUS "OpenCV: ${ENABLE_OPENCV}")
message(STATUS "AWS SDK: ${ENABLE_AWS_SDK}")
message(STATUS "Boost UUID: ON")
message(STATUS "GStreamer: ${GSTREAMER_VERSION}")
message(STATUS "ZeroMQ: ${ZMQ_VERSION}")
message(STATUS "Boost: ${Boost_VERSION}")
message(STATUS "spdlog: ENABLED")
message(STATUS "===============================")
message(STATUS "")
