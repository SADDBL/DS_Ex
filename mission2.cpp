#include"mission2.h"
//#include"mission1.h"
#define StepBackMax 30
#define BackSteps 20

/*************************************************
���ܣ��������������ڽӱ�
������Block_Graph��ΪͼƬ�����鴴�����ڽӱ�
	  Edge_img������ͼ
	  SeedPoint_Num��ʵ�ʲ��������ӵ���
**************************************************/
int Creat_AdjList(AdjGraphList& Block_Graph,Mat Edge_img,int SeedPoint_num)
{
	Block_Graph.vexnum = SeedPoint_num;//�������������ӵ���
	for (int i = 0; i < Block_Graph.vexnum; i++)
	{
		Block_Graph.vertices[i].adjvex = i + 1;
		Block_Graph.vertices[i].firstarc = NULL;
		
	}
	int this_block = 1, neighbor_block = 1;
	int neighbor_block_dot[8];

	//�������ص�
	for (int i = 1; i < Edge_img.rows - 1; i++)
	{
		int* PixelPoint_col_1 = Edge_img.ptr<int>(i - 1);
		int* PixelPoint_col_2 = Edge_img.ptr<int>(i);//��i�����ص�ͷָ��
		int* PixelPoint_col_3 = Edge_img.ptr<int>(i + 1);
		for (int j = 1; j < Edge_img.cols - 1; j++)
		{
			int this_dot = PixelPoint_col_2[j];
			neighbor_block_dot[0] = PixelPoint_col_1[j-1];
			neighbor_block_dot[1] = PixelPoint_col_1[j];
			neighbor_block_dot[2] = PixelPoint_col_1[j+1];
			neighbor_block_dot[3] = PixelPoint_col_2[j - 1];
			neighbor_block_dot[4] = PixelPoint_col_2[j + 1];
			neighbor_block_dot[5] = PixelPoint_col_3[j - 1];
			neighbor_block_dot[6] = PixelPoint_col_3[j];
			neighbor_block_dot[7] = PixelPoint_col_3[j + 1];
			// nd  nd  nd
			// nd  td  nd
			// nd  nd  nd
			if (this_dot == -1)//����õ��������Ե
			{
				for (int k = 0; k < 8; k++)
				{
					if (neighbor_block_dot[k] > 0)
					{
						this_block = neighbor_block_dot[k];
						break;
					}
				}
				for (int k = 0; k < 8; k++)
				{
					if (neighbor_block_dot[k] > 0 && 
						neighbor_block_dot[k] != this_block)
					{
						neighbor_block = neighbor_block_dot[k];
						break;
					}
				}

				//���this_block������δ����ͼ�Ķ����б�
				if (Block_Graph.vertices[this_block - 1].firstarc == NULL)
				{
					ArcNode* p = NULL;
					if ((p = (ArcNode*)malloc(sizeof(ArcNode))) == NULL)
					{
						cout << "ArcNode Failed to Request memory" << endl;
						exit(1);
					}
					//����������붥���б�
					p->nextarc = NULL;
					p->adjvex = neighbor_block;//��ָ��Ķ�������
					Block_Graph.vertices[this_block - 1].firstarc = p;
				}
				//���this_block�����Ѿ�����ͼ�Ķ����б�
				else if (Block_Graph.vertices[this_block - 1].firstarc != NULL)
				{
					int IsDuplicate = FALSE;//�Ƿ��ظ��ı�ʶ��
					ArcNode* p = NULL;
					ArcNode* temp = Block_Graph.vertices[this_block - 1].firstarc;
					//��������
					while (temp != NULL)
					{
						if (temp->adjvex != neighbor_block && temp->nextarc != NULL)
						{
							temp = temp->nextarc;
						}
						else if (temp->adjvex == neighbor_block ||
							Block_Graph.vertices[this_block - 1].adjvex == neighbor_block)
						{
							IsDuplicate = TRUE;
							break;
						}
						else break;
					}
					//��û������
					if (IsDuplicate == FALSE)
					{
						ArcNode* p = NULL;
						if ((p = (ArcNode*)malloc(sizeof(ArcNode))) == NULL)
						{
							cout << "ArcNode Failed to Request memory" << endl;
							exit(1);
						}
						p->adjvex = neighbor_block;
						p->nextarc = NULL;
						temp->nextarc = p;
					}
				}

				//���neighbor_block������δ����ͼ�Ķ����б�
				if (Block_Graph.vertices[neighbor_block - 1].firstarc == NULL)
				{
					ArcNode* p = NULL;
					if ((p = (ArcNode*)malloc(sizeof(ArcNode))) == NULL)
					{
						cout << "ArcNode Failed to Request memory" << endl;
						exit(1);
					}
					//����������붥���б�
					p->nextarc = NULL;
					p->adjvex = this_block;//��ָ��Ķ�������
					Block_Graph.vertices[neighbor_block - 1].firstarc = p;
				}
				//���neighbor_block�����Ѿ�����ͼ�Ķ����б�
				else if (Block_Graph.vertices[neighbor_block - 1].firstarc != NULL)
				{
					int IsDuplicate = FALSE;//�Ƿ��ظ��ı�ʶ��
					ArcNode* p = NULL;
					ArcNode* temp = Block_Graph.vertices[neighbor_block - 1].firstarc;
					//��������
					while (temp != NULL)
					{
						if (temp->adjvex != this_block && temp->nextarc != NULL)
						{
							temp = temp->nextarc;
						}
						else if (temp->adjvex == this_block ||
							Block_Graph.vertices[neighbor_block - 1].adjvex == neighbor_block)
						{
							IsDuplicate = TRUE;
							break;
						}
						else break;
					}
					//��û������
					if (IsDuplicate==FALSE)
					{
						ArcNode* p = NULL;
						if ((p = (ArcNode*)malloc(sizeof(ArcNode))) == NULL)
						{
							cout << "ArcNode Failed to Request memory" << endl;
							exit(1);
						}
						p->adjvex = this_block;
						p->nextarc = NULL;
						temp->nextarc = p;

					}
				}
			}
		}
	}

	if (AdjList_Sort(Block_Graph) != OK) return ERROR;
	return OK;
}

