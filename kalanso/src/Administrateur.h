#ifndef ADMINISTRATEUR_H
#define ADMINISTRATEUR_H
#include "EDT.h"
#include "cour.h"
#include "Etudiant.h"
#include "Matiere.h"
#include "Professeur.h"
#include "Personne.h"
#include "Actualite.h"
#include "ClassEtu.h"
#include "Database.h"
#include"Date.h"

#include <string>
#include <vector>
#include <cassert>

class Administrateur{
    private:
        Personne p;
    public:
        Administrateur();
        Administrateur(const Personne& profil);

        Personne getProfil()const;

        Etudiant creerCompteEtudiant(const unsigned int id, const std::string& nom, const std::string& prenom,
                                 const std::string& motDePasse,const unsigned int& j,const unsigned int& m,const unsigned int& a) const;
        Professeur creerCompteProfesseur(const unsigned int id, const std::string& nom, const std::string& prenom,
                                 const std::string& motDePasse,const unsigned int& j,const unsigned int& m,const unsigned int& a) const;
        Administrateur creerCompteAdmin(unsigned int id, const std::string& nom, const std::string& prenom,
                                 const std::string& motDePasse,const unsigned int& j,const unsigned int& m,const unsigned int& a) const ;
        Matiere creerMatiere(unsigned int id, const std::string& nom, const std::string& apogee, float coef) const;

        ClassEtu creerClass(const unsigned int id,const int numclass,const std::string niv,const std::string voi)const;

        void inscrireClassMatiere(ClassEtu& classetu, const Matiere& matiere) const;
        void inscrireEtudiantClass(ClassEtu& classetu, Etudiant& etudiant) const;
        void affecterProfesseurMatiere(Professeur& professeur, const Matiere& matiere)const;

        void attribuerEDTClass(ClassEtu& classetu, const EDT& edt)const;
        void attribuerEDTProfesseut(Professeur& prof, const EDT& edt) const;
        void attribuerClassProfesseut(Professeur& prof, const ClassEtu& cla) const;


        void ajouterCours(EDT& edt, const cour& c) const;
        bool modifierCours(EDT& edt, cour& cours) const;
        bool supprimerCours(EDT& edt, unsigned int cour) const;
        bool suprrimerEtudiantClass(ClassEtu& cla,const unsigned int IdE);
        std::vector<PersonneDB> getTousUtilisateurs(Database& bd) const;
        std::vector<NoteDB> getToutesNotes(Database& bd) const;
        void modifierNote(Database& bd, int idNote, float nouvelleValeur) const;
        void supprimerNote(Database& bd, int idNote) const;
        void supprimerCompte(Database& bd, int idUtilisateur) const;
        int creerCompte(Database& bd, const std::string& nom, const std::string& prenom,
                        const std::string& motDePasse, int roleChoisi) const;
        int creerMatiere(Database& bd, const std::string& nom, const std::string& apogee, float coef) const;
        int creerClasse(Database& bd, const std::string& nom, const std::string& niveau, const std::string& voie) const;
        bool affecterMatiereClasse(Database& bd, int classeId, int matiereId, int profId, bool obligatoire) const;
        void assignerProfMatiere(Database& bd, int profId, int matiereId) const;
        void inscrireEtudiantMatiere(Database& bd, int etudiantId, int matiereId) const;
        bool assignerEtudiantClasse(Database& bd, int etudiantId, int classeId) const;
        bool assignerProfClasse(Database& bd, int profId, int classeId) const;
        bool attribuerEDTProfesseur(Database& bd, int profId, int classeId, int matiereId,
                                    const std::string& jour, const std::string& heureDebut,
                                    const std::string& heureFin, const std::string& salle) const;
        bool attribuerEDTEtudiant(Database& bd, int etudiantId, int classeId) const;
        int ajouterCours(Database& bd, const std::string& jour, const std::string& heureDebut,
                         const std::string& heureFin, const std::string& salle,
                         int classeId, int matiereId, int profId) const;
        void modifierCours(Database& bd, int coursId, const std::string& jour,
                           const std::string& heureDebut, const std::string& heureFin,
                           const std::string& salle) const;
        void supprimerCours(Database& bd, int coursId) const;
        int publierActualite(Database& bd, const std::string& titre, const std::string& contenu,
                             const std::string& datePublication) const;
        void supprimerActualite(Database& bd, int actualiteId) const;
        void synchroniserBD(Database& bd) const;

        void testRegression(Database& bd) const;
};




#endif