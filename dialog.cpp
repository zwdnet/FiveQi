#include "dialog.h"

SelectDialog::SelectDialog(QWidget * parent, Qt::WindowFlags f)
	    : QDialog(parent)
{
	bAlgorithm = 0;
	wAlgorithm = 0;
	times = 100;
	setFixedSize(250, 150);
	QTextCodec::setCodecForTr(QTextCodec::codecForLocale());
	setWindowTitle(tr("选择算法及对战次数"));
	blackAlgorithm = new QComboBox;
	blackAlgorithm->addItem(tr("算法1(随机算法)"), 1);
	blackAlgorithm->addItem(tr("算法2(贪心算法)"), 2);
	blackAlgorithm->addItem(tr("算法3(改进的随机算法)"), 3);
	connect(blackAlgorithm, SIGNAL(currentIndexChanged(int)),
		this, SLOT(bAlgorithmChanged(int)));
	whiteAlgorithm = new QComboBox;
	whiteAlgorithm->addItem(tr("算法1(随机算法)"), 1);
	whiteAlgorithm->addItem(tr("算法2(贪心算法)"), 2);
	whiteAlgorithm->addItem(tr("算法3(改进的随机算法)"), 3);
	connect(whiteAlgorithm, SIGNAL(currentIndexChanged(int)),
		this, SLOT(wAlgorithmChanged(int)));

	label1 = new QLabel(tr("黑棋的算法为:"), this);
	QHBoxLayout * layout1 = new QHBoxLayout;
	layout1->addWidget(label1);
	layout1->addWidget(blackAlgorithm);
	
	label2 = new QLabel(tr("白棋的算法为:"), this);
	QHBoxLayout * layout2 = new QHBoxLayout;
	layout2->addWidget(label2);
	layout2->addWidget(whiteAlgorithm);

	label3 = new QLabel(tr("对战的次数为:"), this);
	currentTimesSlider = new QSpinBox(this);
	currentTimesSlider->setRange(0,1000);
	currentTimesSlider->setSingleStep(100);
	currentTimesSlider->setValue(100);
	connect(currentTimesSlider, SIGNAL(valueChanged(int)), this,
		SLOT(currentTimesChanged(int)));
	QHBoxLayout * layout3 = new QHBoxLayout;
	layout3->addWidget(label3);
	layout3->addWidget(currentTimesSlider);

	okButton = new QPushButton(tr("确定"), this);
	connect(okButton, SIGNAL(clicked()), this, SLOT(OK()));
	QHBoxLayout * layout4 = new QHBoxLayout;
	layout4->addWidget(okButton);

	QVBoxLayout * mainLayout = new QVBoxLayout;
	mainLayout->addLayout(layout1);
	mainLayout->addLayout(layout2);
	mainLayout->addLayout(layout3);
	mainLayout->addLayout(layout4);
	
	setLayout(mainLayout);
}

SelectDialog::~SelectDialog()
{
}

void SelectDialog::bAlgorithmChanged(int value)
{
	bAlgorithm = value;
}

void SelectDialog::wAlgorithmChanged(int value)
{
	wAlgorithm = value;
}

void SelectDialog::OK()
{
	close();
}

void SelectDialog::currentTimesChanged(int value)
{
	times = value;
}

void SelectDialog::getValue(int & bAlgo, int & wAlgo, int & currentTimes)
{
	bAlgo = bAlgorithm;
	wAlgo = wAlgorithm;
	currentTimes = times;
}
