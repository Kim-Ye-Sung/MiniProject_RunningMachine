// #include "mainwindow.h"

// #include <QApplication>
// #include <QLocale>
// #include <QTranslator>

// int main(int argc, char *argv[])
// {
//    QApplication a(argc, argv);

//    QTranslator translator;
//    const QStringList uiLanguages = QLocale::system().uiLanguages();
//    for (const QString &locale : uiLanguages) {
//        const QString baseName = "MyFirstQtApp_" + QLocale(locale).name();
//        if (translator.load(":/i18n/" + baseName)) {
//            a.installTranslator(&translator);
//            break;
//        }
//    }
//    MainWindow w;
//    w.show();
//    return QCoreApplication::exec();
// }


// #include <QApplication>
// #include <QWidget>
// #include <QVBoxLayout>
// #include <QLabel>
// #include <QPushButton>

// int main(int argc, char *argv[])
// {
//     QApplication app(argc, argv);

//     QWidget window;
//     window.setWindowTitle("My First");

//     QLabel *label = new QLabel("Hello, Qt!");
//     QPushButton *button = new QPushButton("Click me");

//     QVBoxLayout *layout = new QVBoxLayout;
//     layout->addWidget(label);
//     layout->addWidget(button);

//     window.setLayout(layout);

//     QObject::connect(button, &QPushButton::clicked, [label]() {
//         label->setText("Button clicked?!@!#");
//     });

//     window.show();

//     return app.exec();
// }

// #include <QApplication>
// #include <QWidget>
// #include <QVBoxLayout>
// #include <QLabel>
// #include <QPushButton>

// int main(int argc, char *argv[])
// {
//     QApplication app(argc, argv);

//     QWidget window;
//     window.setWindowTitle("Counter Example");

//     QLabel *label = new QLabel("0");
//     QPushButton *plusButton = new QPushButton("Plus");
//     QPushButton *minusButton = new QPushButton("Minus");

//     QVBoxLayout *layout = new QVBoxLayout;
//     layout->addWidget(label);
//     layout->addWidget(plusButton);
//     layout->addWidget(minusButton);

//     window.setLayout(layout);

//     int *value = new int(0);

//     QObject::connect(plusButton, &QPushButton::clicked, [label, value]() {
//         (*value)++;
//         label->setText(QString::number(*value));
//     });

//     QObject::connect(minusButton, &QPushButton::clicked, [label, value]() {
//         (*value)--;
//         label->setText(QString::number(*value));
//     });

//     window.show();

//     return app.exec();
// }

#include "mainwindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    MainWindow w;
    w.show();

    return a.exec();
}