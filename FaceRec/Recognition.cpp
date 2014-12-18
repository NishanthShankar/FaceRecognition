#include <opencv2\core\core.hpp>
#include <opencv2\features2d\features2d.hpp>
#include <opencv2\imgproc\imgproc.hpp>
#include <opencv2\objdetect\objdetect.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2\contrib\contrib.hpp>
#include <Windows.h>
#include <stdio.h>
#include <conio.h>
#include <iostream>
#include <string>
#include <opencv\dirent.h>
#include <irrKlang.h>
#include <sql.h>


using namespace std;
using namespace cv;

#pragma comment(lib, "irrKlang.lib")
std::string faceXML = "C:\\OpenCV\\opencv\\sources\\data\\haarcascades\\haarcascade_frontalface_alt2.xml";
CascadeClassifier faceDet(faceXML);
void testing();
void training();
void update(string,string);
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
		cv::Mat inter, skinMat;
		cvtColor(src, inter, COLOR_BGR2YCrCb);
		inRange(inter, cv::Scalar(Y_MIN, Cr_MIN, Cb_MIN), cv::Scalar(Y_MAX, Cr_MAX, Cb_MAX), skinMat);
		src.copyTo(dst, skinMat);
		return dst;
	}
}skinD;

void main()
{
	//training();
	update("LBPHAllNew", "LBPHAllNew2");
            	string names[] = { "Ashish", "Chintak", "Mike", "Nishanth", "Pranav", "Vishal", "Vivek", "Raji", "Natasha", "Girish", "Manikam", "Gaurav"};
	//Sound Test
	irrklang::ISoundEngine* soundPlayer = irrklang::createIrrKlangDevice();
	if (!soundPlayer)
	{
		cout << "Could not start player\n";
		return;
	}
	//while (1);//training();
	int predicted;
	double confidence;
	
	//cv::Ptr<cv::FaceRecognizer> model = cv::createFisherFaceRecognizer();//
	cv::Ptr<cv::FaceRecognizer> model = cv::createLBPHFaceRecognizer(1, 8, 8, 8, 90);//
	model->load("C://LensBricks//models/face/LBPHAllNew.yml");
	DIR *fd;
	string sPath = "C:\\LensBricks\\Datasets\\Office\\Face\\images3\\front/";
	dirent* folder;
	string filename;
	cv::Mat frame;
	char ttryKey = cv::waitKey();
	int iter = 10;
	string folderPath = sPath;
	char *path = new char[folderPath.length() + 1];
	strcpy(path, folderPath.c_str());
	fd = opendir(path);
	while (folder = readdir(fd))
	{
		filename = folder->d_name;
		if (filename == "." || filename == "..")
			continue;
		frame = imread(folderPath + filename);
		resize(frame, frame, cv::Size(150, 150));
		cvtColor(frame, frame, CV_BGR2GRAY);
		cv::imshow("image", frame);
		cv::waitKey(1);
		model->predict(frame, predicted, confidence);
		cout << predicted << "\t" << confidence << endl;
	}
	{
		cout << (int)predicted << "\t" << confidence << "\t" << names[predicted - 1] << endl;
		if (predicted < 0) 
		{
			string soundFile = "C:\\LensBricks\\gitProjects\\FaceRecognition\\Sound\\bell.wav";
			//soundPlayer->play2D(soundFile.c_str());
			int countdown = 1000;
			Sleep(1000);
			cout << "Please smile at the camera\n";
			//continue;
		}
		string soundFile = "C:\\LensBricks\\gitProjects\\FaceRecognition\\Sound\\" + names[predicted - 1] + ".mp3";
	//	soundPlayer->play2D(soundFile.c_str());
	//	Sleep(1000);
		cv::waitKey(1);
	}
}

void testing()
{
	cv::Ptr<cv::FaceRecognizer> model = cv::createFisherFaceRecognizer();;
	model->load("C://LensBricks//gender.yml");
	DIR *fd;
	string sPath = "C:\\LensBricks\\Datasets\\Office\\Face\\classes\\training\\";
	dirent* folder;
	string filename;
	cv::Mat frame;
	for (int i = 8; i < 10; i++)
	{
		int iter = 10;
		string folderPath = sPath + to_string(i) + "/";
		char *path = new char[folderPath.length() + 1];
		strcpy(path, folderPath.c_str());
		fd = opendir(path);
		while (folder = readdir(fd))
		{
			filename = folder->d_name;
			if (filename == "." || filename == "..")
				continue;
			frame = imread(folderPath + filename);
			resize(frame, frame, cv::Size(150, 150));
			cvtColor(frame, frame, CV_BGR2GRAY);
			cv::imshow("image",frame);
			cv::waitKey();
			cout << model->predict(frame) << endl;
		}
	}
}

