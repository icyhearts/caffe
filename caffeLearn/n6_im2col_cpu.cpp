#include <iostream>  
#include "caffe/util/im2col.hpp"
#include<vector>
#include <caffe/blob.hpp>  

//using namespace caffe;  
using namespace std;  
using caffe::Blob;
template<typename Dtype>
void printBlob(const caffe::Blob<Dtype> & a)
{
	int n,c,h,w;
	string tmp;
	cout<<"[";//wrap
	for(n = 0; n < a.shape(0); n ++)   // Deprecated legacy shape accessor num: use shape(0) instead. 
	{
		cout<<"[";// one block=one channel
		for(c = 0; c < a.shape(1); c ++){  
			cout<<"[";//1 block=1 row
			for(h = 0; h < a.shape(2); h ++){
//				tmp = (h == 0) ? ("[") : ("  [");
				tmp = "[";
				cout<<tmp;
				for(w = 0; w < a.shape(3)-1; w ++){  
					cout<<a.data_at(n,c,h,w)<<",";
				}//w 
					cout<<a.data_at(n,c,h,w);
				tmp = (h == a.shape(2)-1) ? ("]") : ("],\n");
				cout<<tmp;
//				cout<<"]\n";
			}
			tmp = (c == a.shape(1)-1) ? ("]") : ("],\n\n");
			cout<<tmp;//1 block=1 row
		}
		tmp = (n==a.shape(0)-1) ? ("]") : ("],\n\n\n");
		cout<<tmp ;
	}cout<<"]\n";//out wrap
}




int main(void)  
{  
	Blob<float> a;  

	cout<<"Size :"<<a.shape_string()<<endl;  
	std::vector<int> shape={2,2,5,5};
	a.Reshape(shape);

	cout<<"Size :"<<a.shape_string()<<endl;  

	float *p = a.mutable_cpu_data();  

	for(int i = 0; i < a.count(); i ++)  
	{  
		p[i] = 1+i;
	}  
	printBlob(a);
	//
	int cnt1image = a.count(1);
	int Nin = a.shape(0), Cin = a.shape(1);
	int Hin = a.shape(2), Win = a.shape(3);
	vector<int> pad_ = {1,1};
	vector<int> dilation_ = {2,2};
	vector<int> kernel_ = {3,3};
	vector<int> stride_ = {2,2};

	const int output_h = (Hin + 2 * pad_[0] -
			(dilation_[0] * (kernel_[0] - 1) + 1)) / stride_[0] + 1;
	const int output_w = (Win + 2 * pad_[1] -
			(dilation_[1] * (kernel_[1] - 1) + 1)) / stride_[1] + 1;

	vector<int> col_buffer_shape_ = {1, Cin*kernel_[0]*kernel_[1], output_h, output_w};
	caffe::Blob<float> col_buffer_;
	col_buffer_.Reshape(col_buffer_shape_);



	cout<<cnt1image<<" elements per image=CxHxW\n";
	const float *data_im = a.cpu_data();
	float *data_col = col_buffer_.mutable_cpu_data();
	for(int n=0; n<Nin; ++n){
		caffe::im2col_cpu<float>(data_im, Cin,
				Hin, Win, kernel_[0], kernel_[1],
				pad_[0], pad_[1],
				stride_[0], stride_[1],
				dilation_[0], dilation_[1],
				data_col);
		cout<<"------------------------------------------------------------"<<endl;
		vector<int> tmp_shape = {1,Cin, kernel_[0]*kernel_[1], output_h*output_w};
		col_buffer_.Reshape(tmp_shape);
		printBlob(col_buffer_);
		col_buffer_.Reshape(col_buffer_shape_);
		data_im += cnt1image;
	}
	
	return 0;   
}
