build_type = $1
if [ $build_type == "-d" ];then
	build_type = "Debug"
else
	build_type = "Release"
fi

cmake -DCMAKE_BUILD_TYPE=$build_type -DTARGET_BIT=64  -DPLAT=LINUX-X86_64 ../../
