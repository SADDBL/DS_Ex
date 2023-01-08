#pragma once
#include"mission1.h"
#include"mission2.h"

void HeapSort(vector<AreaNode>& H);
void HeapAdjust(vector<AreaNode>& H, int s, int m);
void HeapSort_Result(AdjGraphList& Block_Graph, Mat EdgeImg);// , vector<AreaNode>& H);
int Binary_Search(vector<AreaNode> AreaList, int vexnum, int& highNo, int& lowNo, clock_t& time_);
void HighLight(vector<AreaNode> AreaList, int highNo, int lowNo, AdjList vertices, Mat watershed_img, Mat EdgeImg, clock_t& time_);
