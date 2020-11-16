// OpenCVApplication.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "common.h"
#include <random>
#include <queue>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <queue>
#include <fstream>
#include "OpenCVApplication.h"

void testOpenImage()
{
	char fname[MAX_PATH];
	while(openFileDlg(fname))
	{
		Mat src;
		src = imread(fname);
		imshow("image",src);
		waitKey();
	}
}

void testOpenImagesFld()
{
	char folderName[MAX_PATH];
	if (openFolderDlg(folderName)==0)
		return;
	char fname[MAX_PATH];
	FileGetter fg(folderName,"bmp");
	while(fg.getNextAbsFile(fname))
	{
		Mat src;
		src = imread(fname);
		imshow(fg.getFoundFileName(),src);
		if (waitKey()==27) //ESC pressed
			break;
	}
}

void testImageOpenAndSave()
{
	Mat src, dst;
	src = imread("Images/Lena_24bits.bmp", CV_LOAD_IMAGE_COLOR);	// Read the image
	if (!src.data)	// Check for invalid input
	{
		printf("Could not open or find the image\n");
		return;
	}
	Size src_size = Size(src.cols, src.rows);
	const char* WIN_SRC = "Src"; //window for the source image
	namedWindow(WIN_SRC, CV_WINDOW_AUTOSIZE);
	cvMoveWindow(WIN_SRC, 0, 0);

	const char* WIN_DST = "Dst"; //window for the destination (processed) image
	namedWindow(WIN_DST, CV_WINDOW_AUTOSIZE);
	cvMoveWindow(WIN_DST, src_size.width + 10, 0);

	cvtColor(src, dst, CV_BGR2GRAY); //converts the source image to a grayscale one
	imwrite("Images/Lena_24bits_gray.bmp", dst); //writes the destination to file

	imshow(WIN_SRC, src);
	imshow(WIN_DST, dst);
	printf("Press any key to continue ...\n");
	waitKey(0);
}

void testBGR2HSV()
{
	char fname[MAX_PATH];
	while (openFileDlg(fname))
	{
		Mat src = imread(fname);
		int height = src.rows;
		int width = src.cols;

		// Componentele d eculoare ale modelului HSV
		Mat H = Mat(height, width, CV_8UC1);
		Mat S = Mat(height, width, CV_8UC1);
		Mat V = Mat(height, width, CV_8UC1);

		// definire pointeri la matricele (8 biti/pixeli) folosite la afisarea componentelor individuale H,S,V
		uchar* lpH = H.data;
		uchar* lpS = S.data;
		uchar* lpV = V.data;

		Mat hsvImg;
		cvtColor(src, hsvImg, CV_BGR2HSV);

		// definire pointer la matricea (24 biti/pixeli) a imaginii HSV
		uchar* hsvDataPtr = hsvImg.data;

		for (int i = 0; i<height; i++)
		{
			for (int j = 0; j<width; j++)
			{
				int hi = i*width * 3 + j * 3;
				int gi = i*width + j;

				lpH[gi] = hsvDataPtr[hi] * 510 / 360;		// lpH = 0 .. 255
				lpS[gi] = hsvDataPtr[hi + 1];			// lpS = 0 .. 255
				lpV[gi] = hsvDataPtr[hi + 2];			// lpV = 0 .. 255
			}
		}

		imshow("input image", src);
		imshow("H", H);
		imshow("S", S);
		imshow("V", V);

		waitKey();
	}
}

int start_x = 0, start_y = 0;
std::default_random_engine gen;
std::uniform_int_distribution<int> d(0, 255);
Mat etichetare(Mat src) {
	int height = src.rows;
	int width = src.cols;
	int label = 0;
	Mat labels = Mat(height, width, CV_32SC1, Scalar(0));
	Mat dst = Mat(height, width, CV_8UC3, Scalar(0));

	for (int i = 1; i <= src.rows - 1; i++) {
		for (int j = 1; j <= src.cols - 1; j++)
		{
			if ((src.at<uchar>(i, j) == 0) && (labels.at<int>(i, j) == 0)) {
				label++;
				std::queue<Point2i> Q;
				labels.at<int>(i, j) = label;
				Q.push(Point2i(j, i));
				while (!(Q.empty())) {
					Point2i q = Q.front();
					Q.pop();
					for (int k = q.y - 1; k <= q.y + 1; k++) {
						for (int m = q.x - 1; m <= q.x + 1; m++) {
							if ((k < src.rows) && (k >= 0) && (m < src.cols) && (m >= 0)) {
								if (src.at<uchar>(k, m) == 0) {
									if (labels.at<int>(k, m) == 0) {
										labels.at<int>(k, m) = label;
										Q.push(Point2i(m, k));
									}
								}
							}
						}
					}
				}
			}
		}
	}
	Vec3b* colors = new Vec3b[label + 1];
	for (int i = 0; i < label + 1; i++)
		colors[i] = Vec3b(0, 69, 255);
	int contor = 0;
	for (int i = 1; i <= src.rows - 1; i++) {
		for (int j = 1; j <= src.cols - 1; j++) {
			if (labels.at<int>(i, j) > 0) {
				dst.at<Vec3b>(i, j) = colors[labels.at<int>(i, j)];
				contor++;
			}
		}
	}
	printf("%d\n", label);
	return dst;


}



