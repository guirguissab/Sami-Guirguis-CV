#ifndef EDT_H
#define EDT_H
#include "cour.h"
#include <vector>
#include <cassert>

class EDT {
    private:
        std::vector<cour> ListeCour;
    public:
        EDT();
        ~EDT();
        std::vector<cour>& getCours();
        void ajouterCours(const cour& cours);
        bool supprimerCours(unsigned int nidC);
        bool modifierCours(cour& cours);
        int getSize();

        void testRegression() const;

};


#endif