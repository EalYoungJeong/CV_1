#include <iostream>
#include <fstream>
#include<opencv2/opencv.hpp>
#include<opencv2/core/mat.hpp>
#include <opencv2/highgui.hpp>
#include <windows.h>
using namespace std;
#pragma warning(disable:4819)

int main()
{
	
	int ROW = 352;//���� ����
	int COL = 288;//���� ����
	int image_size = ROW * COL;//�̹��� ������
	int fp_size = image_size * 3;
	unsigned char* RGB = new unsigned char[fp_size];//array to save file
	unsigned char* R = new unsigned char[image_size];	//Red �迭
	unsigned char* G = new unsigned char[image_size];	//Green �迭
	unsigned char* B = new unsigned char[image_size];	//Blue �迭


	FILE* fp = fopen("Suzie_CIF_352x288.raw", "rb"); //open raw file
	if (fp == NULL)//����ó��
	{
		cout << "file read error" << endl;
		return 0;
	}
	fread(RGB, sizeof(char), fp_size, fp); //raw �����͸� RGB unsigned char �迭�� ����

	for (int i = 0; i< image_size; i++)//b, g, r ������ ä�� ������ �и�
	{
		B[i] = RGB[i];//blue
		G[i] = RGB[image_size + i];//green
		R[i] = RGB[2*image_size + i];//red
	}
	
	//raw data �ȼ���ġ Ȯ�ο�
	cout << "raw ����, �ȼ� �迭 ���� Ȯ��" << endl << endl;
	Sleep(500);
	cv::Mat bgr[3];
	cv::Mat showRGB(864, 352, CV_8UC1, RGB);
	cv::imshow("RGB", showRGB);
	cout << "Ȯ�� ��� B(height*width), G(height*width), R(height*width) ������ ���ĵ�" << endl;
	cout << "�ƹ� Ű�� �����ּ���" << endl << endl;
	cv::waitKey(0);
	cv::destroyAllWindows();

	cout << "B,G,R �� RGB ��� Ȯ��" << endl << endl;
	Sleep(500);
	bgr[0] = cv::Mat(288, 352, CV_8UC1, B);//blue : unsigned char -> Mat
	bgr[1] = cv::Mat(288, 352, CV_8UC1, G);//green : unsigned char -> Mat
	bgr[2] = cv::Mat(288, 352, CV_8UC1, R);//red : unsigned char -> Mat
	cv::imshow("B", bgr[0]);
	cv::imshow("G", bgr[1]);
	cv::imshow("R", bgr[2]);

	cv::Mat rgb[3];//RGB
	for (int i = 0; i < 3; i++)// bgr -> rgb
	{//raw�� bgr ������ �����Ͱ� ����Ǿ������� rgb�� r,g,b ������ ����Ǳ� ������ 
	 //������ �ٲ��־���Ѵ�.
		rgb[i] = bgr[2 - i];
	}
	cv::Mat dest;
	cv::merge(rgb, 3, dest);//R G B ä�� ����
	cv::imshow("RGB", dest);

	cout << "RGB Ȯ�� �Ϸ�, �ƹ�Ű�� �����ּ���" << endl << endl;
	cv::waitKey(0);
	cv::destroyAllWindows();

	cout << "RGB�� YCbCr444�� ��ȯ �� Ȯ��" << endl << endl;
	Sleep(500);
	unsigned char* Y = new unsigned char[image_size];//Y �迭

	unsigned char* Cb = new unsigned char[image_size];//Cb �迭
	unsigned char* ShowCb = new unsigned char[image_size/4];//Cb 420 �迭(�����ֱ��)
	unsigned char* Cb2 = new unsigned char[image_size];//Cb 420 �迭 (420 ���տ�)

	unsigned char* Cr = new unsigned char[image_size];//Cr �迭
	unsigned char* ShowCr = new unsigned char[image_size / 4];//Cr 420 �迭(�����ֱ��)
	unsigned char* Cr2 = new unsigned char[image_size];//Cr 420 �迭(420 ���տ�)

	for (int i = 0; i < image_size; i++)// ���� ����
	{
		Y[i] = (R[i] * 77 / 256) + (G[i] * 150 / 256) + (B[i] * 29 / 256);
		Cb[i] = 128 - (R[i] * 44 / 256) - (G[i] * 87 / 256) + (B[i] * 131 / 256);
		Cr[i] = (128 + R[i] * 131 / 256) - (G[i] * 110 / 256) - (B[i] * 21 / 256);
	}

	cv::Mat YCbCr444[3];//Y, Cb, Cr ���տ� �迭
	YCbCr444[0] = cv::Mat(288, 352, CV_8UC1, Y);//Y : unsigned char -> Mat
	YCbCr444[1] = cv::Mat(288, 352, CV_8UC1, Cb);//Cb : unsigned char -> Mat
	YCbCr444[2] = cv::Mat(288, 352, CV_8UC1, Cr);//Cr : unsigned char -> Mat
	cv::Mat destYCbCr;
	cv::merge(YCbCr444, 3, destYCbCr);//Y Cb Cr ����

	//YCbCr ���
	cv::imshow("Y", YCbCr444[0]);
	cv::imshow("Cb444", YCbCr444[1]);
	cv::imshow("Cr444", YCbCr444[2]);
	cv::imshow("YCbCr", destYCbCr);
	cout << "�̹��� Ȯ�� �Ϸ�, �ƹ�Ű�� �����ּ���" << endl << endl;
	cv::waitKey(0);
	cv::destroyAllWindows();


	cout << "YCbCr444������ RGB �̹����� Ȯ��" << endl << endl;
	Sleep(500);
	unsigned char* YCbCr2R = new unsigned char[image_size];//YCbCr -> Red
	unsigned char* YCbCr2G = new unsigned char[image_size];//YCbCr -> Green
	unsigned char* YCbCr2B = new unsigned char[image_size];//YCbCr -> Blue
	for (int i = 0; i < image_size; i++)//YCbCr -> RGB : RGB->YCbCr������ ���Լ�
	{
		YCbCr2R[i] = Y[i] + (1.36 * (Cr[i] - 128)); //Y + 1.36(Cr-128) 
		YCbCr2G[i] = Y[i] + (-0.7 * (Cr[i] - 128)) + (-0.33 * (Cb[i] - 128)); // Y -0.33(Cb-128) -0.7(Cr-128)
		YCbCr2B[i] = Y[i] + (1.73 * (Cb[i] - 128)); //Y + 1.73(Cb-128)
	}
	cv::Mat YCbCr4442RGB[3];//YCbCr->RGB. R,G,B ���տ� �迭
	YCbCr4442RGB[0] = cv::Mat(288, 352, CV_8UC1, YCbCr2R);//Red
	YCbCr4442RGB[1] = cv::Mat(288, 352, CV_8UC1, YCbCr2G);//Green
	YCbCr4442RGB[2] = cv::Mat(288, 352, CV_8UC1, YCbCr2B);//Blue
	cv::Mat destYCbCr2RGB;//���� ��� Mat
	cv::merge(YCbCr4442RGB, 3, destYCbCr2RGB);//R G B ����

	//RGB���
	cv::imshow("YCbCr2R", YCbCr4442RGB[0]);
	cv::imshow("YCbCr2G", YCbCr4442RGB[1]);
	cv::imshow("YCbCr2B", YCbCr4442RGB[2]);
	cv::imshow("YCbCr2RGB", destYCbCr2RGB);
	cout << "�̹��� Ȯ�� �Ϸ�" << endl;
	cout << "�ƹ�Ű�� �����ּ���" << endl << endl;
	cv::waitKey(0);
	cv::destroyAllWindows();

	cout << "RGB�� YCbCr420���� ��ȯ �� Ȯ��" << endl << endl;
	Sleep(500);
	int m = 0, n = 0;// 1/4 ũ�� ����� �迭 �ε���
	int flag = 0;
	for (int i = 0; i < COL; i++)//420���� ���μ��� 1/2�� ��ҵ� Cb, Cr ���ϱ�
	{
		for (int j = 0; j < ROW; j++)
		{
			if ((i % 2 == 0) && (j%2 == 0))//��, �� ��� 2�� ����� ��츸 
			{//�迭�� �ȼ� ����
				Cb2[m] = Cb[352 * i + j];
				Cb2[m + 1] = Cb[352 * i + j];
				Cb2[m + 352] = Cb[352 * i + j];
				Cb2[m + 353] = Cb[352 * i + j];
				ShowCb[n] = Cb[352 * i + j];
				Cr2[m] = Cr[352 * i + j];
				Cr2[m + 1] = Cr[352 * i + j];
				Cr2[m + 352] = Cr[352 * i + j];
				Cr2[m + 353] = Cr[352 * i + j];
				ShowCr[n] = Cr[352 * i + j];
				//���� �� �ε��� ����
				m=m+2;
				if (m % 352 == 0) m = m + 352;
				
				n++;
			}
		}
	}
	cv::Mat YCbCr420[3];
	YCbCr420[0] = YCbCr444[0];//Y�� ����
	YCbCr420[1] = cv::Mat(288, 352, CV_8UC1, Cb2);//Cb2 : unsigned -> Mat
	YCbCr420[2] = cv::Mat(288, 352, CV_8UC1, Cr2);//Cr2 : unsigned -> Mat
	cv::Mat showCb420(144, 176, CV_8UC1, ShowCb);//1/4 �����ֱ�� Cb
	cv::Mat showCr420(144, 176, CV_8UC1, ShowCr);//1/4 �����ֱ�� Cr
	cv::Mat destYCbCr420;
	cv::merge(YCbCr420, 3, destYCbCr420);// ����
	
	//���
	cv::imshow("Y", YCbCr420[0]);
	cv::imshow("Cb420", YCbCr420[1]);
	cv::imshow("Cr420", YCbCr420[2]);
	cv::imshow("YCbCr420", destYCbCr420);
	cout << "�̹��� ��ȯ Ȯ��" << endl;
	cout<<"�ƹ�Ű�� �����ּ���" << endl << endl;
	cv::waitKey(0);
	cv::destroyAllWindows();

	cout << "���� ���� 1/2 ��ҵ� Cb�� Cr Ȯ��" << endl << endl;
	Sleep(500);
	cv::imshow("1/2Cb420", showCb420);//1/4 Cb 
	cv::imshow("1/2Cr420", showCr420);//1/4 Cr
	cout << "�̹��� ��ȯ Ȯ��" << endl;
	cout << "�ƹ�Ű�� �����ּ���" << endl << endl;
	cv::waitKey(0);
	cv::destroyAllWindows();


	cout << "YCbCr420 �̹����� RGB�� Ȯ��" << endl << endl;
	Sleep(500);
	unsigned char* YCbCr4202R = new unsigned char[image_size];//YCbCr420 -> Red
	unsigned char* YCbCr4202G = new unsigned char[image_size];//YCbCr420 -> Green
	unsigned char* YCbCr4202B = new unsigned char[image_size];//YCbCr420 -> Blue
	for (int i = 0; i < image_size; i++)//YCbCr -> RGB : RGB->YCbCr������ ���Լ�
	{
		YCbCr4202R[i] = Y[i] + (1.36 * (Cr2[i] - 128)); //Y + 1.36(Cr-128) 
		YCbCr4202G[i] = Y[i] + (-0.7 * (Cr2[i] - 128)) + (-0.33 * (Cb2[i] - 128)); // Y -0.33(Cb-128) -0.7(Cr-128)
		YCbCr4202B[i] = Y[i] + (1.73 * (Cb2[i] - 128)); //Y + 1.73(Cb-128)
	}
	cv::Mat YCbCr4202RGB[3];
	YCbCr4202RGB[0] = cv::Mat(288, 352, CV_8UC1, YCbCr4202R);
	YCbCr4202RGB[1] = cv::Mat(288, 352, CV_8UC1, YCbCr4202G);
	YCbCr4202RGB[2] = cv::Mat(288, 352, CV_8UC1, YCbCr4202B);
	cv::Mat destYCbCr4202RGB;
	cv::merge(YCbCr4202RGB, 3, destYCbCr4202RGB);

	cv::imshow("YCbCr420 -> R", YCbCr4202RGB[0]);
	cv::imshow("YCbCr420 -> G", YCbCr4202RGB[1]);
	cv::imshow("YCbCr420 -> B", YCbCr4202RGB[2]);
	cv::imshow("YCbCr420 -> RGB", destYCbCr4202RGB);
	cout << "�̹��� ��ȯ Ȯ��" << endl;
	cout << "�ƹ�Ű�� �����ּ���" << endl << endl;
	cv::waitKey(0);
	cv::destroyAllWindows();


	//yuv ���� ���
	cout << "YUV 420 ���� Ȯ��" << endl << endl;
	Sleep(500);
	FILE* player = fopen("RaceHorses_416x240_30.yuv", "rb");//yuv ���� ���� ����

	int Y_size = 416 * 240;
	int UV_size = Y_size / 4;

	unsigned char* raceY = new unsigned char[Y_size];//Y �б��
	unsigned char* raceU = new unsigned char[UV_size];//U �б��
	unsigned char* w_raceU = new unsigned char[Y_size];//U size Ȯ���
	unsigned char* raceV = new unsigned char[UV_size];//V �б��
	unsigned char* w_raceV = new unsigned char[Y_size];//V size Ȯ���

	unsigned char* raceR = new unsigned char[Y_size];//��¿� Red
	unsigned char* raceG = new unsigned char[Y_size];//��¿� Green
	unsigned char* raceB = new unsigned char[Y_size];//��¿� Blue

	
	//Cb = U, Cr = V

	while (!feof(player))
	{
		fread(raceY, sizeof(char), Y_size, player);//Y �б�
		fread(raceU, sizeof(char), UV_size, player);//U �б�
		fread(raceV, sizeof(char), UV_size, player);//V �б�

		for (int i = 0; i < 120; i++)//�о���� UV ���� Y ũ�⿡ �°� Ȯ���ϱ�
		{
			for (int j = 0; j < 208; j++)
			{//i,j ���� ����, ����, �����ʿ� ������ ����
				//U Ȯ��
				w_raceU[(416 * 2*i) + (2 * j)] = raceU[208 * i + j];
				w_raceU[(416 * 2*i) + (2 * j) + 1] = raceU[208 * i + j];
				w_raceU[(416 * 2*i) + (2 * j)+416] = raceU[208 * i + j];
				w_raceU[(416 * 2*i) + (2 * j) + 417] = raceU[208 * i + j];

				//V Ȯ��
				w_raceV[(416 * 2*i) + (2 * j)] = raceV[208 * i + j];
				w_raceV[(416 * 2*i) + (2 * j) + 1] = raceV[208 * i + j];
				w_raceV[(416 * 2*i) + (2 * j)+416] = raceV[208 * i + j];
				w_raceV[(416 * 2*i) + (2 * j) + 417] = raceV[208 * i + j];
			}
		}
		
		for (int i = 0; i < Y_size; i++)//YCbCr -> RGB : RGB->YCbCr������ ���Լ�
		{
			raceR[i] = raceY[i] + (1.36 * (w_raceV[i] - 128)); //Y + 1.36(Cr-128) 
			raceG[i] = raceY[i] + (-0.7 * (w_raceV[i] - 128)) + (-0.33 * (w_raceU[i] - 128)); // Y -0.33(Cb-128) -0.7(Cr-128)
			raceB[i] = raceY[i] + (1.73 * (w_raceU[i] - 128)); //Y + 1.73(Cb-128)
		}

		cv::Mat YUV2RGB[3];//RGB ����� Mat
		YUV2RGB[0] = cv::Mat(240, 416, CV_8UC1, raceB);
		YUV2RGB[1] = cv::Mat(240, 416, CV_8UC1, raceG);
		YUV2RGB[2] = cv::Mat(240, 416, CV_8UC1, raceR);
		cv::imshow("R", YUV2RGB[0]);
		cv::imshow("G", YUV2RGB[1]);
		cv::imshow("B", YUV2RGB[2]);


		cv::Mat destYUV;//RGB ���տ� Mat
		cv::merge(YUV2RGB, 3, destYUV);

		cv::imshow("race", destYUV);
		//cv::waitKey(10); //���� �ڵ� �����
		cv::waitKey(0);	//���� �����Ӻ� Ȯ�ο�
	}
	cout << "���� ��� Ȯ�� �Ϸ�" << endl;
	cout << "���α׷� ����" << endl;
	return 0;
}