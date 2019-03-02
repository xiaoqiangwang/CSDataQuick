/*****************************************************************************\
 * Copyright 2012-2018 Paul Scherrer Institut, Villigen, Switzerland
\*****************************************************************************/

#include "utils.h"
#include "enums.h"
#include "csdata.h"
#include "parser.h"
#include "parsermanager.h"

#include <postfix.h>
#include <cvtFast.h>

#include <QVector>
#include <QVariant>
#include <QProcess>
#include <QtDebug>

#include <QQuickItem>
#include <QQmlEngine>
#include <QQmlComponent>
#include <QQuickWindow>
#include <QQuickView>
#include <QGuiApplication>
#include <QClipboard>

#include <QDir>
#include <QFileInfo>

#include <QProcessEnvironment>
#include <QDateTime>


/*!
    \qmltype Utils
    \inqmlmodule CSDataQuick.Components
    \brief Helper functions

    This is a group of helper functions, which are called by QML components.
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

    _inPuppet = QProcessEnvironment::systemEnvironment().contains("QML_PUPPET_MODE");
}


/*!
    \qmlmethod Utils::vectorGet(vector, array, index, count)

    Get the subarray from a \a vector variant into \a array, starting at position \a index (default 0).
    If \a count is -1 (the default), all elements after \a index are included.
    \sa QVector::mid,
*/
void QCSUtils::vectorGet(QVariant vector, QJSValue array, int index, int count)
{
    quint32 i = 0;

    if (vector.userType() == qMetaTypeId< QVector<double> >()) {
        foreach(double d, vector.value< QVector<double> >().mid(index, count))
            array.setProperty(i++, d);
    }
    else if (vector.userType() == qMetaTypeId< QVector<float> >()) {
        foreach(float d, vector.value< QVector<float> >().mid(index, count))
            array.setProperty(i++, double(d));
    }
    else if (vector.userType() == qMetaTypeId< QVector<int> >()) {
        foreach(int d, vector.value< QVector<int> >().mid(index, count))
            array.setProperty(i++, d);
    }
    else if (vector.userType() == qMetaTypeId< QVector<ushort> >()) {
        foreach(ushort d, vector.value< QVector<ushort> >().mid(index, count))
            array.setProperty(i++, d);
    }
    else if (vector.userType() == qMetaTypeId< QVector<uchar> >()) {
        foreach(uchar d, vector.value< QVector<uchar> >().mid(index, count))
            array.setProperty(i++, d);
    }
    else if (vector.userType() == QVariant::StringList) {
        foreach(QString d, vector.value<QStringList>().mid(index, count))
            array.setProperty(i++, d);
    }
}
/*!

    \qmlmethod Utils::vectorSet(vector, value, index)

    Set the \a index element from \a vector variant to \a value and return the modified copy.
*/
QVariant QCSUtils::vectorSet(QVariant vector, QJSValue value, int index)
{
    QVariant v;

    if (vector.userType() == QMetaType::QStringList) {
        QStringList list = qvariant_cast<QStringList>(vector);
        while (index >= list.size())
            list.append("");
        list.replace(index, value.toString());
        v.setValue(list);
    } else {
        QVector<double> list = qvariant_cast< QVector<double> >(vector);
        while (index >= list.size())
            list.append(0);
        list.replace(index, value.toNumber());
        v.setValue(list);
    }

    return v;
}

/*!
    \qmlmethod string Utils::format(format, number)

    Format a number with given format, \sa QString::sprintf()
*/
QString QCSUtils::format(QString format, double number)
{
    return QString("").sprintf(format.toLatin1(), number);
}

/*!
    \qmlmethod double Utils::calculate(expression, input)

    Evalute an \a expression based on given \a input variables.
    This calls \c postfix and \c calcPerform functions from EPICS base.
*/
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

/*!
    \qmlmethod bool Utils::execute(program)

    Execute a program. If \a program ends with "&", the new process will be detached.
    Otherwise it will wait for the new process to finish and return the exit code.

    \sa QProcess::startDetached(), QProcess::execute()
*/
bool QCSUtils::execute(QString program)
{
    // remove whitespaces from the start and the end
    program = program.trimmed();
    // replace medm with ADLViewer
    if (program.startsWith("medm")) {
        program = qApp->applicationFilePath() + program.mid(4);
    }
    // replace caqtdm with ADLViewer
    if (program.startsWith("caqtdm")) {
        program = qApp->applicationFilePath() + program.mid(6);
    }
    // only if program ends with "&", start detached
    if (program.endsWith("&")) {
        program.chop(1);
        return QProcess::startDetached(program);
    }
    else
        return QProcess::execute(program) == 0;
}

