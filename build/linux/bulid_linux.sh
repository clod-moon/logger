if [[ $1 == "-d" ]];then
	build_type="Debug"
else
	build_type="Release"
fi

echo $build_type

cmake -DCMAKE_BUILD_TYPE=$build_type -DTARGET_BIT=64  -DPLAT=LINUX-X86_64 ../../
