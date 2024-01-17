#include "Label.hh"

Label::Label(QWidget *parent) : QWidget(parent) {

}

void Label::setPixmap(const QPixmap &pixmap)
{
    mPixmap = pixmap;
    update();
}

void Label::paintEvent(QPaintEvent *event)
{
    if (mPixmap.isNull()) {
        return;
    }
    /*double width = this->width();
    double height = this->height();
    double pixmapWidth = mPixmap.width();
    double pixmapHeight = mPixmap.height();
    //double scale = qMax(width / pixmapWidth, height / pixmapHeight);
    double scale = qMax(pixmapWidth, pixmapHeight);
    QTransform transform;
    transform.translate(width / 2, height / 2);
    transform.scale(scale, scale);
    transform.translate(-pixmapWidth / 2, -pixmapHeight / 2);
*/

    double pixmapWidth = mPixmap.width();
    double pixmapHeight = mPixmap.height();
    double scale = qMax(pixmapWidth, pixmapHeight);
    QTransform transform;
    transform.scale(scale, scale);
    QPainter painter(this);
    //painter.setTransform(transform);
    painter.drawPixmap(QPoint(0,0), mPixmap);

}