/*!
    \qmlmethod string Utils::convert(TextFormat format, real value, int precision)

    Convert a number \a value with given \a format.
    This routine calls cvsFast functions from EPICS base and also functions from MEDM for certain special formats.
*/
QString QCSUtils::convert(int format, QVariant value, unsigned short precision)
{
    char textField[128] = {0};
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

/*!
    \qmlmethod double Utils::parse(int format, QString textValue)

    Parse a numeric value in \a format from \a textValue. This is the opposite of Utils::convert.
 */
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
        value = strtoul(textField, &end, 8);
        break;
    case TextFormat::Hexadecimal:
        value = strtoul(textField, &end, 16);
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
            value = strtoul(textField, &end, 16);
        else
            value = strtod(textField, &end);
        break;
    }
    if (*end == '\0' && end != textField)
        return value;
    else
        return qSNaN();
}

/*!
    \qmlmethod url Utils::searchDisplayFile(fileName, window)

    Search a display file with given \a fileName from the current directory, the file path of \a window
    and EPICS_DISPLAY_PATH (.adl) or QML_DISPLAY_PATH (.qml) environment variable.
 */
QUrl QCSUtils::searchDisplayFile(QString fileName, QWindow *window)
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
    QByteArray paths;
    if (QString::compare(fi.suffix(), "adl", Qt::CaseInsensitive) == 0)
        paths = qgetenv("EPICS_DISPLAY_PATH");
    else if (QString::compare(fi.suffix(), "edl", Qt::CaseInsensitive) == 0)
        paths = qgetenv("EDMDATAFILES");
    else if (QString::compare(fi.suffix(), "ui", Qt::CaseInsensitive) == 0)
        paths = qgetenv("CAQTDM_DISPLAY_PATH");
    else if (QString::compare(fi.suffix(), "qml", Qt::CaseInsensitive) == 0)
        paths = qgetenv("QML_DISPLAY_PATH");

#ifdef Q_OS_WIN
    char sep = ';';
#else
    char sep = ':';
