#!/usr/bin/python3
# *-* coding: utf-8 *-*
# aim: predict lenet mnist, read jpg
# date:
# idea:
import sys
sys.path.insert(0, "/usr/local/caffe/python/")
import caffe  
import numpy as np  
import os
labels = [str(i) for i in range(10)]
deploy = 'examples/mnist/lenet-deploy.prototxt'
caffe_model ='examples/mnist/saved/lenet__iter_10000.caffemodel'  
net = caffe.Net(deploy,caffe_model,caffe.TEST)   #加载model和network  
#图片预处理设置  
transformer = caffe.io.Transformer({'data': net.blobs['data'].data.shape})  #设定图片的shape格式(1,3,28,28)  
transformer.set_transpose('data', (2,0,1))    #改变维度的顺序，由原始图片(28,28,1)变为(1,28,28)  
testListFile = "examples/mnist/test.txt"
readFP = open(testListFile, "r")
TEST_RESULT = "examples/mnist/testResult.txt"
writeFP = open(TEST_RESULT, "w")
while 1:
	line = readFP.readline()
	if line=="": # line="" means end of file
		break
	line = line.strip()
	if line=="": # after strip, line=="": white line
		continue
	if (line[0]=='#'): # skip comment line
		continue
	filename, realLabel = line.split(' ')
	IMAGE_FILE, realLabel = filename.strip(), realLabel.strip()
	try:
		input_image = caffe.io.load_image(IMAGE_FILE, color=False) #input_image: numpy array, shape:(28, 28, 1)
		print(IMAGE_FILE,", max value=", np.max(input_image))
	except Exception as e:
		print(e)
		exit(1)
	net.blobs['data'].data[...] = transformer.preprocess('data', input_image) #执行上面设置的图片预处理操作，并将图片载入到blob中
	out = net.forward()
	prediction = net.blobs['prob'].data
	high2lowOrder =  prediction[0].argsort()[::-1]
	[pred1, pred2, pred3] = high2lowOrder[0:3]
	predLabels = [labels[pred1], labels[pred2], labels[pred3] ]
	lineBuf = IMAGE_FILE+" {}|| {}|{}|{}".format(realLabel, predLabels[0], predLabels[1], predLabels[2] )
	if realLabel != predLabels[0]:
		lineBuf += " |<ERROR1>"
	if realLabel not in predLabels:
		lineBuf += " |<ERROR3>"
	lineBuf += "\n"
	writeFP.write(lineBuf)
	print(lineBuf, end="")
writeFP.close()
readFP.close()
