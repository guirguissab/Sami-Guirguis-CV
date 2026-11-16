#include "Matiere.h"
#include <cassert>
#include <iostream>

Matiere::Matiere() {
    nom = "";
    apogee = "";
    coef = 0;
    idM=0;
}

Matiere::Matiere(const std::string& n, const std::string& a, const int c,const unsigned int Id) {
    nom = n;
    apogee = a;
    coef = c;
    idM=Id;
}

Matiere::~Matiere(){
}

std::string Matiere::getnom() const{
    return nom;
}

unsigned int Matiere::getId(){
    return idM;
}

std::string Matiere::getapogee() const{
    return apogee;
}
void Matiere::setnometapogee(const std::string& n, const std::string& a){
    nom = n;
    apogee = a;
}
void Matiere::setcoef(int c){
    coef = c;
}

void Matiere::testRegression() const {
    Matiere m1;
    assert(m1.nom == "" && m1.apogee == "" && m1.coef == 0);

    Matiere m2("Math", "MATH101", 5, 1);
    assert(m2.getId() == 1);
    assert(m2.getnom() == "Math");
    assert(m2.getapogee() == "MATH101");

    m2.setnometapogee("Anglais", "ANG303");
    m2.setcoef(4);
    assert(m2.getnom() == "Anglais");
    assert(m2.getapogee() == "ANG303");
    std::cout <<std::endl;
    std::cout<<"Ok matiere"<<std::endl;
}
