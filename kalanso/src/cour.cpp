#include "cour.h"
#include <cassert>
#include <iostream>

cour::cour():idC(0),jour("0") , salle("0"),debut(), fin(), mat(){}

cour::cour(const std::string& j, const std::string& s, const Heure& d, const Heure& f, const Matiere& m ,const unsigned int Id)
    : idC(Id),jour(j), salle(s), debut(d), fin(f), mat(m) {
}

unsigned int cour::getId(){
    return idC;
}

cour::~cour(){
}

std::string cour::getJour() const{
    return jour; 
}

std::string cour::getSalle() const{ 
    return salle; 
}

Heure cour::getdebut() const{
	return debut;
}

Heure cour::getfin() const{
	return fin;
}

void cour::testRegression() const {
    cour c1;
    assert (c1.getId()==0 && c1.getJour()=="0" && c1.getSalle()=="0" && c1.getdebut().afficheHeure()=="0:0");
    std::cout <<std::endl;
    std::cout<<"Ok Cour"<< std::endl;

}