void training()
{
	DIR *fd;
	string sPath = "C:\\LensBricks\\Datasets\\Office\\Face\\classes\\training\\";
	dirent* folder;
	string filename;
	cv::Mat frame;
	std::vector<cv::Mat> training;
	std::vector<int>classes;
	for (int i = 1; i < 13; i++)
	{
		int iter = 100;
		string folderPath = sPath + to_string(i) + "/";
		char *path = new char[folderPath.length() + 1];
		strcpy(path, folderPath.c_str());
		cout << path << endl;
		fd = opendir(path);
		while (folder = readdir(fd))
		{
			if (!iter--)
				break;
			cout << folder->d_name << endl;
			filename = folder->d_name;
			if (filename == "." || filename == "..")
				continue;
			frame = imread(folderPath + filename);
			resize(frame, frame, cv::Size(150, 150));
			cvtColor(frame, frame, CV_BGR2GRAY);
			
			training.push_back(frame);
			classes.push_back(i);
			
			cv::imshow("display", frame);
			cv::waitKey(1);
		}
	}
	cv::Ptr<cv::FaceRecognizer> fischerModel = cv::createFisherFaceRecognizer();
	cv::Ptr<cv::FaceRecognizer> LBPModel = cv::createLBPHFaceRecognizer();
	fischerModel->train(training, classes);
	LBPModel->train(training, classes);
	fischerModel->save("C://LensBricks//FischerAllNew.yml");
	LBPModel->save("C://LensBricks//models/face/LBPHAllNew.yml");
	Mat testFrame = imread("C:/LensBricks/Datasets/Office/Face/classes/Mike/4_54.png");
	resize(testFrame, testFrame, cv::Size(150, 150));
	cvtColor(testFrame, testFrame, CV_BGR2GRAY);
	//fischerModel->load("C://LensBricks//gender.yml");
	cout << fischerModel->predict(testFrame)<< endl << LBPModel->predict(testFrame);
}

void recognize()
{
	cv::VideoCapture cam(0);
	cv::Mat frame, skinFrame, face;
	cv::CascadeClassifier faceDetector;
	cv::CascadeClassifier eyeDetector;
	cv::Ptr<cv::FaceRecognizer> FischerModel = cv::createFisherFaceRecognizer();
	cv::Ptr<cv::FaceRecognizer> LBPHModel = cv::createLBPHFaceRecognizer(1, 8, 8, 8, 100);
	//FischerModel->load();
	//LBPHModel->load();

	while (1)
	{
		std::vector<cv::Rect> faceBox, eyeBox;
		cam >> frame;
		if (frame.empty())
			break;
		skinFrame = skinD.skinDetect(frame);
		faceDetector.detectMultiScale(skinFrame, faceBox, 1.1, 3, 0, cv::Size(120, 120), cv::Size(170, 170));
		if (faceBox.empty())
			continue;
		Mat face = frame(faceBox[0]);
		cv::Rect tempBox = faceBox[0];
		tempBox.height *= 0.7;
		eyeDetector.detectMultiScale(frame(tempBox), eyeBox);
		if (eyeBox.empty())
			continue;
		resize(face, face, cv::Size(150, 150));
		cvtColor(face, face, CV_BGR2GRAY);
		//predict gender then person
		//play sound acordingly
	}
}

void update(string ModelName, string outModelName)
{
	cv::Ptr<cv::FaceRecognizer> model = createLBPHFaceRecognizer();
	model->load("C:/LensBricks/models/face/" + ModelName + ".yml");
	DIR *fd;
	string sPath = "C:\\LensBricks\\Datasets\\Office\\Face\\882014\\results\\mFace/";
	dirent* folder;
	string filename;
	
	std::vector<cv::Mat> training;
	std::vector<int>classes;
	string folderPath = sPath;
	char *path = new char[folderPath.length() + 1];
	strcpy(path, folderPath.c_str());
	fd = opendir(path);
	int label;
	std::vector<cv::Mat> trainings;
	std::vector<int> labels;
	while (folder = readdir(fd))
	{
		cv::Mat frame;
		cout << folder->d_name << endl;
		filename = folder->d_name;
		if (filename == "." || filename == "..")
			continue;
		frame = imread(folderPath + filename);
		cv::imshow("display", frame);
		label = cv::waitKey();
		//cout << label - 48 << endl;
		resize(frame, frame, cv::Size(150, 150));
		cvtColor(frame, frame, CV_BGR2GRAY);
		if (label == 27)
			break;
		if (label > 60)
			continue;
		trainings.push_back(frame);
		labels.push_back(label-48);
	}
	model->update(trainings, labels);
	model->save("C:/Lensbricks/models/face/" + outModelName + ".yml");
}