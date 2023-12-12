#include "FPHOTO.h"
#include "Global.h"

#include <QtWidgets/QApplication>

Global imageData;

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	FPHOTO w;
	w.show();
	return a.exec();
}