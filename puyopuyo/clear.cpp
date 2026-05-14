#include"clear.h"
#include<cstring>
#include<vector>

const int dx[4]={1,-1,0,0};
const int dy[4]={0,0,1,-1};

//该dfs函数借助AI
//从(x,y)这个噗哟开始，查找是否有四个同色相连的可相消噗哟，放进list里
void dfs(int x,int y,int color,vector<pair<int,int>>&list,bool vis[ROW][COL],int map[ROW][COL]){

    if(x<0||x>=COL||y<0||y>=ROW)//出界，停止寻找
        return;
    if(vis[y][x])//找过了，停止寻找
        return;
    if(map[y][x]!=color)//不同色，停止寻找
        return;

    vis[y][x]=true;
    list.push_back({x,y});

    for(int d=0;d<4;d++){
        int nx=x+dx[d];
        int ny=y+dy[d];
        dfs(nx,ny,color,list,vis,map);
    }
}

void markRemove(int map[ROW][COL],bool needDelete[ROW][COL]){
    memset(needDelete,0,sizeof(bool)*ROW*COL);
    bool vis[ROW][COL]={false};

    for(int i=0;i<ROW;i++){
        for(int j=0;j<COL;j++){
            int c=map[i][j];
            if(c==EMPTY||vis[i][j])
                continue;
            vector<pair<int,int>>block;
            dfs(j,i,c,block,vis,map);//注意！j对应的才是横坐标！！

            if(block.size()>=4){
                for(auto&p:block){
                    needDelete[p.second][p.first]=true;
                }
            }
        }
    }
}

void doRemove(int map[ROW][COL],bool needDelete[ROW][COL]){
    for(int i=0;i<ROW;i++){
        for(int j=0;j<COL;j++){
            if(needDelete[i][j])
                map[i][j]=EMPTY;
        }
    }
}
//单列下落
static void dropCol(int map[ROW][COL],int col){
    int idx=ROW-1;
    for(int r=ROW-1;r>=0;r--){
        if(map[r][col]!=EMPTY){
            map[idx][col]=map[r][col];
            if(idx!=r)
                map[r][col]=EMPTY;  //???
            idx--;
        }
    }
}

//整体下落:从单列拓展到整个map里
static void dropAll(int map[ROW][COL]){
    for(int c=0;c<COL;c++){
        dropCol(map,c);
    }
}

void clearAll(int map[ROW][COL]){
    bool vis[ROW][COL];
    bool del[ROW][COL];
    memset(vis,0,sizeof(vis));
    memset(del,0,sizeof(del));

    for(int y=0;y<ROW;y++){
        for(int x=0;x<COL;x++){
            int color=map[y][x];
            if(color==EMPTY||vis[y][x])
                continue;

            vector<pair<int,int>>block;  //???
            dfs(x,y,color,block,vis,map);

            if(block.size()>=4){
                for(auto&p:block){  //???
                       del[p.second][p.first]=true;
                }
            }
        }
    }

    //下落后还要再看一次有没有可以消除的噗哟
    bool hasDel=false;
    for(int y=0;y<ROW;y++){
        for(int x=0;x<COL;x++){
            if(del[y][x]){
                map[y][x]=EMPTY;
                hasDel=true;
            }
        }
    }

    if(hasDel){
        dropAll(map);
        clearAll(map);//实现递归！！
    }
}

