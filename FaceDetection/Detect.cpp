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
String eyeFile = "C:\\OpenCV\\opencv\\sources\\data\\haarcascades\\haarcascade_mcs_righteye.xml";

void rotate(cv::Mat& src, double angle, cv::Mat& dst, cv::Point centre = cv::Point())
{
	
	
	int len = std::max(src.cols, src.rows);
	cv::Point2f pt(len / 2., len / 2.);
	cv::Mat r = cv::getRotationMatrix2D(pt, angle, 1.0);
	cv::warpAffine(src, dst, r, cv::Size(len, len));
}

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
		Y_MIN = 0;
		Y_MAX = 255;
		Cr_MIN = 133;
		Cr_MAX = 173;
		Cb_MIN = 77;
		Cb_MAX = 127;
	}

	cv::Mat skinDetect(cv::Mat src)
	{
		cv::Mat dst;
		cv::Mat inter,skinMat;
		cvtColor(src, inter, COLOR_BGR2YCrCb);
		inRange(inter, cv::Scalar(Y_MIN, Cr_MIN, Cb_MIN), cv::Scalar(Y_MAX, Cr_MAX, Cb_MAX), skinMat);
		src.copyTo(dst, skinMat);
		return dst;
	}
}skinD;



int main()
{
	int big, i, times = 0;
	std::ostringstream oss;
	string path = "C:/Users/Nishanth/Downloads/Games/Recordfiles/";
	string filename = path + "1.avi";
	std::vector<Rect> faceBox; 
	cv::Rect tempBox;
	std::vector<Rect> eyeBox;
	cv::Rect leftEyeBox;
	cv::Rect rightEyeBox;
	cv::Mat frame;
	cv::Mat cropped;
	cv::Mat processed;
	cv::Mat rec;
	cv::namedWindow("Input");
	cv::namedWindow("Recognition");
	cv::VideoCapture video("C:/LensBricks/depth.gdp");
	CascadeClassifier faceClassifier;
	CascadeClassifier eyeClassifier;
	eyeClassifier.load(eyeFile);
	if (!faceClassifier.load(faceFile) && !eyeClassifier.load(eyeFile))
	{
		cout << "xml File Error";
		return 1;
	}
	/*if (!video.isOpened())
	{
		cout << "No input found";
		return 1;
	}
	*/
	char CFE = 0;
	cv::Mat skinFrame;
	cv::Scalar lower(0, 135, 100);
	cv::Scalar upper(255, 180, 170);
	while (CFE != 27)
	{
		video >> frame;
		frame = imread("C:/LensBricks/pics/mywonderfulteam/43.jpg");
		double dummyLow, dummyHigh;
		if (frame.empty())
		{
			cout << "No feed";
			return 1;
		}
		big = 0;
		skinFrame=skinD.skinDetect(frame);
		faceClassifier.detectMultiScale(skinFrame, faceBox, 1.1, 3, 0, cv::Size(50, 50));
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
			cropped = frame(faceBox[i]);
			imshow("cropeed", cropped);
			waitKey(1);
			tempBox = faceBox[i];
			tempBox.height =round(tempBox.height/1.75);
			Mat temp;
			temp = frame(tempBox);
			eyeClassifier.detectMultiScale(temp, eyeBox,1.1,2,0);
			tempBox = faceBox[i];
			tempBox.x = tempBox.width / 2;
			tempBox.y = tempBox.height / 2;
			float rotation;
			if (eyeBox.size()>1)
			{
				Point leftPoint(eyeBox[0].x / 2, eyeBox[0].y / 2), rightPoint(eyeBox[1].x / 2, eyeBox[1].y / 2);
				rotation = fastAtan2(rightPoint.y - leftPoint.y, rightPoint.x - leftPoint.x);
				//rotation = -atan2((eyeBox[0].y - eyeBox[1].y)/2, (eyeBox[0].x - eyeBox[1].x)/2);
				//rotation *= 57.295;
				//int trial = distance(leftPoint, rightPoint);
				Mat rotMatrix;
				cv::Size destSize = cropped.size();
				rotMatrix = getRotationMatrix2D(cv::Point(cropped.rows / 2, cropped.cols / 2), rotation, 1);
				warpAffine(cropped, cropped, rotMatrix, cropped.size());
				tempBox.width = floor(0.80*tempBox.width);
				tempBox.height = floor(0.80*tempBox.height);
				tempBox.x = floor(tempBox.x - tempBox.width / 2);
				tempBox.y = floor(tempBox.y - tempBox.height / 2);
				cropped = cropped(tempBox);
				imshow("Rotated", cropped);
				cv::waitKey(1);
			}		
		}

	}
	return 1;
}