
#include "stdafx.h"
#include<opencv2\core\core.hpp>
#include<opencv2\highgui\highgui.hpp>
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<iostream>
#include<conio.h>
#include<opencv2\opencv.hpp>
using namespace std;
using namespace cv;
int main()
{
	

	CvHaarClassifierCascade* frontal_cascade;								//declare cascade file
	frontal_cascade = (CvHaarClassifierCascade*)cvLoad("haarcascade_frontalface_alt.xml", 0, 0, 0);		//call cascade file


	CvMemStorage* storage = cvCreateMemStorage(0);
	CvMemStorage* storage1 = cvCreateMemStorage(0);
	
	int j,count=0,count1=0,x=0;

	for (j = 1; j <=38; j++)
	{
		char path[50] = "E:/python opencv project/minor_test/";
		char file[4];
		char extn[5] = ".jpg";
		sprintf_s(file, "%d", j);
		strcat_s(path, file);
		strcat_s(path, extn);
		//printf("%s\n",path);

		IplImage* img = cvLoadImage(path, 1);
		//IplImage* img = cvLoadImage("C:/Users/Sayan/Pictures/trial.jpg", 1);							//including the source image file
		IplImage* gray = cvCreateImage(cvSize(img->width, img->height), 8, 1);					//create a grayscale blank image file

		const int DETECTION_WIDTH = 320;
		double  scale = (img->width) / (double)DETECTION_WIDTH;


		IplImage* small_img = cvCreateImage(cvSize(cvRound(img->width / scale), cvRound(img->height / scale)), 8, 1);	//create a small copy image file
		IplImage* equi_img = cvCreateImage(cvSize(cvRound(img->width / scale), cvRound(img->height / scale)), 8, 1);	//create a equilized copy image file
		



		cvCvtColor(img, gray, CV_BGR2GRAY);									//convert RGB image to grayscale
		cvResize(gray, small_img, CV_INTER_LINEAR);								//resize image to small image
		cvEqualizeHist(small_img, equi_img);									//equilize the histogram of image
		

		cvClearMemStorage(storage);
		
		CvSeq* objects = cvHaarDetectObjects(equi_img, frontal_cascade, storage, 1.1, 4, 0, cvSize(20, 20));
		

		static CvScalar colors[] = { { { 0, 0, 255 } }, { { 0, 128, 255 } }, { { 0, 255, 255 } }, { { 0, 255, 0 } }, { { 255, 128, 0 } }, { { 255, 255, 0 } }, { { 255, 0, 0 } }, { { 255, 0, 255 } } };

		int i = 0;
		x = (objects ? objects->total : 0);
		
		if (x!=0)
		{
			for (i = 0; i < (objects ? objects->total : 0); i++)
			{
				CvRect* r = (CvRect*)cvGetSeqElem(objects, i);
				cvRectangle(equi_img, cvPoint(r->x, r->y), cvPoint(r->x + r->width, r->y + r->height), colors[i % 8]);
				count++;
				cvSetImageROI(equi_img, cvRect(r->x, r->y, r->height, r->width));
				IplImage *cut = cvCreateImage(cvGetSize(equi_img), equi_img->depth, equi_img->nChannels);
				cvCopy(equi_img, cut, NULL);

				if (cut != NULL)
				{
					count1++;

				}
				cvResetImageROI(equi_img);
				char path1[60] = "E:/python opencv project/minor_extracted_faces/";
				char file1[4];
				char extn1[5] = ".jpg";
				sprintf_s(file1, "%d", count1);
				strcat_s(path1, file1);
				strcat_s(path1, extn1);
				printf_s("%s\n", path1);
				cvSaveImage(path1, cut);
			}
		}
		
		else
		{
			
			char path2[60] = "E:/python opencv project/minor_extracted_nonfaces/";
			char file2[4];
			char extn2[5] = ".jpg";
			sprintf_s(file2, "%d", j);
			strcat_s(path2, file2);
			strcat_s(path2, extn2);
			printf_s("%s\n", path2);
			cvSaveImage(path2, img);
		}


		cvNamedWindow("Original", CV_WINDOW_AUTOSIZE);								//showing orignal image
		cvShowImage("Original", img);
		cvNamedWindow("smallimg", CV_WINDOW_AUTOSIZE);								//showing small image
		cvShowImage("smallimg", small_img);
		cvNamedWindow("grayimg", CV_WINDOW_AUTOSIZE);								//showing gray image
		cvShowImage("grayimg", gray);
		cvNamedWindow("equilisedimg", CV_WINDOW_AUTOSIZE);							//showing equilized image
		cvShowImage("equilisedimg", equi_img);

		cvWaitKey(1);
		cvDestroyWindow("Original");
		cvDestroyWindow("smallimg");
		cvDestroyWindow("grayimg");
		cvDestroyWindow("equilisedimg");
		cvReleaseImage(&small_img);
		cvReleaseImage(&gray);
		cvReleaseImage(&equi_img);
		cvReleaseImage(&img);
		
		

	}

	printf_s("\n\nNo. of faces detected:\t%d", count);
	printf_s("\n\nNo. of faces extracted:\t%d", count1);
	_getch();
	return 0;
}

