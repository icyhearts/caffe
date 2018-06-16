#!/usr/bin/python3
# *-* coding: utf-8 *-*
# aim: write base name and label to train.txt and cat.txt 
# date:
# idea:
import numpy as np
import os
labels = [str(i) for i in range(10)]
# pathSpliter = "/"
# trainNoLabelTxt = "texts/trainNoLabel.txt"
# trainLabelTxt = "texts/train.txt"
testNoLabelTxt = "examples/mnist/testNolabel.txt"
testLabelTxt = "examples/mnist/test.txt"
class ioLabel(object):
	def __init__(self, inText, outText, labels):
		self.inText = inText
		self.outText = outText
		self.labels = labels
	def writeToList(self):
		inFP = open(self.inText, "r")
		outFP = open(self.outText, "w")
		while 1:
			line = inFP.readline()
			if line=="": # line="" means end of file
				break
			line = line.strip()
			if line=="": # after strip, line=="": white line
				continue
			if (line[0]=='#'): # skip comment line
				continue
			strBuf = line
			baseName = os.path.basename(strBuf)
			currentStrLabel =  baseName.split('.')[-2]
			strBuf += " "+currentStrLabel + "\n"
			outFP.write(strBuf)
		inFP.close()
		outFP.close()
# trainIoLabel = ioLabel(trainNoLabelTxt, trainLabelTxt, labels)
# trainIoLabel.writeToList()
testIoLabel = ioLabel(testNoLabelTxt, testLabelTxt, labels)
testIoLabel.writeToList()
