/*****************************************************************************\
 * Copyright 2012-2015 Paul Scherrer Institut, Villigen, Switzerland
\*****************************************************************************/

#include "utils.h"
#include "enums.h"

#include <postfix.h>
#include <cvtFast.h>

#include <QVector>
#include <QVariant>
#include <QProcess>
#include <QtDebug>

#include <QApplication>

#include <QQmlEngine>
#include <QQmlComponent>
#include <QQuickWindow>
#include <QDesktopWidget>


#include <QDir>
#include <QFileInfo>

#include <QDateTime>

#include <fstream>

#include "parser.h"

/*!
    \qmltype Utils
    \inqmlmodule PvComponents
    \brief Helper functions

    This is a group of helper functions, which are called by QML components.
*/

/*!
    \qmlmethod string Utils::format(string format, real number)

    Format a number with given format, \sa QString::sprintf()
*/

/*!
    \qmlmethod real Utils::calculate(string expr, array input)

    Evalute an expression based on given input.
*/

/*!
    \qmlmethod bool Utils::execute(string program)

    Execute a program, \sa QProcess::startDetached(), QProcess::execute()
*/

/*!
    \qmlmethod string Utils::convert(TextFormat format, real value, int precision)

    Convert a number with given format.
*/

#define M_PI 3.14159265358979323846

// conversion functions from conversion.c
extern "C" {
extern int localCvtLongToHexString(long source, char *pdest);
extern void localCvtDoubleToString(double flt_value, char  *pstr_value, unsigned short precision);
extern void localCvtDoubleToExpNotationString(double value, char *textField, unsigned short precision);
extern void medmLocalCvtDoubleToSexaStr (double value,  char *string, unsigned short prec,
                                 double hopr, double lopr, int *status);
extern double strtos(char *string, char **rptr, int *status);
}

extern QVariantMap parseGeometry(const QByteArray &a);

QCSUtils::QCSUtils(QObject *parent)
    : QObject(parent)
{

}

QString QCSUtils::format(QString format, double number)
{
    return QString("").sprintf(format.toLatin1(), number);
}

double QCSUtils::calculate(QString expr, QVariantList input)
{
    double result = 0.0;
    short error;
    long status;
    char infix[INFIX_TO_POSTFIX_SIZE(80)];
    
    QVector<double> d;
    foreach(QVariant v, input)
        d.push_back(v.toDouble());
    
    status = postfix(expr.toLatin1().constData(), infix, &error);
    if (status != 0)
        return qQNaN();
    
    status = calcPerform(d.data(), &result, infix);
    if (status != 0)
        return qQNaN();

    return result;
}

bool QCSUtils::execute(QString program)
{
    // remove whitespaces from the start and the end
    program = program.trimmed();
    // replace medm with ADLViewer
    if (program.startsWith("medm")) {
        program = qApp->applicationFilePath() + program.mid(4);
    }
    // only if program ends with "&", start detached
    if (program.endsWith("&")) {
        program.chop(1);
        return QProcess::startDetached(program);
    }
    else
        return QProcess::execute(program) == 0;
}

QString QCSUtils::convert(int format, QVariant value, int precision)
{
    char textField[128];
    int status;

    switch (format) {
    case TextFormat::String:
    case TextFormat::Decimal:
        cvtDoubleToString(value.toDouble(), textField, precision);
        break;
    case TextFormat::Exponential:
        cvtDoubleToExpString(value.toDouble(), textField, precision);
        break;
    case TextFormat::EngNotation:
        localCvtDoubleToExpNotationString(value.toDouble(), textField, precision);
        break;
    case TextFormat::Compact:
        cvtDoubleToCompactString(value.toDouble(), textField, precision);
        break;
    case TextFormat::Truncated:
        cvtLongToString(value.toInt(), textField);
        break;
    case TextFormat::Hexadecimal:
        localCvtLongToHexString(value.toInt(), textField);
        break;
    case TextFormat::Octal:
        cvtLongToOctalString(value.toInt(), textField);
        break;
    case TextFormat::Sexagesimal:
        medmLocalCvtDoubleToSexaStr(value.toDouble(), textField, precision,
          0.0, 0.0, &status);
        break;
    case TextFormat::SexagesimalHMS:
        medmLocalCvtDoubleToSexaStr(value.toDouble()*12.0/M_PI, textField, precision,
          0.0,0.0,&status);
        break;
    case TextFormat::SexagesimalDMS:
        medmLocalCvtDoubleToSexaStr(value.toDouble()*180.0/M_PI, textField, precision,
          0.0,0.0,&status);
        break;
    default:
        break;
    }
    return QString::fromLatin1(textField);
}

