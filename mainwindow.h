#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QtWidgets/QMainWindow>

#include <QCoreApplication>
#include <QDebug>

#include <QtCore/qglobal.h>

//Qt5.1 中提供了QtSerialPort模块
#include <QtSerialPort/QSerialPort>//QSerialPort 类提供了操作串口的各种接口。
#include <QtSerialPort/QSerialPortInfo>//QSerialPortInfo 是一个辅助类，可以提供计算机中可用串口的各种信息。

//Qt4 和 Qt5 则提供了 QtXmlPatterns 模块
#include <QtXmlPatterns/QtXmlPatterns>
//xpath读取
#include <QtXmlPatterns/QXmlQuery>

QT_BEGIN_NAMESPACE

namespace Ui {
class MainWindow;
}

QT_END_NAMESPACE

class xmlhelp;



class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    //返回配置解析结果
    bool readXMLFile();


    //mysql连接字符串
    QString sqlconstr="";

protected:
    void changeEvent(QEvent *e);

private:
    void initActions();
    Ui::MainWindow *ui;

    xmlhelp *xmlModel;

};

#endif // MAINWINDOW_H
