#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <QMetaType>

#include "ContactModel.h"
#include "QueueWorker.h"

int main(int argc, char *argv[]) {
    QGuiApplication app(argc, argv);

    // Register Contact type for queued connections
    qRegisterMetaType<Contact>("Contact");

    ContactModel model;
    QueueWorker worker(&model);
    worker.start();

    QQmlApplicationEngine engine;
    engine.rootContext()->setContextProperty("contactModel", &model);
    engine.rootContext()->setContextProperty("queueWorker", &worker);

    engine.load(QUrl("qrc:/main.qml"));

    return app.exec();
}
