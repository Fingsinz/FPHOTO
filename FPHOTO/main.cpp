#include <QtWidgets/QApplication>

#include "FPHOTO.h"
#include "ImgInstance.h"

int main(int argc, char *argv[]) {
    QApplication a(argc, argv);
    FPHOTO w;
    w.show();
    return a.exec();
}