#include "Date.h"
#include <iostream>
using namespace std;
#include <cassert>

Date::Date(){
    j=0;
    m=0;
    a=0;
}

Date::Date(const unsigned int& jj,const unsigned int& mm,const unsigned int& aa){
    j=jj;
    m=mm;
    a=aa;
}

Date::~Date(){
}    

unsigned int Date::getjour() const {
    return j;
}

unsigned int Date::getmois() const {
    return m;
}

unsigned int Date::getanne() const {
    return a;
}

void Date::affichedate() const {
    std::cout<<j<<'/'<<m<<'/'<<a<<std::endl;
}


void Date::testRegression()const {
    Date d1;
    assert(d1.j==0 && d1.m==0 && d1.a==0);

    Date d2(1,2,3);
    assert(d2.j==1 && d2.m==2 && d2.a==3);
    assert(d2.getjour()==1);
    assert(d2.getmois()==2);
    assert(d2.getanne()==3);
    cout <<endl;
    cout<<"ok date"<<endl;

}