/*************************************************
���ܣ��Ż��������ڽӱ����ݶ���ĶȽ��������ڽ�����
������Block_Graph��ΪͼƬ�����鴴�����ڽӱ�
**************************************************/
int AdjList_Sort(AdjGraphList& Block_Graph)
{
	int AdjList_len = 0;
	int Block_No_List[Max_Vertex_Num];
	for (int i = 0; i < Max_Vertex_Num; i++) Block_No_List[i] = 0;
	//������ڽӱ�������
	for (int i = 0; i < Block_Graph.vexnum; i++)
	{
		ArcNode* temp = Block_Graph.vertices[i].firstarc;
		while (temp != NULL)
		{
			AdjList_len++;
			temp = temp->nextarc;
		}
		Block_No_List[i] = AdjList_len;
		AdjList_len = 0;
	}
	for (int i = 0; i < Block_Graph.vexnum; i++)
	{
		Block_Graph.vertices[i].Block_No = Block_No_List[i];
		ArcNode* temp = Block_Graph.vertices[i].firstarc;
		while (temp != NULL)
		{
			temp->degree = Block_No_List[temp->adjvex - 1];
			temp = temp->nextarc;
		}
	}

	for (int i = 0; i < Block_Graph.vexnum; i++)
	{
		for (int j = 0; j < Block_No_List[i] - 1; j++)
		{
			ArcNode* temp = Block_Graph.vertices[i].firstarc;
			ArcNode* shadow = NULL;//Ӱ�ӽ��
			for (int k = 0; k < Block_No_List[i] - j - 1; k++)
			{
				//����Ҫ����˳��
				if (temp->degree < temp->nextarc->degree)
				{
					//����һ��������ű���һ�����С������
					if (Block_Graph.vertices[i].firstarc == temp)
					{
						ArcNode* NewFirst = temp->nextarc;
						temp->nextarc = temp->nextarc->nextarc;
						NewFirst->nextarc = temp;
						Block_Graph.vertices[i].firstarc = NewFirst;
						shadow = NewFirst;
					}
					else
					{
						ArcNode* TempVex = temp->nextarc;
						temp->nextarc = temp->nextarc->nextarc;
						TempVex->nextarc = temp;
						shadow->nextarc = TempVex;//***
						shadow = TempVex;
					}
				}
				//������Ҫ����˳��˳����һ������
				else
				{
					shadow = temp;
					temp = temp->nextarc;
				}
			}
		}
	}
	return OK;
}

