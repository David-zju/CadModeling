/* 图形图像设计 */
#pragma once
#include <QVector3D>

class matrix4d
{
public:
    matrix4d();
    matrix4d(QVector3D axis,double angle);
    QVector3D dotProduct(QVector3D &T);
    double value[4][4];
};

