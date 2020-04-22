import cv2
import caffe
import numpy as np

image = np.zeros((100, 200, 3), dtype=np.uint8)
image[:,:,2] = 255 # red
imageFilename = "red.jpg"
cv2.imwrite(imageFilename, image)

caffeImage = caffe.io.load_image(imageFilename)

# transformer = caffe.io.Transformer({'data': (1,3, image.shape[0], image.shape[1]) })
transformer = caffe.io.Transformer({'data': (1,3, 100, 200) })
transformer.set_raw_scale('data', 255)


# transformer2 = caffe.io.Transformer({'data': (1,3, image.shape[0], image.shape[1]) })
transformer2 = caffe.io.Transformer({'data': (1,3, 100, 200) })
transformer2.set_transpose('data', (2, 0, 1))
transformer2.set_channel_swap('data', (2,1,0))

transformed_image = transformer.preprocess('data', caffeImage)
transformed_image2 = transformer2.preprocess('data', transformed_image)

diff = transformed_image2 - image
