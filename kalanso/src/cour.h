#ifndef COUR_H
#define COUR_H
#include "Heure.h"
#include "Matiere.h"
#include <string>
#include <cassert>

class cour{
    private:
        unsigned int idC;
        std::string jour;
        std::string salle;
        Heure debut;
        Heure fin;

    public:
        Matiere mat;
        cour();
        cour(const std::string& j, const std::string& s, const Heure& d, const Heure& f, const Matiere& m,const unsigned int Id);
        ~cour();
        std::string getJour() const;
        std::string getSalle() const;
        unsigned int getId();
        Heure getdebut() const;
        Heure getfin() const;

        void testRegression()const;
};

#endif