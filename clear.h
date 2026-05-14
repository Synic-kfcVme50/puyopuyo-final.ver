#ifndef CLEAR_H
#define CLEAR_H

#include"mainwindow.h"
#include<vector>

using namespace std;


bool clearAll(int map[ROW][COL]);
void dfs(int x,int y,int color,vector<pair<int,int>>& list,bool vis[ROW][COL],int map[ROW][COL]);
bool scanAndRemove(int map[ROW][COL]);
void dropAll(int map[ROW][COL]);

int scanAndMark(int map[ROW][COL],vector<PuyoToRemove>&marked);



#endif//CLEAR_H
