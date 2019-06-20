cmake_minimum_required(VERSION 2.6)

INCLUDE_DIRECTORIES(
)

MESSAGE(STATUS "Current Dir: " ${CMAKE_CURRENT_SOURCE_DIR})
FILE(GLOB_RECURSE SRC_LIST "*.c" "*.cpp" "*.h")

#平台无关库
SET(COMMON_LIBS 
	)
	

SET(PROJECT logger)

#设置debug版本输出文件名后缀
set(CMAKE_DEBUG_POSTFIX "d")

#编译
add_library(${PROJECT} SHARED ${SRC_LIST})


#连接
target_link_libraries(${PROJECT} ${COMMON_LIBS})
