#include <glog/logging.h>

int main(int argc, char* argv[]) {
	// Initialize Google's logging library.
	google::InitGoogleLogging(argv[0]);
	// ...
	int num_cookies=23;
	CHECK_NE(1, 2) << ": The world must be ending!";
	LOG(INFO) << "Found " << num_cookies << " cookies";
}
