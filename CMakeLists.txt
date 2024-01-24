cmake_minimum_required(VERSION 3.27)
project(BDP_Server)

set(CMAKE_CXX_STANDARD 14)

include_directories(lib/enet/include)
include_directories(lib/enet/include/enet)

# Link against the static ENet library
add_library(enet STATIC IMPORTED)
set_target_properties(enet PROPERTIES IMPORTED_LOCATION ${CMAKE_SOURCE_DIR}/lib/enet/.libs/libenet.a)

set(SOURCE_FILES
        main.cpp
        server/source/server.cpp
)

set(HEADERS
        server/content/playerUpdate.h
        server/content/server.h
)

# Add your executable and link against ENet
add_executable(BDP_Server ${SOURCE_FILES} ${HEADERS})
target_link_libraries(BDP_Server PRIVATE enet)