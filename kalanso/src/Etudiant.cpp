#include "Etudiant.h"
#include "ClassEtu.h"
#include "EDT.h"
#include "Personne.h"
#include "Note.h"
#include "Absence.h"
#include "Matiere.h"
#include "Database.h"
#include <iostream>
#include <vector>
#include<string>
#include <sstream>

#include <cassert>

using namespace std;

namespace {
Date parseDateISO(const string& value) {
    unsigned int year = 0;
    unsigned int month = 0;
    unsigned int day = 0;
    char sep1 = '\0';
    char sep2 = '\0';
    istringstream stream(value);
    if (!(stream >> year >> sep1 >> month >> sep2 >> day) || sep1 != '-' || sep2 != '-') {
        return Date();
    }
    return Date(day, month, year);
}

Heure parseHeureHHMM(const string& value) {
    unsigned int hour = 0;
    unsigned int minute = 0;
    char sep = '\0';
    istringstream stream(value);
    if (!(stream >> hour >> sep >> minute) || sep != ':') {
        return Heure();
    }
    return Heure(hour, minute);
}
} // namespace

Etudiant::Etudiant(): p(), Notes(), ab(),Classactu(nullptr){}


Etudiant::Etudiant(const Personne pe,ClassEtu* Cl):p(pe), Notes(), ab(),Classactu(Cl){}

Personne& Etudiant::getProfil(){
    return p;
}

unsigned int Etudiant::getId()const{
    return p.getId();
}

EDT Etudiant::getEDT()const {
    return Classactu->getedt();
}


vector<Note> Etudiant::getNote() const{
    return Notes;
}

vector<Matiere> Etudiant::getMatieres() const{
    return Classactu->getMatieres();
}

vector<Absence> Etudiant::getAbsence() const{
    return ab;
}

ClassEtu Etudiant::getClass()const{
    return *Classactu;

}

string Etudiant::getniveau()const{
    return Classactu->getniveau();

}
string Etudiant::getvoie()const{
    return Classactu->getvoie();
}


int Etudiant::getIdclass()const{
    return Classactu->getId();
}

string Etudiant::getApprNote(const unsigned int& idN) const{
    for (vector<Note>::const_iterator it =Notes.begin();it != Notes.end();++it){
        if (it->getId()==idN){
            return it->getAppr();
        }
    }
    return "Note Non Trouver";
}

void Etudiant::addnote(const Note& n){
    Notes.push_back(n);
}

void Etudiant::addAbsence(const Absence& absence){
    ab.push_back(absence);
}

bool Etudiant::modifiernote(const unsigned int nIdn,const float nvValeur){
    for (vector<Note>::iterator it =Notes.begin();it != Notes.end();++it){
        if (it->getId()==nIdn){
            it->setValeur(nvValeur);
            return true;
        }
    }

    cout<<"Note Non Trouver"<<endl;
    return false;
}


void Etudiant::changerMDP(const string& nouvellemdp){
    p.modifierMDP(nouvellemdp);
}

void Etudiant::setClass(ClassEtu* classetu){
    Classactu=classetu;
}

void Etudiant::chargerDepuisDatabase(Database& bd) {
    Notes.clear();
    ab.clear();
    if (getId() == 0) {
        return;
    }

    vector<NoteDB> notesDb = bd.getNotes(int(getId()));
    for (int i = 0; i < int(notesDb.size()); ++i) {
        Matiere mat(notesDb[i].matiere, notesDb[i].matiere, int(notesDb[i].coef), notesDb[i].matiere_id);
        Notes.push_back(Note(notesDb[i].valeur, notesDb[i].coef, notesDb[i].appreciation, mat, notesDb[i].id));
    }

    vector<AbsenceDB> absencesDb = bd.getAbsences(int(getId()));
    for (int i = 0; i < int(absencesDb.size()); ++i) {
        Absence absence;
        absence.setAbsc(
            (unsigned int)(absencesDb[i].id),
            parseDateISO(absencesDb[i].date),
            parseHeureHHMM(absencesDb[i].heure),
            absencesDb[i].motif,
            absencesDb[i].justifie);
        ab.push_back(absence);
    }
}

float Etudiant::calculerMoyenne(Database& bd, int matiereId) const {
    if (getId() == 0) {
        return -1.0f;
    }
    return bd.getMoyenne(int(getId()), matiereId);
}


void Etudiant::testRegression() const {
    Date d(1, 1, 2000);
    Personne p(1, "Belic", "Roman", "password123", d);
    ClassEtu classEtu(1, 101, "2nd", "D");
    Etudiant etu(p, &classEtu);
    assert(etu.getId() == 1);
    assert(etu.getProfil().getNom() == "Belic");
    assert(etu.getProfil().getPrenom() == "Roman");
    assert(etu.getProfil().verifierMDP("password123") == true);
    assert(etu.getClass().getId() == 1);
    assert(etu.getniveau() == "2nd");
    assert(etu.getvoie() == "D");
    cout <<endl;
    std::cout<<"ok etudiant"<<std::endl;
}
