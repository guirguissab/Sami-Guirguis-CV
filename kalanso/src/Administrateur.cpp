#include "Administrateur.h"
#include "EDT.h"
#include "ClassEtu.h"
#include "Etudiant.h"
#include "Matiere.h"
#include "Professeur.h"
#include "Personne.h"
#include "Actualite.h"
#include "ClassEtu.h"
#include"Date.h"

#include "Database.h"

#include <string>
#include <vector>
#include <iostream>
#include <cassert>
using namespace std;

Administrateur::Administrateur(): p(){}
Administrateur::Administrateur(const Personne& profil): p(profil){}

Personne Administrateur::getProfil() const {
    return p;
}

Etudiant Administrateur::creerCompteEtudiant(const unsigned int id, const string& nom, const string& prenom,
                                 const string& motDePasse,const unsigned int& j,const unsigned int& m,const unsigned int& a) const{
                                    Date d(j,m,a);
                                    Personne  pr(id,nom,prenom,motDePasse,d);
                                    return Etudiant(pr,nullptr);
                                 }

Professeur Administrateur::creerCompteProfesseur(const unsigned int id, const string& nom, const string& prenom,
                                 const string& motDePasse,const unsigned int& j,const unsigned int& m,const unsigned int& a) const{
                                    Date d(j,m,a);
                                    Personne  pr(id,nom,prenom,motDePasse,d);
                                    return Professeur(pr);
                                 }


ClassEtu Administrateur::creerClass(const unsigned int id,const int numclass,const string niv,const string voi)const{
    return ClassEtu(id,numclass,niv,voi);
}

Administrateur Administrateur::creerCompteAdmin (unsigned int id, const string& nom, const string& prenom,
                                 const string& motDePasse,const unsigned int& j,const unsigned int& m,const unsigned int& a)const{
                                    Date d(j,m,a);
                                    Personne pr(id,nom,prenom,motDePasse,d);
                                    return Administrateur(pr);
                                }



void Administrateur::inscrireClassMatiere(ClassEtu& classetu, const Matiere& matiere) const{
    classetu.addMatiere(matiere);
}

void Administrateur::inscrireEtudiantClass(ClassEtu& classetu, Etudiant& etudiant)const{
    classetu.addEtu(etudiant);
    etudiant.setClass(&classetu);
}

void Administrateur::affecterProfesseurMatiere(Professeur& professeur, const Matiere& matiere)const{
    professeur.addMatiere(matiere);
}

void Administrateur::attribuerEDTProfesseut(Professeur& prof, const EDT& edt)const{
    prof.setEDT(edt);
}

void Administrateur::attribuerEDTClass(ClassEtu& classetu, const EDT& edt)const{
    classetu.setedt(edt);
}

void Administrateur::attribuerClassProfesseut(Professeur& prof, const ClassEtu& cla)const{
    prof.addClass(cla);
}

void Administrateur::ajouterCours(EDT& edt, const cour& c)const{
    edt.ajouterCours(c);
}

bool Administrateur::modifierCours(EDT& edt, cour& cours) const{
    return edt.modifierCours(cours);
}

bool Administrateur::supprimerCours(EDT& edt, unsigned int cour) const{
    return edt.supprimerCours(cour);
}

bool Administrateur::suprrimerEtudiantClass(ClassEtu& cla, const unsigned int IdE){
    return cla.rmEtu(IdE);
}

vector<PersonneDB> Administrateur::getTousUtilisateurs(Database& bd) const {
    return bd.getTousUtilisateurs();
}

vector<NoteDB> Administrateur::getToutesNotes(Database& bd) const {
    return bd.getAllNotes();
}

void Administrateur::modifierNote(Database& bd, int idNote, float nouvelleValeur) const {
    bd.modifierNote(idNote, nouvelleValeur);
}

void Administrateur::supprimerNote(Database& bd, int idNote) const {
    bd.supprimerNote(idNote);
}

void Administrateur::supprimerCompte(Database& bd, int idUtilisateur) const {
    bd.supprimerCompte(idUtilisateur);
}

int Administrateur::creerCompte(Database& bd, const string& nom, const string& prenom,
                                const string& motDePasse, int roleChoisi) const {
    if (roleChoisi == 1) {
        return bd.creerEtudiant(nom, prenom, motDePasse);
    }
    if (roleChoisi == 2) {
        return bd.creerProfesseur(nom, prenom, motDePasse);
    }
    if (roleChoisi == 3) {
        return bd.creerAdmin(nom, prenom, motDePasse);
    }
    return -1;
}

int Administrateur::creerMatiere(Database& bd, const string& nom, const string& apogee, float coef) const {
    return bd.ajouterMatiere(nom, apogee, coef);
}

int Administrateur::creerClasse(Database& bd, const string& nom, const string& niveau, const string& voie) const {
    return bd.ajouterClasse(nom, niveau, voie);
}

