//混合高斯模型通过视频流生成一个干净的背景

#include <iostream>
#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/opencv.hpp>

//#include "VideoProcessor.h"
//#include "FeatureTracker.h"
//#include "BGFSSegmentor.h"

using namespace std;
using namespace cv;


int main1(int argc, char* argv[])
{

	VideoCapture capture("../video/video3.mp4");
	if (!capture.isOpened())
		return 0;
	Mat frame;
	Mat foreground, foreground2;
	Mat bgimg;
	BackgroundSubtractorMOG mog;
	BackgroundSubtractorMOG2 mog2;
	int framestart = 1;
	int framestop = 100;
	int currentframe = framestart;
	bool stop(false);
	//namedWindow("Extracted Foreground");
	long long t_start = getTickCount();
	while (!stop)
	{
		if (!capture.read(frame))
			break;
		//cvtColor(frame, frame, CV_BGR2GRAY);
		//long long t = getTickCount();
		//mog(frame, foreground, 0.01);
		//long long t1 = getTickCount();
		mog2(frame, foreground2, -1);
		//long long t2 = getTickCount();
		mog2.getBackgroundImage(bgimg);
		//cout << "t1 = " << (t1 - t) / getTickFrequency() << " t2 = " << (t2 - t1) / getTickFrequency() << endl;
		//cout << "t = " << (t2 - t1) / getTickFrequency() << endl;
		//imshow("Extracted Foreground", foreground);
		//imshow("Extracted Foreground2", foreground2);
		//imshow("Background Image", bgimg);
		//imshow("video", frame);
		if (waitKey(10) >= 0)
			stop = true;
		//if (currentframe == framestop)
		//{
		//	break;
		//}
		currentframe++;
	}
	long long t_end = getTickCount();
	cout << "total time： " << (t_end - t_start )/getTickFrequency() << endl;
	cout << "total frame: " << currentframe << endl;
	imshow("BackgroundImage", bgimg);
	waitKey();
	return 0;

}
