#ifndef _Note_h_
#define _Note_h_
#include <iostream>
#include "Matiere.h"
#include<string>
#include <cassert>
class Note{
    private:
        unsigned int idN;
        float Valeur,Coef;
        std::string Appreciation;
        Matiere mat;
    public:
        Note();

        Note(float val, float cf,const std::string& Appr,const Matiere& m,const unsigned int IdN);

        Note(const Note &n);

        ~Note();

        float getValeur() const;
        float getcoef() const;
        unsigned int getId()const;
        std::string getAppr() const;

        void setValeur(const float& val);

        void setCoef(const float& nCoef);


        void setApprec(const std::string& Appr);


        void setNote (const float val, const float cf,const std::string& Appr, const Matiere m,const unsigned int Id);


        void testRegression() const;
};

#endif