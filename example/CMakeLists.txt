cmake_minimum_required(VERSION 2.6)

SET(PROJECT example)
IF(DEFINED  OPS_SOFT_VERSION )
	ADD_DEFINITIONS(-DOPS_VER_NUM="${OPS_SOFT_VERSION}")
	MESSAGE("OPS_VER_NUM  is " ${OPS_SOFT_VERSION})
	ELSE()
	MESSAGE("OPS_VER_NUM  is not set")
ENDIF()

# 库文件的输出路径
#源码
#aux_source_directory(. SRC_LIST)
FILE(GLOB_RECURSE SRC_LIST 
			"*.c" 
			"*.cpp" 
			"*.h" 
			"*.hpp" 
			"*.inl" 
			)

# 头文件
include_directories(
	.
	${DIR_INCLUDE}/
	)

#连接的动态库
SET(COMMON_LIBS
	debug loggerd 		
	optimized logger

	)
#编译
add_executable(${PROJECT} ${SRC_LIST})

#连接
target_link_libraries(${PROJECT}  ${COMMON_LIBS})

#Debug版本输出文件名后缀
set_target_properties(${PROJECT} PROPERTIES DEBUG_POSTFIX "d")


