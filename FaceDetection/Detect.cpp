#include<opencv2/highgui/highgui.hpp>
#include<opencv2/objdetect/objdetect.hpp>
#include<opencv2/imgproc/imgproc.hpp>
#include "opencv2/core/core.hpp"
#include "opencv2/contrib/contrib.hpp"
#include <time.h>
#include <iostream>
#include <stdio.h>
#include <fstream>

using namespace std;
using namespace cv;

String faceFile = "C:\\OpenCV\\opencv\\sources\\data\\haarcascades\\haarcascade_frontalface_alt2.xml";
String eyeFile;
//String MouthFile = ;

const std::string currentDateTime()
{
	time_t     now = time(0);
	struct tm  tstruct;
	char       buf[80];
	tstruct = *localtime(&now);
	strftime(buf, sizeof(buf), "%d-%m-%Y\t%X", &tstruct);
	return buf;
}

class SkinDetector
{
public:
	int Y_MIN;
	int Y_MAX;
	int Cr_MIN;
	int Cr_MAX;
	int Cb_MIN;
	int Cb_MAX;
public:
	SkinDetector(void)
	{
		//YCrCb threshold
		// You can change the values and see what happens
		Y_MIN = 0;
		Y_MAX = 255;
		Cr_MIN = 133;
		Cr_MAX = 173;
		Cb_MIN = 77;
		Cb_MAX = 127;
	}
}skinD;

int main()
{
	int big, i, times = 0;
	std::ostringstream oss;
	string path = "C:/Users/Nishanth/Downloads/Games/Recordfiles/";
	string filename = path + "1.avi";
	string classNames[5] = { "Ashish Rao", "Chintak Sheth", "Natasha Singh", "Pranav Misra", "Vishal Kumar" };
	std::vector<Rect> faceBox;
	cv::Mat Inp;
	cv::Mat cropped;
	cv::Mat processed;
	cv::Mat rec;
	cv::namedWindow("Input");
	cv::namedWindow("Recognition");
	cv::VideoCapture video("C:/LensBricks/depth.gdp");
//	cv::VideoCapture video("rtsp://admin:admin@192.168.0.97:88/videoMain");
	CascadeClassifier faceH;
	Ptr<FaceRecognizer> model = createLBPHFaceRecognizer();
	//model->load("C:\\LensBricks\\modelLBPH.yml");
	//model->set("threshold", 81.0);
	if (!faceH.load(faceFile))
	{
		cout << "Face File Error";
		return 1;
	}
	if (!video.isOpened())
	{
		cout << "No input found";
		return 1;
	}
	char CFE = 0;
	cv::Scalar lower(0, 135, 100);
	cv::Scalar upper(255, 180, 170);
	while (CFE != 27)
	{
		video >> Inp;
		double dummyLow, dummyHigh;
		if (Inp.empty())
		{
			cout << "No feed";
			return 1;
		}
		//	imshow("", Inp);
		cv::waitKey(1);
		big = 0;
		cropped = Inp;
		//Inp = imread("C:/LensBricks/skin-colors.jpg");
		//cvtColor(Inp, cropped, COLOR_BGR2YUV);
		//	imshow("Input", cropped);
		//	waitKey(1);
		//cv::inRange(cropped, cv::Scalar(skinD.Y_MIN, skinD.Cr_MIN, skinD.Cb_MIN), cv::Scalar(skinD.Y_MAX, skinD.Cr_MAX, skinD.Cb_MAX), cropped);
		faceH.detectMultiScale(cropped, faceBox, 1.1, 2, 0, cv::Size(50, 50));
		//	imshow("Input", cropped);
		//	waitKey(1);

		if (!faceBox.empty())
		{
			times++;
			for (int idx = 0; idx < faceBox.size(); idx++)
			{
				if (faceBox[idx].width>big)
				{
					big = faceBox[idx].width;
					i = idx;
				}
			}
			//cout << faceBox[i].width<<endl;
			cropped = Inp(faceBox[i]);
			//	imshow("Input", cropped);
			string outPath = path + "/faces/" + std::to_string(times) + ".jpg";
			imwrite(outPath, cropped);
			CFE = cv::waitKey(1);
		}

	}
	return 1;
}