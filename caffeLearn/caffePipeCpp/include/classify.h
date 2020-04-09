#ifndef  CLASSIFY_H
#define CLASSIFY_H
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

/* Pair (label, confidence) representing a prediction. */
typedef std::pair<string, float> Prediction;

class Classifier {
	public:
		Classifier(const string& model_file,
				const string& trained_file,
				const string& mean_file,
				const string& label_file);

		std::vector<std::vector<Prediction> > Classify(const std::vector<cv::Mat>& imgs, int N = 5);
		vector<string> getLabels(){return labels_; }

	private:
		void SetMean(const string& mean_file);

		std::vector<std::vector<float> > Predict(const std::vector<cv::Mat>& imgs);

		void WrapInputLayer(std::vector<cv::Mat>* input_channels, int n);

		void Preprocess(const cv::Mat& img,
				std::vector<cv::Mat>* input_channels);

	private:
		shared_ptr<Net<float> > net_;
		cv::Size input_geometry_;
		int num_channels_;
		cv::Mat mean_;
		std::vector<string> labels_;
};
struct MyRect{
	cv::Point leftTop;
	cv::Point rightBottom;
};
#endif // CLASSIFY_H
