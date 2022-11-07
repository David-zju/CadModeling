/* 图像转换 */
#include "matrix4d.h"

matrix4d::matrix4d()
{

}

matrix4d::matrix4d(QVector3D axis, double angle)
{
    axis.normalize();
    double n1=axis.x();
    double n2=axis.y();
    double n3=axis.z();
    double a[4][4]={
      n1*n1+(1-n1*n1)*cos(angle), n1*n2*(1-cos(angle))+n3*sin(angle),n1*n3*(1-cos(angle))-n2*sin(angle),0,
       n1*n2*(1-cos(angle))-n3*sin(angle),  n2*n2+(1-n2*n2)*cos(angle), n3*n2*(1-cos(angle))+n1*sin(angle),0,
         n1*n3*(1-cos(angle))+n2*sin(angle), n2*n3*(1-cos(angle))-n1*sin(angle),  n3*n3+(1-n3*n3)*cos(angle), 0,
        0,0,0,1
    };
    for(int i=0;i<4;i++)
        for(int j=0;j<4;j++)
            value[i][j]=a[i][j];
}

QVector3D matrix4d::dotProduct(QVector3D &T)
{
    double a[4]={T.x(),T.y(),T.z(),1};
    double b[4]={0,0,0,0};
    for(int i=0;i<4;i++){
        for(int j=0;j<4;j++){
            b[i]+=value[i][j]*a[j];
        }
    }
    return QVector3D(b[0],b[1],b[2]);
}