double QCSUtils::parse(int format, QString textValue)
{
    char textField[128];
    double value = 0;
    char *end;
    int status;

    strncpy(textField, textValue.toLatin1().constData(), sizeof(textField));
    textField[sizeof(textField) - 1] = '\0';

    switch (format) {
    case TextFormat::Octal:
        value = (double)strtoul(textField, &end, 8);
        break;
    case TextFormat::Hexadecimal:
        value = (double)strtoul(textField, &end, 16);
        break;
    case TextFormat::Sexagesimal:
        value = strtos(textField, &end, &status);
        break;
    case TextFormat::SexagesimalHMS:
        value = strtos(textField, &end, &status);
        value *= M_PI / 12.0;
        break;
    case TextFormat::SexagesimalDMS:
        value = strtos(textField, &end, &status);
        value *= M_PI / 180.0;
        break;
    default:
        if (textValue.startsWith("0x", Qt::CaseInsensitive))
            value = (double)strtoul(textField, &end, 16);
        else
            value = (double)strtod(textField, &end);
        break;
    }
    if (*end == '\0' && end != textField)
        return value;
    else
        return qSNaN();
}

QUrl QCSUtils::searchADLFile(QString fileName, QUrl parentFileName)
{
    // Check url scheme
    if (fileName.startsWith("http://")
            || fileName.startsWith("https://")
            || fileName.startsWith("ftp://")) {
        qWarning() << "Only local file is supported";
        return QUrl();
    }
    if (fileName.startsWith("file://")) {
        fileName = QUrl(fileName).toLocalFile();
    }

    QFileInfo fi(fileName);
#ifdef Q_OS_WIN
    char sep = ';';
#else
    char sep = ':';
#endif
    if (!fi.exists() && fi.isRelative()) {
        QByteArray paths = qgetenv("EPICS_DISPLAY_PATH");
        if (!parentFileName.isEmpty()) {
            QFileInfo pfi(parentFileName.toString());
            paths = pfi.absolutePath().toLocal8Bit() + sep + paths;
        }
        foreach (QByteArray path, paths.split(sep)) {
            fi.setFile(QDir(path), fileName);
            if (fi.exists())
                break;
        }
    }
    if (!fi.exists())
        return QUrl();
    else
        return QUrl::fromLocalFile(fi.absoluteFilePath());
}

QString QCSUtils::openADLDisplay(QUrl fileName, QString macro)
{
    std::map<std::string, std::string> macroMap;

    foreach(QString m, macro.split(',')) {
        if (m.isEmpty()) continue;
        QStringList paires = m.split('=');
        if (paires.length() == 2)
            macroMap[paires[0].trimmed().toStdString()] =  paires[1].trimmed().toStdString();
        else
            qDebug() << "macro unclear" << m;
    }

    std::string qmlBody = parseADLDisplay(fileName.toLocalFile().toStdString(), macroMap);

    return QString::fromStdString(qmlBody);
}

QString QCSUtils::openADLComposite(QUrl fileName, QString macro)
{
    std::map<std::string, std::string> macroMap;

    foreach(QString m, macro.split(',')) {
        if (m.isEmpty()) continue;
        QStringList paires = m.split('=');
        if (paires.length() == 2)
            macroMap[paires[0].trimmed().toStdString()] =  paires[1].trimmed().toStdString();
        else
            qWarning() << "macro unclear" << m;
    }

    std::string qmlBody = parseADLComposite(fileName.toLocalFile().toStdString(), macroMap);

    return QString::fromStdString(qmlBody);
}

QWindow * QCSUtils::createDisplay(QString qml, QObject *display, QUrl filePath)
{
    QWindow *window = NULL;
    QQmlEngine *engine = qmlEngine(display);
    if (engine) {
        QQmlComponent component(engine);
        component.setData(qml.toLocal8Bit(), filePath);
        while(!component.isReady()) {
            if (component.isError()) {
                foreach(QQmlError error, component.errors())
                    qDebug() << error;
                return NULL;
            }
        }
        window = qobject_cast<QQuickWindow *>(component.create());
        window->setTitle(filePath.fileName());
    }
    return window;
}

QVariantMap QCSUtils::parseX11Geometry(QString geometry)
{
    return parseGeometry(geometry.toLatin1());
}

QString QCSUtils::currentDateTime()
{
    return QDateTime::currentDateTime().toString("yyyy-MM-dd HH:mm:ss");
}
