#include "mainwindow.h"
#include "ui_mainwindow.h"
//类引用
#include "xmlhelp.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    //xmlhelp类初始
    xmlhelp xmlModel;

    //初始化
    initActions();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::changeEvent(QEvent *e)
{
    QMainWindow::changeEvent(e);
    switch (e->type()) {
    case QEvent::LanguageChange:
        ui->retranslateUi(this);
        break;
    default:
        break;
    }
}


void MainWindow::initActions()
{


    bool bFlag=readXMLFile();
}

//读取xml配置文件
bool MainWindow::readXMLFile()
{

    QString path = QDir::currentPath();//获取程序当前目录
    //将地址中的"/"替换为"\"，因为在Windows下使用的是"\"。
    path.replace("/","\\");
    path = path + tr("\\config\\configfile.xml");

    bool bFlag=xmlModel->readFile(path);
    if (bFlag)
    {
        xmlModel->modbus_sets=xmlModel->getStructSets();
        xmlModel->sqlconstr=xmlModel->getSqlStr();

    }

    return bFlag;
}

