#ifndef CLEAR_H
#define CLEAR_H

#include"mainwindow.h"
#include<vector>
#include<utility>
using namespace std;


void clearAll(int map[ROW][COL]);
void dfs(int x,int y,int color,vector<pair<int,int>>& list,bool vis[ROW][COL],int map[ROW][COL]);
void markRemove(int map[ROW][COL],bool needDelete[ROW][COL]);
void doRemove(int map[ROW][COL],bool needDelete[ROW][COL]);
void dropAll(int map[ROW][COL],bool needDelete[ROW][COL]);

#endif//CLEAR_H
