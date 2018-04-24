#include "textformatter.h"
#include "csdata.h"
#include "utils.h"

TextFormatter::TextFormatter(QObject *parent) : QObject(parent), _data(Q_NULLPTR)
{
}

void TextFormatter::setData(QCSData *data)
{
    if (_data != Q_NULLPTR)
        disconnect(_data);

    _data = data;
    if (_data != Q_NULLPTR) {
        connect(_data, SIGNAL(stateStringsChanged()), this, SLOT(stateStringsChanged()));
        connect(_data, SIGNAL(valueChanged()), this, SLOT(valueChanged()));
        connect(_data, SIGNAL(sourceChanged()), this, SLOT(sourceChanged()));
    }
}

void TextFormatter::setText(QString text)
{
    if (_text != text) {
        _text = text;
        emit textChanged();
    }
}

void TextFormatter::setFormat(int format)
{
    _format = format;
    setText(QCSUtils::formatString(_data, _format, _precision));
}

void TextFormatter::setPrecision(int precision)
{
    _precision = precision;
    QCSUtils::formatString(_data, _format, _precision);
}

void TextFormatter::stateStringsChanged()
{
    setText(QCSUtils::formatString(_data, _format, _precision));
}

void TextFormatter::valueChanged()
{
    setText(QCSUtils::formatString(_data, _format, _precision));
}

void TextFormatter::sourceChanged()
{
    setText(QCSUtils::formatString(_data, _format, _precision));
}
