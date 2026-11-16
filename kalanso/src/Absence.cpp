#include "Absence.h"

#include "Date.h"
#include "Heure.h"

#include <string>
#include<iostream>
#include <cassert>

using namespace std;

Absence::Absence(): idAb(0),Dateabsc(), heureAbsc(),motif("non justifier"),justif(false) {}

Absence::~Absence(){
    cout<<"abscence enleve"<<endl;
}

const Date& Absence::getDate(){
    return Dateabsc;
}

Heure& Absence::getHeure(){
    return heureAbsc;
}

const string& Absence::getMotif(){
    return motif;
}

const bool& Absence::getJustif(){
    return justif;
}

const unsigned int& Absence::getId(){
    return idAb;
}

void Absence::setDate(const Date d){
    Dateabsc=d;
}

void Absence::setHeure(const Heure he){
    heureAbsc=he;
}

void Absence::setMotif(const string mot){
    motif=mot;
}

void Absence::setJustif(const bool j){
    justif=j;
}

void Absence::setId(const unsigned int nIda){
    idAb=nIda;
}

void Absence::setAbsc(const unsigned int nIda ,const Date d,const Heure he,const std::string mot,const bool j){
    setDate(d);
    setHeure(he);
    setMotif(mot);
    setJustif(j);
    setId(nIda);
}

void Absence::testRegression() const {
    Absence a1;
    assert(a1.getId() == 0);
    assert(a1.getDate().getjour() == 0);
    assert(a1.getDate().getmois() == 0);
    assert(a1.getDate().getanne() == 0);
    assert(a1.getHeure().afficheHeure() == "0:0");
    assert(a1.getMotif() == "non justifier");
    assert(a1.getJustif() == false);

    Date d(1, 2, 2026);
    Heure h(9, 30);
    Absence a2;
    a2.setAbsc(1, d, h, "maladie", true);

    assert(a2.getId() == 1);
    assert(a2.getDate().getjour() == 1);
    assert(a2.getDate().getmois() == 2);
    assert(a2.getDate().getanne() == 2026);
    assert(a2.getHeure().afficheHeure() == "9:30");
    assert(a2.getMotif() == "maladie");
    assert(a2.getJustif() == true);
    cout <<endl;
    cout<<"Ok abscence"<<endl;
}
