
#include <iostream>
//#include "tools.h"
#include <QtCore>
using namespace std;
int main()
{
    QString str;
    QString data = "forename**middlename**surname**phone";

    str = data.section(QString("sssS"), 0, 1); // str == "surname"
   // str.contains()
      qDebug()<<str;
    str = data.section(QString("**"), -3, -2); // str == "middlename**surname"
    qDebug()<<str;
   // prt(info,"sss");
    cout<<"sss"<<endl;
    return 0;
}
