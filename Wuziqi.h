#ifndef WUZIQI_H
#define WUZIQI_H
/*五子棋游戏的游戏模块的实现
  zwdnet
  2010年3月
  zwdnet@163.com
  2011年9月修改，移植到Linux下的Qt环境中
*/
#include <iostream>
#include <vector>
#include <list>
#include <cmath>
#include <algorithm>
#include <fstream>

using namespace std;

class POINT
{
public:
	int x;
	int y;
	POINT(const int X=-1, const int Y=-1)
	    {x = X; y = Y;};
};

//五子棋游戏类，进行游戏相关的操作
class Wuziqi
{
private:
    int N; //棋盘大小
    vector<vector<int> > Data; //棋盘上每个格子的状态,0为啥也没有，1为黑棋，2为白棋
    POINT ps_Now; //记录白棋最后一步的位置,用于特别标示
    int Total; //落子总数，用于判断是否已经下满，下满则为和棋

public: //供外界调用的接口
    Wuziqi(const int n = 15); //构造函数，默认大小为20
    ~Wuziqi(void);
    void Init(void); //数据初始化
    void GetData(vector<vector<int> > & data); //获取游戏数据，用于画图
    void Judge(int & Who); //判断当前局面是否出现输赢，Who = 1黑方(人)胜，Who = 2白方
                            //胜，Who = 0则没有结束
    bool Fall(const POINT & ps, const int & who,
	     const int & beginPos, const int & endPos); 
          //人落子,若鼠标点击区域无效返回false;
    bool UnFall(const POINT & ps, const int & who,
	     const int & beginPos, const int & endPos);
         //取消落子，用于算法4
    void Clear(void); //清空游戏数据
    void Run(POINT & ps, const int & Which, const int & Who); //计算机走棋
		 
private://工具函数
    bool ChangePos(const POINT & srcPos, POINT & desPos,
	     const int & beginPos, const int & endPos);  //将座标进行转换

private://核心算法模块
    //算法1
    void Way1(POINT & ps); //方法1，分开写，增加新的算法时就不必改动类之外的调用的程序
    //算法2
    void Way2(POINT & ps);
    void SetScore(const int i, const int j, const int Who,
                  vector<vector<int> > & State); //评分函数
    int p_Score(int num, int bp[]); //实际的评分函数
    bool first; //是否是第一步
    //算法3：随机算法
    void Way3(POINT & ps);
    /*
    //算法4: 博弈树算法
    void Way4(POINT & ps);
    int maxDepth;  //最大搜索深度，默认为1
    int depth;
    void setDepth(const int & dep); //设置最大搜索深度
    void Search(const int & i, const int & j);  //搜索函数
    //vector<vector<int> > & bestStep; //记录整个搜索结果
    */
};
#endif
