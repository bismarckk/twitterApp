cmake_minimum_required(VERSION 2.6)
project(IOT)
option(USE_DEBUG "Włączono z opcją debug" ON)
if(USE_DEBUG)
    add_compile_options(-O0 -g)
else()
    add_compile_options(-O2)
endif()
add_compile_options(-Wall -Wextra -std=c++14 -lssl -lcrypto)
include_directories("${PROJECT_SOURCE_DIR}/Socket")
include_directories("${PROJECT_SOURCE_DIR}/EncodeFunctions")
include_directories("${PROJECT_SOURCE_DIR}/hmacSha1/src/hmac")
set(CURL_LIBRARY "-lcurl")
find_package(CURL REQUIRED)
include_directories("${CURL_INCLUDE_DIR}")

add_subdirectory(rapidjson)
add_subdirectory(hmacSha1)

set(CPP_FILES ${CPPFILES} twitterClient.cpp 
                          httpConstans.cpp oauthConstans.cpp 
                          EncodeFunctions/encodeFunctions.cpp 
                          Socket/httpClient.cpp 
                          TwitsContainer/twittsContainer.cpp)
add_library(Twitter ${CPP_FILES})
set(EXTRA_LIBS ${EXTRALIBS} Twitter Sha  )
add_executable(main main.cpp )
target_link_libraries(main ${EXTRA_LIBS} ${CURL_LIBRARIES})
