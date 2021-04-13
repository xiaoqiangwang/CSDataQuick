#include <QtTest>

#include "csdata.h"
#include "utils.h"
#include "enums.h"

class TestUtils : public QObject
{
    Q_OBJECT

private:
    QCSData *data;
    QCSUtils *utils;

private slots:
    void initTestCase() {
        data = new QCSData();
        utils = new QCSUtils();
    }
    void cleanupTestCase() {
        delete data;
        delete utils;
    }

    void testConvert_data() {
        QTest::addColumn<QString>("source");
        QTest::addColumn<TextFormat::TextFormatEnum>("format");
        QTest::addColumn<int>("precision");
        QTest::addColumn<QString>("output");

        QTest::newRow("loc://double") << "loc://double.{\"type\":\"double\",\"value\":3.14}" << TextFormat::String << 5 << "3.14000";
        QTest::newRow("loc://string") << "loc://string.{\"type\":\"string\",\"value\":\"message\"}" << TextFormat::String << 0 << "message";
        QTest::newRow("loc://enum") << "loc://enum.{\"type\":\"enum\",\"enums\":[\"Off\",\"On\"],\"value\":1}" << TextFormat::String << 0 << "On";
    }

    void testConvert() {
        QFETCH(QString, source);
        QFETCH(TextFormat::TextFormatEnum, format);
        QFETCH(int, precision);
        QFETCH(QString, output);

        data->setSource(source);

        QCOMPARE(utils->formatString(data, format, precision), output);
    }

    void testProcess() {
        QCOMPARE(utils->execute("sleep 3"), true);
        QCOMPARE(utils->execute("nonexsting_command whatsever"), false);
    }
};

QTEST_MAIN(TestUtils)
#include "test_utils.moc"
