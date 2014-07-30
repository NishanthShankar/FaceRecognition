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
String earFile = "C:\\OpenCV\\opencv\\sources\\data\\haarcascades\\haarcascade_mcs_rightear.xml";


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

int addd(int x, int y)
{
	return(x + y);
}

int faceDetect(int fileNumber)
{
	HOGDescriptor hog;
	hog.setSVMDetector(HOGDescriptor::getDefaultPeopleDetector());
	
	int big, i, times = 0;
	std::ostringstream oss;
	string path = "C:/LensBricks/Datasets/Office/FaceDatabase/";
	string filename = to_string(fileNumber) + ".avi";
	std::vector<Rect> faceBox; 
	cv::Rect tempBox;
	std::vector<Rect> eyeBox;
	cv::Rect leftEyeBox;
	cv::Rect rightEyeBox;
	cv::Mat frame;
	cv::Mat cropped;
	cv::VideoCapture video(path + filename);
	CascadeClassifier faceClassifier;
	CascadeClassifier earClassifier;
	CascadeClassifier eyeClassifier;
	unsigned int frameNumber = 0;
	if (!earClassifier.load(earFile))
	{
		cout << " ear xml File Error";
		return 1;

	}
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
		frame = imread("C:/LensBricks/Samples/ear2.jpg");
		flip(frame, frame, 1);
		earClassifier.detectMultiScale(frame, faceBox);
		for (size_t i = 0; i < faceBox.size(); i++)
		{
			cv::rectangle(frame, faceBox[i], Scalar(255, 0, 255));
		}
		
		imshow("Ear detection", frame);
		waitKey();
		double dummyLow, dummyHigh;
		if (frame.empty())
		{
			cout << "No feed";
			return 1;
		}
		big = 0;
		skinFrame=skinD.skinDetect(frame);
		imshow("SkinDetected", skinFrame);
		cv::waitKey(1);
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
				string writePath = "C:/LensBricks/datasets/Office/FaceDatabase/images/" + to_string(fileNumber) + "_" + to_string(frameNumber) + ".png";
				imwrite(writePath,cropped);

			}
			frameNumber++;
		}

	}
	return 1;
}

void peopleDetect()
{
	HOGDescriptor hog;
	CascadeClassifier fullbody("C:/OpenCV/opencv/sources/data/haarcascades/haarcascade_mcs_upperbody.xml");
	hog.setSVMDetector(HOGDescriptor::getDefaultPeopleDetector());
	cv::VideoCapture video("C:\\LensBricks\\Datasets\\EPFL\\4p-c0.avi");
	cv::Mat frame;
	std::vector<cv::Rect> found;
	while (1)
	{
		video >> frame;
		cv::Mat ups;
		//hog.detectMultiScale(frame, found, 0);
		fullbody.detectMultiScale(frame, found);
		for (size_t i = 0; i < found.size(); i++)
		{
			cv::rectangle(frame, found[i], cv::Scalar(255, 0, 255), 2);

		}
		imshow("frame", frame);
		cv::waitKey(1);
	}
}
void main()
{
	cv::VideoCapture cam1(2);
	cv::VideoCapture cam2(1);
	CascadeClassifier sideFace("C:/OpenCV/opencv/sources/data/haarcascades/haarcascade_profileface.xml");
	CascadeClassifier frontFace("C:\\OpenCV\\opencv\\sources\\data\\haarcascades\\haarcascade_frontalface_alt2.xml");
	cv::VideoWriter frontCap, sideCap;
	Mat fram1, fram2,skinFrame;
	cam1 >> fram1;
	cam2 >> fram2;
	std::vector<cv::Rect> sideBox;
	std::vector<cv::Rect> frontBox;
	frontCap.open("frontCap.avi", CV_FOURCC('D', 'I', 'V', 'X'), 30, fram1.size(), true);
	sideCap.open("sideCap.avi", CV_FOURCC('D', 'I', 'V', 'X'), 30, fram2.size(), true);
	int writeCount = 0;
	while (1)
	{
		cam1 >> fram1;
		cam2 >> fram2;
		
		sideFace.detectMultiScale(fram2, sideBox);
		skinFrame = skinD.skinDetect(fram1);
		frontFace.detectMultiScale(skinFrame, frontBox);
		if (sideBox.size() > 0 || frontBox.size() > 0)
			writeCount = 20;
		if (writeCount--)
		{
			frontCap.write(fram1);
			sideCap.write(fram2);
		}
	}	
	int length = 60;
	for (size_t i = 4; i < length; i++)
	{
		faceDetect(i);
	}
}