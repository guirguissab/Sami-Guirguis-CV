#include "Actualite.h"
#include "Date.h"

#include <string>
#include <cassert>
#include <iostream>


Actualite::Actualite():idA(0),titre(""),contenu(""),datePublication(){}
Actualite::Actualite(const unsigned int& id,const std::string& tit,const std::string& cont,const Date& datepub):
            idA(id),titre(tit),contenu(cont),datePublication(datepub){}

unsigned int  Actualite::getId()const{
    return idA;
}

std::string Actualite::getTitre()const{
    return titre;
}

std::string Actualite::getContenu()const{
    return contenu;
}

Date Actualite::getDate()const{
    return datePublication;
}

void Actualite::setTitre(const std::string& tit){
    titre=tit;
}

void Actualite::setDate(const Date& datepub){
    datePublication=datepub;
}

void Actualite::testRegression() const {
    Actualite a1;
    assert(a1.getId() == 0);
    assert(a1.getTitre() == "");
    assert(a1.getContenu() == "");
    assert(a1.getDate().getjour() == 0);
    assert(a1.getDate().getmois() == 0);
    assert(a1.getDate().getanne() == 0);

    Date d(1, 2, 2026);
    Actualite a2(1, "Titre 1", "Contenu 1", d);
    assert(a2.getId() == 1);
    assert(a2.getTitre() == "Titre 1");
    assert(a2.getContenu() == "Contenu 1");
    assert(a2.getDate().getjour() == 1);
    assert(a2.getDate().getmois() == 2);
    assert(a2.getDate().getanne() == 2026);
    std::cout <<std::endl;
    std::cout<<"ok actualite"<< std::endl;
}
