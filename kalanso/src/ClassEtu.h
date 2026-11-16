#ifndef CLASSETU_H
#define CLASSETU_H

#include"Matiere.h"
#include "EDT.h"
#include "Note.h"
#include"Absence.h"

#include<iostream>
#include<string>
#include<vector>
#include <cassert>

class Etudiant;
class Database;

class ClassEtu{
    private:
        unsigned int idCE;
        int numerDeClasse;
        std::string niveau;
        std::string voie;
        std::vector<Matiere> ListeMat;
        EDT edt;
        std::vector<Etudiant> ListEtu;

    public:
        ClassEtu();
        ClassEtu(const unsigned int id,const int numclass,const std::string niv,const std::string voi);
        unsigned int getId() const ;
        int getnumClass()const;
        std::string getniveau()const;
        std::string getvoie()const;
        std::vector<Matiere> getMatieres()const;
        EDT getedt()const;
        std::vector<Etudiant> getListEtu()const;
        Etudiant* getEtu(const unsigned int& IdE);

        void addEtu(const Etudiant& Etu);
        void addMatiere(const Matiere& mat);
        bool addNoteEtu(const unsigned int& IdE,const Note& n);
        bool modifNoteEtu(const unsigned int& IdE,const Note& n);
        bool addAbsenceEtu(const unsigned int& IdE,const Absence& ab);


        bool rmEtu(const unsigned int IdE);
        void setid(const unsigned int nId);
        void setnumclass(const int numclass);
        void setniveau(const std::string niv);
        void setvoie(const std::string voi);
        void addmat(const Matiere mat);
        void setedt(const EDT& nedt);
        void chargerDepuisDatabase(Database& bd); //j'ai pas compris comment ca fonction dans le code, pardon

        void testRegression()const;
};



#endif
