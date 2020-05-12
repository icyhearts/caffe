.PHONY:clean
CXX=g++
DEBUG=1

LIBRARIES=glog gflags boost_system protobuf
COMMON_FLAGS+=-DUSE_OPENCV
OPENCV_FLAGS=`pkg-config --cflags --libs opencv`
INCLUDE_DIRS=/usr/include/refcblas/ /usr/local/caffeDebug/include/ /opt/cuda/include/
INCLUDE_FLAGS:=$(foreach incdir,${INCLUDE_DIRS}, -I${incdir})
ifeq (${DEBUG}, 1)
LIBRARIES+=caffe-d 
COMMON_FLAGS+=-g
LIB_DIRS=/home/ice/packageLinux/caffe/caffeDebug/build/lib/ /home/dice/packageLinux/caffe/caffeDebug/build/lib/

else

LIBRARIES+=caffe
LIB_DIRS=/usr/local/caffe/lib64/

LDFLAGS += $(foreach libdir,${LIB_DIRS}, -L${libdir}) $(foreach library,$(LIBRARIES),-l$(library))
endif
LDFLAGS += $(foreach libdir,${LIB_DIRS}, -L${libdir}) $(foreach library,$(LIBRARIES),-l$(library))
n1_blob.elf:n1_blob.cpp
	${CXX} ${COMMON_FLAGS} ${INCLUDE_FLAGS} ${OPENCV_FLAGS} ${LDFLAGS} $< -o  $@
n2_glogLearn.elf:n2_glogLearn.cpp
	${CXX} ${COMMON_FLAGS} ${INCLUDE_FLAGS} ${OPENCV_FLAGS} ${LDFLAGS} $< -o  $@
bin/n5_namespce.elf:n5_namespce.cpp
	${CXX} ${COMMON_FLAGS} ${INCLUDE_FLAGS} ${OPENCV_FLAGS} ${LDFLAGS} $< -o  $@
bin/n6_im2col_cpu.elf:n6_im2col_cpu.cpp
	${CXX} ${COMMON_FLAGS} ${INCLUDE_FLAGS} ${OPENCV_FLAGS} ${LDFLAGS} $< -o  $@
bin/n7_softmax_layer.elf:n7_softmax_layer.cpp
	${CXX} ${COMMON_FLAGS} -D CPU_ONLY ${INCLUDE_FLAGS} ${OPENCV_FLAGS} ${LDFLAGS} $< -o  $@
bin/n8_booost_thread.elf:n8_booost_thread.cpp
	${CXX} ${COMMON_FLAGS} -D CPU_ONLY ${INCLUDE_FLAGS} ${OPENCV_FLAGS} ${LDFLAGS} $< -o  $@
clean:
	rm -rf cppPipe/obj/*.o cppPipe/bin/*.elf *.elf
