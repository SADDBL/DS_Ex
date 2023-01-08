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
#define M 608//622//608//651//608//686//纵轴，行数
#define N 591//582//591//650//591//640//横轴，列数
#define OK 0
#define ERROR -1
#define TRUE 0
#define FALSE -1

using namespace cv;
using namespace std;

class grid_cell
{
public:
	Point pos;//若该cell中存在种子点，则pos为种子点坐标
	int flag;//1：存在种子点，0：不存在种子点

	//grid_cell初始化
	void Init_grid_cell()
	{
		flag = 0;
		pos.x = -1;
		pos.y = -1;
	}

	//向gird中添加点
	void Add_NewPoint(Point NewPoint)
	{

		flag = 1;
		this->pos.x = NewPoint.x;
		this->pos.y = NewPoint.y;

	}

	//删除grid_cell中的点
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
