#include <caffe/caffe.hpp>
#include<assert.h>
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
#include<sys/time.h>

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
using cv::VideoCapture;
using cv::Range;
using cv::Point;


int main(int argc, char** argv) {
	ParameterReader paraReader("pipe/paraFile.txt");
	string model_file   = paraReader.getData("model_file");
	string trained_file = paraReader.getData("trained_file");
	string mean_file    = paraReader.getData("mean_file");
	string label_file   = paraReader.getData("label_file");
	string testMode = paraReader.getData("testMode");
	Classifier classifier(model_file, trained_file, mean_file, label_file);
	vector<string> mainLabels = classifier.getLabels();
	/*l_rect = [80,150,680,1070] # xmin,ymin, xmax, ymax←
 m_rect = [800,700,1100,1080]←
 r_rect = [1100,150,1800,950]←
 t_rect = [690,40, 1090, 320]←
*/
	int l_rect[4] = {80,150,680,1070};  //  xmin,ymin, xmax, ymax←
	int m_rect[4] = {800,700,1100,1080};
	int r_rect[4] = {1100,150,1800,950};
	int t_rect[4] = {690,40, 1090, 320};
	vector<MyRect> allRect = {
		{Point(80,150), Point(680, 1070)},// left
		{Point(800,700), Point(1100, 1080)},// mid
		{Point(1100,150), Point(1800,950)},// right
		{Point(690,40), Point(1090, 320)}// top
	};
	float abnormThresh = 0.8;
	if (! testMode.compare("VIDEO_FILE")){
		string inVideo = paraReader.getData("inVideo");
		VideoCapture capture(inVideo);
		if(!capture.isOpened())
		{
			cout<<"open video  failed\n";
			return -1;
		}
		size_t frame_count = (int)capture.get(CAP_PROP_FRAME_COUNT);
		Mat frame;
		std::vector<cv::Mat> imgs;
		imgs.clear();
		struct timeval tv_start, tv_stop;
		for(size_t iFrame=0; iFrame<frame_count; ++iFrame){
			gettimeofday(&tv_start, NULL);
			capture>>frame;
			if( frame.empty() ){
				cout<<__LINE__<<": reading frame "<<iFrame<<" failed\n";
			}
			/* cv::Mat subImg = img(cv::Range(0, 100), cv::Range(0, 100)); */
			for(size_t j=0; j<allRect.size(); ++j){
				Mat subMat = frame(
				Range(allRect[j].leftTop.y, allRect[j].rightBottom.y),
				Range(allRect[j].leftTop.x, allRect[j].rightBottom.x)
				);
				imgs.push_back(subMat);
			}
			// all_predictions should have batchSize element: <vector<Prediction>>
			std::vector<std::vector<Prediction> > all_predictions = classifier.Classify(imgs);
			assert(all_predictions.size() == allRect.size());
			// prepare variable
			Prediction patchPred;
			string predLabel; 
			float predProb;
			// rect
			Scalar rectColor(0,0, 255);
			int rectThickness = 2;
			// text
			int textFontface = 1;
			int textFontScale = 3;
			Scalar textColor(255, 0,0);
			int textThick = 1;
			Point textOrgShift(0, -5);
			// end of prepare variable
// we only need top-1, so only extract all_predictions[iPred] [0]
			patchPred = all_predictions[0] [0];// left
			predLabel = patchPred.first;
			predProb = patchPred.second;
			if( (!predLabel.compare("left_abnormal")) && predProb>abnormThresh){
//void rectangle(InputOutputArray img, Point pt1, Point pt2, const Scalar& color, int thickness=1, int lineType=LINE_8, int shift=0 )
//oid putText(InputOutputArray img, const String& text, Point org, int fontFace, double fontScale, Scalar color, int thickness=1, int lineType=LINE_8, bool bottomLeftOrigin=false )
				cv::rectangle(frame, allRect[0].leftTop, allRect[0].rightBottom, rectColor, rectThickness);
				cv::putText(frame, predLabel, allRect[0].leftTop+textOrgShift, textFontface, textFontScale, textColor, textThick);
			
			}
			patchPred = all_predictions[1] [0];// mid
			predLabel = patchPred.first;
			predProb = patchPred.second;
			if( (!predLabel.compare("mid_abnormal")) && predProb>abnormThresh){
				cv::rectangle(frame, allRect[1].leftTop, allRect[1].rightBottom, rectColor, rectThickness);
				cv::putText(frame, predLabel, allRect[1].leftTop+textOrgShift, textFontface, textFontScale, textColor, textThick);
			}
			patchPred = all_predictions[2] [0];// right
			predLabel = patchPred.first;
			predProb = patchPred.second;
			if( (!predLabel.compare("right_abnormal")) && predProb>abnormThresh){
				cv::rectangle(frame, allRect[2].leftTop, allRect[2].rightBottom, rectColor, rectThickness);
				cv::putText(frame, predLabel, allRect[2].leftTop+textOrgShift, textFontface, textFontScale, textColor, textThick);
			}
			patchPred = all_predictions[3] [0];// top
			predLabel = patchPred.first;
			predProb = patchPred.second;
			if( (!predLabel.compare("top_abnormal")) && predProb>abnormThresh){
				cv::rectangle(frame, allRect[3].leftTop, allRect[3].rightBottom, rectColor, rectThickness);
				cv::putText(frame, predLabel, allRect[3].leftTop+textOrgShift, textFontface, textFontScale, textColor, textThick);
			}
//			cv::imshow(testMode, frame);
//			cv::waitKey(1);
			imgs.clear();
			gettimeofday(&tv_stop, NULL);
			unsigned long totalUsec = 1e6 * (tv_stop.tv_sec-tv_start.tv_sec) + tv_stop.tv_usec - tv_start.tv_usec;
			double totalMs = 1.0*totalUsec*1e-3;
			double fps = 1.0 / (totalMs*1e-3);
			cout<<"one total frame, totalMs="<<totalMs<<" ms, fps="<<fps<<" \n";

		}// for(size_t i=0; i<frame_count; ++i)
		capture.release();
	}//if (! testMode.compare("VIDEO_FILE")){
int ppp;
cout<<"enter ppp\n";
cin>>ppp;
	// ///////////////////
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