/**
 * @brief  ���ö����������Ϳɫ·��
 * @param  Block_Graph: �ڽӱ�
 * @param  Path: Ϳɫ·������
 * @return ����ִ��״̬
 */
int SortPath(AdjGraphList& Block_Graph, LinkQueue& Path)
{
	LinkQueue Q;
	int* visited;
	if ((visited = (int*)malloc(Block_Graph.vexnum * sizeof(int))) == NULL)
	{
		cout << "Visited Failed to Request Memory" << endl;
		exit(1);
	}
	for (int i = 0; i < Block_Graph.vexnum; i++)
	{
		visited[i] = FALSE;
		if (Block_Graph.vertices[i].firstarc == NULL)
			visited[i] = TRUE;
	}
	InitQueue(Q);

	while (Is_AllVisited(Block_Graph, visited) == FALSE)
	{
		int BlockNo;
		BlockNo = BestColorBlock(Block_Graph, visited);
		if (BlockNo == ERROR)
		{
			cout << "ERROR" << endl;
			return ERROR;
		}
		if (visited[BlockNo - 1] == FALSE)
		{
			visited[BlockNo - 1] = TRUE;

			InQueue(Path, BlockNo);
			InQueue(Q, BlockNo);
			while (EmptyQueue(Q) == FALSE)
			{
				int e;
				DeQueue(Q, e);
				ArcNode* temp = Block_Graph.vertices[e - 1].firstarc;
				if (temp == NULL) break;
				while (temp != NULL)
				{
					if (visited[temp->adjvex - 1] == FALSE)
					{
						visited[temp->adjvex - 1] = TRUE;
						InQueue(Q, temp->adjvex);
						InQueue(Path, temp->adjvex);
					}
					temp = temp->nextarc;
				}
			}
		}
	}

	cout << "�����ɫ·��Ϊ��" << endl;
	LinkQueue temp = Path;
	temp.front = temp.front->next;
	while (temp.front!=temp.rear)
	{
		cout << temp.front->data << "->";
		temp.front = temp.front->next;
	}
	cout << temp.front->data << endl;
	return OK;
}

