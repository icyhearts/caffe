#include <iostream>  
#include<float.h>
#include "caffe/util/im2col.hpp"
#include "caffe/layers/softmax_layer.hpp"
#include<vector>
#include <caffe/blob.hpp>  

//using namespace caffe;  
using namespace std;  
using caffe::Blob;
template<typename Dtype>
void printBlob3(const caffe::Blob<Dtype> & a)
{
	int c,h,w;
	string tmp;
	int C=a.shape(0), H=a.shape(1), W=a.shape(2);
	const Dtype *p = a.cpu_data();
	for(int c=0; c<C; ++c){
		cout<<"[";
		for(int h=0; h<H; ++h){
			cout<<"[";
			for(int w=0; w<W; ++w){
				int idx = (c*H + h)*W + w;
				cout<<p[idx];
				tmp = (w==W-1) ? ("") : (", ");
				cout<<tmp;
			}
			tmp = (h==H-1) ? ("]") : ("]\n");
			cout<<tmp;
		}
		tmp = (c==C-1) ? ("]\n") : ("]\n\n");
		cout<<tmp;
	}
}




int main(void)  
{  
	Blob<float> a;  
	Blob<float> b;  
	vector<int> shape_ = {2,3,4};
	a.Reshape(shape_);
	int cnt = a.count();
	float *dptr = a.mutable_cpu_data();
	for(int i=0; i<cnt; ++i){
		dptr[i] = i;
	}
	printBlob3(a);

	caffe::SoftmaxParameter softmaxParam();
	caffe::LayerParameter param;
	cout<<"param.has_softmax_param()="<<param.has_softmax_param()<<endl;
	caffe::SoftmaxParameter *pSoftmaxParam = param.mutable_softmax_param();
	cout<<"param.has_softmax_param()="<<param.has_softmax_param()<<endl;
//	pSoftmaxParam->set_axis(1);
	cout<<"softmax axis="<<pSoftmaxParam->axis()<<endl;
	caffe::SoftmaxLayer<float>  softmaxLayer(param);
	vector<caffe::Blob<float>* > bottom = {&a};
	vector<caffe::Blob<float>* > top = {&b};
	softmaxLayer.Reshape(bottom, top);
	softmaxLayer.Forward(bottom, top);
	printBlob3(b);
	float fmin = FLT_MIN;
	cout<<"fmin="<<fmin<<endl;
	
	return 0;   
}
