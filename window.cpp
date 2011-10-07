#include "window.h"

Window::Window(QWidget * parent)
	: QMainWindow(parent)
{
	QTextCodec::setCodecForTr(QTextCodec::codecForLocale());
	setWindowTitle(tr("五子棋游戏"));
	N = 15;  //棋盘规模 NXN
	MenuBarSize = 10;//菜单栏所占的位置
	setFixedSize(320+MenuBarSize, 320+MenuBarSize);
	bRun = true;
	Method = 2;
	vector<int> temp(N, 0);
	for (int i = 0; i < N; i++)
	{
		gamedata.push_back(temp);
	}
	dealMenu();
}

Window::~Window()
{
}

void Window::paintEvent(QPaintEvent * event)
{
	//画背景
	QPainter painter(this);
	painter.setBrush(Qt::lightGray);
	painter.drawRect(0, 0, size().width(), size().height());
        //获取棋盘信息
	game.GetData(gamedata);
	//画棋盘
	for (int x = 20+MenuBarSize; x <= N*20+MenuBarSize; x += 20)
	{
		painter.drawLine(x, 20+MenuBarSize, x, N*20+MenuBarSize); 
	}
	for (int y = 20+MenuBarSize; y <= N*20+MenuBarSize; y += 20)
	{
		painter.drawLine(20+MenuBarSize, y, N*20+MenuBarSize, y);
	}
	QPainter blackPainter(this);
	QPainter whitePainter(this);
	QPainter greenPainter(this);
	blackPainter.setBrush(Qt::black);
	whitePainter.setBrush(Qt::white);
	greenPainter.setBrush(Qt::green);
	for (int x = 0; x < N; x++)
	{
		for (int y = 0; y < N; y++)
		{
			if (gamedata[x][y] == 1) //黑棋子
			{
				blackPainter.drawEllipse((x+1)*20-10+MenuBarSize, 
						(y+1)*20-10+MenuBarSize, 20, 20);
			}
			else if (gamedata[x][y] == 2) //白棋子
			{
				whitePainter.drawEllipse((x+1)*20-10+MenuBarSize,
						(y+1)*20-10+MenuBarSize, 20, 20);
			}
		}
	}
	//将最后一步画的白棋子变绿，这样人能看清
	if (lastPs.x >=0 && lastPs.y >= 0)
	{
		greenPainter.drawEllipse((lastPs.x+1)*20-10+MenuBarSize,
				(lastPs.y+1)*20-10+MenuBarSize, 20, 20);
	}
	QMainWindow::paintEvent(event);
}

void Window::mousePressEvent(QMouseEvent * event)
{
	//接受鼠标左键事件
	if (event->button() == Qt::LeftButton && bRun == true)
	{
		int x = event->x();
		int y = event->y();
		POINT ps;
		ps.x = x;
		ps.y = y;
		if (ps.x < 10+MenuBarSize || ps.y < 10+MenuBarSize ||
		    ps.x > 300+20+MenuBarSize || ps.y > 300+20+MenuBarSize)
		{
			return;
		}
		if (!game.Fall(ps, 1, 20+MenuBarSize, 300+20+MenuBarSize))
		{
			return;
		}
		game.Run(ps, Method, 2);
		lastPs = ps;
		ps.x = 20 + MenuBarSize + ps.x*20;
		ps.y = 20 + MenuBarSize + ps.y*20;
		game.Fall(ps, 2, 20+MenuBarSize, 300+20+MenuBarSize);
		game.GetData(gamedata);
		update();
		int who = 0; //看谁赢了
		game.Judge(who);
		if (who == 1) //人赢了
		{
			QMessageBox::information(this, tr("游戏结束"),
				tr("你赢了，游戏结束!"));
			bRun = false;
		}
		else if (who == 2) //计算机赢了
		{
			QMessageBox::information(this, tr("游戏结束"),
				tr("你输了，游戏结束!"));
			bRun = false;
		}
	}
}

void Window::dealMenu(void)
{
	QMenuBar * MenuBar = new QMenuBar(this);
	QMenu * GameMenu = new QMenu(tr("游戏"), MenuBar);
	QMenu * AlgorithmMenu = new QMenu(tr("算法"), MenuBar);
	QMenu * HelpMenu = new QMenu(tr("帮助"), MenuBar);
	//添加游戏菜单项
	QAction * StartGame = new QAction(tr("开始游戏"), GameMenu);
	QAction * StopGame = new QAction(tr("结束游戏"), GameMenu);
	QAction * QuitGame = new QAction(tr("退出游戏"), GameMenu);
	GameMenu->addAction(StartGame);
	GameMenu->addAction(StopGame);
	GameMenu->addAction(QuitGame);
	MenuBar->addMenu(GameMenu);
	connect(StartGame, SIGNAL(triggered()), this, SLOT(startGame()));
	connect(StopGame, SIGNAL(triggered()), this, SLOT(stopGame()));
	connect(QuitGame, SIGNAL(triggered()), this, SLOT(close()));

	//添加算法选择菜单
	QAction * Algorithm1 = new QAction(tr("算法1:完全随机下棋"), AlgorithmMenu);
	QAction * Algorithm2 = new QAction(tr("算法2:贪心法(AI较好)"), AlgorithmMenu);
	QAction * Algorithm3 = new QAction(tr("算法3:改进的随机算法(AI较差)"), AlgorithmMenu);
	QAction * Against = new QAction(tr("算法之间对战"), AlgorithmMenu);
	AlgorithmMenu->addAction(Algorithm1);
	AlgorithmMenu->addAction(Algorithm2);
	AlgorithmMenu->addAction(Algorithm3);
	AlgorithmMenu->addAction(Against);
	MenuBar->addMenu(AlgorithmMenu);
	connect(Algorithm1, SIGNAL(triggered()), this, SLOT(setAlgorithm1()));
	connect(Algorithm2, SIGNAL(triggered()), this, SLOT(setAlgorithm2()));
	connect(Algorithm3, SIGNAL(triggered()), this, SLOT(setAlgorithm3()));
	connect(Against, SIGNAL(triggered()), this, SLOT(fightAgainst()));

	//添加帮助菜单
	QAction * About = new QAction(tr("关于"), HelpMenu);
	HelpMenu->addAction(About);
	MenuBar->addMenu(HelpMenu);
	connect(About, SIGNAL(triggered()), this, SLOT(showAbout()));

	//设置菜单栏
	setMenuBar(MenuBar);
}

