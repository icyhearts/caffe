#include <caffe/caffe.hpp>
#include <memory>
#include"caffe/blob.hpp"
#include "caffe/layers/memory_data_layer.hpp"
#include "caffe/layers/relu_layer.hpp"

#include<iostream>
using namespace std;
using namespace caffe;
#define MYDEBUG

class Base
{};
int main()
{

//	caffe::ReLULayer< Dtype >::ReLULayer	(	const LayerParameter & 	param	)
	caffe::LayerParameter param;
	cout<<param.type()<<endl;
	param.set_type("ReLU");
	cout<<param.type()<<endl;
	std::shared_ptr<caffe::ReLULayer<float> > reluptr = 
		make_shared<caffe::ReLULayer<float> >(param);
	std::shared_ptr<Base> bptr = std::make_shared<Base> ();
//	void 	SetUp (const vector< Blob< Dtype > *> &bottom, const vector< Blob< Dtype > *> &top)
	vector<caffe::Blob<float> *> bottom = {
	new Blob<float>(2,3,4,5)
	};
	float off = -2.*3*4*5/2;
	float *bp = bottom[0]->mutable_cpu_data();
	for(int i=0; i<2*3*4*5; ++i){
		bp[i] = i+off;
	}
	vector<caffe::Blob<float> *> top = {
	new Blob<float>(2,3,4,5)
	};
	//
//	reluptr->SetUp(bottom, top);
	reluptr->Forward(bottom, top);
//
	for(int u = 0; u < bottom[0]->shape(0); u ++)   // Deprecated legacy shape accessor num: use shape(0) instead. 
	{  
		for(int v = 0; v <bottom[0]->shape(1); v ++)   
		{  
			for(int w = 0; w <bottom[0]->shape(2); w ++)  
			{  
				for(int x = 0; x <bottom[0]->shape(3); x ++)  
				{  
					cout<<"a["<<u<<"]["<< v << "][" << w << "][" << x << "] =" << bottom[0]->data_at(u,v,w,x)<< endl;  
				}  
			}  
		}  
	}  
//
	for(int u = 0; u < top[0]->shape(0); u ++)   // Deprecated legacy shape accessor num: use shape(0) instead. 
	{  
		for(int v = 0; v <top[0]->shape(1); v ++)   
		{  
			for(int w = 0; w <top[0]->shape(2); w ++)  
			{  
				for(int x = 0; x <top[0]->shape(3); x ++)  
				{  
					cout<<"a["<<u<<"]["<< v << "][" << w << "][" << x << "] =" << top[0]->data_at(u,v,w,x)<< endl;  
				}  
			}  
		}  
	}  

	return 0;
}
