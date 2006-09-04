#ifndef _Dialog_h_
#define _Dialog_h_

#include <QDialog>
#include <QGroupBox>
#include <QPushButton>
#include <QString>
#include <QLabel>
#include <string>

// Gotta hate forward declarations
/*class QGroupBox;
class QPushButton;*/

/**
 * Main Application Dialog
 */
class FlashDialog : public QDialog{
	Q_OBJECT

public:
	FlashDialog();
	virtual ~FlashDialog();
private:
	// Attributes
	//! Name of the executable file
	const std::string executable;

	//! Parameters
	std::string parameters;

	//! Number of elements in the vertical groupbox
	const static unsigned verticalOptions = 3;

	//! Button enumeration
	enum{ flashImage = 0, noFlashImage, resultImage };
	//! Group of buttons to choose the image files (2 input/1 output)
	QGroupBox* gridGroupBox;
	//! Buttons pointer array
	QPushButton *buttons[verticalOptions];
	//! Image boxes
	QLabel* imageBoxes[verticalOptions];
	//! Go button (starts the main program)
	QPushButton *	goButton,
	//! Cancel button (quits the program: GUI and core)
				*	cancelButton;
	//! Quit Action
	QAction* quitAction;

	//! Flash Image Path
	QString	flashImagePath,
	//! No Flash Image Path
			noFlashImagePath,
	//! Result Path
			resultImagePath;

private:
	// Methods
	void createLayout();

private slots:
	// Slots
	void openFlashImage();
	void openNoFlashImage();
	void saveResultImage();
	void launchFlashExec();
};


#endif // _Dialog_h_
