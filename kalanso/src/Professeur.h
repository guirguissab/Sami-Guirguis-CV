#ifndef _PROFESSEUR_H_
#define _PROFESSEUR_H_
#include "Matiere.h"
#include "Etudiant.h"
#include "Note.h"
#include "Absence.h"
#include "EDT.h"
#include "ClassEtu.h"
#include"Personne.h"

#include <string>
#include <vector>
#include <cassert>

class Database;
struct NoteDB;

class Professeur{
    private:
        Personne p;
        std::vector<Matiere> matieresEnseignees;
        std::vector<ClassEtu> classesEnseignees;
        EDT edt;
    public:
        Professeur();
        Professeur(const Personne pe);

        const Personne& getProfil() const;
        const std::vector<Matiere>& getMatieresEnseignees() const;
        const std::vector<ClassEtu>& getClassesEnseignees() const;
        const EDT& consulterEDT() const;
        
        void addMatiere(const Matiere& mat);
        void setEDT(const EDT& nEDT);
        void addClass(const ClassEtu& cla);

        bool addNoteEtu(const unsigned int& idCE,const unsigned int& idE,const Note& note);
        bool modifierNote(const unsigned int& idCE,const unsigned int& idE,const Note& note); 
        bool addAbsenceEtu(const unsigned int& idCE,const unsigned int& idE,const Absence& ab);
        void chargerDepuisDatabase(Database& bd);
        void saisirNote(Database& bd, unsigned int etudiantId, unsigned int matiereId, float valeur,
                    float coef, const std::string& appreciation) const;
        void modifierNote(Database& bd,unsigned int noteId, float nouvelleValeur) const;
        void enregistrerAbsence(Database& bd, unsigned int etudiantId, const std::string& date,
                            const std::string& heure, const std::string& motif,
                            bool justifie) const;
        std::vector<NoteDB> consulterNotes(Database& bd) const;

        void testRegression() const;

};



#endif