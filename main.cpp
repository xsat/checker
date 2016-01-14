#include <QCoreApplication>
#include <QProcess>
#include <QString>
#include <QTextStream>
#include <QFile>
#include <QDateTime>

bool ping(const QString &address);

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    QString report;
    QTextStream out(stdout);

    out << "Start [" << endl;

    for (int i = 0; i < 2; i++) {
        QString address("10.246.4." + QString::number(i));

        if (ping(address)) {
            report += address + ";\n";
        }

        out << "    " << address << endl;
    }

    QFile file("reports/" + QDate().currentDate().toString(Qt::ISODate) + ".txt");

    if (file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        file.write(report.toUtf8());
        file.close();
    }

    out << "] End" << endl;

    return a.exec();
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