/*************************************************
���ܣ�������������㷨��������ɫ
������Block_Graph��ΪͼƬ�����鴴�����ڽӱ�
	  Path����ɫ·�����б�
	  EdgeImg����ˮ��ͼ��
	  ColorImg��������ɫͼ��
**************************************************/
void DFS_FullColor(AdjGraphList& Block_Graph, LinkQueue Path, Mat EdgeImg, Mat ColorImg)
{
	int pop_record[Max_Vertex_Num] = { 0 };
	// ��ʼ��һ����¼��ɫ�Ƿ��õļ�¼��
	int colors_used_matrix[Max_Vertex_Num][4] = { 0 };
	// ����һ��������к�һ����תվ��ǰ�����ڴ洢���˳��������򣬺�����������������е�˳��
	LinkQueue buffer;
	LinkQueue transfer_station;
	LinkStack Buffer;
	InitStack(Buffer);
	InitQueue(buffer);
	InitQueue(transfer_station);
	// ������ջ
	LinkStack stack = NULL;
	InitStack(stack);
	int e;
	// ��ʼͿɫ��Ҫ�󾡿�������ͬһ����ɫȥͿ��ʵ�ڳ�ͻ�ٻ���ɫ
	// ��������޷�Ϳɫ���������һ��������ɫ�ٳ���
	// ���Ϳɫ���зǿգ���δ���Ϳɫ�����������
	while (EmptyQueue(Path) == FALSE || EmptyQueue(buffer) == FALSE)
	{
		// ���Ϳɫ����һֱ����ĳ���ط����Ȳ��ܼ�����ǰͿ���ֲ��������ˣ�˵������������ͬ
		// ��ô��������50�����������ƶ�Ϳɫ�������˹��̷ǳ���Ҫ
		for (int i = 0; i < Block_Graph.vexnum; i++)
		{
			// ���ĳ�����Ƶ�����ˣ����ﵽ��ֵ����ֱ�ӻ���50�����
			if (pop_record[i] >= 250)
			{
				pop_record[i] = 0;
				for (int j = 0; j < 50; j++)
				{
					int temp_area_number;
					if (EmptyStack(stack) == TRUE) break;
					StackPop(stack, temp_area_number);
					// ������תվ
					InQueue(transfer_station, temp_area_number);
					//StackPush(Buffer, temp_area_number, NoColor);

					while (EmptyQueue(buffer) == FALSE)
					{
						int transfer;
						DeQueue(buffer, transfer);
						InQueue(transfer_station, transfer);
					}

					while (EmptyQueue(transfer_station) == FALSE)
					{
						int transfer;
						DeQueue(transfer_station, transfer);
						InQueue(buffer, transfer);
					}
				}
			}
		}
		// ���buffer�Ƿ�Ϊ�գ���ΪҪ���ȴ���buffer�еĽ�㣬��buffer���޽�㣬�ٴ�Path��ȡ����Ϳɫ���
		if (EmptyQueue(buffer) == FALSE)
			//if(EmptyStack(Buffer) == FALSE)
		{
			bool left_colors_table[4] = { 0 };
			DeQueue(buffer, e);
			//StackPop(Buffer, e);
			// ֻ�е�ջ�ĳ��ȴ���0��ȥ���
			if (EmptyStack(stack) == FALSE)
			{
				// ����ʱ��ҪͿɫ��������ڽ��������ɫ�Ƿ��ͻ
				// �������ڵ�ջ�������Ƿ������ڵ�����
				LinkStack search_stack = stack;
				while (search_stack != NULL)
				{
					ArcNode* temp = Block_Graph.vertices[e - 1].firstarc;
					// ��ʼ���Լ����ڽ�����ȥ�����Ƿ������Ԫ��
					while (temp != NULL)
					{
						if (temp->adjvex == search_stack->adjvex)
							left_colors_table[search_stack->color] = 1;
						temp = temp->nextarc;
					}
					search_stack = search_stack->next;
				}
				// ��Ѱ���������ڿ�����ʣ��ʲô��ɫ����Ϳ
				int tinted = FALSE;
				// �������ɫʣ�£��������ջ�Ȳ���
				for (int i = 0; i < 4; i++)
				{
					if (left_colors_table[i] != 1 && colors_used_matrix[e - 1][i] == 0)
					{
						StackPush(stack, e, i);
						colors_used_matrix[e - 1][i] = 1;
						tinted = TRUE;
						break;
					}
				}
				// �����û����ɫʣ�£�֤����ǰ�����ɫͿ��Ҳ�����⣬����Ҫ����
				if (tinted == FALSE)
				{
					for (int i = 0; i < 4; i++)
						colors_used_matrix[e - 1][i] = 0;


					int temp_area_number;
					StackPop(stack, temp_area_number);
					pop_record[temp_area_number - 1]++;

					/*StackPush(Buffer, temp_area_number, NoColor);
					StackPush(Buffer, e, NoColor);*/

					// ������תվ
					InQueue(transfer_station, temp_area_number);
					InQueue(transfer_station, e);

					while (EmptyQueue(buffer) == FALSE)
					{
						int transfer;
						DeQueue(buffer, transfer);
						InQueue(transfer_station, transfer);
					}

					while (EmptyQueue(transfer_station))
					{
						int transfer;
						DeQueue(transfer_station, transfer);
						InQueue(buffer, transfer);
					}
				}
			}
			// ���ջΪ��
			else
			{
				int flag = FALSE;
				// ˵���ǵ�һ������
				for (int i = 0; i < 4; i++)
				{
					if (colors_used_matrix[e - 1][i] != 1)
					{
						StackPush(stack, e, i);
						colors_used_matrix[e - 1][i] = 1;
						flag = TRUE;
						break;
					}
				}
				if (flag == FALSE)
					cout << "Ϳɫʧ�ܣ�" << endl;
			}
		}


		else if (EmptyQueue(buffer) == TRUE)
			//else if(EmptyStack(Buffer) == TRUE)
		{
			// һ��Ϳɫ������ų�����
			DeQueue(Path, e);
			// ���RGBY��ʹ�������0��ʾδʹ�ã�1��ʾ��ʹ��
			bool left_colors_table[4] = { 0 };
			// ֻ�е�ջ�ĳ��ȴ���0��ȥ���
			if (EmptyStack(stack) == FALSE)
			{
				// ����ʱ��ҪͿɫ��������ڽ��������ɫ�Ƿ��ͻ
				// �������ڵ�ջ�������Ƿ������ڵ�����
				LinkStack search_stack = stack;
				while (search_stack != NULL)
				{
					ArcNode* temp = Block_Graph.vertices[e - 1].firstarc;
					// ��ʼ���Լ����ڽ�����ȥ�����Ƿ������Ԫ��
					while (temp != NULL)
					{
						if (temp->adjvex == search_stack->adjvex)
							left_colors_table[search_stack->color] = 1;
						temp = temp->nextarc;
					}
					search_stack = search_stack->next;
				}
				// ��Ѱ���������ڿ�����ʣ��ʲô��ɫ����Ϳ
				int tinted = FALSE;
				// �������ɫʣ�£��������ջ�Ȳ���
				for (int i = 0; i < 4; i++)
				{
					if (left_colors_table[i] != 1)
					{
						StackPush(stack, e, i);
						colors_used_matrix[e - 1][i] = 1;
						tinted = TRUE;
						break;
					}
				}
				// ���û����ɫʣ�£�֤��ǰ����ɫͿ�������⣬��Ҫ����
				if (tinted == FALSE)
				{
					int temp_area_number;
					StackPop(stack, temp_area_number);
					pop_record[temp_area_number]++;
					InQueue(buffer, temp_area_number);
					InQueue(buffer, e);
					/*StackPush(Buffer, temp_area_number, NoColor);
					StackPush(Buffer, e, NoColor);*/
				}
			}
			// ���ջΪ��
			else
			{
				// ˵���ǵ�һ���������뿼���Ƿ��ͻ��ֱ����ջ
				StackPush(stack, e, Red);
				colors_used_matrix[e - 1][Red] = 1;
			}
		}
	}


	//Ϳɫ
	Mat Img = imread("C:\\Users\\Dragon\\Desktop\\1.jpg"); //("C:\\Users\\Dragon\\Desktop\\new.jpg");// ("C:\\Users\\Dragon\\Desktop\\1.jpg"); //("C:\\Users\\Dragon\\Desktop\\982cebcc84b82ef50b87d7a098424449.jpg");//imread("C:\\Users\\Dragon\\Desktop\\1.jpg");
	Mat Img_Gray;
	cvtColor(Img, Img_Gray, COLOR_BGR2GRAY);
	cvtColor(Img_Gray, Img_Gray, COLOR_GRAY2BGR);
	Mat FourColor_Img = Mat(EdgeImg.size(), CV_8UC3);
	for (int i = 0; i < EdgeImg.rows; i++)
	{
		int* pixel_row = EdgeImg.ptr<int>(i);
		for (int j = 0; j < EdgeImg.cols; j++)
		{
			//int pixel_data = EdgeImg.at<int>(i, j);
			//int pixel_data = EdgeImg.ptr<int>(i)[j];
			int pixel_data = pixel_row[j];
			SNode* Stemp = stack;
			if (pixel_data > 0)
			{
				while (Stemp->next != NULL)
				{
					if (Stemp->adjvex == pixel_data)
					{
						switch ((Stemp->color + 1))
						{
						case Red:
							FourColor_Img.at<Vec3b>(i, j) = Vec3b(0, 0, 255);
							ColorImg.at<Vec3b>(i, j) = Vec3b(0, 0, 255);
							break;
						case Green:
							FourColor_Img.at<Vec3b>(i, j) = Vec3b(0, 255, 0);
							ColorImg.at<Vec3b>(i, j) = Vec3b(0, 255, 0);
							break;
						case Blue:
							FourColor_Img.at<Vec3b>(i, j) = Vec3b(255, 0, 0);
							ColorImg.at<Vec3b>(i, j) = Vec3b(255, 0, 0);
							break;
						case Yellow:
							FourColor_Img.at<Vec3b>(i, j) = Vec3b(0, 255, 255);
							ColorImg.at<Vec3b>(i, j) = Vec3b(0, 255, 255);
							break;
						default:
							break;
						}
					}
					Stemp = Stemp->next;
				}
			}
			else if (pixel_data == -1)
			{
				FourColor_Img.at<Vec3b>(i, j) = Vec3b(255, 255, 255);
				ColorImg.at<Vec3b>(i, j) = Vec3b(255, 255, 255);
			}
		}
	}
	FourColor_Img = 0.5 * FourColor_Img + 0.5 * Img_Gray;
	imshow("FourColor", FourColor_Img);
}

