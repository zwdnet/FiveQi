#include "Wuziqi.h"

Wuziqi::Wuziqi(const int n)
{
    N = n;
    Init();
}

Wuziqi::~Wuziqi(void)
{
}

void Wuziqi::Init(void)
{
    vector<int> temp(N, 0);
    for (int i = 0; i < N; i++)
    {
        Data.push_back(temp);
    }
    first = true; //是否是第一步，算法二要用
}

void Wuziqi::GetData(vector<vector<int> > & data)
{
	data.clear();
	data.resize(N);
	for (int i = 0; i < N; i++)
	{
		data[i].resize(N);
		for (int j = 0; j < N; j++)
		{
			data[i][j] = Data[i][j];
		}
	}
}

void Wuziqi::Judge(int & Who)
{
    /*算法：对盘面上所有有子的点一一遍历，计算其前后左右上下8个方向的最大连线值，若
      大于等于5则游戏结束，这一方获胜，若无则游戏继续
      只朝一个方向检测，横向从左到右，竖向从上到下，左右或者右左都是从上到下,这样就不会
      重复计数。
    */
    if (Total == N*N) //棋盘满了，和棋
    {
        Who = 3;
        return;
    }
    int state = 0;
    for (int x = 0; x < N; x++)
    {
        for (int y = 0; y < N; y++)
        {
            if (Data[x][y] == 0) //若该点没子，退出该次循环，看下一个点
            {
                continue;
            }
            for (int i = 0; i < 4; i++)
            {
                int k, j;
                switch (i)
                {
                //黑白连一块儿了。
                case 0: //横向
                    k = x+1;//从左向右检测
                    while (k < N && Data[k][y] == Data[x][y])
                    {
                        state++;
                        k++;
                    }
                    if (state >= 4)
                    {
                        Who = Data[x][y];
                        return;
                    }
                    else
                    {
                        state = 0;
                    }
                    break;
                case 1: //竖向
                    j = y + 1; //从上往下
                    while (j < N && Data[x][j] == Data[x][y])
                    {
                        state++;
                        j++;
                    }
                    if (state >= 4)
                    {
                        Who = Data[x][y];
                        return;
                    }
                    else
                    {
                        state = 0;
                    }
                    break;
                case 2: //左上到右下
                    k = x + 1;
                    j = y + 1;
                    while (k < N && j < N && Data[k][j] == Data[x][y])
                    {
                        state++;
                        k++;
                        j++;
                    }
                    if (state >= 4)
                    {
                        Who = Data[x][y];
                        return;
                    }
                    else
                    {
                        state = 0;
                    }
                    break;
                case 3: //右上到左下
                    k = x - 1;
                    j = y + 1;
                    while (k >= 0 && j < N && Data[k][j] == Data[x][y])
                    {
                        state++;
                        k--;
                        j++;
                    }
                    if (state >= 4)
                    {
                        Who = Data[x][y];
                        return;
                    }
                    else
                    {
                        state = 0;
                    }
                    break;
                default:
                    break;
                }

            }
        }
    }
    Who = 0;
    return;
}

bool Wuziqi::ChangePos(const POINT & srcPos, POINT & desPos,
	     const int & beginPos, const int & endPos)  //将座标进行转换
{
    if (srcPos.x < beginPos || srcPos.y < beginPos || 
	srcPos.x > endPos || srcPos.y > endPos)
    {
        return false;
    }
    desPos.x = -1;
    desPos.y = -1;

    //获得转换后的坐标
    for (int i = 0; i < N; i++)
    {
        if (srcPos.x > beginPos + i*20 - 10 &&
            srcPos.x <= beginPos + i*20 + 10)
        {
            desPos.x = i;
            break;
        }
    }
    for (int i = 0; i < N; i++)
    {
        if (srcPos.y > beginPos + i*20 - 10 &&
            srcPos.y <= beginPos + i*20 + 10)
        {
            desPos.y = i;
            break;
        }
    }
    return true;
}

bool Wuziqi::Fall(const POINT & ps, const int & who,
		  const int & beginPos, const int & endPos)
{
    POINT tempPos;
    if (ChangePos(ps, tempPos, beginPos, endPos) == false)
    {
	    return false;
    }
    if (tempPos.x < 0 || tempPos.y < 0)
    {
	    return false;
    }
    if (Data[tempPos.x][tempPos.y] == 0)
    {
        if (who == 1 || who == 2)
        {
            Data[tempPos.x][tempPos.y] = who;
            if (who == 2)
            {
                ps_Now.x = tempPos.x;
                ps_Now.y = tempPos.y;
            }
            Total++; //棋盘上棋子的总数
            return true;
        }
        else
        {
            return false;
        }
    }
    else
    {
        return false;
    }
}

