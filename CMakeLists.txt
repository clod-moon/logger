# 最顶层的cmake
# 需要定义：
# CMAKE_BUILD_TYPE=Debug/Release

IF(NOT CMAKE_BUILD_TYPE)
	SET(CMAKE_BUILD_TYPE Debug)	
ENDIF()

cmake_minimum_required(VERSION 2.6)
project(Logger)

message(STATUS "project dir:" ${PROJECT_SOURCE_DIR})
message(STATUS "build type is:" ${CMAKE_BUILD_TYPE})
message(STATUS "platform is:" ${PLAT})

# 指定编译
if(WIN32)
	add_definitions(-DWIN32_LEAN_AND_MEAN)
else()
	set(CMAKE_SHARED_LINKER_FLAGS 	"${CMAKE_SHARED_LINKER_FLAGS} -Wl,-rpath=.")
	set(CMAKE_EXE_LINKER_FLAGS 		"${CMAKE_EXE_LINKER_FLAGS} -Wl,-rpath=.")
	add_definitions(-fPIC -DLINUX)
	add_definitions(-m64 -fpermissive)
	add_definitions(-DLINUX-X86_64)
	set(CMAKE_C_COMPILER gcc)
	set(CMAKE_CXX_COMPILER g++)
	set(CMAKE_SHARED_LINKER_FLAGS 	"${CMAKE_SHARED_LINKER_FLAGS} -m64")
	set(CMAKE_EXE_LINKER_FLAGS 		"${CMAKE_EXE_LINKER_FLAGS} -m64")
	ADD_DEFINITIONS(-DLINUX_X86_64)
endif(WIN32)



#指定debug的共同选项
if(CMAKE_BUILD_TYPE STREQUAL "Debug")
	add_definitions(-DDEBUG)
	if(NOT WIN32)
		add_definitions(-g)
	endif()
elseif(CMAKE_BUILD_TYPE STREQUAL "Release")
	add_definitions(-O2)
else()
	message(FATAL_ERROR "请定义 CMAKE_BUILD_TYPE=Debug/Release")		
endif()

#设定全局选项
SET(CMAKE_SKIP_RPATH ON)

# 设置目录
set(DIR_INCLUDE ${PROJECT_SOURCE_DIR}/logger)

if(WIN32)
	link_directories(${PROJECT_BINARY_DIR}/lib/win ${PROJECT_BINARY_DIR}/lib/win)##这个还不能按config区分，所以debug和release路径都加上了	
	set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY_DEBUG 	${PROJECT_BINARY_DIR}/lib/win)
	set(CMAKE_LIBRARY_OUTPUT_DIRECTORY_DEBUG 	${PROJECT_BINARY_DIR}/bin/win)
	set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG 	${PROJECT_BINARY_DIR}/bin/win)
	set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY_RELEASE 	${PROJECT_BINARY_DIR}/lib/win)
	set(CMAKE_LIBRARY_OUTPUT_DIRECTORY_RELEASE 	${PROJECT_BINARY_DIR}/bin/win)
	set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE 	${PROJECT_BINARY_DIR}/bin/win)

else(PLAT STREQUAL "LINUX-X86_64")
	if(CMAKE_BUILD_TYPE STREQUAL "Debug")
		link_directories(${PROJECT_BINARY_DIR}/lib)
		set(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} -Wl,-rpath-link=${PROJECT_BINARY_DIR}/lib/linux -Wl,-rpath-link=${PROJECT_BINARY_DIR}/bin/linux")
		set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -Wl,-rpath-link=${PROJECT_BINARY_DIR}/lib/linux -Wl,-rpath-link=${PROJECT_BINARY_DIR}/bin/linux")
		set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY_DEBUG ${PROJECT_BINARY_DIR}/lib/linux)
		set(CMAKE_LIBRARY_OUTPUT_DIRECTORY_DEBUG ${PROJECT_BINARY_DIR}/bin/linux)
		set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG ${PROJECT_BINARY_DIR}/bin/linux)
	elseif(CMAKE_BUILD_TYPE STREQUAL "Release")		
		link_directories(${PROJECT_BINARY_DIR}/lib/linux ${PROJECT_BINARY_DIR}/bin/linux)
		set(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} -Wl,-rpath-link=${PROJECT_BINARY_DIR}/lib/linux -Wl,-rpath-link=${PROJECT_BINARY_DIR}/bin/linux")
		set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -Wl,-rpath-link=${PROJECT_BINARY_DIR}/lib/linux -Wl,-rpath-link=${PROJECT_BINARY_DIR}/bin/linux")
		set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY_RELEASE ${PROJECT_BINARY_DIR}/lib/linux)
		set(CMAKE_LIBRARY_OUTPUT_DIRECTORY_RELEASE ${PROJECT_BINARY_DIR}/bin/linux)
		set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE ${PROJECT_BINARY_DIR}/bin/linux)
	endif()
endif(WIN32)

add_definitions(-std=c++11)

message(STATUS " Include dir: " ${DIR_INCLUDE})
message(STATUS " Build lib dir " ${LIBRARY_OUTPUT_PATH})
message(STATUS " Build exe dir" ${EXECUTABLE_OUTPUT_PATH})


add_subdirectory(example)
add_subdirectory(logger)