void Init_AreaList(vector<AreaNode>& AreaList, int len)
{
	for (int i = 0; i < len; i++)
	{
		AreaNode temp;
		temp.area = 0;
		temp.Block_No = i + 1;
		AreaList.push_back(temp);
	}
}

/*************************************************
���ܣ��������������
������Block_Graph��ΪͼƬ�����鴴�����ڽӱ�
	  AreaList������б�
**************************************************/
void Area_Measure(AdjGraphList& Block_Graph, Mat EdgeImg, vector<AreaNode>& AreaList)
{
	for (int i = 0; i < EdgeImg.rows-  1; i++)
	{
		int* data = EdgeImg.ptr<int>(i);
		for (int j = 0; j < EdgeImg.cols - 1; j++)
		{
			if (data[j] != -1 && data[j] > 0)
			{
				AreaList[data[j]].area++;
			}
		}
	}
	for (int i = 0; i < Block_Graph.vexnum; i++)
	{
		ArcNode* temp = Block_Graph.vertices[i].firstarc;
		while (temp != NULL)
		{
			temp->area = AreaList[temp->adjvex - 1].area;
			temp = temp->nextarc;
		}
	}
}

int SelectColor(AdjGraphList& Block_Graph,int *Block_Color,int Color_BlockNo)
{
	if (Block_Color[Color_BlockNo - 1] < 0) Block_Color[Color_BlockNo - 1] = 0;
	int color0 = Block_Color[Color_BlockNo - 1];
	int Is_Settled = TRUE;
	int count = 0;
	do
	{
		count++;
		if (count == 4) cout << count;
		Is_Settled = TRUE;
		if (Block_Color[Color_BlockNo - 1] != Yellow)
		{
			Block_Color[Color_BlockNo - 1]++;
		}
		else Block_Color[Color_BlockNo - 1] = Red;

		ArcNode* temp = Block_Graph.vertices[Color_BlockNo - 1].firstarc;
		while (temp->nextarc != NULL && temp != NULL)
		{
			if (Block_Color[Color_BlockNo - 1] == Block_Color[temp->adjvex - 1])
			{
				//Block_Color[Color_BlockNo - 1] = color0;
				//return ERROR;//������������ͬ��ɫ
				Is_Settled = FALSE;
				break;
			}
			temp = temp->nextarc;
			//else return OK;
		}
		if (Block_Color[Color_BlockNo - 1] == color0) cout << "InsideOut" << endl;
		if (Is_Settled == TRUE)
		{
			
			return OK;
		}
	} while (Block_Color[Color_BlockNo - 1] != color0);//&& Block_Color[Color_BlockNo - 1] != Yellow);
	if (count == 4) cout << "Out" << endl;
	Block_Color[Color_BlockNo - 1] = color0;
	return ERROR;
}

