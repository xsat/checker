#include <QString>
#include <QCoreApplication>
#include <QProcess>
#include <QFile>
#include <QDateTime>
#include <QTimer>

void createReport(QString &report);
bool ping(const QString &address);
void writeReport(const QString &report);

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    QString report("");
    createReport(report);
    writeReport(report);
    QTimer::singleShot(0, &a, SLOT(quit()));

    return a.exec();
}

void createReport(QString &report)
{
    for (int i = 0; i <= 255; i++) {
        QString address("10.246.4." + QString::number(i));

        if (!ping(address)) {
            report += address + ";\n";
        } else {
            report += "\n";
        }
    }
}

bool ping(const QString &address)
{
    QProcess ping;
    ping.start("ping", QStringList() << address);
    QString line("");

    if (ping.waitForFinished() ) {
        while(ping.canReadLine()) {
            line += QString::fromLocal8Bit(ping.readLine());
        }
    }

    return line.contains("TTL");
}

void writeReport(const QString &report)
{
    QFile file(QDate().currentDate().toString(Qt::ISODate) + ".txt");

    if (file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        file.write(report.toUtf8());
        file.close();
    }
}