void Portocale()
{
	char fname[MAX_PATH];
	while (openFileDlg(fname))
	{
		Mat src = imread(fname);


		int height = src.rows;
		int width = src.cols;
		int contor = 0, contor2 = 0;
		Mat matrice = Mat(height, width, CV_8UC1);


		for (int i = 0; i < height; i++)
			for (int j = 0; j < width; j++)
			{
				Vec3b v3 = src.at<Vec3b>(i, j);
				int b = v3[0];
				int g = v3[1];
				int r = v3[2];

				if (b >= 0 && b <= 30 && g <= 215 && g >= 50 && r >= 180 && r <= 255)
				{
					matrice.at<uchar>(i, j) = 0;
					contor++;
				}
			}
		imshow("input image", src);
		imshow("Matrice rezultata", matrice);

		Mat dst = Mat(height, width, CV_8UC1);

		for (int i = 0; i < height; i++)
		{
			for (int j = 0; j < width; j++)
			{
				dst.at<uchar>(i, j) = 255;
			}
		}


		for (int i = 0; i < height; i++) {
			for (int j = 0; j < width; j++) {

				if (matrice.at<uchar>(i, j) == 0) {

					if ((i > 0) && (i < height - 1) && (j > 0) && (j < width - 1) &&
						(matrice.at<uchar>(i - 1, j - 1) == 0 || matrice.at<uchar>(i - 1, j) == 0 || matrice.at<uchar>(i - 1, j + 1) == 0 || matrice.at<uchar>(i, j - 1) == 0 || matrice.at<uchar>(i, j + 1) == 0 || matrice.at<uchar>(i + 1, j - 1) == 0 || matrice.at<uchar>(i + 1, j) == 0 || matrice.at<uchar>(i + 1, j + 1) == 0))
					{

						dst.at<uchar>(i, j) = matrice.at<uchar>(i, j);
						dst.at<uchar>(i - 1, j - 1) = matrice.at<uchar>(i, j);
						dst.at<uchar>(i - 1, j) = matrice.at<uchar>(i, j);
						dst.at<uchar>(i - 1, j + 1) = matrice.at<uchar>(i, j);
						dst.at<uchar>(i, j - 1) = matrice.at<uchar>(i, j);
						dst.at<uchar>(i, j + 1) = matrice.at<uchar>(i, j);
						dst.at<uchar>(i + 1, j - 1) = matrice.at<uchar>(i, j);
						dst.at<uchar>(i + 1, j) = matrice.at<uchar>(i, j);
						dst.at<uchar>(i + 1, j + 1) = matrice.at<uchar>(i, j);


					}
					else dst.at<uchar>(i, j) = matrice.at<uchar>(i, j);
				}

			}
		}

		//imshow("Image", src);
		imshow("Imagine dilata", dst);

		Mat matrice2 = Mat(height, width, CV_8UC1);

		for (int i = 0; i < height; i++) {
			for (int j = 0; j < width; j++) {

				matrice2.at<uchar>(i, j) = dst.at<uchar>(i, j);

			}
		}
		imshow("Imagine de care am nevoie", matrice2);

		for (int i = 1; i < height - 1; i++)
			for (int j = 1; j < width - 1; j++)
				if (matrice2.at<uchar>(i, j) == 0)
					if (matrice2.at<uchar>(i - 1, j - 1) == 255 && matrice2.at<uchar>(i - 1, j) == 255 && matrice2.at<uchar>(i - 1, j + 1) == 255 && matrice2.at<uchar>(i, j - 1) == 255 && matrice2.at<uchar>(i, j + 1) == 255 && matrice2.at<uchar>(i + 1, j - 1) == 255 && matrice2.at<uchar>(i + 1, j) == 255 && matrice2.at<uchar>(i + 1, j + 1) == 255)
					{
						matrice2.at<uchar>(i, j) = 255;

					}

		imshow("Imagine blana etichetata", etichetare(matrice2));
	}
}



int main()
{
	int op;
	do
	{
		system("cls");
		destroyAllWindows();
		printf("Menu:\n");
		printf(" 1 - Open image\n");
		printf(" 2 - Open BMP images from folder\n");
		printf(" 3 - BGR->HSV\n");
		printf(" 4 - portocale\n");

		printf(" 0 - Exit\n\n");
		printf("Option: ");
		scanf("%d",&op);
		switch (op)
		{
			case 1:
				testOpenImage();
				break;
			case 2:
				testOpenImagesFld();
				break;
			case 3:
				testBGR2HSV();
				break;
			case 4:
				Portocale();
				break;

		}
	}
	while (op!=0);
	return 0;
}