int SelectColor(AdjGraphList& Block_Graph, int Color_BlockNo,int &color, LinkStack stack,int (*color_table)[4])
{
	int left_color[4] = { 0 };
	StackNode* search_stack = stack;
	//ɸѡ������ɫ
	while (search_stack != NULL)// && search_stack->next != NULL)
	{
		ArcNode* temp = Block_Graph.vertices[Color_BlockNo - 1].firstarc;
		while (temp != NULL)// && temp->nextarc != NULL)
		{
			if (temp->adjvex == search_stack->adjvex)
			{
				left_color[search_stack->color - 1] = 1;
			}
			temp = temp->nextarc;
		}
		search_stack = search_stack->next;
	}

	for (int i = 0; i < 4; i++)
	{
		if (left_color[i] != 1 && color_table[Color_BlockNo - 1][i] == 0)
		{
			color = i + 1;
			return OK;
		}
	}
	return ERROR;
}

/**  ������غ���  **/

/*************************************************
���ܣ���ʼ������
**************************************************/
void InitQueue(LinkQueue& Q)
{
	if ((Q.front = (Queue)malloc(sizeof(QNode))) == NULL)
	{
		cout << "QNode Failed to Request memory" << endl;
		exit(1);
	}
	Q.rear = Q.front;
	Q.front->next = NULL;
}

