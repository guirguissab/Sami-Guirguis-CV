#include "EDT.h"
#include<iostream>
#include<vector>
#include <cassert>

using namespace std;

EDT::EDT(): ListeCour(){}

EDT::~EDT(){
    cout<<"EDT enlever";
}

void EDT::ajouterCours(const cour& cour){
    ListeCour.push_back(cour);

}

vector<cour>& EDT::getCours(){
    return ListeCour;
}


bool EDT::supprimerCours(unsigned int nidC){
    for (vector<cour>::iterator it = ListeCour.begin();it != ListeCour.end();++it){
        if(it->getId()==nidC){
            ListeCour.erase(it);
            cout<<"cours suprrimer"<<endl;
            return true;
        }

    }
    cout<<"cour non trouver return erreur"<<endl;
    return false;
}

bool EDT::modifierCours(cour& c){
    for (vector<cour>::iterator it = ListeCour.begin();it != ListeCour.end();++it){
        if(it->getId()==c.getId()){
            *it=c;
            cout<<"cour changer"<<endl;
            return true;
        }

    }
    cout<<"cour non trouver return erreur"<<endl;
    return false;
}
int EDT::getSize(){
    return ListeCour.size();
}


void EDT::testRegression() const {
    EDT edt;
    assert(edt.getCours().empty());

    Matiere m("Science De la Vie et de la Terre", "SVT201", 5, 1);
    Heure d(9, 0);
    Heure f(10, 30);
    cour c("Lundi", "Salle 101", d, f, m, 1);

    edt.ajouterCours(c);
    assert(edt.getCours().size() == 1);
    assert(edt.getCours()[0].getId() == 1);

    c = cour("Mardi", "Salle 102", d, f, m, 1); 
    assert(edt.modifierCours(c) == true);
    assert(edt.getCours()[0].getJour() == "Mardi");

    assert(edt.supprimerCours(1) == true);
    assert(edt.getCours().empty());
    cout <<endl;
    cout<<"Ok EDT"<<endl;
}