bool Wuziqi::UnFall(const POINT & ps, const int & who,
		  const int & beginPos, const int & endPos)
{
    POINT tempPos;
    if (ChangePos(ps, tempPos, beginPos, endPos) == false)
    {
	    return false;
    }
    if (tempPos.x < 0 || tempPos.y < 0)
    {
	    return false;
    }
    if (Data[tempPos.x][tempPos.y] == who)
    {
	Data[tempPos.x][tempPos.y] = 0;
	Total--;
	return true;
    }
    else
    {
        return false;
    }
}

void Wuziqi::Clear(void)
{
    for (int x = 0; x < N; x++)
    {
        for (int y = 0; y < N; y++)
        {
            Data[x][y] = 0;
        }
    }
    Total = 0;
}

void Wuziqi::Run(POINT & ps, const int & Which, const int & Who)
{
    if (Which == 1)
    {
        Way1(ps);
    }
    else if (Which == 2)
    {
	Way2(ps);
	if (ps.x == -1 || ps.y == -1)
	{
		return;
	}
    }
    else if (Which == 3)
    {
        Way3(ps);
    }
    return;
}

//算法1：随机算法,测试用的AI = 0，模拟完全不会玩棋的人
void Wuziqi::Way1(POINT & ps)
{
    int x = 0, y = 0;
    srand(time(NULL));
    x = rand()%N;
    y = rand()%N;
    while (Data[x][y] != 0)
    {
        x = rand()%N;
        y = rand()%N;
    }
    ps.x = x;
    ps.y = y;
}

//算法2：只考虑当前情况，不递归的（贪心法)
//参考Zhong_Zw的程序，下载自CSDN
void Wuziqi::Way2(POINT & ps)
{
    //如果是开局先走，随机下第一个点
    if (Total == 0)
    {
	ps.x = rand()%N;
	ps.y = rand()%N;
	return;
    }
    //初始化计算机和人的状态矩阵
    vector<vector<int> > HumanState, ComputerState;
    vector<int> temp(N, 0);
    for (int i = 0; i < N; i++)
    {
        HumanState.push_back(temp);
        ComputerState.push_back(temp);
    }
    POINT ps1, ps2;

    for (int i = 0; i < N; i++)
    {
        for (int j = 0; j < N; j++)
        {
            if (Data[i][j] == 0)
            {
                SetScore(i, j, 1, HumanState);
                SetScore(i, j, 2, ComputerState);
            }
        }
    }

    int maxP = 0, maxC = 0; //计算机和人状态值的最大值
    for (int i = 0; i < N; i++)
    {
        for (int j = 0; j < N; j++)
        {
            if (HumanState[i][j] > maxP)
            {
                maxP = HumanState[i][j];
                ps1.x = i;
                ps1.y = j;
            }
            if (ComputerState[i][j] > maxC)
            {
                maxC = ComputerState[i][j];
                ps2.x = i;
                ps2.y = j;
            }
        }
    }
    if (maxP >= maxC)
    {
        ps.x = ps1.x;
        ps.y = ps1.y;
    }
    else
    {
        ps.x = ps2.x;
        ps.y = ps2.y;
    }
}

