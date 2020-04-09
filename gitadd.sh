#!/bin/bash
git add gitadd.sh examples/cifar10/cifar10_quick_solver.prototxt examples/cifar10/cifar10_quick_train_test.prototxt examples/cifar10/create_cifar10.sh examples/mnist/lenet_solver.prototxt python/draw_net.py examples/cpp_classification/n1_classificationBatch.cpp
git add examples/cpp_classification/n2_selfClassificationBatch.cpp examples/mnist/lenet-deploy.prototxt
git add examples/mnist/n1_readLmdb.py examples/mnist/n2_writeListTxt.py examples/mnist/n3_predictNet.py
git add cmake/Dependencies.cmake
