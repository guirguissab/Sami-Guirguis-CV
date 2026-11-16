#include "Matiere.h"
#include "Note.h"
#include <iostream>
#include<string>
#include <cassert>

using namespace std;

Note::Note(){
    Valeur = 0;
    Coef = 0;
    Appreciation="0";
}

Note::Note(float val, float cf, const std::string& Appr, const Matiere& m,const unsigned int IdN){
    Valeur=val;
    Coef = cf;
    Appreciation=Appr;
    idN=IdN;
}

Note::Note(const Note &n){
    Valeur = n.Valeur;
    Coef=n.Coef;
    Appreciation=n.Appreciation;
    mat=n.mat;
    idN=n.idN;
}


Note::~Note(){
    std::cout<<"Class deleted"<<endl;
}

unsigned int Note::getId()const{
    return idN;
}

float Note::getValeur() const{
    return Valeur;
}

float Note::getcoef() const{
    return Coef;
}

std::string Note::getAppr() const{
    return Appreciation;
}

void Note::setValeur(const float& val){
    Valeur=val;
}

void Note::setCoef(const float& nCoef){
    Coef=nCoef;
}

void Note::setApprec(const std::string& Appr){
    Appreciation=Appr;
}

void Note::setNote (const float val, const float cf,const std::string& Appr, const Matiere m,const unsigned int Id){
    Valeur=val;
    Coef = cf;
    Appreciation=Appr;
    mat=m;
    idN=Id;
}



void Note::testRegression() const {
    Matiere m("Physique", "Physique", 5, 1);
    Note n(16, 5, "Tres Bien", m, 1);

    assert(n.getId() == 1);
    assert(n.getValeur() == 16);
    assert(n.getcoef() == 5);
    assert(n.getAppr() == "Tres Bien");

    n.setNote(18.0, 4, "Tres Bien", m, 1);
    assert(n.getValeur() == 18.0);
    assert(n.getcoef() == 4);
    assert(n.getAppr() == "Tres Bien");
    std::cout <<std::endl;
    std::cout<<"ok Note."<<std::endl;
}
