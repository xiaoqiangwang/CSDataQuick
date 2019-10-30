#include <QtTest>

#include "csdata.h"

class TestQCSData : public QObject
{
    Q_OBJECT

private:
    QCSData *data;

private slots:
    void initTestCase() {
        data = new QCSData();
    }
    void cleanupTestCase() {
        delete data;
    }

    void testDataEngine_data() {
        QTest::addColumn<QString>("source");
        QTest::addColumn<bool>("connected");
        QTest::addColumn<QCSData::FieldType>("fieldType");
        QTest::addColumn<unsigned long long>("count");
        QTest::addColumn<QVariant>("value");

        QTest::newRow("sim://sin") << "sim://sin" << true << QCSData::Double << 1ULL << QVariant();
        QTest::newRow("sim://wave") << "sim://wave" << true << QCSData::Double << 20ULL << QVariant();
        QTest::newRow("ca://invalid") << "whatever_invalid" << false << QCSData::Invalid << 0ULL << QVariant();
        QTest::newRow("loc://double") << "loc://double.{\"type\":\"double\",\"value\":3.14}" << true << QCSData::Double << 1ULL << QVariant(3.14);
        QTest::newRow("loc://integer") << "loc://integer.{\"type\":\"int\",\"value\":12}" << true << QCSData::Integer << 1ULL << QVariant(12);
        QTest::newRow("loc://string") << "loc://string.{\"type\":\"string\",\"value\":\"message\"}" << true << QCSData::String << 1ULL << QVariant("message");
    }

    void testDataEngine() {
        QFETCH(QString, source);
        QFETCH(bool, connected);
        QFETCH(QCSData::FieldType, fieldType);
        QFETCH(unsigned long long, count);
        QFETCH(QVariant, value);

        data->setSource(source);

        QCOMPARE(data->connected(), connected);
        QCOMPARE(data->fieldType(), fieldType);
        QCOMPARE(data->count(), count);
        if (value.isValid())
            QCOMPARE(data->value(), value);
    }
};

QTEST_MAIN(TestQCSData)
#include "test_qcsdata.moc"
