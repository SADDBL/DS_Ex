#include"mission2.h"
#include"mission1.h"
#include"mission3.h"

using namespace cv;
using namespace std;

float Cal_Dis2(Point P1, Point P2);
int FastPossionDisc(int K, vector<Point>* ActPointList);
void draw_Point(vector<Point> SeedPointList, int K, Mat img_gray);

clock_t time_count_check, newpoint_generate_time, add_delete_time;

int main()
{
	cout << "����һ�����ȡ�㼰��ˮ���㷨" << endl;
	vector<Point> SeedPointList;//���ӵ��б�
	int K = 0;//���ӵ���
	string Temp;
	while (1)
	{
		try
		{
			cout << "���������ӵ������" << endl;
			cin >> Temp;
			K = stoi(Temp);
		}
		catch (exception& invalid_argument)
		{
			cout << "�����ʽ����������һ��������" << endl;
			continue;
		}
		if (K <= 0) cout << "����Ϊ��������������һ��������" << endl;
		else if (K > 5000) cout << "����ֵ�������С����ֵ" << endl;
		else break;
	}
	clock_t start, end;//���ڼ���ʱ��
	start = clock();
	Mat img = imread("C:\\Users\\Dragon\\Desktop\\1.jpg"); //("C:\\Users\\Dragon\\Desktop\\new.jpg");// ("C:\\Users\\Dragon\\Desktop\\1.jpg"); //("C:\\Users\\Dragon\\Desktop\\982cebcc84b82ef50b87d7a098424449.jpg");// ("C:\\Users\\Dragon\\Desktop\\1.jpg");
	Mat img_gray;
	Mat marker_mask;
	Mat Edge_img = Mat::zeros(img.size(), CV_32S);//��������
	//���ڴ�������
	vector<vector<Point>> Edge;
	vector<Vec4i> Hier;
	cout << img.size() << endl;
	//ͼ��Ԥ����
	cvtColor(img, marker_mask, COLOR_BGR2GRAY);
	cvtColor(marker_mask, img_gray, COLOR_GRAY2BGR);
	marker_mask = Scalar::all(0);


	/* preparation */
	/* ���ٲ���Բ���㷨�ɼ������ */
	if (FastPossionDisc(K, &SeedPointList) == OK)
	{
		cout << "ʵ�ʲ������ӵ���Ϊ��" << SeedPointList.size() << endl;
		draw_Point(SeedPointList, K, marker_mask);
		
	}
	else cout << "ERROR" << endl;

	/* mission1 */
	/* ��ˮ���㷨 */
	//Ѱ������
	cout << "����һ����ˮ���㷨" << endl;
	findContours(marker_mask, Edge, Hier, RETR_CCOMP, CHAIN_APPROX_SIMPLE);
	if (Edge.empty()) cout << "ERROR" << endl;
	int j=0;
	for (int i = 0; i >=0; i=Hier[i][0],j++)
	{
		//��marks���б�ǣ��Բ�ͬ������������б�ţ��൱������עˮ�㣬�ж������������ж���עˮ��
		drawContours(Edge_img, Edge, i, Scalar::all(j + 1), -1, 8, Hier, INT_MAX);
	}
	//���������ɫ��
	vector<Vec3b> colorTab;
	for (int i = 0; i < j; i++)
	{
		int b = theRNG().uniform(0, 255);
		int g = theRNG().uniform(0, 255);
		int radius = theRNG().uniform(0, 255);
		colorTab.push_back(Vec3b((uchar)b, (uchar)g, (uchar)radius));
	}
	//���Ʒ�ˮ��ͼ��
	watershed(img, Edge_img);
	Mat WaterShed_img = Mat(Edge_img.size(), CV_8UC3);
	for (int i = 0; i < Edge_img.rows; i++)
	{
		for (int k = 0; k < Edge_img.cols; k++)
		{
			int index = Edge_img.at<int>(i, k);
			if (index == -1)
				WaterShed_img.at<Vec3b>(i, k) = Vec3b(255, 255, 255);
			else if (index <= 0 || index > j)
				WaterShed_img.at<Vec3b>(i, k) = Vec3b(0, 0, 0);
			else
				WaterShed_img.at<Vec3b>(i, k) = colorTab[index - 1];
		}
	}
	WaterShed_img = WaterShed_img * 0.5 + img_gray * 0.5;

	float Rmin = sqrt((M * N) / (float)K);

	/*for (vector<Point>::iterator it = SeedPointList.begin(); it != SeedPointList.end(); it++)
	{
		circle(WaterShed_img, *it, 2, Scalar(0, 0, 0), -1);
	}*/

	imshow("watershed", WaterShed_img);
	cout << "Ԥ�ڲ������ӵ���Ϊ��" << K << endl;
	cout << "ʵ�ʲ������ӵ���Ϊ��" << SeedPointList.size() << endl;
	end = clock();
	cout << "����һ���ѵ�ʱ��" << (float)(end - start) / CLOCKS_PER_SEC << "s" << endl;
	cout << endl;
	cout << "�������������һ������" << endl;
	waitKey(0);
	destroyAllWindows();

	int mission = 0;
	while (1)
	{
		cout << "���� 1 ִ������2������ԭɫ��ɫ" << endl;
		cout << "���� 2 ִ������3������������۰����" << endl;
		cout << "���� 3 �˳�" << endl;
		cin >> Temp;
		if (Temp == "1")
		{
			mission = 1;
			break;
		}
		else if (Temp == "2")
		{
			mission = 2;
			break;
		}
		else if (Temp == "3") exit(0);
		else continue;
	}

	AdjGraphList Block_Graph;
	//�����ڽӱ�
	Creat_AdjList(Block_Graph, Edge_img, SeedPointList.size());

	vector<AreaNode> AreaList;
	Init_AreaList(AreaList, Block_Graph.vexnum);
	Area_Measure(Block_Graph, Edge_img, AreaList);

	/* mission2 */
	/* ��ԭɫ��ɫ*/
	if (mission == 1)
	{
		start = clock();
		Mat ColorImg = Mat(Edge_img.size(), CV_8UC3);
		

		//ShowBlock_Graph(Block_Graph);
		//�������У�׼��������ȱ���
		LinkQueue ColorFill_Path;
		ColorFill_Path.front = NULL;
		ColorFill_Path.rear = NULL;
		InitQueue(ColorFill_Path);
		if (SortPath(Block_Graph, ColorFill_Path) == ERROR)
		{
			return 0;
		}

	//	BFS_SortPath(Block_Graph, ColorFill_Path);
		DFS_FullColor(Block_Graph, ColorFill_Path, Edge_img, ColorImg);

		end = clock();
		cout << "��������ѵ�ʱ��" << (float)(end - start) / CLOCKS_PER_SEC << "s" << endl;
	}

	/***������***/
	else if (mission == 2)
	{
		//�����򣬵ݼ�����
		start = clock();
		HeapSort(AreaList);
		end = clock();
		clock_t time_ = end - start;
//		HeapSort_Result(Block_Graph, Edge_img);
		/*Mat HeapResultImg;
		WaterShed_img.copyTo(HeapResultImg);
		for (int i = 0; i < Block_Graph.vexnum; i++)
		{
			char str[5];
			sprintf_s(str, "%d", i + 1);
			putText(HeapResultImg, str, Block_Graph.vertices[AreaList[i].Block_No - 1].gross,
				FONT_HERSHEY_PLAIN, 0.8, Scalar(0, 0, 0));
		}
		imshow("Result", HeapResultImg);*/

		//�۰����
		cout << "����Ͻ�Ϊ��" << AreaList[0].area << endl;
		cout << "����½�Ϊ��" << AreaList[AreaList.size() - 1].area << endl;
		//cout<<"������Ҫ"
		int highNo, lowNo;
		if (Binary_Search(AreaList, Block_Graph.vexnum, highNo, lowNo, time_) == OK)
		{
			HighLight(AreaList, highNo, lowNo, Block_Graph.vertices, WaterShed_img, Edge_img, time_);
		}
		cout << "���������ѵ�ʱ��" << (float)(time_) / CLOCKS_PER_SEC << "s" << endl;
	}
	waitKey(0);
	destroyAllWindows();
	return 0;
}






