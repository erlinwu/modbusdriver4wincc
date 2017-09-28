#include "xmlhelp.h"


xmlhelp::xmlhelp()
{


}
//读取xml配置文件
//注意 流的方式读取xml 文件会被锁定。再用 QXmlQuery去读取文件的时候，无法进行搜索。
//混用过于复杂，采用xpath方式。但是 QXmlQuery类 qt5不进行维护.归到核心类中。
bool xmlhelp::readFile(const QString &fileName)
{
    QString  errMsg;
    QFile file(fileName);
    QFile file_t(fileName);//流式读取和xpath读取2个对象的QFile一定要分开取

    //文件找不到
    if ( ! file.exists()) {
        errMsg = "Xml文件未找到！";
        return false;
    }
    //文件打不开
//    if (!file.open(QFile::ReadOnly | QFile::Text)) {
//        errMsg = "文件打不开！";
//        return false;
//    }
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)){
        errMsg = "文件只读";
        return false;
    }
    QXmlQuery xmlquery;
    xmlquery.setFocus(&file);//xpath方式
    xmlquery.setQuery("/root/modbus/modbus_set/@name/string()");
    if ( ! xmlquery.isValid())
               return false;

    xmlquery.evaluateTo(&errMsg);

    //test
    QXmlStreamReader reader;
    reader.setDevice(&file);
    QString xmlfilename = fileName;

    while (!reader.atEnd()) {
        if (reader.isStartElement()) {
            if (reader.name() == "root") {
                readSetElement(reader,xmlfilename,xmlquery);//解析配置文件
            }
            else {
                reader.raiseError(QObject::tr("配置文件格式不对"));
            }
        }
        else {
            reader.readNext();
        }
    }
    file.close();
    if (reader.hasError()) {
//        QMessageBox::critical(0, QObject::tr("Error"),
//                              QObject::tr("配置文件解析失败 %1").arg(fileName));
        return false;
    } else if (file.error() != QFile::NoError) {
//        QMessageBox::critical(0, QObject::tr("Error"),
//                              QObject::tr("无法读取配置文件 %1").arg(fileName));
        return false;
    }
    return true;//
}
//解析配置文件
void xmlhelp::readSetElement(QXmlStreamReader &reader,const QString &xmlfilename,QXmlQuery &xmlquery)
{
    //注意第一行加了一个断言。意思是，如果在进入函数的时候，reader 不是StartElement状态，
    //或者说标签不是 root，就认为出错。然后继续调用readNext()，获取下面的数据。
    Q_ASSERT(reader.isStartElement() && reader.name() == "root");
    reader.readNext();
    //
    QString  errMsg;
    while (!reader.atEnd()) {//xml文件结尾
        if (reader.isEndElement()) {
            reader.readNext();
            //break;
        }

        if (reader.isStartElement()) {
            if (reader.name() == "dbinfo") {
                QString sqlconstr;
                sqlconstr=reader.attributes().value("sqlconstr").toString();

                //数据库配置读取
//                xmlquery.setQuery("/root/dbinfo/@sqlconstr/string()");
//                if ( !xmlquery.isValid()){
//                    errMsg = "查询参数错误，没有结果。";
//                }
//                else{
//                    xmlquery.evaluateTo(&sqlconstr) ;
//                }

            }
            else if (reader.name() == "modbus_set") {
                //
                QString s_temp="";
                bool ok;
                //modbus配置读取
                QString s_tempname=reader.attributes().value("name").toString();
                QString s_querystr="/root/modbus/modbus_set[@name=\""+s_tempname+"\"]";

                Settings m_settings;


                xmlquery.setQuery("/root");
                if ( ! xmlquery.isValid())
                           return;
                xmlquery.evaluateTo(&m_settings.type);

                //type
                xmlquery.setQuery(s_querystr+"/@type/string()");
                xmlquery.evaluateTo(&m_settings.type);
                //modbustype
                xmlquery.setQuery(s_querystr+"/@modbustype/string()");
                xmlquery.evaluateTo(&m_settings.modubstype);
                //responsetime
                xmlquery.setQuery(s_querystr+"/set/@responsetime/string()");
                xmlquery.evaluateTo(&s_temp);
                m_settings.responseTime=s_temp.toInt(&ok,10);
                //retry
                xmlquery.setQuery(s_querystr+"/set/@retry/string()");
                xmlquery.evaluateTo(&s_temp);
                m_settings.numberOfRetries=s_temp.toInt(&ok,10);
                //modbusid
                xmlquery.setQuery(s_querystr+"/set/@modbusid/string()");
                xmlquery.evaluateTo(&m_settings.modbusid);
                //ip
                xmlquery.setQuery(s_querystr+"/tcp/@ip/string()");
                xmlquery.evaluateTo(&m_settings.ip);
                //port
                xmlquery.setQuery(s_querystr+"/tcp/@port/string()");
                xmlquery.evaluateTo(&m_settings.port);
                //com
                xmlquery.setQuery(s_querystr+"/com/@com/string()");
                xmlquery.evaluateTo(&m_settings.com);
                //baudrate
                xmlquery.setQuery(s_querystr+"/com/@baudrate/string()");
                xmlquery.evaluateTo(&s_temp);
                m_settings.baud=s_temp.toInt(&ok,10);
                //databit
                xmlquery.setQuery(s_querystr+"/com/@databit/string()");
                xmlquery.evaluateTo(&s_temp);
                m_settings.dataBits=s_temp.toInt(&ok,10);
                //parity
                xmlquery.setQuery(s_querystr+"/com/@parity/string()");
                xmlquery.evaluateTo(&s_temp);
                m_settings.parity=s_temp.toInt(&ok,10);
                //stopbit
                xmlquery.setQuery(s_querystr+"/com/@stopbit/string()");
                xmlquery.evaluateTo(&s_temp);
                m_settings.stopBits=s_temp.toInt(&ok,10);
                //数组容器类 配置添加
                modbus_sets.append(m_settings);

            }
            else {
                //skipUnknownElement(reader);
            }
            reader.readNext();
        }
        else {
            reader.readNext();
        }
    }
}

void xmlhelp::skipUnknownElement(QXmlStreamReader &reader)
{
    reader.readNext();
    while (!reader.atEnd()) {
        if (reader.isEndElement()) {
            reader.readNext();
            break;
        }

        if (reader.isStartElement()) {
            skipUnknownElement(reader);
        } else {
            reader.readNext();
        }
    }
}

//返回查询语句
QString xmlhelp::getSqlStr()
{
    return sqlconstr;
}
//
QVector<xmlhelp::Settings> xmlhelp::getStructSets()
{
    return modbus_sets;
}

