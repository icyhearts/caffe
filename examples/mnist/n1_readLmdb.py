#!/usr/bin/python3
# *-* coding: utf-8 *-*
# aim: write base name and label to train.txt and cat.txt 
# date:
# idea:
import caffe
import lmdb
import numpy as np
import cv2
from caffe.proto import caffe_pb2

import numpy as np
import os
# lmdb_env = lmdb.open('traindb/')
class DbReader(object):
	def __init__(self, lmdbPath, writeDir):
		self.lmdbPath = lmdbPath
		self.writeDir = writeDir
	def write(self):
		lmdb_env = lmdb.open(self.lmdbPath)
		lmdb_txn = lmdb_env.begin()
		lmdb_cursor = lmdb_txn.cursor()
		datum = caffe_pb2.Datum()
		iImg = 0
		for key, value in lmdb_cursor:
			datum.ParseFromString(value)
			label = datum.label
			data = caffe.io.datum_to_array(datum)
			#CxHxW to HxWxC in cv2
			image = np.transpose(data, (1,2,0))
			filename = os.path.join(self.writeDir, str(iImg)+"."+str(label)+".jpg")
			cv2.imwrite(filename, image)
			iImg += 1
lmdbPath = "/home/ice/packageLinux/caffe/caffeDebug/examples/mnist/mnist_train_lmdb/"
writeDir = "/home/storage/datasetAcademic/mnistDataset/jpgs/train"
dbReader = DbReader(lmdbPath, writeDir)
dbReader.write()
lmdbPath = "/home/ice/packageLinux/caffe/caffeDebug/examples/mnist/mnist_test_lmdb/"
writeDir = "/home/storage/datasetAcademic/mnistDataset/jpgs/test/"
dbReader = DbReader(lmdbPath, writeDir)
dbReader.write()
