#include <QGuiApplication>
#include <QQmlApplicationEngine>

int main(int argc, char *argv[])
{

	//Creates a Gui application https://doc.qt.io/qt-6/qguiapplication.html#details
	QGuiApplication app(argc, argv);

	//Create an engine to load the QML main file https://doc.qt.io/qt-6/qqmlapplicationengine.html#details
	QQmlApplicationEngine engine;

	// Add `/esterVtech.com/imports` to the QML Import Path https://doc.qt.io/qt-6/qtqml-syntax-imports.html
	// So our custom QML module(and the app window.qml) is available to the engine
	engine.addImportPath("qrc:/esterVtech.com/imports");

	const QUrl url(u"qrc:/esterVtech.com/imports/App/qml/window.qml"_qs);

	// Load the main QML file
	engine.load(url);

	return app.exec();
}
