#ifndef _PERSONNE_h_
#define _PERSONNE_h_

#include "Date.h"
#include <iostream>
#include <cassert>


class Personne {
    private:
        unsigned int idP;
        std::string nom, prenom,mdp;
        Date date_Naiss;
    public:
        Personne();

        Personne(const unsigned int ID,const std::string& nm, const std::string& pr, const std::string& mp, const Date& d);

        ~Personne();

        std::string getNom() const;

        std::string getPrenom() const;

        bool verifierMDP (const std::string MDP) const;

        unsigned int getId() const;

        void modifierMDP(const std::string& nMDP);

        void testRegression() const;

};
#endif