#include "ClassEtu.h"
#include "Etudiant.h"
#include"Matiere.h"
#include "EDT.h"
#include "Note.h"
#include "Database.h"

#include<iostream>
#include<string>
#include<vector>
#include <cassert>

using namespace std;

ClassEtu::ClassEtu(): idCE(0),numerDeClasse(0),niveau(""),voie(""),ListeMat(),edt(){}

ClassEtu::ClassEtu(const unsigned int id,const int numclass,const std::string niv,const std::string voi):idCE(id),
                    numerDeClasse(numclass),niveau(niv),voie(voi),ListeMat(),edt(){}


unsigned int ClassEtu::getId() const{
    return idCE;
}

int ClassEtu::getnumClass()const {
    return numerDeClasse;
}

string ClassEtu::getniveau() const {
    return niveau;
}

string ClassEtu::getvoie() const {
    return voie;
}

vector<Matiere> ClassEtu::getMatieres() const{
    return ListeMat;
}

EDT ClassEtu::getedt() const {
    return edt;
}


vector<Etudiant> ClassEtu::getListEtu() const {
    return ListEtu;
}

Etudiant* ClassEtu::getEtu(const unsigned int& IdE){
    for (vector<Etudiant>::iterator it = ListEtu.begin();it !=  ListEtu.end();++it){
        if (it->getId()==IdE){
            return &(*it);
        }
    }
    return nullptr;    
}


void ClassEtu::addMatiere(const Matiere& mat){
    ListeMat.push_back(mat);
}

void ClassEtu::addEtu(const Etudiant& Etu){
    ListEtu.push_back(Etu);
}

bool ClassEtu::addNoteEtu(const unsigned int& IdE,const Note& n){
    for (vector<Etudiant>::iterator it = ListEtu.begin();it !=  ListEtu.end();++it){
        if (it->getId()==IdE){
            it->addnote(n);
            return true;
        }
    }
    cout<<"etudiant non trouver return error"<<endl;
    return false;
}

bool ClassEtu::modifNoteEtu(const unsigned int& IdE,const Note& n){
    for (vector<Etudiant>::iterator it = ListEtu.begin();it !=  ListEtu.end();++it){
        if (it->getId()==IdE){
            if(it->modifiernote(n.getId(),n.getValeur())){
                cout<<"note modifier"<<endl;
                return true;
            } else{
                cout<<"note pas trouver retourne erreur"<<endl;
                return false;
            }
        }
    }
    cout<<"etudiant non trouver return error"<<endl;
    return false;
}

bool ClassEtu::addAbsenceEtu(const unsigned int& IdE,const Absence& ab){
    for (vector<Etudiant>::iterator it = ListEtu.begin();it !=  ListEtu.end();++it){
        if (it->getId()==IdE){
            it->addAbsence(ab);
            return true;
        }
    }
    cout<<"etudiant non trouver return error"<<endl;
    return false;
}


bool ClassEtu::rmEtu (const unsigned int IdE){
    for (unsigned int i = 0; i < ListEtu.size(); i++) {
        if (ListEtu[i].getId() == IdE) {
            ListEtu.erase(ListEtu.begin() + i);
            cout<<"etudiant enlever"<<endl;
            return true;
        }
    }
    cout<<"etudiant non trouver"<<endl;
    return false; 
}

void ClassEtu::setid(const unsigned int nId){
    idCE=nId;
}


void ClassEtu::setnumclass(const int numclass){
    numerDeClasse=numclass;
}

void ClassEtu::setniveau(const string niv){
    niveau=niv;
}


void ClassEtu::setvoie(const string voi){
    voie=voi;
}

void ClassEtu::addmat(const Matiere mat){
    ListeMat.push_back(mat);
}

void ClassEtu::setedt(const EDT& nedt){
    edt=nedt;
}

void ClassEtu::chargerDepuisDatabase(Database& bd) {
    ListeMat.clear();
    ListEtu.clear();
    edt = EDT();
    if (idCE == 0) {
        return;
    }

    vector<ClasseMatiereDB> matieresDb = bd.getMatieresClasse(int(idCE));
    for (int i = 0; i < int(matieresDb.size()); ++i) {
        ListeMat.push_back(Matiere(matieresDb[i].matiere_nom, matieresDb[i].matiere_nom, 1, matieresDb[i].matiere_id));
    }

    vector<PersonneDB> etudiantsDb = bd.getEtudiantsClasse(int(idCE));
    for (int i = 0; i < int(etudiantsDb.size()); ++i) {
        Personne profil(etudiantsDb[i].id, etudiantsDb[i].nom, etudiantsDb[i].prenom, "", Date());
        Etudiant etudiant(profil, this);
        etudiant.chargerDepuisDatabase(bd);
        ListEtu.push_back(etudiant);
    }
}


void ClassEtu::testRegression()const {
    ClassEtu classEtu(1, 101, "5eme", "C");
    assert(classEtu.getId() == 1);
    assert(classEtu.getnumClass() == 101);
    assert(classEtu.getniveau() == "5eme");
    assert(classEtu.getvoie() == "C");
    assert(classEtu.getMatieres().empty());
    assert(classEtu.getListEtu().empty());

    Matiere mat("Math", "MATH501", 5, 1);
    classEtu.addMatiere(mat);
    assert(classEtu.getMatieres().size() == 1);
    assert(classEtu.getMatieres()[0].getnom() == "Math");

    Date d(1, 1, 2000);
    Personne p(1, "Clinton", "Franklin", "San Andreas123", d);
    Etudiant etu(p, &classEtu);
    classEtu.addEtu(etu);
    assert(classEtu.getListEtu().size() == 1);
    assert(classEtu.getListEtu()[0].getId() == 1);

    EDT edt;
    classEtu.setedt(edt);
    assert(classEtu.getedt().getCours().empty());
    Heure h1(9, 0);
    Heure h2(10, 30);

    cour c("Lundi", "Salle 101", h1, h2, mat, 1);
    edt.ajouterCours(c);
    classEtu.setedt(edt);
    assert(classEtu.getedt().getCours().size() == 1);

    Note n(12,3,"Assez Bien",mat,1);
    assert(classEtu.addNoteEtu(1,n) == true);
    assert(classEtu.getListEtu()[0].getNote().size() == 1);
    assert(classEtu.getListEtu()[0].getNote()[0].getValeur() == 12);
    Note n2(20,3,"Excelent",mat,1);
    assert(classEtu.modifNoteEtu(1,n2) == true);
    assert(classEtu.getListEtu()[0].getNote()[0].getValeur() == 20);

    cout <<endl;
    std::cout<<"ok ClassEtu"<<std::endl;
}