//评分函数
void Wuziqi::SetScore(const int i, const int j, const int Who,
                    vector<vector<int> > & State)
{
    int Another; //与要求的状态相对的另一方
    if (Who == 1)
    {
        Another = 2;
    }
    else if (Who == 2)
    {
        Another = 1;
    }
    else
    {
        return;
    }
    int bp[2], k, num, max = 0, temp = 0;

    //横向
    //向右
    bp[0] = 1; bp[1] = 1;
    num = 0;
    for (k = 1; k < N - i; k++)
    {
        if (Data[i+k][j] == Who)
        {
            num++;
            continue;
        }
        if (Data[i+k][j] == 0)
        {
            bp[1] = 0;
            break;
        }
        if (Data[i+k][j] == Another)
        {
            bp[1] = 1;
            break;
        }
    }
    //向左
    for (k = 1; k <= i; k++)
    {
        if (Data[i-k][j] == Who)
        {
            num++;
            continue;
        }
        if (Data[i-k][j] == 0)
        {
            bp[0] = 0;
            break;
        }
        if (Data[i-k][j] == Another)
        {
            bp[0] = 1;
            break;
        }
    }
    //调用评分函数
    temp = p_Score(num, bp);
    max += temp;

    //纵向
    bp[0] = 1; bp[1] = 1;
    num = 0;
    //向下
    for (k = 1; k < N - j; k++)
    {
        if (Data[i][j+k] == Who)
        {
            num++;
            continue;
        }
        if (Data[i][j+k] == 0)
        {
            bp[1] = 0;
            break;
        }
        if (Data[i][j+k] == Another)
        {
            bp[1] = 1;
            break;
        }
    }
    //向上
    for (k = 1; k <= j; k++)
    {
        if (Data[i][j-k] == Who)
        {
            num++;
            continue;
        }
        if (Data[i][j-k] == 0)
        {
            bp[0] = 0;
            break;
        }
        if (Data[i][j-k] == Another)
        {
            bp[0] = 1;
            break;
        }
    }
    temp = p_Score(num, bp);
    max += temp;

    //从左上到右下
    bp[0] = 1; bp[1] = 1;
    num = 0;

    //下
    for (k = 1; k < N-i && k < N-j; k++)
    {
        if (Data[i+k][j+k] == Who)
        {
            num++;
            continue;
        }
        if (Data[i+k][j+k] == 0)
        {
            bp[1] = 0;
            break;
        }
        if (Data[i+k][j+k] == Another)
        {
            bp[1] = 1;
            break;
        }
    }
    //上
    for (k = 1; k <= j && k <= i; k++)
    {
        if (Data[i-k][j-k] == Who)
        {
            num++;
            continue;
        }
        if (Data[i-k][j-k] == 0)
        {
            bp[0] = 0;
            break;
        }
        if (Data[i-k][j-k] == Another)
        {
            bp[0] = 1;
            break;
        }
    }
    temp = p_Score(num, bp);
    max += temp;

    //从右上到左下
    bp[0] = 1; bp[1] = 1;
    num = 0;
    //下
    for (k = 1; k < N-j && k <= i; k++)
    {
        if (Data[i-k][j+k] == Who)
        {
            num++;
            continue;
        }
        if (Data[i-k][j+k] == 0)
        {
            bp[1] = 0;
            break;
        }
        if (Data[i-k][j+k] == Another)
        {
            bp[1] = 1;
            break;
        }
    }
    //上
    for (k = 1; k <= j && k < N-i; k++)
    {
        if (Data[i+k][j-k] == Who)
        {
            num++;
            continue;
        }
        if (Data[i+k][j-k] == 0)
        {
            bp[0] = 0;
            break;
        }
        if (Data[i+k][j-k] == Another)
        {
            bp[0] = 1;
            break;
        }
    }
    temp = p_Score(num, bp);
    max += temp;

    if (max > State[i][j])
    {
        State[i][j] = max;
    }
}

//实际的评分函数
int Wuziqi::p_Score(int num, int bp[])
{
    int max = 0;
    if (num >= 4)
    {
        max += 10000; //成五
    }
    else if (num == 3)
    {
        if (bp[1] == 1 && bp[0] == 1) //不成五
        {
            max += 0;
        }
        else if (bp[1] == 0 && bp[0] == 0) //活四
        {
            max += 3000;
        }
        else
        {
            max += 900; //冲四
        }
    }
    else if (num == 2)
    {
        if (bp[0] == 0 && bp[1] == 0)
        {
            max += 460; //活三
        }
        else if (bp[0] == 1 && bp[1] == 1)
        {
            max += 0; //不成五
        }
        else
        {
            max += 30; //死三
        }
    }
    else if (num == 1)
    {
        if (bp[0] == 0 && bp[1] == 0)
        {
            max += 45;  //活二
        }
        else if (bp[0] == 1 && bp[1] == 1)
        {
            max += 0;
        }
        else
        {
            max += 5;
        }
    }
    else if (num == 0)
    {
        if (bp[0] == 0 && bp[1] == 0)
        {
            max += 3;
        }
        else if (bp[0] == 1 && bp[1] == 1)
        {
            max += 0;
        }
        else
        {
            max += 1;
        }
    }
    return max;
}

//算法3：随机算法，随机从棋盘上还空着的格子里选出ChooseN个（如果剩余格子小于ChooseN则全选），
//评分，分值最高的就是算法选择的落子点
void Wuziqi::Way3(POINT & ps)
{
    const int ChooseN = 100;
    //决定随机选取的格子数
    int n;
    if (N*N - Total <= ChooseN)
    {
        n = N*N - Total;
    }
    else
    {
        n = ChooseN;
    }

    srand(time(NULL));
    vector<POINT> Choose(2*n);  //候选的点的数组
    vector<int> Value(2*n);    //评估的值的数组

    //初始化计算机和人的状态矩阵
    vector<vector<int> > HumanState, ComputerState;
    vector<int> temp(N, 0);
    for (int i = 0; i < N; i++)
    {
        HumanState.push_back(temp);
        ComputerState.push_back(temp);
    }

    for (int i = 0; i < n; i++)
    {
        POINT tempPs;
        //随机找到一个没有棋子的点
        do
        {
            tempPs.x = rand()%N;
            tempPs.y = rand()%N;
        }while (Data[tempPs.x][tempPs.y] != 0);
        SetScore(tempPs.x, tempPs.y, 1, HumanState);
        SetScore(tempPs.x, tempPs.y, 2, ComputerState);
        //人的
        Choose[i].x = tempPs.x;
        Choose[i].y = tempPs.y;
        Value[i] = HumanState[tempPs.x][tempPs.y];
        //计算机的
        Choose[i+ChooseN].x = tempPs.x;
        Choose[i+ChooseN].y = tempPs.y;
        Value[i+ChooseN] = HumanState[tempPs.x][tempPs.y];
    }

    int MaxP = 0, MaxC = 0;
    for (int i = 0; i < ChooseN; i++)
    {
        if (Value[i] > MaxP)
        {
            MaxP = i;
        }
        if (Value[i+ChooseN] > MaxC)
        {
            MaxC = i+ChooseN;
        }
    }
    if (Value[MaxP] >= Value[MaxC])
    {
        ps.x = Choose[MaxP].x;
        ps.y = Choose[MaxP].y;
    }
    else
    {
        ps.x = Choose[MaxC].x;
        ps.y = Choose[MaxC].y;
    }
}

