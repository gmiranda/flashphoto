#include "Dialog.h"

#include <QtGui>
#include <QImage>
#include <QCoreApplication>

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
	executable("flash")
#elif __MACH__
	// Solve bundle path
	executable((QCoreApplication::applicationDirPath() + "/flash").toStdString())
#else
	// Unix systems used to have a different call in previous version of this app
	executable("flash")
#endif
{

	// Create image selection layout
	createImageLayout();

	// Create info & options layout
	createOptionLayout();
	QGridLayout* optionAndInfoLayout = new QGridLayout;
	optionAndInfoLayout->addWidget(optionBox,0,0,-1,1);
	//optionAndInfoLayout->setStretchFactor(optionBox);
	infoLabel = new QLabel("Info");
	infoLabel->setWordWrap(true);
	infoLabel->setText(tr("Instructions:")+"\n"+tr("You need two exact photographs, first select the ")
		+tr("Flash Image")+" "+tr("to choose the photo that was registered using the flash of the camera.")
		+"\n"+tr("Then, go for the photo without flash, select the ")+tr("No Flash Image")+tr(" button.")+"\n"
		+tr("DO NOT forget to add the format (i.e.: result.JPEG)")+"\n"
		+tr("Finally, just choose where do you want to save the ")+tr("Result Image")+tr(", press ")
		+tr("Go!")+tr(" button and wait.")+"\n"
		+tr("Note that it will take a while, and big images (1280x960) may take several hours."));

	optionAndInfoLayout->addWidget(infoLabel,0,1,-1,4);

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
	mainLayout->addLayout(optionAndInfoLayout);
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
 * Creates the vertical group of 3 buttons,
 * with their correspondent image labels.
 */
void FlashDialog::createImageLayout(){
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
 * Creates the layout that contains options like which
 * algoritm (cross bilateral, plain bilateral) shall be used
 */
void FlashDialog::createOptionLayout(){
	optionBox = new QGroupBox(tr("Options"));
	QVBoxLayout *optionLayout = new QVBoxLayout;
	optionLayout->addStretch(1);

	// The default option
	plainBilateral = new QRadioButton(tr("Default bilateral filter"));
	plainBilateral->setChecked(true);
	// Alternative option
	crossBilateral = new QRadioButton(tr("Cross bilateral")+"\n"+ tr("(better for dark images)"));

	optionLayout->addWidget(plainBilateral);
	optionLayout->addWidget(crossBilateral);

	optionBox->setLayout(optionLayout);
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
	img.scaledToWidth(160);
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
	img.scaledToWidth(160);
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
		tr("Portable Network Graphics (*.png);;JPEG (*.jpeg);;BMP (*.bmp);;True Image File Format (*.tiff);;All Files (*)"));

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
	ss << parameters();
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
	//QMessageBox::information(this, "Launching", ss.str().c_str());
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
	img.scaledToWidth(160);
	// And insert the image into the label
	imageBoxes[resultImage]->setPixmap(QPixmap::fromImage(img).scaledToWidth(160));
}

/**
 * @return A string with the parameters selected for the
 * filtering, such as the bilateral filter to use.
 */
std::string FlashDialog::parameters()const{

	// If plain bilateral is selected
	if(plainBilateral->isChecked())
		// Bilateral filter, but also do detail correction
		return "d";
	// If cross bilateral is selected
	if(crossBilateral->isChecked())
		// Just compute cross bilateral
		return "c";
	// Normal mode, but the control shouldn't arrive here...
	return "n";
}
