//#include "opencv2/core/core.hpp"
//#include "opencv2/imgproc/imgproc.hpp"
//#include "opencv2/contrib/contrib.hpp"
//#include "opencv2/highgui/highgui.hpp"
//#include <iostream>
//#include <fstream>
//#include <sstream>
//#include <time.h>
//
//using namespace cv;
//using namespace std;
//
//const std::string currentDateTime() 
//{
//	time_t     now = time(0);
//	struct tm  tstruct;
//	char       buf[80];
//	tstruct = *localtime(&now);
//	strftime(buf, sizeof(buf), "%d-%m-%Y\t%X", &tstruct);
//	return buf;
//}
//int main()
//{
//	cv::Mat testFace;
//	cv::namedWindow("TestImage");
//	cv::namedWindow("RecognizedImage");
//	int idx = 80;
//	string classNames[5] = { "Ashish Rao", "Chintak Sheth", "Natasha Singh", "Pranav Misra", "Vishal Kumar" };
//	string path = "C:\\LensBricks\\Datasets\\Office\\Chintaks\\Input\\";
//	ostringstream s, res;
//	Ptr<FaceRecognizer> model1 = createLBPHFaceRecognizer();
//	model1->load("C:\\LensBricks\\modelLBPH.yml");
//	for (int subject = 0; subject < 5; subject++)
//	{
//		s.str("");
//		s.clear();
//		res.str("");
//		res.clear();
//		s << path << subject << "\\" << idx << ".jpg";
//		testFace = imread(s.str().c_str(), 0);
//		imshow("TestImage", testFace);
//		int predict = -1;
//		double confidence;
//		model1->predict(testFace, predict, confidence);
//		cout << classNames[predict] << "\t" <<currentDateTime() << endl;
//		res << path << predict << "\\1.jpg";
//		testFace = imread(res.str().c_str());
//		imshow("RecognizedImage", testFace);
//		cv::waitKey();
//	}
//	return 0;
//}
//
//// Get current date/time, format is YYYY-MM-DD.HH:mm:ss
