#include <caffe/caffe.hpp>
#include"caffe/common.hpp"
#include"caffe/net.hpp"
#include <memory>
#include "caffe/layers/memory_data_layer.hpp"

#include<iostream>
using namespace std;
#define MYDEBUG

int main()
{
//	float *data = new float[64*1*1*3*400];
	caffe::Caffe::set_random_seed(21);
	float *data = new float[64*1*1*2*400];
	float *label = new float[64*1*1*1*400];


	for(int i = 0; i<64*1*1*400; ++i)
	{
		int a = rand() % 2;
        int b = rand() % 2;
        int c = a ^ b;
		data[i*2 + 0] = a;
		data[i*2 + 1] = b;
		label[i] = c;
	}

    caffe::SolverParameter solver_param;
    caffe::ReadSolverParamsFromTextFileOrDie("./solver.prototxt", &solver_param);

    boost::shared_ptr<caffe::Solver<float> > solver(caffe::SolverRegistry<float>::CreateSolver(solver_param));
    caffe::MemoryDataLayer<float> *dataLayer_trainnet = (caffe::MemoryDataLayer<float> *) (solver->net()->layer_by_name("inputdata").get());
	# ifdef MYDEBUG
	vector<boost::shared_ptr<caffe::Net<float> > > test_nets_ = solver->test_nets();
	#endif
    caffe::MemoryDataLayer<float> *dataLayer_testnet_ = (caffe::MemoryDataLayer<float> *) (solver->test_nets()[0]->layer_by_name("test_inputdata").get());

    float testab[] = {0, 0, 0, 1, 1, 0, 1, 1};
    float testc[] = {0,     1,    1,    0};
// void MemoryDataLayer<Dtype>::Reset(Dtype* data, Dtype* labels, int n)
    dataLayer_testnet_->Reset(testab, testc, 4);

    dataLayer_trainnet->Reset(data, label, 25600);

    solver->Solve();

    boost::shared_ptr<caffe::Net<float> > testnet;

    testnet.reset(new caffe::Net<float>("./model.prototxt", caffe::TEST));
    testnet->CopyTrainedLayersFrom("savedModel/XOR__iter_80000.caffemodel");

//    testnet->ShareTrainedLayersWith(solver->net().get());

    caffe::MemoryDataLayer<float> *dataLayer_testnet = (caffe::MemoryDataLayer<float> *) (testnet->layer_by_name("test_inputdata").get());

    dataLayer_testnet->Reset(testab, testc, 4);

    testnet->Forward();

    boost::shared_ptr<caffe::Blob<float> > output_layer = testnet->blob_by_name("output");
	#ifdef MYDEBUG
    boost::shared_ptr<caffe::Net<float> > trainnet;

    trainnet.reset(new caffe::Net<float>("./model.prototxt", caffe::TRAIN));
	//
	//
	cout<<"train ----------------------------------------"<<endl;
	cout<<"trainnet->blob_by_name(\"fulldata\")->shape_string()="<<trainnet->blob_by_name("fulldata")->shape_string()<<endl;
	cout<<"trainnet->blob_by_name(\"fakelabel\")->shape_string()="<<trainnet->blob_by_name("fakelabel")->shape_string()<<endl;

	cout<<"trainnet->blob_by_name(\"fc6\")->shape_string()="<<trainnet->blob_by_name("fc6")->shape_string()<<endl;
	cout<<"trainnet->blob_by_name(\"loss\")->shape_string()="<<trainnet->blob_by_name("loss")->shape_string()<<endl;
	//
	cout<<"test ----------------------------------------"<<endl;
	cout<<"testnet->blob_by_name(\"fulldata\")->shape_string()="<<testnet->blob_by_name("fulldata")->shape_string()<<endl;
	cout<<"testnet->blob_by_name(\"fakelabel\")->shape_string()="<<testnet->blob_by_name("fakelabel")->shape_string()<<endl;

	cout<<"testnet->blob_by_name(\"fc6\")->shape_string()="<<testnet->blob_by_name("fc6")->shape_string()<<endl;
	cout<<"testnet->blob_by_name(\"output\")->shape_string()="<<testnet->blob_by_name("output")->shape_string()<<endl;
	cout<<"testnet->blob_by_name(\"loss\")->shape_string()="<<testnet->blob_by_name("loss")->shape_string()<<endl;
	#endif

    const float* begin = output_layer->cpu_data();
    const float* end = begin + 4;
    
    std::vector<float> result(begin, end);
	float totalError = 0.;
    for(int i = 0; i< result.size(); ++i)
    {
    	printf("input: %d xor %d,  truth: %f result by nn: %f\n", (int)testab[i*2 + 0], (int)testab[i*2+1], testc[i], result[i]);
		totalError += fabs(testc[i]-result[i]);
    }
	printf("totalError=%f\n", totalError);

	return 0;
}