/*************************************************
���ܣ��Ӷ�β���
**************************************************/
int InQueue(LinkQueue& q,int e)
{
	QNode* temp;
	if ((temp = (Queue)malloc(sizeof(QNode))) == NULL)
	{
		cout << "QNode Failed to Request memory" << endl;
		exit(1);
	}
	temp->data = e;
	temp->next = NULL;
	if (q.rear == NULL)//�ж��Ƿ�Ϊ�ն���
	{
		q.front = q.rear = temp;
	}
	else
	{
		q.rear->next = temp;
		q.rear = temp;
	}
	/*q.rear->next = temp;
	q.rear = temp;*/
	return OK;
}

/*************************************************
���ܣ��Ӷ�ͷ����
**************************************************/
int DeQueue(LinkQueue& q, int& e)
{
	if (q.rear == q.front) return ERROR;//������Ϊ��
	QNode* temp;
	if ((temp = (Queue)malloc(sizeof(QNode))) == NULL)
	{
		cout << "QNode Failed to Request memory" << endl;
		exit(1);
	}
	temp = q.front->next;
	e = temp->data;
	q.front->next = temp->next;
	if (temp == q.rear) q.rear = q.front;//��ɾ�������һ��Ԫ�أ�βָ��ָ��ͷָ��
	free(temp);
	temp = NULL;
	return OK;
}

/*************************************************
���ܣ��ж϶����Ƿ�Ϊ��
����ֵ��FALSE�����в�Ϊ��
		TRUE������Ϊ��
**************************************************/
int EmptyQueue(LinkQueue q)
{
	if (q.front->next == NULL) return TRUE;
	return FALSE;
}

/**  ������غ�������  **/

void ShowBlock_Graph(AdjGraphList &Block_Graph)
{
	cout << "�����ڽӱ�Ϊ��" << endl;
	for (int i = 0; i < Block_Graph.vexnum; i++)
	{
		cout << Block_Graph.vertices[i].adjvex << " - ";
		ArcNode* temp = Block_Graph.vertices[i].firstarc;
		while (temp != NULL && temp->nextarc != NULL)
		{
			cout << temp->adjvex << "," << temp->degree << " - ";
			temp = temp->nextarc;
		}
		if (temp != NULL && temp->nextarc == NULL)
		{
			cout << temp->adjvex << "," << temp->degree << endl;
		}
		else if (temp == NULL) cout << "NULL" << endl;
	}
}

