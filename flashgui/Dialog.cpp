#include "Dialog.h"

#include <QtGui>
#include <QImage>

#include <iostream>
#include <sstream>
using std::endl;
using std::cerr;

/**
 * Constructor.
 * Initializes the go and cancel button, and the vertical group.
 */
FlashDialog::FlashDialog()
:
	// The name of the executable
#ifdef WIN32
	executable("flash"),
#else
	executable("./flash"),
#endif
	parameters("d")
{

	// Create image selection layout
	createLayout();

	// Button configuration
	goButton = new QPushButton(tr("Go!"));
	cancelButton = new QPushButton(tr("Cancel"));
	goButton->setDefault(true);
	// Connections
	connect(goButton, SIGNAL(clicked()), this, SLOT(launchFlashExec()));
	connect(cancelButton, SIGNAL(clicked()), this, SLOT(reject()));

	// Go and cancel button layout
	QHBoxLayout *buttonLayout = new QHBoxLayout;
	buttonLayout->addStretch(1);
	buttonLayout->addWidget(goButton);
	buttonLayout->addWidget(cancelButton);

	// And now for the main layout
	QVBoxLayout* mainLayout = new QVBoxLayout();
	mainLayout->addWidget(gridGroupBox);
	mainLayout->addLayout(buttonLayout);
    setLayout(mainLayout);
	setWindowTitle(tr("Flash Photography GUI"));
}

/**
 * Destructor
 */
FlashDialog::~FlashDialog(){

}

/**
 * Creates the vertical group of 3 buttons.
 */
//void FlashDialog::createVerticalBox(){


void FlashDialog::createLayout(){
	gridGroupBox = new QGroupBox(tr("Choose Files"));
	QGridLayout *gLayout = new QGridLayout();

	// Create buttons
	buttons[flashImage] = new QPushButton(tr("Flash Image"));
	buttons[noFlashImage] = new QPushButton(tr("No Flash Image"));
	buttons[resultImage] = new QPushButton(tr("Result Image"));


	// Add each item to the layout
	for(unsigned i = 0; i < verticalOptions; i++){
		// Row 0, Column i
		gLayout->addWidget(buttons[i],0,i);
	}

	// Create Images and arrange them
	for(unsigned int i = 0; i < verticalOptions; i++){
		imageBoxes[i] = new QLabel;
		imageBoxes[i]->setMinimumSize(120,120);
		imageBoxes[i]->setMaximumWidth(160);
		// Row 1, Column i
		gLayout->addWidget(imageBoxes[i],1,i);
	}

	gridGroupBox->setLayout(gLayout);

	// Connect 'em
	connect(buttons[flashImage], SIGNAL(clicked()), this, SLOT( openFlashImage() ));
	connect(buttons[noFlashImage], SIGNAL(clicked()), this, SLOT( openNoFlashImage() ));
	connect(buttons[resultImage], SIGNAL(clicked()), this, SLOT( saveResultImage() ));

}

/**
 * Opens the Flash Image dialog.
 */
void FlashDialog::openFlashImage(){
	flashImagePath = QFileDialog::getOpenFileName(this,
		tr("Select Flash Image"),
		"./",
		tr("All Images (*.png *.jpg *.jpeg *.bmp *.tiff);;All Files (*)"));
	// Open the image
	QImage img(flashImagePath);

	// If not ok...
	if(img.isNull()){
		// Warn the user
		QMessageBox::information(this, tr("Flash Image"),
			tr("Cannot load %1.").arg(flashImagePath));
		return;
	}
	// Else, resize it
	img.scaledToWidth(150);
	// And insert the image into the label
	imageBoxes[flashImage]->setPixmap(QPixmap::fromImage(img).scaledToWidth(160));
}

/**
 * Opens the No Flash Image dialog.
 */
void FlashDialog::openNoFlashImage(){
	noFlashImagePath = QFileDialog::getOpenFileName(this,
		tr("Select No Flash Image"),
		"./",
		tr("All Images (*.png *.jpg *.jpeg *.bmp *.tiff);;All Files (*)"));
		// Open the image
	QImage img(noFlashImagePath);

	// If not ok...
	if(img.isNull()){
		// Warn the user
		QMessageBox::information(this, tr("No Flash Image"),
			tr("Cannot load %1.").arg(noFlashImagePath));
		return;
	}
	// Else, resize it
	img.scaledToWidth(150);
	// And insert the image into the label
	imageBoxes[noFlashImage]->setPixmap(QPixmap::fromImage(img).scaledToWidth(160));
}

/**
 * Shows a dialog to choose the file to save
 * the reconstructed image
 */
void FlashDialog::saveResultImage(){
	resultImagePath = QFileDialog::getSaveFileName(this,
		tr("Select file to save the Result Image"),
		"./",
		tr("All Images (*.png *.jpg *.jpeg *.bmp *.tiff);;All Files (*)"));


}

/**
 * Launches the flash executable and waits
 * until it finished.
 * ToDo: signal when the process is completed.
 * ToDo: Show any kind of progressbar and/or a cancel option.
 */
void FlashDialog::launchFlashExec(){
	// Check if the user has already choosen the file to save to
	if(resultImagePath.isEmpty()){
		resultImagePath="result.bmp";
	}

	// Construct command & parameters
	std::stringstream ss;
	ss << executable;
	ss << " ";
	ss << parameters;
	// We must add "" to the path, so C:\A Directory will be
	// "C:\A Directory" and will be recognized as one single parameter
	ss << " \"";
	ss << flashImagePath.toStdString();
	ss << "\" \"";
	ss << noFlashImagePath.toStdString();
	ss << "\" \"";
	ss << resultImagePath.toStdString();
	ss << '\"';

	cerr << "Parameters: " << ss.str() << endl;

	// Launch
	std::system(ss.str().c_str());

	// Show resulting image
		// Open the image
	QImage img(resultImagePath);

	// If not ok...
	if(img.isNull()){
		// Warn the user
		QMessageBox::information(this, tr("Result Image"),
			tr("Cannot load %1.").arg(resultImagePath));
		return;
	}
	// Else, resize it
	img.scaledToWidth(150);
	// And insert the image into the label
	imageBoxes[resultImage]->setPixmap(QPixmap::fromImage(img).scaledToWidth(160));
}
