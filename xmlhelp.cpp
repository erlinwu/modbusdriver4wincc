#include "xmlhelp.h"


xmlhelp::xmlhelp()
{


}
//读取xml配置文件
bool xmlhelp::readFile(const QString &fileName)
{
    QString  errMsg;
    QFile file(fileName);

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

    QXmlStreamReader reader;
    reader.setDevice(&file);
    QString xmlfilename = fileName;

    while (!reader.atEnd()) {
        if (reader.isStartElement()) {
            if (reader.name() == "root") {
                readSetElement(reader,file,xmlfilename,xmlquery);//解析配置文件
            } else {
                reader.raiseError(QObject::tr("配置文件格式不对"));
            }
        } else {
            reader.readNext();
        }
    }
    file.close();
    if (reader.hasError()) {
//        QMessageBox::critical(0, QObject::tr("Error"),
//                              QObject::tr("配置文件解析失败 %1").arg(fileName));
//        return false;
    } else if (file.error() != QFile::NoError) {
//        QMessageBox::critical(0, QObject::tr("Error"),
//                              QObject::tr("无法读取配置文件 %1").arg(fileName));
        return false;
    }
    return true;//
}
//解析配置文件
void xmlhelp::readSetElement(QXmlStreamReader &reader,QFile &file,const QString &xmlfilename,QXmlQuery xmlquery)
{
    Q_ASSERT(reader.isStartElement() && reader.name() == "root");
    reader.readNext();
    QString  errMsg;
    while (!reader.atEnd()) {
        if (reader.isEndElement()) {
            reader.readNext();
            break;
        }

        if (reader.isStartElement()) {
            if (reader.name() == "dbinfo") {

                //数据库配置读取
                xmlquery.setQuery("/root/dbinfo/@sqlconstr/string()");
                if ( !xmlquery.isValid()){
                    errMsg = "查询参数错误，没有结果。";
//                    return errMsg;
                }
                else{
                    xmlquery.evaluateTo(&sqlconstr) ;
                }
                file.close();
                reader.readNext();

            }
            else if (reader.name() == "modbus_set") {

                QString s_temp="";
                bool ok;
                //modbus配置读取
                QString s_tempname=reader.attributes().value("name").toString();
                QString s_querystr="/root/dbinfo/modbus/modbus_set[@name='"+s_tempname+"']";
                QXmlQuery xmlquery;//xpath方式
                QFile file(xmlfilename);
                xmlquery.setFocus(&file);//xpath方式
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

                reader.readNext();
            }
            else {
                skipUnknownElement();
            }
        } else {
            reader.readNext();
        }
    }
}

void xmlhelp::skipUnknownElement()
{
    reader.readNext();
    while (!reader.atEnd()) {
        if (reader.isEndElement()) {
            reader.readNext();
            break;
        }

        if (reader.isStartElement()) {
            skipUnknownElement();
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

