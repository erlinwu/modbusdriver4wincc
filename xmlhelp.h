#ifndef XMLHELP_H
#define XMLHELP_H

#include <QCoreApplication>
#include <QDebug>

#include <QtCore/qglobal.h>

//Qt5.1 中提供了QtSerialPort模块
#include <QtSerialPort/QSerialPort>//QSerialPort 类提供了操作串口的各种接口。
#include <QtSerialPort/QSerialPortInfo>//QSerialPortInfo 是一个辅助类，可以提供计算机中可用串口的各种信息。


//Qt4 和 Qt5 则提供了 QtXmlPatterns 模块
#include <QtXmlPatterns/QtXmlPatterns>
//xpath读取
#include <QtXmlPatterns>
#include <QtXmlPatterns/QXmlQuery>

//tr
#include <QObject>
//QMessageBox
#include <QMessageBox>
//数组
#include <QVariant>
//自定义类继承QObject 利用qt的特性自动释放资源 不用自己写析构函数
#include <QObject>

class xmlhelp : public QObject
{
    Q_OBJECT
public:
    xmlhelp();

    //modbus 配置
    struct Settings {
        QString linkname="demo";
        QString type="tcp";
        QString modubstype="server";
        QString modbusid="";
        int responseTime = 1000;
        int numberOfRetries = 3;

        //串口方式
        QString com="com1";
        int parity = QSerialPort::EvenParity;
        int baud = QSerialPort::Baud19200;
        int dataBits = QSerialPort::Data8;
        int stopBits = QSerialPort::OneStop;

        //tcp方式
        QString ip="127.0.0.1";
        QString port="502";
    };
    //自定义类 宏
    //Q_DECLARE_METATYPE(Settings)

    //modbus配置数组 返回给主窗体，开多线程用。
    QVector<Settings> modbus_sets;
    //mysql连接字符串`
    QString sqlconstr="";

    QXmlQuery xmlquery;//xpath方式

    QString xmlfilename;

    QString errMsg;//错误信息

    QXmlStreamReader reader_public;//xml流对象
    QXmlStreamReader reader;//xml流对象

    //返回配置文件中的连接字符串
    QString getSqlStr();
    QVector<Settings> getStructSets();

    //返回配置的结构体数组
    bool readFile(const QString &fileName);

private:

    //
    void readSetElement(QXmlStreamReader &reader,const QString &xmlfilename,QXmlQuery &xmlquery);

    Settings m_settings;//内部变量

    QFile file;

    void skipUnknownElement(QXmlStreamReader &reader);



public slots:

signals:


};


#endif // XMLHELP_H
