#include "Personne.h"
#include <cassert>
#include <iostream>

Personne::Personne():idP(0), nom(""),prenom(""),mdp(""),date_Naiss() {}

Personne::Personne(const unsigned int Id,const std::string& nm, const std::string& pr, const std::string& mp, const Date& d){
    idP=Id;
    nom=nm;
    prenom=pr;
    mdp=mp;
    date_Naiss=d;
}

Personne::~Personne() {}

bool Personne::verifierMDP(const std::string MDP) const{
    return (mdp==MDP);
}

std::string Personne::getNom() const{
    return nom;
}

std::string Personne::getPrenom() const{
    return prenom;
}


void Personne::modifierMDP(const std::string& nMDP){
    mdp=nMDP;
}


unsigned int Personne::getId() const {
    return idP;
}

void Personne::testRegression() const {
    Date d(1, 1, 2000);
    Personne p(1, "Crest", "Lester", "password123", d);

    assert(p.getId() == 1);
    assert(p.getNom() == "Crest");
    assert(p.getPrenom() == "Lester");
    assert(p.verifierMDP("password123") == true);
    assert(p.verifierMDP("non?") == false);

    p.modifierMDP("pass321");
    assert(p.verifierMDP("pass321") == true);
    std::cout <<std::endl;
    std::cout<<"ok Personne"<<std::endl;
}