#endif
    if (!fi.exists() && fi.isRelative()) {
        if (window && !window->filePath().isEmpty()) {
            QUrl fileUrl = QUrl(window->filePath());
            if (fileUrl.isLocalFile()) {
                QFileInfo pfi(fileUrl.toLocalFile());
                paths = pfi.absolutePath().toLocal8Bit() + sep + paths;
            }
        }
        foreach (QByteArray path, paths.split(sep)) {
            if (path.isEmpty())
                continue;
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

/*!
    \qmlmethod Window Utils::createDisplay(qml, display, filePath)

    Create a top level window based on \a qml source, represented by \a filePath.
    The QQmlEngine used is which \a display was created in.
 */
QWindow * QCSUtils::createDisplay(QString qml, QObject *display, QUrl filePath, QString macro)
{
    QQuickWindow *window = Q_NULLPTR;
    QQmlEngine *engine = qmlEngine(display);
    if (!engine) {
        qCritical() << "No valid QML engine from object" << display->objectName();
        return Q_NULLPTR;
    }

    QQmlComponent component(engine);
    component.setData(qml.toLocal8Bit(), filePath);
    if(!component.isReady()) {
        qWarning() << component.errorString();
        return Q_NULLPTR;
    }

    QObject *topLevel = component.create();
    if (!topLevel) {
        qCritical() << component.errorString();
        return Q_NULLPTR;
    }

    window = qobject_cast<QQuickWindow *>(topLevel);
    if (!window) {
        if(qobject_cast<QQuickItem *>(topLevel)) {
            QQuickView* qxView = new QQuickView(engine, Q_NULLPTR);
            qxView->setResizeMode(QQuickView::SizeRootObjectToView);
            qxView->setWidth(topLevel->property("width").toInt());
            qxView->setHeight(topLevel->property("height").toInt());
            qxView->setContent(filePath, &component, topLevel);
            // object without parent returned to QML will be garbage collected
            // if not referenced anymore. so we declare the ownership.
            engine->setObjectOwnership(qxView, QQmlEngine::CppOwnership);
            window = qxView;
        } else {
            // neither a visual item or a window, just delete
            delete topLevel;
            return Q_NULLPTR;
        }
    }
    if (QString(topLevel->metaObject()->className()).startsWith("BaseWindow_QMLTYPE")) {
        QString title = topLevel->property("title").toString();
        if (!title.isEmpty())
            window->setTitle(title);
        // copy position from toplevel item to window
        window->setX(topLevel->property("x").toInt());
        window->setY(topLevel->property("y").toInt());
        topLevel->setProperty("x", 0);
        topLevel->setProperty("y", 0);
    } else {
        // attach context menu to the existing window
        QString temp = QString("import QtQuick 2.0\n"
                               "import CSDataQuick.Components.Private 1.0\n"
                               "ContextMenu {anchors.fill: parent; z:-1}\n");
        QQmlComponent component(engine);
        component.setData(temp.toLocal8Bit(), QUrl());
        QQuickItem *contextMenu = qobject_cast<QQuickItem *>(component.create());
        if (contextMenu) {
            contextMenu->setParentItem(window->contentItem());
        }
    }

    if (window->title().isEmpty())
        window->setTitle(filePath.fileName());
    window->setFilePath(filePath.toString());
    // dynamic properties, which can be accessible from C++ but not QML
    window->setProperty("filePath", filePath);
    window->setProperty("macro", macro);
    return window;
}

/*!
    \qmlmethod Window Utils::createDisplayByFile(display, filePath, macro)

    Create a top level window from a file with given \a filePath with \a macro expansion.
    The file can be either ADL or QML.

    It calls parser library to create the qml source, and then calls Utils::createDisplay to actually create the display window.
    The QQmlEngine used is which \a display was created in.
 */
QWindow * QCSUtils::createDisplayByFile(QObject *display, QUrl filePath, QString macro)
{
    QMap<QString, QString> macroMap;
    foreach(QString m, macro.split(',')) {
        if (m.isEmpty()) continue;
        QStringList paires = m.split('=');
        if (paires.length() == 2)
            macroMap[paires[0].trimmed()] =  paires[1].trimmed();
        else
            qWarning() << "macro unclear" << m;
    }

    QFileInfo fi(filePath.toLocalFile());
    QCSParser *parser = parserManager->parserForExtension(fi.suffix());
    if (!parser) {
        qCritical() << fi.absoluteFilePath() << " is not supported";
        return Q_NULLPTR;
    }

    QString qml = parser->parseDisplayFile(fi.absoluteFilePath(), macroMap);
    if (qml.isEmpty()) {
        qCritical() << "parser cannot parse " << fi.absoluteFilePath();
        return Q_NULLPTR;
    }

    return createDisplay(qml, display, filePath, macro);
}
/*!
    \qmlmethod Window Utils::createComponentByFile(display, filePath, macro)

    Create a qml component from a file with given \a filePath with \a macro expansion.

    It calls parser library to create the qml source, and then call QQmlComponent to create an instance.
    The QQmlEngine used is which \a display was created in.
 */
QQuickItem * QCSUtils::createComponentByFile(QQuickItem *display, QUrl filePath, QString macro)
{
    QMap<QString, QString> macroMap;
    foreach(QString m, macro.split(',')) {
        if (m.isEmpty()) continue;
        QStringList paires = m.split('=');
        if (paires.length() == 2)
            macroMap[paires[0].trimmed()] =  paires[1].trimmed();
        else
            qWarning() << "macro unclear" << m;
    }

    QFileInfo fi(filePath.toLocalFile());
    QCSParser *parser = parserManager->parserForExtension(fi.suffix());
    if (!parser) {
        qCritical() << fi.absoluteFilePath() << " is not supported";
        return Q_NULLPTR;
    }

    QString qml = parser->parseDisplayFile(fi.absoluteFilePath(), macroMap, true);
    if (qml.isEmpty()) {
        qCritical() << "parser cannot parse " << fi.absoluteFilePath();
        return Q_NULLPTR;
    }

    QQmlEngine *engine = qmlEngine(display);
    if (!engine) {
        qCritical() << "No valid QML engine from object" << display->objectName();
        return Q_NULLPTR;
    }

    QQmlComponent component(engine);
    component.setData(qml.toLocal8Bit(), filePath);
    if(!component.isReady()) {
        qDebug() << qml;
        qWarning() << component.errorString();
        return Q_NULLPTR;
    }

    QQuickItem *item = qobject_cast<QQuickItem *>(component.create());
    if (!item) {
        qCritical() << component.errorString();
        return Q_NULLPTR;
    }
    engine->setObjectOwnership(item, QQmlEngine::JavaScriptOwnership);

    item->setParentItem(display);

    return item;
}

/*!
    \qmlmethod object Utils::parseX11Geometry(geometry)

    Parse a token of a X11 geometry specification, e.g. "200x100+10-20".
    The returned map contains the following fields,
    \list
    \li xOffset - x offset
    \li yOffset - y offset
    \li corner - the corner from where the offset counts, see Qt::Corner.
    \li width - window width
    \li height - window height
    \endlist
 */
QVariantMap QCSUtils::parseX11Geometry(QString geometry)
{
    return parseGeometry(geometry.toLatin1());
}

/*!
    \qmlmethod string Utils::currentDateTime()

    Return the current time in the form of "yyyy-MM-dd HH:mm:ss".
 */
QString QCSUtils::currentDateTime()
{
    return QDateTime::currentDateTime().toString("yyyy-MM-dd HH:mm:ss");
}

/*!
    \qmlmethod Window Utils::parentWindow(item)

    Return the parent window for \a item.
*/
QWindow *QCSUtils::parentWindow(QQuickItem *item)
{
    if (!item)
        return Q_NULLPTR;

    return item->window();
}
/*!
    \qmlmethod point Utils::mapToGlobal(item, point)

    Maps the given \a point in this item's coordinate system to the equivalent point
    within global screen coordinate system, and returns the mapped coordinate.

    This is an equivalent implementation of QQuickItem::mapToGlobal that first appeared in Qt 5.7.
*/
QPoint QCSUtils::mapToGlobal(QQuickItem *item, const QPoint point)
{
    if (!item)
        return point;
    
    QWindow *window = item->window();
    if (!window)
        return point;

    return window->mapToGlobal(item->mapToItem(Q_NULLPTR, point).toPoint());
}
/*!
    \qmlmethod list<object> Utils::parseExecList()

    Return the command list defined by \e MEDM_EXEC_LIST environment variable.
    Each command object contains the following property,
    \list
    \li label - menu text
    \li command - system command
    \endlist
*/
QVariantList QCSUtils::parseExecList()
{
    QVariantList list;

    QByteArray arg;
    QByteArray label;
    QByteArray command;
    QByteArray envs = qgetenv("MEDM_EXEC_LIST");
    if (!envs.isEmpty() && !envs.endsWith(':'))
        envs.append(':');
    for(int i=0; i<envs.length(); i++) {
        char c = envs.at(i);
        if (c == ';') {
            label = arg;
            arg = "";
        } else if (c == ':') {
            // look ahead if there follows backlash '\'
            // whihc can be part of file path on Win32
            if (i + 1 < envs.length() && envs.at(i + 1) == '\\') {
                arg += c;
            } else {
                command = arg;
                arg = "";
                if (!label.isEmpty() && !command.isEmpty()) {
                    QVariantMap map;
                    map.insert("label", QString(label));
                    map.insert("command", QString(command));
                    list.append(map);
                }
            }
        } else {
            arg += c;
        }
    }
    return list;
}

/*!
    \qmlmethod var Utils::getProperty(object, name)

    Returns the value of the \a object's \a name property.
*/
QVariant QCSUtils::getProperty(QObject *object, QString name)
{
    if (name == "winId") {
        QWindow *window = qobject_cast<QWindow*>(object);
        if (window)
            return window->winId();
        else
            return QVariant();
    } else {
        if (object)
            return object->property(name.toLocal8Bit());
        else
            return QVariant();
    }
}

/*!
    \qmlmethod Utils::copyToClipboard(text)

    Copy the \a text to the global clipboard.
*/
void QCSUtils::copyToClipboard(const QString& text)
{
    QGuiApplication::clipboard()->setText(text);
}

/*!
    \qmlmethod int Utils::qtVersion()

    Return the QT_VERSION macro.
*/
int QCSUtils::qtVersion()
{
    return QT_VERSION;
}

/*!
    \qmlmethod string Utils::formatString(data, format, precision)

    Convert control system \a data according to \a format and \a precision.
 */
QString QCSUtils::formatString(QCSData* data, int format, unsigned short precision, QVariant value)
{
    if (!value.isValid())
        value = data->value();

    switch(data->fieldType()) {
    case QCSData::Enum:
    {
        int index = value.toInt();
        if (index < data->stateStrings().length())
            return data->stateStrings().at(index);
        else
            return QString::number(index);
    }
    case QCSData::String:
        return value.toString();
    case QCSData::Char:
        if (format == TextFormat::String) {
            if (value.canConvert<QVariantList>()) {
                QString res;
                foreach(QVariant v, value.value<QSequentialIterable>()) {
                    QChar c = v.toChar();
                    if (c.isNull())
                        break;
                    res += c;
                }
                return res;
            } else {
                QChar c = value.toChar();
                if (c.isNull())
                    return "";
                else
                    return c;
            }
        }
        /* fall through if format is other than string*/
    default:
        if (value.canConvert<QVariantList>())
            value = value.value<QSequentialIterable>().at(0);
        return convert(format, value, precision);
    }
}

/*!
    \qmlmethod Utils::resizeChildItems(parent, rw, rh)

    Resize all child items from \a parent by \a rw horizontally and \a rh vertically,
    and move their top left position as well.
 */
void QCSUtils::resizeChildItems(QQuickItem *parent, qreal rw, qreal rh)
{
    foreach(QQuickItem *child, parent->childItems()) {
        if (!qFuzzyCompare(rw, 1)) {
            child->setX(child->x() * rw);
            child->setWidth(child->width() * rw);
        }
        if (!qFuzzyCompare(rh, 1)) {
            child->setY(child->y() * rh);
            child->setHeight(child->height() * rh);
        }
    }
}
