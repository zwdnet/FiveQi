#ifndef SELECTDIALOG_H
#define SELECTDIALOG_H

#include <QtGui>

//用来选择算法之间对战的算法和对战次数的对话框

class QComboBox;
class QLayout;
class QSpinBox;
class Window;

class SelectDialog : public QDialog
{
	Q_OBJECT
public:
	SelectDialog(QWidget * parent = 0, Qt::WindowFlags f = Qt::Dialog);
	~SelectDialog();
	void getValue(int & bAlgo, int & wAlgo, int & currentTimes);

//自定义槽函数
private slots:
	void bAlgorithmChanged(int value); //黑棋算法改变
	void wAlgorithmChanged(int value); //白棋算法改变
	void OK();                         //点击确定运行算法间的对抗
	void currentTimesChanged(int value); //对战次数改变

private:
	QComboBox * blackAlgorithm; //选择黑棋所用算法的下拉列表
	QComboBox * whiteAlgorithm; //选择白棋所用算法的下拉列表	
	QLabel * label1, * label2, * label3;
	QSpinBox * currentTimesSlider; //选择对战次数的微调框
	QPushButton * okButton; //确定按钮

	int bAlgorithm, wAlgorithm, times; //黑，白棋所用的算法，对战次数
};

#endif
