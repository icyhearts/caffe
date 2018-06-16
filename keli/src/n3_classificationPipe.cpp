#include <caffe/caffe.hpp>
#include<fstream>
#include<map>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <algorithm>
#include <iosfwd>
#include <memory>
#include <string>
#include <utility>
#include <vector>

// self
#include"include/utils.h"
#include"include/classify.h"
// end of self
using namespace caffe;  // NOLINT(build/namespaces)
using std::string;
using std::cout;
using std::cerr;
using std::cin;
using std::endl;
using std::ifstream;
using std::ofstream;
using cv::Mat;


int main(int argc, char** argv) {
	ParameterReader paraReader("paraFile.txt");
	string model_file   = paraReader.getData("model_file");
	string trained_file = paraReader.getData("trained_file");
	string mean_file    = paraReader.getData("mean_file");
	string label_file   = paraReader.getData("label_file");
	string testMode = paraReader.getData("testMode");
	if (! testMode.compare("VIDEO_FILE")){
		
	}

	Classifier classifier(model_file, trained_file, mean_file, label_file);
	vector<string> mainLabels = classifier.getLabels();
	//
	string filelist = argv[5];
	ifstream fin(filelist);
	if (!fin)
	{
		cerr<<"parameter file does not exist."<<endl;
		return 1;
	}
	//	map<string, int> imgLabelList;
	vector<pair<string, int> > imgLabelList;
	while(!fin.eof())
	{
		string str;
		getline( fin, str );
		if (str[0] == '#')
		{
			// 以‘＃’开头的是注释
			continue;
		}

		int pos = str.find(" ");
		if (pos == -1)
			continue;
		string imgName = str.substr( 0, pos );
		string label = str.substr( pos+1, str.length() );
		// remove spaces from string
		imgName.erase( std::remove_if(imgName.begin(), imgName.end(), [](char x){return std::isspace(x);}),
				imgName.end());
		label.erase( std::remove_if(label.begin(), label.end(), [](char x){return std::isspace(x);}),
				label.end());
		imgLabelList.push_back(make_pair(imgName, stoi(label)) );
		if ( !fin.good() )
			break;
	}
	fin.close();
	ofstream fout("cppPred.txt");
	for(int i=0; i<imgLabelList.size(); ++i){
		std::vector<cv::Mat> imgs;
		cv::Mat img = cv::imread(imgLabelList[i].first, -1);
		CHECK(!img.empty()) << "Unable to decode image " << imgLabelList[i].first;
		imgs.push_back(img);
		// all_predictions should have batchSize element: <vector<Prediction>>
		std::vector<std::vector<Prediction> > all_predictions = classifier.Classify(imgs);
		std::vector<Prediction>& predictions = all_predictions[0];// predictions has N element
		Prediction p = predictions[0]; // we only need top-1
		string predLabel = p.first;
		string realLabel = mainLabels[ imgLabelList[i].second];
		string imgName = imgLabelList[i].first;
		string lineBuffer = imgName + " |" + realLabel + " |" + predLabel;
		if(!realLabel.compare(predLabel) ){//  == =>0
			lineBuffer += "\n";
		}else{
			lineBuffer += " | ERROR1\n";
		}
		cout<<lineBuffer;
		fout<<lineBuffer;
	}
	fout.close();

}
