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

typedef struct ArcNode//�߱���
{
	int adjvex;//�������,�洢�ڽӶ����Ӧ���±�
	int degree;//����Ķ�
	int area;//��������
	struct ArcNode* nextarc;
}ArcNode;

typedef struct VertexPoint//�������
{
	int adjvex;//������ţ���Ϊ������
	int Block_No;
	Point gross;
	ArcNode* firstarc;//ָ���һ�������ڸö���Ļ�
}VertexPoint,AdjList[Max_Vertex_Num];

typedef struct//�ڽӱ�
{
	AdjList vertices;
	int vexnum;//ͼ�Ķ�����
}AdjGraphList;

typedef struct QNode//���н��
{
	int data;//�ڴ������У��洢�������
	struct QNode* next;
}QNode,*Queue;

typedef struct//����
{
	QNode* front;//����ͷ
	QNode* rear;//����β
}LinkQueue;

typedef struct SNode//��ջ���
{
	int color;
	int adjvex;//�ڽӱ������
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