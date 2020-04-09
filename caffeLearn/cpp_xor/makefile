.PHONY:clean all
CXX=g++
DEBUG=1
OPTIMIZE_FLAG=
LIB_DIRS=
LIBRARIES=glog gflags protobuf boost_thread boost_system boost_filesystem pthread
INCLUDE_DIRS=/usr/include/refcblas/ /usr/local/caffeDebug/include/ /opt/cuda/include/
INCLUDE_FLAGS:=$(foreach incdir,${INCLUDE_DIRS}, -I${incdir})
OPENCV_FLAGS=`pkg-config --cflags --libs opencv`
COMMON_FLAGS=${OPTIMIZE_FLAG} -DUSE_OPENCV -std=c++11
ifeq (${DEBUG}, 1)

COMMON_FLAGS+=-g
LIBRARIES+=caffe-d  
LIB_DIRS+=/home/ice/packageLinux/caffe/caffeDebug/build/lib/

else

LIBRARIES+=caffe
LIB_DIRS+=/usr/local/caffe/lib64/
endif
LDFLAGS=$(foreach libdir,${LIB_DIRS}, -L${libdir}) $(foreach library,$(LIBRARIES),-l$(library))
all:cppPipe/bin/n3_classificationPipe.elf doTestCamera.elf

cpp_xor.elf:cpp_xor.cpp
	ccache ${CXX} ${COMMON_FLAGS} ${INCLUDE_FLAGS} ${OPENCV_FLAGS} ${LDFLAGS} $^ -o  $@
learn.elf:learn.cpp
	ccache ${CXX} ${COMMON_FLAGS} ${INCLUDE_FLAGS} ${OPENCV_FLAGS} ${LDFLAGS} $^ -o  $@
clean:
	rm -rf cppPipe/obj/*.o cppPipe/bin/*.elf *.elf tmp/*.elf
