#include"mission1.h"

/*************************************************
功能：快速泊松圆盘采样
返回值：OK：种子采集完毕
		ERROR：出错
*************************************************/
int FastPossionDisc(int K, vector<Point>* SeedPointList)
{
	vector<Point> ActPointList;
	float R_min2 = (M * N) / (float)K;
	int max_retry = 120;//在活动点周围生成的尝试点的个数，一般取30
	int cell_size = int(sqrt(R_min2 / 16));//区块大小
	int col_x = int(N / cell_size) + 1;//+1 防止溢出
	int row_y = int(M / cell_size) + 1;

	vector<vector<grid_cell>> grids;//区块矩阵，row_y行，col_x列
	grids.resize(row_y);
	for (vector<vector<grid_cell>>::iterator it = grids.begin(); it != grids.end(); it++)
	{
		it->resize(col_x);
		for (vector<grid_cell>::iterator jt = it->begin(); jt < it->end(); jt++) {
			jt->Init_grid_cell();
		}
	}
	RNG rng((unsigned)time(NULL));//用于生成随机数
	//第一个随机点
	//Point p0 = Point(0,0);// (rng.uniform(0, N), rng.uniform(0, M));
	//if (ActPointList.size() != 0)
	//{
	//	cout << "ActPointList不为空" << endl;
	//	return ERROR;
	//}
	//else {
	//	ActPointList.push_back(p0);//若ActPointList为空，则将p0插入，并作为第一个活动点
	//	col_x = floor(p0.x / cell_size);
	//	row_y = floor(p0.y / cell_size);
	//	grids[row_y][col_x].Add_NewPoint(p0);//表示该区块已经有点
	//}
	//从四个顶点开始采集，有效利用边缘空间
	vector<Point> p0;
	p0.push_back(Point(1, 1));
	p0.push_back(Point(1, M - 1));
	p0.push_back(Point(N - 1, 1));
	p0.push_back(Point(N - 1, -1));
	for (int i = 0; i < 4; i++)
	{
		ActPointList.push_back(p0[i]);
		SeedPointList->push_back(p0[i]);
		col_x = int(p0[i].x / cell_size);
		row_y = int(p0[i].y / cell_size);
		grids[row_y][col_x].Add_NewPoint(p0[i]);//表示该区块已经有点
	}
	int try_again = 3;//当Act Point List为空时，重试的次数
	while (!ActPointList.empty() && try_again > 0)
	{
		int If_Found = FALSE;
		//从活动点列表中随机取一个点，在其周围生成新采样点，并对检验
		int ActPointNum;//活动点序号
		//获取活动点序号
		if (ActPointList.size() == 1) ActPointNum = 0;
		else if (ActPointList.size() == 0 && try_again == 3)
		{
			cout << "ActPoint Get ERROR" << endl;
			return ERROR;
		}
		else if (try_again < 3) ActPointNum = floor((rng.uniform(0, SeedPointList->size())));
		else ActPointNum = floor((rng.uniform(0, ActPointList.size())));
		if (ActPointNum > ActPointList.size() || ActPointNum < 0)
		{
			cout << "ActPointNum out of range" << endl;
			return ERROR;
		}

		Point ActPoint;
		if (try_again == 3) ActPoint = ActPointList[ActPointNum];
		else //如果是再次尝试，从SeedPointList中取点
			ActPoint = (*SeedPointList)[ActPointNum];
		//在快速泊松采样中，如果生成的新点与周围的点距离不满足要求，立即退出，这样会导致空间存在浪费，error_count提供了几个容错次数
		int error_count = 0;
		//在活动点周围生成max_retry个点
		for (int i = 0; i < max_retry; i++)
		{
			Point NewPoint;
			int NewR = int(sqrt(R_min2)) * rng.uniform(1, 2);
			float NewAngle = rng.uniform(0., 2 * PI);
			//计算采样点坐标
			NewPoint.x = ActPoint.x + int(NewR * cos(NewAngle));
			NewPoint.y = ActPoint.y + int(NewR * sin(NewAngle));

			//验证新点
			//如果新点超出范围，将其放在边缘
			if (NewPoint.x<0 || NewPoint.x>N ||
				NewPoint.y<0 || NewPoint.y>M)
			{
				if (NewPoint.x < 0) NewPoint.x = 0;
				else if (NewPoint.x > N) NewPoint.x = N;
				if (NewPoint.y < 0) NewPoint.y = 0;
				else if (NewPoint.y > M) NewPoint.y = M;
			}
			//计算采样点的对应的区块
			col_x = floor(NewPoint.x / cell_size);
			row_y = floor(NewPoint.y / cell_size);
			if (grids[row_y][col_x].flag == 1)//该区块已经存在采样点
				continue;
			int flag = OK;
			//检查周围区块是否有小于最小距离的点
			//在这些区块之外的点距离一定满足，无需检验
			int min_x = floor((NewPoint.x - sqrt(R_min2)) / cell_size);
			int max_x = floor((NewPoint.x + sqrt(R_min2)) / cell_size);
			int min_y = floor((NewPoint.y - sqrt(R_min2)) / cell_size);
			int max_y = floor((NewPoint.y + sqrt(R_min2)) / cell_size);

			for (int i = min_x; i <= max_x; i++)
			{
				if (i<0 || i>floor(N / cell_size)) continue;
				for (int j = min_y; j <= max_y; j++)
				{
					if (j<0 || j>floor(M / cell_size)) continue;
					if (grids[j][i].flag == 1)//该区块存在活动点
					{
						if (j == int(ActPoint.y / cell_size) && i == int(ActPoint.x / cell_size))continue;
						if (Cal_Dis2(grids[j][i].pos, NewPoint) < R_min2)
						{
							flag = ERROR;
							break;
						}
					}
				}
			}

			//新采样点符合要求
			if (flag == OK)
			{
				ActPointList.push_back(NewPoint);
				SeedPointList->push_back(NewPoint);
				col_x = floor(NewPoint.x / cell_size);
				row_y = floor(NewPoint.y / cell_size);
				grids[row_y][col_x].Add_NewPoint(NewPoint);
				If_Found = TRUE;
				continue;
			}

			//新采样点不符合要求
			if (If_Found == FALSE)
			{
				if (error_count == 12 || error_count + i == max_retry)
				{
					vector<Point>::iterator it = ActPointList.begin() + ActPointNum;
					ActPointList.erase(it);
					if (ActPointList.size() == 0)
						try_again--;
					break;
				}
				else error_count++;
			}
		}
	}
	return OK;
}

/*************************************************
功能：计算两点之间距离的平方
返回值：R；两点距离的平方
*************************************************/
float Cal_Dis2(Point P1, Point P2)
{
	float R = pow((float)(P1.x - P2.x), 2.0) + pow((float)(P1.y - P2.y), 2.0);//两点距离
	return R;
}

void draw_Point(vector<Point> SeedPointList, int K, Mat img_gray)
{
	Mat img(M, N, CV_8UC3, Scalar(255, 255, 255));
	float Rmin = sqrt((M * N) / (float)K);
	int i = 0;
	for (vector<Point>::iterator it = SeedPointList.begin(); it != SeedPointList.end(); it++, i++)
	{
		circle(img_gray, *it, 2, Scalar(255, 255, 255), -1);
		circle(img, *it, 2, Scalar(0, 0, 0), -1);
		string Num = to_string(i + 1);
		putText(img, Num, *it, FONT_HERSHEY_PLAIN, 0.8f, Scalar(0, 0, 0));
//		ellipse(img, *it, Size(Rmin / 2, Rmin / 2), 0, 0, 360, Scalar(0, 0, 255), 1);
	}
	imshow("ex", img);
}

