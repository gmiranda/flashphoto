#include <QtGui>
#include <QWidget>
#include <QApplication>
#include <QPushButton>
#include <QFont>
//! Translates strings
#include <QTranslator>
#include <iostream>
#include "Dialog.h"


int main( int argc, char **argv ){
	QApplication app( argc, argv );

	// The main Window
	/*QWidget window;
	window.resize(200, 120);

	// Create the Quit button
	QPushButton quit(QPushButton::tr("Cancel"), &window );
	quit.resize( 100, 30 );
	QObject::connect(&quit, SIGNAL(clicked()), &app, SLOT(quit()));

	QString locale = QLocale::system().name();

	QTranslator translator;
	//translator.load(QString("dialog_") + locale);
	translator.load("dialog_es_ES");
	std::cout << "Locale: " << locale.toStdString();
	app.installTranslator(&translator);

	// Show
	window.show();

	return app.exec();*/
	FlashDialog dialog;
	QString locale = QLocale::system().name();

	QTranslator translator;
	//translator.load(QString("dialog_") + locale);
	translator.load("dialog_es");
	std::cout << "Locale: " << locale.toStdString();
	app.installTranslator(&translator);
	dialog.show();

	return app.exec();
}
