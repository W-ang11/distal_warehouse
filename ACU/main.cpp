#include "tab_widget.h"
#include <QApplication>
#include "keyBoard.h"
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    keyBoard keyBoard;
    keyBoard.hide();

    Tab_Widget w;
    w.show();

    return a.exec();
}
