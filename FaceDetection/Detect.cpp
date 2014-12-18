#include<opencv2/highgui/highgui.hpp>
#include<opencv2/objdetect/objdetect.hpp>
#include<opencv2/imgproc/imgproc.hpp>
#include "opencv2/core/core.hpp"
#include "opencv2/contrib/contrib.hpp"
#include <time.h>
#include <iostream>
#include <stdio.h>
#include <fstream>
#include <flandmark_detector.h>


using namespace std;
using namespace cv;

String faceFile = "C:\\OpenCV\\opencv\\sources\\data\\haarcascades\\haarcascade_frontalface_alt2.xml";
String profileFaceFile = "C:\\OpenCV\\opencv\\sources\\data\\haarcascades\\haarcascade_profileface.xml";
String eyeFile = "C:\\OpenCV\\opencv\\sources\\data\\haarcascades\\haarcascade_mcs_righteye.xml";
String earFile = "C:\\OpenCV\\opencv\\sources\\data\\haarcascades\\haarcascade_mcs_leftear.xml";
CascadeClassifier fullbody("C:/OpenCV/opencv/sources/data/haarcascades/haarcascade_fullbody.xml");
int addd(int, int);
#if 0
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

int faceDetect(int fileNumber, string view)
{
	int big, i, times = 0;
	std::ostringstream oss;
	string View = view;
	string path = "C:/LensBricks/Datasets/Office/Face/new2/";
	string filename = view + to_string(fileNumber) + ".avi";
	std::vector<Rect> faceBox; 
	cv::Rect tempBox;
	std::vector<Rect> eyeBox;
	cv::Rect leftEyeBox;
	cv::Rect rightEyeBox;
	cv::Mat frame;
	cv::Mat cropped;
	cv::VideoCapture video(path + filename);
	CascadeClassifier faceClassifier;
	CascadeClassifier profileClassifier;
	CascadeClassifier earClassifier;
	CascadeClassifier eyeClassifier;
	unsigned int frameNumber = 0;
	if (!earClassifier.load(earFile))
	{
		cout << " ear xml File Error";
		return 1;
	}
	if (!faceClassifier.load(faceFile))
	{
		return 1;
	}
	if (!eyeClassifier.load(eyeFile))
	{
		return 1;
	}
	
	char CFE = 0;
	cv::Mat skinFrame;
	cv::Scalar lower(0, 135, 100);
	cv::Scalar upper(255, 180, 170);
	cv::Mat	dispFrame;
	while (CFE != 27)
	{
		int side = 0;
		video >> frame;
		if (frame.empty())
		{
			cout << "No feed\n";
			return 1;
		}
		dispFrame = frame.clone();
		skinFrame=skinD.skinDetect(frame);
		if (View == "front")
		{
			faceClassifier.detectMultiScale(frame, faceBox, 1.1,3,0,cv::Size(120,120),cv::Size(190,190));
		}
		if (View == "side")
		{
			earClassifier.detectMultiScale(frame, faceBox), side = 1;
		}
		big = 0;
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
			cv::rectangle(dispFrame, faceBox[i], Scalar(255, 0, 255));
		
			cropped = frame(faceBox[i]);
			tempBox = faceBox[i];
			tempBox.height = round(tempBox.height / 1.75);
			eyeClassifier.detectMultiScale(frame(tempBox), eyeBox, 1.1, 2, 0);
			if (eyeBox.size() > 0 || side == 1 )
			{
				string writePath = "C:/LensBricks/datasets/Office/Face/images3/" + View + "/" + to_string(fileNumber) + "_" + to_string(frameNumber) + ".png";
				resize(cropped, cropped, cv::Size(150, 150));
				imwrite(writePath, cropped);
			}
			frameNumber++;
		}
		cv::imshow("display", dispFrame);
		
		CFE=cv::waitKey(1);
	}
	return 1;
}

