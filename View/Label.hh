#pragma once

#include <QLabel>
#include <QPainter>


class Label : public QWidget
{
    Q_OBJECT

public:
    explicit Label(QWidget *parent = nullptr);
    void setPixmap(const QPixmap& image);

protected:
    void paintEvent(QPaintEvent *event);
    QPixmap mPixmap;
};

