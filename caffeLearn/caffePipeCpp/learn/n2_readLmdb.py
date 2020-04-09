import sys
sys.path.insert(0, '/usr/local/caffeDebug/python/')
import numpy as np
import lmdb
import caffe
# read single item
env = lmdb.open('mylmdb', readonly=True)
with env.begin() as txn:
	 raw_datum = txn.get(b'00000000')

datum = caffe.proto.caffe_pb2.Datum()
datum.ParseFromString(raw_datum) # corresponding to SerializeToString

flat_x = np.fromstring(datum.data, dtype=np.uint8)
x = flat_x.reshape(datum.channels, datum.height, datum.width)
y = datum.label
# Iterating <key, value> pairs is also easy:

with env.begin() as txn:
	cursor = txn.cursor()
	for key, value in cursor:
		datum.ParseFromString(value)
		flat_x = np.fromstring(datum.data, dtype=np.uint8)
		x = flat_x.reshape(datum.channels, datum.height, datum.width)
		label = datum.label
		caffeData = caffe.io.datum_to_array(datum) # caffeData is np array of channel x height x width, 和我们手工合成的x是一样的 
# 		print(key, value)
