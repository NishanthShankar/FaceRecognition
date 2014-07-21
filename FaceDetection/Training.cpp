
#include "opencv2/core/core.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/contrib/contrib.hpp"
#include "opencv2/highgui/highgui.hpp"
#include <iostream>
#include <fstream>
#include <sstream>

using namespace cv;
using namespace std;

int train()
{
	int total = 50;
	int subject = 5;
	int i, j;
	string path = "C:\\LensBricks\\Datasets\\Office\\Chintaks\\Input\\";
	std::ostringstream s;
	std::vector<Mat> images;
	std::vector<int> labels;
	Mat temp;
	namedWindow("Test");
	for (i = 0; i < subject; i++)
	{
		for (j = 1; j < total; j++)
		{
			s.str("");
			s.clear();
			s << path << i << "\\" << j << ".jpg";
			temp = imread(s.str().c_str(), 0);
			images.push_back(imread(s.str().c_str(), 0));
			labels.push_back(i);
		}
	}
	int height = images[0].cols;
	Ptr<FaceRecognizer> model = createLBPHFaceRecognizer(1, 8, 8, 8, 80);
	model->train(images, labels);
	model->save("C:\\LensBricks\\modelLBPH2.yml");
	return 0;
}
