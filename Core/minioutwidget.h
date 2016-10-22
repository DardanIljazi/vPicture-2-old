#ifndef MINIOUTWIDGET_H
#define MINIOUTWIDGET_H

#include <QWidget>
#include <QDragEnterEvent>
#include <QMimeData>

namespace Ui {
class MiniOutWidget;
}

class MiniOutWidget : public QWidget
{
    Q_OBJECT

public:
    explicit MiniOutWidget(QWidget *parent = 0);
    ~MiniOutWidget();

signals:
    void logoPressedEvent();
    void screenshotEvent();
    void picturesEvent();
    void dragEvent(QDragEnterEvent *event);

protected:
    /*
     * Functions called when user Drag and Drop a file
     */
    void dragEnterEvent(QDragEnterEvent *event);

private:
    Ui::MiniOutWidget *ui;
};

#endif // MINIOUTWIDGET_H
