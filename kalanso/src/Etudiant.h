#ifndef ETUDIANT_H
#define ETUDIANT_H

#include "EDT.h"
#include "Personne.h"
#include "Note.h"
#include "Absence.h"
#include "Matiere.h"
#include <iostream>
#include <vector>

#include <cassert>

class ClassEtu;
class Database;

class Etudiant{
    private:
        Personne p;
        std::vector<Note> Notes;
        std::vector<Absence> ab;
        ClassEtu* Classactu;
    public:
        Etudiant();
        Etudiant(const Personne pe,ClassEtu* Cl);
        Personne& getProfil();
        unsigned int getId() const;
        EDT getEDT()const;
        std::vector<Note> getNote() const;
        std::vector<Absence> getAbsence() const;
        std::vector<Matiere> getMatieres()const;
        ClassEtu getClass()const;
        std::string getniveau()const;
        std::string getvoie()const;
        int getIdclass()const;

        std::string getApprNote(const unsigned int& idN) const ;
        void addnote(const Note& n);
        bool modifiernote(const unsigned int nIdn,const float nvValeur);
        void addAbsence(const Absence& absence);
        void setClass(ClassEtu* classetu);
        void changerMDP(const std::string& nouvellemdp);
        void chargerDepuisDatabase(Database& bd); // pareille
        float calculerMoyenne(Database& bd, int matiereId) const; //pareille

        void testRegression() const;

};



#endif