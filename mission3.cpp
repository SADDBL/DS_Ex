#include"mission3.h"
#include<string>

/*************************************************
���ܣ����ɴ󶥶�
������H�������
**************************************************/
void HeapAdjust(vector<AreaNode>& H,int s,int m)
{
	AreaNode temp = H[s - 1];
	for (int i = 2 * s; i <= m ; i *= 2)
	{
		if (i<m && H[i - 1].area>=H[i - 1 + 1].area)
			i++;
		if (temp.area < H[i-1].area) break;
		H[s-1] = H[i-1];
		s = i;
	}
	H[s-1] = temp;
}

/*************************************************
���ܣ�������
������H�������
**************************************************/
void HeapSort(vector<AreaNode>& H)
{
	for (int i = H.size() / 2; i > 0; i--)//��H[1...H.len]���ɴ󶥶�
	{
		HeapAdjust(H, i, H.size());
	}
	for (int i = H.size(); i > 1; i--)
	{
		AreaNode temp = H[0];
		H[0] = H[i - 1];
		H[i - 1] = temp;

		HeapAdjust(H, 1, i - 1);
	}
}

void HeapSort_Result(AdjGraphList &Block_Graph,Mat EdgeImg)//,vector<AreaNode>& H)
{
	for (int i = 0; i < Block_Graph.vexnum; i++)
	{
		int center_x = 0, center_y = 0, pixel_number = 0;
		for (int j = 1; j < EdgeImg.cols - 1; j++)
		{
			int* temp = EdgeImg.ptr<int>(j);
			for (int k = 1; k < EdgeImg.rows - 1; k++)
			{
				if (temp[k] == i + 1)
				{
					center_x += k;
					center_y += j;
					pixel_number++;
				}
			}
		}
		if (pixel_number != 0)
		{
			Block_Graph.vertices[i].gross.x = center_x / pixel_number;
			Block_Graph.vertices[i].gross.y = center_y / pixel_number;
		}
		else
		{
			Block_Graph.vertices[i].gross.x = center_x;
			Block_Graph.vertices[i].gross.y = center_y;
		}
	}
}

/*************************************************
���ܣ��۰����
������highNo��keyֵ����ţ���ֵ�����С��
	  lowNo��keyֵ����ţ���ֵ��С����
**************************************************/
int Binary_Search(vector<AreaNode> AreaList,int vexnum,int &highNo,int &lowNo,clock_t &time_)
{
	int max, min;
	string input;
	while (1)
	{
		try
		{
			cout << "����������½磺" << endl;
			cin >> input;
			min = stof(input);
		}
		catch (exception& invalid_argument)
		{
			cout << "�����ʽ����������һ����" << endl;
			continue;
		}
		if (min >= AreaList[0].area) cout << "�����½����" << endl;
		else break;
	}
	while (1)
	{
		try
		{
			cout << "����������Ͻ磺" << endl;
			cin >> input;
			max = stof(input);
		}
		catch (exception& invalid_argument)
		{
			cout << "�����ʽ����������һ����" << endl;
			continue;
		}
		if (max <= AreaList[AreaList.size() - 1].area) cout << "�����Ͻ��С" << endl;
		else if (max < min) cout << "�����Ͻ�С���½�" << endl;
		else break;
	}
	clock_t start, end;//���ڼ���ʱ��
	start = clock();
	int low = 1, high = vexnum, mid;
	//������lowNo
	while (low <= high)
	{
		mid = (low + high) / 2;
		if (AreaList[mid - 1].area == max)
		{
			highNo = mid;
			break;
		}
		else if (AreaList[mid - 1].area < max)
		{
			high = mid - 1;
			highNo = low;
		}
		else 
			low = mid + 1;
	}
	//������highNo
	high = vexnum;
//	low = 1;
	while (low <= high)
	{
		mid = (low + high) / 2;
		if (AreaList[mid - 1].area == min)
		{
			lowNo = mid;
			break;
		}
		else if (AreaList[mid - 1].area > min)
			low = mid + 1;
		else
		{
			high = mid - 1;
			lowNo = high;
		}
	}
	end = clock();
	time_ += end - start;
	
	if (highNo > lowNo)
	{
		cout << "�������������" << min << "����" << max << "֮��" << endl;
		return ERROR;
	}
	else
	{
		cout << "�������" << min << "����" << max << "֮��������У�" << endl;
		for (int i = highNo; i <= lowNo; i++)
		{
			cout << AreaList[i - 1].Block_No << ":" << AreaList[i - 1].area << endl;
		}
	}
	return OK;
}

void HighLight(vector<AreaNode> AreaList, int highNo, int lowNo, AdjList vertices,Mat watershed_img,Mat EdgeImg, clock_t& time_)
{
	Mat img = imread("C:\\Users\\Dragon\\Desktop\\1.jpg"); //("C:\\Users\\Dragon\\Desktop\\new.jpg");// ("C:\\Users\\Dragon\\Desktop\\1.jpg");
	Mat img_gray;
	cvtColor(img, img_gray, COLOR_BGR2GRAY);
	cvtColor(img_gray, img_gray, COLOR_GRAY2BGR);
	Mat temp;
	clock_t start, end;//���ڼ���ʱ��
	start = clock();
	watershed_img.copyTo(temp);
	for (int i = 0; i < EdgeImg.rows; i++)
	{
		int* pixel = EdgeImg.ptr<int>(i);
		for (int j = 0; j < EdgeImg.cols; j++)
		{
			int pixel_data = pixel[j];
			if (pixel_data > 0)
			{
				int flag = 0;
				for (int k = highNo; k <= lowNo; k++)
				{
					if (pixel_data == AreaList[k - 1].Block_No)
					{
						flag = 1;
						break;
					}
				}
				if (flag == 0)
					temp.at<Vec3b>(i, j) = Vec3b(0, 0, 0);
			}
			else if(pixel_data==-1)
				temp.at<Vec3b>(i, j) = Vec3b(255, 255, 255);
		}
	}
	for (int i = highNo; i <= lowNo; i++)
	{
		char str[6];
		sprintf_s(str, "%d", AreaList[i - 1].area);
		putText(temp, str, vertices[AreaList[i - 1].Block_No - 1].gross, FONT_HERSHEY_PLAIN, 0.8, Vec3b(255, 255, 255));
	}
	end = clock();
	time_ += end - start;
	imshow("Binary Search", temp);
}