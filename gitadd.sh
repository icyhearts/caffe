#!/bin/bash
git add gitadd.sh examples/cifar10/cifar10_quick_solver.prototxt examples/cifar10/cifar10_quick_train_test.prototxt examples/cifar10/create_cifar10.sh examples/mnist/lenet_solver.prototxt python/draw_net.py examples/cpp_classification/n1_classificationBatch.cpp
git add examples/cpp_classification/n2_selfClassificationBatch.cpp examples/mnist/lenet-deploy.prototxt
git add examples/mnist/n1_readLmdb.py examples/mnist/n2_writeListTxt.py examples/mnist/n3_predictNet.py
git add cmake/Dependencies.cmake
git add readme.html readme.css
git add caffeLearn/cpp_xor/{cpp_xor.cpp,learn.cpp,makefile}
git add caffeLearn/{detectInference.py,n1_blob.cpp,n2_glogLearn.cpp,n3_protobufSimple.cpp,n4_caffeIO_Transform.py,n5_namespce.cpp,makefile} doxygenConfig
git add caffeLearn/caffePipeCpp/learn/n1_lmdb.py caffeLearn/caffePipeCpp/learn/readme.css caffeLearn/caffePipeCpp/learn/n3_lmdb.py caffeLearn/caffePipeCpp/learn/readme.html caffeLearn/caffePipeCpp/learn/n4_readcaffeLmdb.py caffeLearn/caffePipeCpp/learn/n2_readLmdb.py caffeLearn/caffePipeCpp/include/classify.h caffeLearn/caffePipeCpp/include/utils.h caffeLearn/caffePipeCpp/src/classify.c caffeLearn/caffePipeCpp/paraFile.txt
git add gitpush.sh gitpull.sh
git add mycmakePython2.sh  mycmake.sh
git add documentImage/*.png

