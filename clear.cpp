#include"clear.h"
#include<cstring>
#include<vector>
using namespace std;

const int dx[4]={1,-1,0,0};
const int dy[4]={0,0,1,-1};

int scanAndMark(int map[ROW][COL], vector<PuyoToRemove> &marked) {
    int groupCount=0;
    bool vis[ROW][COL] = {false};

    for(int y = 0; y < ROW; y++) {
        for(int x = 0; x < COL; x++) {
            int color = map[y][x];
            if(color == EMPTY || vis[y][x]) continue;

            vector<pair<int,int>> group;
            dfs(x, y, color, group, vis, map);

            if(group.size() >= 4) {
                groupCount++;
                for(auto &p : group) {
                    PuyoToRemove temp;

                    marked.push_back({p.first, p.second});
                }
            }
        }
    }
    return groupCount;
}

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

//重大改变！
bool scanAndRemove(int map[ROW][COL]){
    bool del[ROW][COL]={false};
    bool vis[ROW][COL]={false};


    bool hasDel=false;

//先标记要删除的噗哟
    for(int y=0;y<ROW;y++){
        for(int x=0;x<COL;x++){
            int c=map[y][x];
            if(c==EMPTY||vis[y][x])
                continue;
            vector<pair<int,int>>group;
            dfs(x,y,c,group,vis,map);//注意！横纵坐标别写反了。。。

            if(group.size()>=4){
                hasDel=true;
                for(auto&p:group){
                    del[p.second][p.first]=true;
                }
            }
        }
    }
//再统一删除
    for(int y=0;y<ROW;y++){
        for(int x=0;x<COL;x++){
            if(del[y][x]){
                map[y][x]=EMPTY;
            }
        }
    }
    return hasDel;
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
void dropAll(int map[ROW][COL]){
    for(int c=0;c<COL;c++){
        dropCol(map,c);
    }
}





