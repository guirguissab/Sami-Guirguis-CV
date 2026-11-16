#include"Professeur.h"
#include "Matiere.h"
#include "Absence.h"
#include "Etudiant.h"
#include "Note.h"
#include "EDT.h"
#include "ClassEtu.h"
#include"Personne.h"
#include "Database.h"

#include <iostream>

#include <string>
#include <vector>
#include <cassert>
#include <cstdio>
#include <set>
#include <map>

using namespace std;

Professeur::Professeur():p(),matieresEnseignees(),classesEnseignees(),edt(){}

Professeur::Professeur(const Personne pe):p(pe),matieresEnseignees(),classesEnseignees(),edt(){}

const Personne& Professeur::getProfil()const{
    return p;
}

const vector<Matiere>& Professeur::getMatieresEnseignees() const {
    return matieresEnseignees;
}

const vector<ClassEtu>& Professeur::getClassesEnseignees() const{
    return classesEnseignees;
}

const EDT& Professeur::consulterEDT() const{
    return edt;
}

void Professeur::addMatiere(const Matiere& mat){
    matieresEnseignees.push_back(mat);
}


void Professeur::setEDT(const EDT& nEDT){
    edt=nEDT;
}


void Professeur::addClass(const ClassEtu& cla){
    classesEnseignees.push_back(cla);
}


bool Professeur::addNoteEtu(const unsigned int& idCE,const unsigned int& idE,const Note& note){
    for (vector<ClassEtu>::iterator it=classesEnseignees.begin();it !=classesEnseignees.end();++it){
        if (it->getId()==idCE){
            return it->addNoteEtu(idE,note);
        }
    }
    cout<<"classe non trouver retourn erreur"<<endl;
    return false;
}

bool Professeur::modifierNote(const unsigned int& idCE,const unsigned int& idE,const Note& note){
    for (vector<ClassEtu>::iterator it=classesEnseignees.begin();it !=classesEnseignees.end();++it){
        if (it->getId()==idCE){
            return it->modifNoteEtu(idE,note);
        }
    }
    cout<<"classe non trouver retourn erreur"<<endl;
    return false;
}

bool Professeur::addAbsenceEtu(const unsigned int& idCE,const unsigned int& idE,const Absence& ab){
    for (vector<ClassEtu>::iterator it=classesEnseignees.begin();it !=classesEnseignees.end();++it){
        if (it->getId()==idCE){
            return it->addAbsenceEtu(idE,ab);
        }
    }
    cout<<"classe non trouver retourn erreur"<<endl;
    return false;
}

void Professeur::chargerDepuisDatabase(Database& bd) {
    matieresEnseignees.clear();
    classesEnseignees.clear();
    edt = EDT();
    if (p.getId() == 0) {
        return;
    }

    vector<MatiereDB> matieresDb = bd.getMatieresProfesseur(int(p.getId()));
    for (int i = 0; i < int(matieresDb.size()); ++i) {
        matieresEnseignees.push_back(
            Matiere(matieresDb[i].nom, matieresDb[i].apogee, int(matieresDb[i].coef), matieresDb[i].id));
    }

    vector<CoursDB> coursDb = bd.getEDTProfesseur(int(p.getId()));
    map<int, string> classesParId;
    for (int i = 0; i < int(coursDb.size()); ++i) {
        if (coursDb[i].classe_id > 0 && classesParId.find(coursDb[i].classe_id) == classesParId.end()) {
            classesParId[coursDb[i].classe_id] = coursDb[i].classe_nom;
        }
        Matiere mat(coursDb[i].matiere_nom, coursDb[i].matiere_nom, 1, coursDb[i].matiere_id);
        unsigned int hDeb = 0, mDeb = 0, hFin = 0, mFin = 0;
        sscanf(coursDb[i].heure_debut.c_str(), "%u:%u", &hDeb, &mDeb);
        sscanf(coursDb[i].heure_fin.c_str(), "%u:%u", &hFin, &mFin);
        edt.ajouterCours(cour(coursDb[i].jour, coursDb[i].salle, Heure(hDeb, mDeb), Heure(hFin, mFin), mat, coursDb[i].id));
    }

    for (map<int, string>::const_iterator it = classesParId.begin(); it != classesParId.end(); ++it) {
        ClassEtu classe(unsigned(it->first), it->first, "", "");
        classe.chargerDepuisDatabase(bd);
        classesEnseignees.push_back(classe);
    }
}

void Professeur::saisirNote(Database& bd, unsigned int etudiantId, unsigned int matiereId, float valeur,
                            float coef, const string& appreciation) const {
    bd.ajouterNote(int(etudiantId), int(matiereId), valeur, coef, appreciation);
}

void Professeur::modifierNote(Database& bd, unsigned int noteId, float nouvelleValeur) const {
    bd.modifierNote(int(noteId), nouvelleValeur);
}

void Professeur::enregistrerAbsence(Database& bd, unsigned int etudiantId, const string& date,
                                    const string& heure, const string& motif, bool justifie) const {
    bd.ajouterAbsence(int(etudiantId), date, heure, motif, justifie);
}

vector<NoteDB> Professeur::consulterNotes(Database& bd) const {
    vector<NoteDB> visibles;
    vector<MatiereDB> mesMatieres = bd.getMatieresProfesseur(int(p.getId()));
    if (mesMatieres.empty()) {
        return visibles;
    }

    set<int> idsMatieres;
    for (int i = 0; i < int(mesMatieres.size()); ++i) {
        idsMatieres.insert(mesMatieres[i].id);
    }

    vector<NoteDB> toutes = bd.getAllNotes();
    for (int i = 0; i < int(toutes.size()); ++i) {
        if (idsMatieres.count(toutes[i].matiere_id) > 0) {
            visibles.push_back(toutes[i]);
        }
    }
    return visibles;
}

void Professeur::testRegression() const {

    Personne pe(1, "Philips", "Trevor", "pass123", Date(1, 1, 1980));
    Professeur prof(pe);
    assert(prof.getProfil().getId() == 1);
    assert(prof.getProfil().getNom() == "Philips");
    assert(prof.getProfil().getPrenom() == "Trevor");
    assert(prof.getProfil().verifierMDP("pass123") == true);
    assert(prof.getMatieresEnseignees().empty());
    assert(prof.getClassesEnseignees().empty());

    Matiere mat("Chimie", "Chimie100", 5, 1);
    prof.addMatiere(mat);
    assert(prof.getMatieresEnseignees().size() == 1);
    assert(prof.getMatieresEnseignees()[0].getnom() == "Chimie");

    ClassEtu classEtu(1, 101, "1ere", "A");
    prof.addClass(classEtu);
    assert(prof.getClassesEnseignees().size() == 1);
    assert(prof.getClassesEnseignees()[0].getId() == 1);
    std::cout <<std::endl;
    std::cout<<"ok Prof"<<std::endl;
}
