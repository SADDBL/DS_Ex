#pragma once
#include<iostream>
#include"mission1.h"
using namespace std;

#define Max_Vertex_Num 1100
#define NoColor 0
#define Red  1
#define Green 2
#define Blue 3
#define Yellow 4

typedef struct ArcNode//边表结点
{
	int adjvex;//顶点序号,存储邻接顶点对应的下标
	int degree;//顶点的度
	int area;//区块的面积
	struct ArcNode* nextarc;
}ArcNode;

typedef struct VertexPoint//顶点表结点
{
	int adjvex;//顶点序号，亦为区块编号
	int Block_No;
	Point gross;
	ArcNode* firstarc;//指向第一条依附于该顶点的弧
}VertexPoint,AdjList[Max_Vertex_Num];

typedef struct//邻接表
{
	AdjList vertices;
	int vexnum;//图的顶点数
}AdjGraphList;

typedef struct QNode//队列结点
{
	int data;//在此任务中，存储顶点序号
	struct QNode* next;
}QNode,*Queue;

typedef struct//队列
{
	QNode* front;//队列头
	QNode* rear;//队列尾
}LinkQueue;

typedef struct SNode//堆栈结点
{
	int color;
	int adjvex;//邻接表顶点序号
	SNode* next;
}StackNode,*LinkStack;

typedef struct
{
	int area;
	int Block_No;
}AreaNode;

void InitQueue(LinkQueue& Q);
int InQueue(LinkQueue& q, int e);
int DeQueue(LinkQueue& q, int& e);
int EmptyQueue(LinkQueue q);

void InitStack(LinkStack& S);
int EmptyStack(LinkStack S);
void StackPush(LinkStack& S, int adjvex, int color);
int StackPop(LinkStack& S, int& e);

void Init_AreaList(vector<AreaNode>& AreaList, int len);
void Area_Measure(AdjGraphList& Block_Graph, Mat EdgeImg, vector<AreaNode>& AreaList);

int AdjList_Sort(AdjGraphList& Block_Graph);
int Creat_AdjList(AdjGraphList& Block_Graph, Mat Edge_img, int SeedPoint_num);
void ShowBlock_Graph(AdjGraphList &Block_Graph);
int SelectColor(AdjGraphList& Block_Graph, int* Block_Color, int Color_BlockNo);
void DFS_FullColor(AdjGraphList& Block_Graph, LinkQueue Path, Mat EdgeImg, Mat ColorImg);
int SelectColor(AdjGraphList& Block_Graph, int* Block_Color, int Color_BlockNo);
int SelectColor(AdjGraphList& Block_Graph, int Color_BlockNo, int& color, LinkStack stack, int (*color_table)[4]);
int Is_AllVisited(AdjGraphList& BG, int* visited);
int BestColorBlock(AdjGraphList& BG, int* visited);
int UnvistedNum_Count(AdjGraphList& BG, int BlockNo, int* visited);
int SortPath(AdjGraphList& Block_Graph, LinkQueue& Path);