#ifndef WINDOW_H
#define WINDOW_H

#include <QtGui>
#include <iostream>
#include <vector>
#include "Wuziqi.h"
#include "dialog.h"

using namespace std;

class Window : public QMainWindow
{
	Q_OBJECT
public:
	Window(QWidget * parent = 0);
	~Window();

protected:
	void paintEvent(QPaintEvent * event); //处理重绘消息
	void mousePressEvent(QMouseEvent * event); //鼠标事件消息处理

//自定义槽函数
protected slots:
	void startGame(void); //开始游戏
	void stopGame(void);  //结束游戏
	void showAbout(void); //显示关于对话框
	void setAlgorithm1(void); //设置使用第一个算法
	void setAlgorithm2(void); //设置使用第二个算法
	void setAlgorithm3(void); //设置使用第三个算法
	void fightAgainst(void); //算法2,3之间对战

private:
	void dealMenu(void);  //创建菜单

private:
	int MenuBarSize; //菜单栏所占的位置
	Wuziqi game;  //五子棋游戏类
	vector<vector<int> > gamedata; //游戏数据
	int N;  //棋盘边长
	POINT lastPs;  //记录计算机最后落子的座标
	bool bRun; //标志游戏是否开始
	int Method; //选择计算机下棋使用的算法
	SelectDialog SelectDia; //选择对战时的算法的对话框
};

#endif