/*
//此算法未完成，故注释掉，期待高手......
//算法4,博弈树算法
void Wuziqi::Way4(POINT & ps)
{
    //如果是开局先走，随机下第一个点
    if (Total == 0)
    {
	ps.x = rand()%N;
	ps.y = rand()%N;
	return;
    }
    //去除四周2格以内没有其它棋子的点
    for (int i = 0; i < N; i++)
    {
	    for (int j = 0; j < N; j++)
	    {
		    if (Data[i][j] == 0)
		    {
			    if ((i-1 >= 0 && Data[i-1][j] != 0) ||  //左边一格
				(i-2 >= 0 && Data[i-2][j] != 0) ||  //左边两格
				(i+1 < N && Data[i+1][j] != 0)  ||  //右边一格
				(i+2 < N && Data[i+2][j] != 0)  ||  //右边两格
				(j-1 >= 0 && Data[i][j-1] != 0) ||  //上边一格
				(j-2 >= 0 && Data[i][j-2] != 0) ||  //上边两格
				(j+1 < N && Data[i][j+1] != 0)  ||  //下边一格
				(j+2 < N && Data[i][j+2] != 0)  ||  //下边两格
				(i-1 >= 0 && j-1 >= 0 && Data[i-1][j-1] != 0) || //左上一格
				(i-2 >= 0 && j-2 >= 0 && Data[i-2][j-2] != 0) || //左上两格
				(i+1 < N && j+1 < N && Data[i+1][j+1] != 0)   || //右下一格
				(i+2 < N && j+2 < N && Data[i+2][j+2] != 0)   || //右下两格
				(i+1 < N && j-1 >= 0 && Data[i+1][j-1] != 0)  || //右上一格
				(i+2 < N && j-2 >= 0 && Data[i+2][j-2] != 0)  || //右上两格
				(i-1 >= 0 && j+1 < N && Data[i-1][j+1] != 0)  || //左下一格
				(i-2 >= 0 && j+2 < N && Data[i-2][j+2] != 0)  || //左下两格
				(i-1 >= 0 && j-2 >= 0 && Data[i-1][j-2] != 0) || //左斜上1
				(i-2 >= 0 && j-1 >= 0 && Data[i-2][j-1] != 0) || //左斜上2
				(i-1 >= 0 && j+2 < N && Data[i-1][j+2] != 0) || //左斜下1
				(i-2 >= 0 && j+1 < N && Data[i-2][j+1] != 0) || //左斜下2
				(i+1 < N && j-2 >= 0 && Data[i+1][j-2] != 0) || //右斜上1
				(i+2 < N && j-1 >= 0 && Data[i+2][j-1] != 0) || //右斜上2
				(i+1 < N && j+2 < N && Data[i+1][j+2] != 0)  || //右斜下1
				(i+2 < N && j+1 < N && Data[i+2][j+1] != 0))   //右斜下2
				{

				}
		    }
	    }
    }
}

//设置最大搜索深度
void Wuziqi::setDepth(const int & dep)
{
	if (dep >= 1 && dep < 5)
	{
		maxDepth = dep;
	}
}

//递归搜索函数
void Wuziqi::Search(const int & i, const int & j)
{
	if (depth <= maxDepth)
	{
		depth++;
		POINT ps1, ps2;
		Way2(ps1);
		ps1.x = ps1.x*20 + 35;
		ps1.y = ps1.y*20 + 35;
		Fall(ps1, 1, 20+15, 300+20+15);
		Way2(ps2);
		ps2.x = ps2.x*20 + 35;
		ps2.y = ps2.y*20 + 35;
		Fall(ps2, 2, 20+15, 300+20+15);
		//计算盘面总分

		Search(ps1.x, ps1.y);
		UnFall(ps1, 1, 20+15, 300+20+15);
		UnFall(ps2, 2, 20+15, 300+20+15);
	}
}
*/
