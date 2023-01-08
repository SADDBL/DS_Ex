#pragma once
#include <opencv2/core.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>
#include<cmath>
#include<vector>
#include<string>
#include<iostream>
#include<algorithm>

#define PI 3.1415926
#define SeedPoint_TryTimes 3
#define M 608//622//608//651//608//686//���ᣬ����
#define N 591//582//591//650//591//640//���ᣬ����
#define OK 0
#define ERROR -1
#define TRUE 0
#define FALSE -1

using namespace cv;
using namespace std;

class grid_cell
{
public:
	Point pos;//����cell�д������ӵ㣬��posΪ���ӵ�����
	int flag;//1���������ӵ㣬0�����������ӵ�

	//grid_cell��ʼ��
	void Init_grid_cell()
	{
		flag = 0;
		pos.x = -1;
		pos.y = -1;
	}

	//��gird����ӵ�
	void Add_NewPoint(Point NewPoint)
	{

		flag = 1;
		this->pos.x = NewPoint.x;
		this->pos.y = NewPoint.y;

	}

	//ɾ��grid_cell�еĵ�
	void Delete_Point()
	{
		this->pos.x = -1;
		this->pos.y = -1;
		flag = 0;
	}
};

int FastPossionDisc(int K, vector<Point>* SeedPointList);
float Cal_Dis2(Point P1, Point P2);
void draw_Point(vector<Point> SeedPointList, int K, Mat img_gray);
