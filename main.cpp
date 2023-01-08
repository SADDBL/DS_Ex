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
	cout << "任务一：随机取点及分水岭算法" << endl;
	vector<Point> SeedPointList;//种子点列表
	int K = 0;//种子点数
	string Temp;
	while (1)
	{
		try
		{
			cout << "请输入种子点个数：" << endl;
			cin >> Temp;
			K = stoi(Temp);
		}
		catch (exception& invalid_argument)
		{
			cout << "输入格式错误，请输入一个正整数" << endl;
			continue;
		}
		if (K <= 0) cout << "输入为非正数，请输入一个正整数" << endl;
		else if (K > 5000) cout << "输入值过大，请减小输入值" << endl;
		else break;
	}
	clock_t start, end;//用于计算时间
	start = clock();
	Mat img = imread("C:\\Users\\Dragon\\Desktop\\1.jpg"); //("C:\\Users\\Dragon\\Desktop\\new.jpg");// ("C:\\Users\\Dragon\\Desktop\\1.jpg"); //("C:\\Users\\Dragon\\Desktop\\982cebcc84b82ef50b87d7a098424449.jpg");// ("C:\\Users\\Dragon\\Desktop\\1.jpg");
	Mat img_gray;
	Mat marker_mask;
	Mat Edge_img = Mat::zeros(img.size(), CV_32S);//绘制轮廓
	//用于储存轮廓
	vector<vector<Point>> Edge;
	vector<Vec4i> Hier;
	cout << img.size() << endl;
	//图像预处理
	cvtColor(img, marker_mask, COLOR_BGR2GRAY);
	cvtColor(marker_mask, img_gray, COLOR_GRAY2BGR);
	marker_mask = Scalar::all(0);


	/* preparation */
	/* 快速泊松圆盘算法采集随机点 */
	if (FastPossionDisc(K, &SeedPointList) == OK)
	{
		cout << "实际产生种子点数为：" << SeedPointList.size() << endl;
		draw_Point(SeedPointList, K, marker_mask);
		
	}
	else cout << "ERROR" << endl;

	/* mission1 */
	/* 分水岭算法 */
	//寻找轮廓
	cout << "任务一：分水岭算法" << endl;
	findContours(marker_mask, Edge, Hier, RETR_CCOMP, CHAIN_APPROX_SIMPLE);
	if (Edge.empty()) cout << "ERROR" << endl;
	int j=0;
	for (int i = 0; i >=0; i=Hier[i][0],j++)
	{
		//对marks进行标记，对不同区域的轮廓进行编号，相当于设置注水点，有多少轮廓，就有多少注水点
		drawContours(Edge_img, Edge, i, Scalar::all(j + 1), -1, 8, Hier, INT_MAX);
	}
	//设置随机颜色盘
	vector<Vec3b> colorTab;
	for (int i = 0; i < j; i++)
	{
		int b = theRNG().uniform(0, 255);
		int g = theRNG().uniform(0, 255);
		int radius = theRNG().uniform(0, 255);
		colorTab.push_back(Vec3b((uchar)b, (uchar)g, (uchar)radius));
	}
	//绘制分水岭图样
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
	cout << "预期产生种子点数为：" << K << endl;
	cout << "实际产生种子点数为：" << SeedPointList.size() << endl;
	end = clock();
	cout << "任务一花费的时间" << (float)(end - start) / CLOCKS_PER_SEC << "s" << endl;
	cout << endl;
	cout << "按任意键进行下一个任务" << endl;
	waitKey(0);
	destroyAllWindows();

	int mission = 0;
	while (1)
	{
		cout << "输入 1 执行任务2——四原色填色" << endl;
		cout << "输入 2 执行任务3——堆排序和折半查找" << endl;
		cout << "输入 3 退出" << endl;
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
	//创建邻接表
	Creat_AdjList(Block_Graph, Edge_img, SeedPointList.size());

	vector<AreaNode> AreaList;
	Init_AreaList(AreaList, Block_Graph.vexnum);
	Area_Measure(Block_Graph, Edge_img, AreaList);

	/* mission2 */
	/* 四原色填色*/
	if (mission == 1)
	{
		start = clock();
		Mat ColorImg = Mat(Edge_img.size(), CV_8UC3);
		

		//ShowBlock_Graph(Block_Graph);
		//创建队列，准备广度优先遍历
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
		cout << "任务二花费的时间" << (float)(end - start) / CLOCKS_PER_SEC << "s" << endl;
	}

	/***任务三***/
	else if (mission == 2)
	{
		//堆排序，递减排序
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

		//折半查找
		cout << "面积上界为：" << AreaList[0].area << endl;
		cout << "面积下界为：" << AreaList[AreaList.size() - 1].area << endl;
		//cout<<"请输入要"
		int highNo, lowNo;
		if (Binary_Search(AreaList, Block_Graph.vexnum, highNo, lowNo, time_) == OK)
		{
			HighLight(AreaList, highNo, lowNo, Block_Graph.vertices, WaterShed_img, Edge_img, time_);
		}
		cout << "任务三花费的时间" << (float)(time_) / CLOCKS_PER_SEC << "s" << endl;
	}
	waitKey(0);
	destroyAllWindows();
	return 0;
}