bool Administrateur::affecterMatiereClasse(Database& bd, int classeId, int matiereId, int profId, bool obligatoire) const {
    return bd.affecterMatiereClasse(classeId, matiereId, profId, obligatoire);
}

void Administrateur::assignerProfMatiere(Database& bd, int profId, int matiereId) const {
    bd.assignerProfMatiere(profId, matiereId);
}

void Administrateur::inscrireEtudiantMatiere(Database& bd, int etudiantId, int matiereId) const {
    bd.inscrireEtudiantMatiere(etudiantId, matiereId);
}

bool Administrateur::assignerEtudiantClasse(Database& bd, int etudiantId, int classeId) const {
    return bd.assignerEtudiantClasse(etudiantId, classeId);
}

bool Administrateur::assignerProfClasse(Database& bd, int profId, int classeId) const {
    vector<ClasseMatiereDB> matieresClasse = bd.getMatieresClasse(classeId);
    if (matieresClasse.empty()) {
        return false;
    }
    for (int i = 0; i < int(matieresClasse.size()); ++i) {
        bd.assignerProfMatiere(profId, matieresClasse[i].matiere_id);
        bd.affecterMatiereClasse(classeId, matieresClasse[i].matiere_id, profId, matieresClasse[i].obligatoire);
    }
    return true;
}

bool Administrateur::attribuerEDTProfesseur(Database& bd, int profId, int classeId, int matiereId,
                                            const string& jour, const string& heureDebut,
                                            const string& heureFin, const string& salle) const {
    return bd.ajouterCours(jour, heureDebut, heureFin, salle, classeId, matiereId, profId) > 0;
}

bool Administrateur::attribuerEDTEtudiant(Database& bd, int etudiantId, int classeId) const {
    return bd.assignerEtudiantClasse(etudiantId, classeId);
}

int Administrateur::ajouterCours(Database& bd, const string& jour, const string& heureDebut,
                                 const string& heureFin, const string& salle,
                                 int classeId, int matiereId, int profId) const {
    return bd.ajouterCours(jour, heureDebut, heureFin, salle, classeId, matiereId, profId);
}

void Administrateur::modifierCours(Database& bd, int coursId, const string& jour,
                                   const string& heureDebut, const string& heureFin,
                                   const string& salle) const {
    bd.modifierCours(coursId, jour, heureDebut, heureFin, salle);
}

void Administrateur::supprimerCours(Database& bd, int coursId) const {
    bd.supprimerCours(coursId);
}

int Administrateur::publierActualite(Database& bd, const string& titre, const string& contenu,
                                     const string& datePublication) const {
    return bd.publierActualite(titre, contenu, datePublication);
}

void Administrateur::supprimerActualite(Database& bd, int actualiteId) const {
    bd.supprimerActualite(actualiteId);
}

void Administrateur::synchroniserBD(Database& bd) const {
    bd.getTousUtilisateurs();
    bd.getAllNotes();
    bd.getToutesLesClasses();
    bd.getToutesLesmatieres();
    bd.getToutEDT();
    bd.getToutesActualites();
}

void Administrateur::testRegression(Database& bd) const {
    Date d(1, 1, 1988);
    Personne p(1, "davis", "lamar", "password123", d);
    Administrateur admin(p);
    assert(admin.getProfil().getId() == 1);
    assert(admin.getProfil().getNom() == "davis");
    assert(admin.getProfil().getPrenom() == "lamar");
    assert(admin.getProfil().verifierMDP("password123") == true);

    ClassEtu classEtu = admin.creerClass(1, 101, "5eme", "C");
    assert(classEtu.getId() == 1);
    assert(classEtu.getnumClass() == 101);
    assert(classEtu.getniveau() == "5eme");
    assert(classEtu.getvoie() == "C");
    Etudiant etu = admin.creerCompteEtudiant(2, "De Santa", "Micheal", "Los Santos123", 1, 1, 2005);
    assert(etu.getId() == 2);
    assert(etu.getProfil().getNom() == "De Santa");
    assert(etu.getProfil().getPrenom() == "Micheal");
    assert(etu.getProfil().verifierMDP("Los Santos123") == true);
    admin.inscrireEtudiantClass(classEtu, etu);
    assert(classEtu.getListEtu().size() == 1);
    assert(classEtu.getListEtu()[0].getId() == 2);
    assert(etu.getClass().getId() == 1);

    Etudiant etu2 = admin.creerCompteEtudiant(2, "Masry", "Araby","Egypt2005", 2, 2, 2005);
    admin.inscrireEtudiantClass(classEtu, etu2);
    assert(classEtu.getListEtu().size() == 2);
    assert(admin.suprrimerEtudiantClass(classEtu, etu2.getId()) == true);

    synchroniserBD(bd);

    cout <<endl;

    cout<<"ok admin"<<endl;
}