void peopleDetect()
{
	HOGDescriptor hog;
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
#endif
int main()
{
	cv::VideoCapture fileInput("C:\\LensBricks\\study\\3.avi");
	cv::VideoWriter writer("C:\\LensBricks\\study\\3new.avi", CV_FOURCC('D', 'I', 'V', 'X'), 30, cv::Size(600, 480));
	cv::Mat frame;
	int frameCount = 30;
	cv::Rect roi(340, 0, 600, 480);
	fileInput.set(CV_CAP_PROP_POS_FRAMES, frameCount);

	while (true)
	{
		fileInput >> frame;
		frameCount++;
		writer << frame(roi); 
		//cv::rectangle(frame, roi, cv::Scalar(255, 0, 0), 2);
		std::cout << "frame number: " << frameCount << std::endl;
		cv::imshow("frame", frame(roi));
		if (frameCount > 420) break;
		int c = cv::waitKey(10);
		if (c == 27) break;
	}
	writer.release();
	return 0;

	/*
	int fileNumber[] = { 7, 8, 0 };

	cv::VideoCapture fileInput("C:\\LensBricks\\study\\ch01_20140706174402.mp4");
	cv::VideoCapture fileInput2("C:\\LensBricks\\study\\ch04_20140706192738.mp4"); 
	cv::Mat inputFrame;
	cv::Mat inputFrame2;
	fileInput >> inputFrame;
	fileInput2 >> inputFrame2;
	string pathh = "C:\\LensBricks\\study\\ch1.avi";
	string pathh2 = "C:\\LensBricks\\study\\ch2.avi";
	cv::VideoWriter fileOutput;
	cv::VideoWriter fileOutput2;
	fileOutput.open(pathh, CV_FOURCC('D', 'I', 'V', 'X'), 30, inputFrame.size());
	fileOutput2.open(pathh2, CV_FOURCC('D', 'I', 'V', 'X'), 30, inputFrame2.size());
	int framenumber = 0;
	char CFE = 0;
	while (CFE != 27)
	{
		framenumber++;
		fileInput >> inputFrame;
		if (inputFrame.empty())
			break;
		imshow("output", inputFrame);
		fileOutput.write(inputFrame);
		
	}
	fileOutput.release();
	CFE = 0;
	while (CFE != 27)
	{
		fileInput2 >> inputFrame2;
		if (inputFrame2.empty())
			break;
		fileOutput.write(inputFrame2);
		imshow("output", inputFrame);
		CFE = waitKey(1);
	}
	
	fileOutput2.release();
	return;
	*/
	
	/*
	//int frameJump = (0 * 60 + 1.1) * 60 * 30;
	//fileInput.set(CV_CAP_PROP_POS_FRAMES, frameJump);
	CascadeClassifier faceDetector(faceFile);
	std::vector<cv::Rect> faceBoxes;
	FLANDMARK_Model *fModel = flandmark_init("C:/LensBricks/models/flandmark_model.dat");
	double *landmarks = (double*)malloc(2 * fModel->data.options.M*sizeof(double));
	int index = 1;
	while (1)
	{
		std::vector<cv::Rect>::iterator it;
		fileInput >> inputFrame;
		string filename = "";
		//inputFrame = imread(filename);
		//resize(inputFrame, inputFrame, cv::Size(960, 540));
		//cv::Rect ROI(350, 10, 400, 400);
 		fullbody.detectMultiScale(inputFrame, faceBoxes);
		cv::Mat dispFrame = inputFrame.clone();
		cvtColor(inputFrame, inputFrame, CV_BGR2GRAY);
		//faceDetector.detectMultiScale(inputFrame, faceBoxes);
		for (it = faceBoxes.begin(); it < faceBoxes.end(); it++)
		{
			cv::rectangle(dispFrame, *it, cv::Scalar(255, 0, 255), 2);
			cout << *it << endl;
			/*int bbox[4];
			bbox[0] = it->x;
			bbox[1] = it->y;
			bbox[2] = it->x + it->width;
			bbox[3] = it->x + it->height;
			flandmark_detect(&(IplImage)inputFrame, bbox, fModel, landmarks);
			cout << landmarks[0] << landmarks[1];
			circle(dispFrame, cv::Point(landmarks[0], landmarks[1]), 3, cv::Scalar(255, 0, 0), CV_FILLED);
			for (int i = 2; i < 2 * fModel->data.options.M; i += 2)
			{
				circle(dispFrame, cv::Point(landmarks[i], landmarks[i + 1]), 2, cv::Scalar(255, 0, 0), CV_FILLED);
			}*/
	/*	}
		imshow("faces", dispFrame);
		
		imwrite("C:/LensBricks/false.png", dispFrame);
		cv::waitKey();
		index++;
	}
	#endif
	#if 1
	for (int i = 0; i < 68; i++)
	{
		faceDetect(i, "front");
		//faceDetect(i, "side");
	}
	#endif	*/
}