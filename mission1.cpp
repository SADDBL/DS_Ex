#include"mission1.h"

/*************************************************
���ܣ����ٲ���Բ�̲���
����ֵ��OK�����Ӳɼ����
		ERROR������
*************************************************/
int FastPossionDisc(int K, vector<Point>* SeedPointList)
{
	vector<Point> ActPointList;
	float R_min2 = (M * N) / (float)K;
	int max_retry = 120;//�ڻ����Χ���ɵĳ��Ե�ĸ�����һ��ȡ30
	int cell_size = int(sqrt(R_min2 / 16));//�����С
	int col_x = int(N / cell_size) + 1;//+1 ��ֹ���
	int row_y = int(M / cell_size) + 1;

	vector<vector<grid_cell>> grids;//�������row_y�У�col_x��
	grids.resize(row_y);
	for (vector<vector<grid_cell>>::iterator it = grids.begin(); it != grids.end(); it++)
	{
		it->resize(col_x);
		for (vector<grid_cell>::iterator jt = it->begin(); jt < it->end(); jt++) {
			jt->Init_grid_cell();
		}
	}
	RNG rng((unsigned)time(NULL));//�������������
	//��һ�������
	//Point p0 = Point(0,0);// (rng.uniform(0, N), rng.uniform(0, M));
	//if (ActPointList.size() != 0)
	//{
	//	cout << "ActPointList��Ϊ��" << endl;
	//	return ERROR;
	//}
	//else {
	//	ActPointList.push_back(p0);//��ActPointListΪ�գ���p0���룬����Ϊ��һ�����
	//	col_x = floor(p0.x / cell_size);
	//	row_y = floor(p0.y / cell_size);
	//	grids[row_y][col_x].Add_NewPoint(p0);//��ʾ�������Ѿ��е�
	//}
	//���ĸ����㿪ʼ�ɼ�����Ч���ñ�Ե�ռ�
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
		grids[row_y][col_x].Add_NewPoint(p0[i]);//��ʾ�������Ѿ��е�
	}
	int try_again = 3;//��Act Point ListΪ��ʱ�����ԵĴ���
	while (!ActPointList.empty() && try_again > 0)
	{
		int If_Found = FALSE;
		//�ӻ���б������ȡһ���㣬������Χ�����²����㣬���Լ���
		int ActPointNum;//������
		//��ȡ������
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
		else //������ٴγ��ԣ���SeedPointList��ȡ��
			ActPoint = (*SeedPointList)[ActPointNum];
		//�ڿ��ٲ��ɲ����У�������ɵ��µ�����Χ�ĵ���벻����Ҫ�������˳��������ᵼ�¿ռ�����˷ѣ�error_count�ṩ�˼����ݴ����
		int error_count = 0;
		//�ڻ����Χ����max_retry����
		for (int i = 0; i < max_retry; i++)
		{
			Point NewPoint;
			int NewR = int(sqrt(R_min2)) * rng.uniform(1, 2);
			float NewAngle = rng.uniform(0., 2 * PI);
			//�������������
			NewPoint.x = ActPoint.x + int(NewR * cos(NewAngle));
			NewPoint.y = ActPoint.y + int(NewR * sin(NewAngle));

			//��֤�µ�
			//����µ㳬����Χ��������ڱ�Ե
			if (NewPoint.x<0 || NewPoint.x>N ||
				NewPoint.y<0 || NewPoint.y>M)
			{
				if (NewPoint.x < 0) NewPoint.x = 0;
				else if (NewPoint.x > N) NewPoint.x = N;
				if (NewPoint.y < 0) NewPoint.y = 0;
				else if (NewPoint.y > M) NewPoint.y = M;
			}
			//���������Ķ�Ӧ������
			col_x = floor(NewPoint.x / cell_size);
			row_y = floor(NewPoint.y / cell_size);
			if (grids[row_y][col_x].flag == 1)//�������Ѿ����ڲ�����
				continue;
			int flag = OK;
			//�����Χ�����Ƿ���С����С����ĵ�
			//����Щ����֮��ĵ����һ�����㣬�������
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
					if (grids[j][i].flag == 1)//��������ڻ��
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

			//�²��������Ҫ��
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

			//�²����㲻����Ҫ��
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
���ܣ���������֮������ƽ��
����ֵ��R����������ƽ��
*************************************************/
float Cal_Dis2(Point P1, Point P2)
{
	float R = pow((float)(P1.x - P2.x), 2.0) + pow((float)(P1.y - P2.y), 2.0);//�������
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

