#include "configurationdialog.h"
#include "puzzlepiece.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    ConfigurationDialog w;
    w.show();


    /*QGraphicsView view;
    QGraphicsScene scene;
    scene.addItem(new PuzzlePiece(PuzzlePiece::IN, PuzzlePiece::IN, PuzzlePiece::IN, PuzzlePiece::IN));
    view.setScene(&scene);
    view.show();*/

    //QSize x = QSize(5,5);
    //QPixmap image = QPixmap("/Users/pavlov/Desktop/Фото 21.11.2019, 22.12.jpg");
    //QGraphicsView view;
    //logic scene;
    //scene.setup(x,image);
    //view.setScene(&scene);
    //view.show();

    return a.exec();
}
