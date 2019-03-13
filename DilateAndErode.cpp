//图像膨胀腐蚀和边缘提取
//为语义分割图像生成连通区域

#include <iostream>
#include <opencv2\opencv.hpp>
#include <string>
using namespace cv;
using namespace std;


int dilateSize = 1;
int erodeSize = 1;
char * dilateBar = "dilate_KSize";
char * dilate_window = "Dilate";
char * erodeBar = "erode_KSize";
char * erode_window = "Erode";


Mat source, dilate_result, erode_result, dilate_element, erode_element, gray, contours, contoursInv;

//图像膨胀
void onDilateCallBack(int position, void* userdata) {
	if (position <= 0) {
		position = 1;
	}
	dilateSize = position;
	dilate_element = getStructuringElement(MORPH_RECT, Size(dilateSize, dilateSize));
	dilate(gray, dilate_result, dilate_element);
	imshow(dilate_window, dilate_result);
}

//先膨胀再腐蚀,闭运算
void onErodeCallBack(int position, void* userdata) {
	if (position <= 0) {
		position = 1;
	}
	erodeSize = position;
	erode_element = getStructuringElement(MORPH_CROSS, Size(erodeSize, erodeSize));
	erode(dilate_result, erode_result, erode_element);
	imshow(erode_window, erode_result);
}

//遍历图片统计像素数
void pixel(Mat image){  
	set<int> s;
	for (int j = 0; j<image.rows; j++)
	{
		uchar* pdata = image.ptr<uchar>(j);
		for (int i = 0; i<image.cols; i++)
		{
			uchar data = pdata[i];
			if (s.find(data) == s.end()){
				s.insert(data);
			}
		}
	}
	set<int>::iterator it;
	for (it = s.begin(); it != s.end(); it++)
		cout << *it << endl;
}

void find_contours(Mat image){


	vector <vector<Point>> contours;
	vector<Vec4i> hierarchy;

	// findContours可以提取轮廓，但是source必须为8-bit单通道
	findContours(image, contours, hierarchy, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_NONE);

	Mat result(image.size(), CV_8U, Scalar(255));

	drawContours(result, contours, -1, Scalar(0), 2);

	namedWindow("contours");
	imshow("contours", result);

}

int main() {

	source = imread("E:\\c++\\GMM\\opencv-test\\segment_sample\\000026_prediction.png", IMREAD_COLOR);
	imshow("source", source);

	cvtColor(source, gray, CV_BGR2GRAY); //BGR to GRAY
	imshow("gray", gray);
	
	// Canny边缘提取 
	Canny(gray, contours, 10, 20);
	imshow("canny", contours);
	Mat contoursInv;    //反转后的图像  
	threshold(contours, contoursInv, 128, 255, THRESH_BINARY_INV);
	namedWindow("contoursInv");
	imshow("contoursInv", contoursInv);

	dilate_result = Mat(gray.rows, gray.cols, CV_8UC3);
	cvNamedWindow(dilate_window, CV_WINDOW_AUTOSIZE);
	createTrackbar(dilateBar, dilate_window, &dilateSize, 100, onDilateCallBack);
	onDilateCallBack(1, dilateBar);
	cvNamedWindow(erode_window, CV_WINDOW_AUTOSIZE);
	createTrackbar(erodeBar, erode_window, &erodeSize, 100, onErodeCallBack);
	onErodeCallBack(1, erodeBar);

	waitKey(0);
	return 0;
}