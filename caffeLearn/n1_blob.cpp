#include <iostream>  
#include<vector>
#include <caffe/blob.hpp>  

using namespace caffe;  
using namespace std;  

int main(void)  
{  
	Blob<float> a;  

	cout<<"Size :"<<a.shape_string()<<endl;  
	std::vector<int> shape={1,2,3,4};
//	a.Reshape(1,2,3,4);  
	a.Reshape(shape);  

	cout<<"Size :"<<a.shape_string()<<endl;  

	float *p = a.mutable_cpu_data();  

	for(int i = 0; i < a.count(); i ++)  
	{  
		p[i] = i* pow(-1, i);  
	}  

	for(int u = 0; u < a.shape(0); u ++)   // Deprecated legacy shape accessor num: use shape(0) instead. 
	{  
		for(int v = 0; v < a.shape(1); v ++)   
		{  
			for(int w = 0; w < a.shape(2); w ++)  
			{  
				for(int x = 0; x < a.shape(3); x ++)  
				{  
					cout<<"a["<<u<<"]["<< v << "][" << w << "][" << x << "] =" << a.data_at(u,v,w,x)<< endl;  
				}  
			}  
		}  
	}  
	float asum = a.asum_data();
	cout<<"asum="<<asum<<endl;
	return 0;   
}  