void Window::startGame(void)
{
	if (bRun == false)
	{
		bRun = true;
		game.Clear();
		game.Init();
		lastPs.x = -1; lastPs.y = -1;
		update();
	}
}

void Window::stopGame(void)
{
	if (bRun == true)
	{
		bRun = false;
		game.Clear();
		lastPs.x = -1; lastPs.y = -1;
		update();
	}
}

void Window::showAbout(void)
{
	QMessageBox::information(this, tr("关于五子棋游戏"),
		tr("QT程序：五子棋游戏1.0，人机对战。许可协议:GPL3版权所有，侵权必究!"));
}

void Window::setAlgorithm1(void)
{
	if (Method == 1)
	{
		return;
	}
	stopGame();
	Method = 1;
	startGame();
}

void Window::setAlgorithm2(void)
{
	if (Method == 2)
	{
		return;
	}
	stopGame();
	Method = 2;
	startGame();
}

void Window::setAlgorithm3(void)
{
	if (Method == 3)
	{
		return;
	}
	stopGame();
	Method = 3;
	startGame();
}

void Window::fightAgainst(void)
{
	stopGame();
	int Who = 0;
	Wuziqi * testGame;
	SelectDia.exec();
	int bAlgorithm, wAlgorithm, times, alltimes;
	SelectDia.getValue(bAlgorithm, wAlgorithm, times);
	bAlgorithm++;
	wAlgorithm++;
	POINT ps;
	int wTimes = 0, bTimes = 0; //记录两个颜色棋子的胜利次数
	alltimes = times;
	srand(time(NULL));
	for (int i = 0; i < times; i++)
	{
		testGame = new Wuziqi(N);
		int side = rand()%2;  //随机数，用来决定黑棋白棋谁先走，排除先后顺序对结果的影响
		while (Who == 0)
		{
			if (side == 0)  //黑棋先走
			{
				//黑棋走
				testGame->Run(ps, bAlgorithm, 1);
				ps.x = ps.x*20 + 20+MenuBarSize;
				ps.y = ps.y*20 + 20+MenuBarSize;
				testGame->Fall(ps, 1, 20+MenuBarSize, 300+20+MenuBarSize);
				testGame->GetData(gamedata);
				testGame->Judge(Who);
				//白棋走
				testGame->Run(ps, wAlgorithm, 2);
				ps.x = ps.x*20 + 20+MenuBarSize;
				ps.y = ps.y*20 + 20+MenuBarSize;
				testGame->Fall(ps, 2, 20+MenuBarSize, 300+20+MenuBarSize);
				testGame->GetData(gamedata);
				testGame->Judge(Who);
			}
			else if (side == 1)  //白棋先走
			{
				//白棋走
				testGame->Run(ps, wAlgorithm, 2);
				ps.x = ps.x*20 + 20+MenuBarSize;
				ps.y = ps.y*20 + 20+MenuBarSize;
				testGame->Fall(ps, 2, 20+MenuBarSize, 300+20+MenuBarSize);
				testGame->GetData(gamedata);
				testGame->Judge(Who);
				//黑棋走
				testGame->Run(ps, bAlgorithm, 1);
				ps.x = ps.x*20 + 20+MenuBarSize;
				ps.y = ps.y*20 + 20+MenuBarSize;
				testGame->Fall(ps, 1, 20+MenuBarSize, 300+20+MenuBarSize);
				testGame->GetData(gamedata);
				testGame->Judge(Who);
			}
			if (Who == 1)
			{
				bTimes++;
			}
			else if (Who == 2)
			{
				wTimes++;
			}
			else if (Who == 3) //如果和棋，把总数减1
			{
				alltimes--;
			}
		}
		Who = 0;
		testGame->Clear();
		delete testGame;
	}
	if (alltimes == 0)
	{
		alltimes = 1;
	}
	double pbWin = (double)(bTimes)/(double)alltimes;
	double pwWin = (double)(wTimes)/(double)alltimes;
	QString information, numStr; //输出的测试信息
	QTextCodec::setCodecForCStrings(QTextCodec::codecForLocale());
	information += "测试结束，(黑棋）算法";
	numStr.setNum(bAlgorithm);
	information += numStr;
	numStr.clear();
	information += "胜利次数:";
	numStr.setNum(bTimes);
	information += numStr;
	numStr.clear();
	information += " 获胜频率:";
	numStr.setNum(pbWin);
	information += numStr;
	numStr.clear();
	information += " （白棋）算法";
	numStr.setNum(wAlgorithm);
	information += numStr;
	numStr.clear();
	information += "胜利次数:";
	numStr.setNum(wTimes);
	information += numStr;
	numStr.clear();
	information += " 获胜频率:";
	numStr.setNum(pwWin);
	information += numStr;
	numStr.clear();
	QMessageBox::information(this, tr("算法对战结果"),
		information);
	information.clear();
}
