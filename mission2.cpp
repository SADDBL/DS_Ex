#include"mission2.h"
//#include"mission1.h"
#define StepBackMax 30
#define BackSteps 20

/*************************************************
功能：根据轮廓创建邻接表
参数：Block_Graph：为图片中区块创建的邻接表
	  Edge_img：轮廓图
	  SeedPoint_Num：实际产生的种子点数
**************************************************/
int Creat_AdjList(AdjGraphList& Block_Graph,Mat Edge_img,int SeedPoint_num)
{
	Block_Graph.vexnum = SeedPoint_num;//顶点数等于种子点数
	for (int i = 0; i < Block_Graph.vexnum; i++)
	{
		Block_Graph.vertices[i].adjvex = i + 1;
		Block_Graph.vertices[i].firstarc = NULL;
		
	}
	int this_block = 1, neighbor_block = 1;
	int neighbor_block_dot[8];

	//遍历像素点
	for (int i = 1; i < Edge_img.rows - 1; i++)
	{
		int* PixelPoint_col_1 = Edge_img.ptr<int>(i - 1);
		int* PixelPoint_col_2 = Edge_img.ptr<int>(i);//第i行像素点头指针
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
			if (this_dot == -1)//如果该点在区域边缘
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

				//如果this_block区块尚未加入图的顶点列表
				if (Block_Graph.vertices[this_block - 1].firstarc == NULL)
				{
					ArcNode* p = NULL;
					if ((p = (ArcNode*)malloc(sizeof(ArcNode))) == NULL)
					{
						cout << "ArcNode Failed to Request memory" << endl;
						exit(1);
					}
					//将该区块加入顶点列表
					p->nextarc = NULL;
					p->adjvex = neighbor_block;//弧指向的顶点的序号
					Block_Graph.vertices[this_block - 1].firstarc = p;
				}
				//如果this_block区块已经加入图的顶点列表
				else if (Block_Graph.vertices[this_block - 1].firstarc != NULL)
				{
					int IsDuplicate = FALSE;//是否重复的标识量
					ArcNode* p = NULL;
					ArcNode* temp = Block_Graph.vertices[this_block - 1].firstarc;
					//遍历查重
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
					//若没有重名
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

				//如果neighbor_block区块尚未加入图的顶点列表
				if (Block_Graph.vertices[neighbor_block - 1].firstarc == NULL)
				{
					ArcNode* p = NULL;
					if ((p = (ArcNode*)malloc(sizeof(ArcNode))) == NULL)
					{
						cout << "ArcNode Failed to Request memory" << endl;
						exit(1);
					}
					//将该区块加入顶点列表
					p->nextarc = NULL;
					p->adjvex = this_block;//弧指向的顶点的序号
					Block_Graph.vertices[neighbor_block - 1].firstarc = p;
				}
				//如果neighbor_block区块已经加入图的顶点列表
				else if (Block_Graph.vertices[neighbor_block - 1].firstarc != NULL)
				{
					int IsDuplicate = FALSE;//是否重复的标识量
					ArcNode* p = NULL;
					ArcNode* temp = Block_Graph.vertices[neighbor_block - 1].firstarc;
					//遍历查重
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
					//若没有重名
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
功能：优化建立的邻接表，根据顶点的度降序重排邻接链表
参数：Block_Graph：为图片中区块创建的邻接表
**************************************************/
int AdjList_Sort(AdjGraphList& Block_Graph)
{
	int AdjList_len = 0;
	int Block_No_List[Max_Vertex_Num];
	for (int i = 0; i < Max_Vertex_Num; i++) Block_No_List[i] = 0;
	//计算各邻接表链表长度
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
			ArcNode* shadow = NULL;//影子结点
			for (int k = 0; k < Block_No_List[i] - j - 1; k++)
			{
				//若需要调整顺序
				if (temp->degree < temp->nextarc->degree)
				{
					//若第一个顶点序号比下一个序号小，交换
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
				//若不需要调整顺序，顺序到下一个顶点
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
 * @brief  利用队列梳理最佳涂色路径
 * @param  Block_Graph: 邻接表
 * @param  Path: 涂色路径队列
 * @return 函数执行状态
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

	cout << "最佳填色路径为：" << endl;
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
功能：根据深度优先算法，进行填色
参数：Block_Graph：为图片中区块创建的邻接表
	  Path：填色路径，列表
	  EdgeImg：分水岭图样
	  ColorImg：重新填色图样
**************************************************/
void DFS_FullColor(AdjGraphList& Block_Graph, LinkQueue Path, Mat EdgeImg, Mat ColorImg)
{
	int pop_record[Max_Vertex_Num] = { 0 };
	// 初始化一个记录颜色是否被用的记录表
	int colors_used_matrix[Max_Vertex_Num][4] = { 0 };
	// 创建一个缓冲队列和一个中转站，前者用于存储回退出来的区域，后者用来调整缓冲队列的顺序
	LinkQueue buffer;
	LinkQueue transfer_station;
	LinkStack Buffer;
	InitStack(Buffer);
	InitQueue(buffer);
	InitQueue(transfer_station);
	// 创建堆栈
	LinkStack stack = NULL;
	InitStack(stack);
	int e;
	// 开始涂色，要求尽可能先用同一种颜色去涂，实在冲突再换颜色
	// 如果遇到无法涂色，则回退上一步，换颜色再尝试
	// 如果涂色队列非空，即未完成涂色，则继续尝试
	while (EmptyQueue(Path) == FALSE || EmptyQueue(buffer) == FALSE)
	{
		// 如果涂色过程一直卡在某个地方，既不能继续往前涂，又不能往后退，说明陷入了死胡同
		// 那么就往回退50个试试重新制定涂色方案，此过程非常重要
		for (int i = 0; i < Block_Graph.vexnum; i++)
		{
			// 如果某个结点频繁回退，即达到阈值，就直接回退50个结点
			if (pop_record[i] >= 250)
			{
				pop_record[i] = 0;
				for (int j = 0; j < 50; j++)
				{
					int temp_area_number;
					if (EmptyStack(stack) == TRUE) break;
					StackPop(stack, temp_area_number);
					// 先入中转站
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
		// 检查buffer是否为空，因为要优先处理buffer中的结点，当buffer中无结点，再从Path中取出待涂色结点
		if (EmptyQueue(buffer) == FALSE)
			//if(EmptyStack(Buffer) == FALSE)
		{
			bool left_colors_table[4] = { 0 };
			DeQueue(buffer, e);
			//StackPop(Buffer, e);
			// 只有当栈的长度大于0才去检查
			if (EmptyStack(stack) == FALSE)
			{
				// 检查此时将要涂色的区域的邻接区域的颜色是否冲突
				// 遍历现在的栈，看看是否有相邻的区域
				LinkStack search_stack = stack;
				while (search_stack != NULL)
				{
					ArcNode* temp = Block_Graph.vertices[e - 1].firstarc;
					// 开始从自己的邻接区域去搜索是否含有这个元素
					while (temp != NULL)
					{
						if (temp->adjvex == search_stack->adjvex)
							left_colors_table[search_stack->color] = 1;
						temp = temp->nextarc;
					}
					search_stack = search_stack->next;
				}
				// 搜寻结束，现在看看还剩下什么颜色可以涂
				int tinted = FALSE;
				// 如果有颜色剩下，则进行入栈等操作
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
				// 如果还没有颜色剩下，证明再前面的颜色涂的也有问题，又需要回退
				if (tinted == FALSE)
				{
					for (int i = 0; i < 4; i++)
						colors_used_matrix[e - 1][i] = 0;


					int temp_area_number;
					StackPop(stack, temp_area_number);
					pop_record[temp_area_number - 1]++;

					/*StackPush(Buffer, temp_area_number, NoColor);
					StackPush(Buffer, e, NoColor);*/

					// 先入中转站
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
			// 如果栈为空
			else
			{
				int flag = FALSE;
				// 说明是第一个区域
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
					cout << "涂色失败！" << endl;
			}
		}


		else if (EmptyQueue(buffer) == TRUE)
			//else if(EmptyStack(Buffer) == TRUE)
		{
			// 一个涂色区域序号出队列
			DeQueue(Path, e);
			// 存放RGBY的使用情况，0表示未使用，1表示已使用
			bool left_colors_table[4] = { 0 };
			// 只有当栈的长度大于0才去检查
			if (EmptyStack(stack) == FALSE)
			{
				// 检查此时将要涂色的区域的邻接区域的颜色是否冲突
				// 遍历现在的栈，看看是否有相邻的区域
				LinkStack search_stack = stack;
				while (search_stack != NULL)
				{
					ArcNode* temp = Block_Graph.vertices[e - 1].firstarc;
					// 开始从自己的邻接区域去搜索是否含有这个元素
					while (temp != NULL)
					{
						if (temp->adjvex == search_stack->adjvex)
							left_colors_table[search_stack->color] = 1;
						temp = temp->nextarc;
					}
					search_stack = search_stack->next;
				}
				// 搜寻结束，现在看看还剩下什么颜色可以涂
				int tinted = FALSE;
				// 如果有颜色剩下，则进行入栈等操作
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
				// 如果没有颜色剩下，证明前面颜色涂的有问题，需要回退
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
			// 如果栈为空
			else
			{
				// 说明是第一个区域，无须考察是否冲突，直接入栈
				StackPush(stack, e, Red);
				colors_used_matrix[e - 1][Red] = 1;
			}
		}
	}


	//涂色
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
功能：计算各区块的面积
参数：Block_Graph：为图片中区块创建的邻接表
	  AreaList：面积列表
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
				//return ERROR;//相邻区域有相同填色
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
	//筛选可用填色
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

/**  队列相关函数  **/

/*************************************************
功能：初始化队列
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
功能：从队尾入队
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
	if (q.rear == NULL)//判断是否为空队列
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
功能：从队头出队
**************************************************/
int DeQueue(LinkQueue& q, int& e)
{
	if (q.rear == q.front) return ERROR;//若队列为空
	QNode* temp;
	if ((temp = (Queue)malloc(sizeof(QNode))) == NULL)
	{
		cout << "QNode Failed to Request memory" << endl;
		exit(1);
	}
	temp = q.front->next;
	e = temp->data;
	q.front->next = temp->next;
	if (temp == q.rear) q.rear = q.front;//若删除了最后一个元素，尾指针指向头指针
	free(temp);
	temp = NULL;
	return OK;
}

/*************************************************
功能：判断队列是否为空
返回值：FALSE：队列不为空
		TRUE：队列为空
**************************************************/
int EmptyQueue(LinkQueue q)
{
	if (q.front->next == NULL) return TRUE;
	return FALSE;
}

/**  队列相关函数结束  **/

void ShowBlock_Graph(AdjGraphList &Block_Graph)
{
	cout << "区块邻接表为：" << endl;
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

/**  堆栈相关函数开始  **/

//堆栈初始化
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
功能：从栈顶入栈
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
功能：从栈顶出栈
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
功能：判断堆栈是否为空
返回值：FALSE：堆栈不为空
		TRUE：堆栈为空
**************************************************/
int EmptyStack(LinkStack S)
{
	if (S->next == NULL) return TRUE;
	return FALSE;
}

/**  堆栈相关函数结束  **/

/**
 * @brief  判断是否所有的结点都被访问过
 * @param  Block_Graph: 邻接表
 * @param  visited: 记录是否访问过的标志数组
 * @return 函数执行状态
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
 * @brief  选取最优的涂色区域
 * @param  BG: 邻接表
 * @param  visited: 记录是否访问过的标志数组
 * @return 最优的涂色区域的编号，若出现故障，返回ERROR
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
 * @brief  得到邻接表中某一链表没有被访问结点的个数
 * @param  Block_Graph: 邻接表
 * @param  BlockNo: 结点序号
 * @param  visited: 记录是否访问过的标志数组
 * @return 链表中没有被访问结点的个数
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