/**  ��ջ��غ�����ʼ  **/

//��ջ��ʼ��
void InitStack(LinkStack &S)
{
	if ((S = (LinkStack)malloc(sizeof(StackNode))) == NULL)
	{
		cout << "Stack Failed to Require memory" << endl;
		exit(1);
	}
//	S->adjvex = 0;
//	S->color = NoColor;
	S->next = NULL;
}

/*************************************************
���ܣ���ջ����ջ
**************************************************/
void StackPush(LinkStack& S,int adjvex,int color)
{
	LinkStack p;
	if ((p = (LinkStack)malloc(sizeof(StackNode))) == NULL)
	{
		cout << "Stack Failed to Require memory" << endl;
		exit(1);
	}
	p->adjvex = adjvex;
	p->color = color;
	p->next = S;
	S = p;
}

/*************************************************
���ܣ���ջ����ջ
**************************************************/
int StackPop(LinkStack& S, int& e)
{
	if (S == NULL) return ERROR;
	e = S->adjvex;
	LinkStack p;
	/*if ((p = (LinkStack)malloc(sizeof(StackNode))) == NULL)
	{
		cout << "Stack Failed to Require memory" << endl;
		exit(1);
	}*/
	p = S;
	S = S->next;
	free(p);
	p = NULL;
	return OK;
}

/*************************************************
���ܣ��ж϶�ջ�Ƿ�Ϊ��
����ֵ��FALSE����ջ��Ϊ��
		TRUE����ջΪ��
**************************************************/
int EmptyStack(LinkStack S)
{
	if (S->next == NULL) return TRUE;
	return FALSE;
}

/**  ��ջ��غ�������  **/

/**
 * @brief  �ж��Ƿ����еĽ�㶼�����ʹ�
 * @param  Block_Graph: �ڽӱ�
 * @param  visited: ��¼�Ƿ���ʹ��ı�־����
 * @return ����ִ��״̬
 */
int Is_AllVisited(AdjGraphList& BG, int* visited)
{
	for (int i = 0; i < BG.vexnum; i++)
	{
		if (visited[i] == FALSE)
			return FALSE;
	}
	return TRUE;
}

/**
 * @brief  ѡȡ���ŵ�Ϳɫ����
 * @param  BG: �ڽӱ�
 * @param  visited: ��¼�Ƿ���ʹ��ı�־����
 * @return ���ŵ�Ϳɫ����ı�ţ������ֹ��ϣ�����ERROR
 */
int BestColorBlock(AdjGraphList &BG,int *visited)
{
	int BlockNo = -1;
	int length = 0;

	for (int i = 0; i < BG.vexnum; i++)
	{
		if (visited[i] == FALSE)
		{
			int lenTemp = UnvistedNum_Count(BG, i + 1, visited);
			if (lenTemp > length)
			{
				BlockNo = i + 1;
				length = lenTemp;
			}
		}
	}
	if (BlockNo == -1) return ERROR;
	else return BlockNo;
}

/**
 * @brief  �õ��ڽӱ���ĳһ����û�б����ʽ��ĸ���
 * @param  Block_Graph: �ڽӱ�
 * @param  BlockNo: ������
 * @param  visited: ��¼�Ƿ���ʹ��ı�־����
 * @return ������û�б����ʽ��ĸ���
 */
int UnvistedNum_Count(AdjGraphList& BG, int BlockNo, int* visited)
{
	int length = 0;
	if (visited[BG.vertices[BlockNo - 1].adjvex] == FALSE)
		length++;
	ArcNode* temp = BG.vertices[BlockNo - 1].firstarc;
	if (temp == NULL) return length;
	while (temp != NULL)
	{
		if (visited[temp->adjvex - 1] == FALSE)
			length++;
		temp = temp->nextarc;
	}
	return length;
}
