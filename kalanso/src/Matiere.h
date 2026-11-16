//string nom, apogee, int coef
//constructeur, destructeur, string getnom(), getapogee(), 
//void set nometapogee(string nom, apogee), void set coef(int coef)
#ifndef MATIERE_H
#define MATIERE_H

#include <string>
#include <iostream>
#include <cassert>

class Matiere {
    private:
        unsigned int idM;
        std::string nom;
        std::string apogee;
        int coef;

    public:
        Matiere();
        Matiere(const std::string& n, const std::string& a, const int c , const unsigned int Id );
        ~Matiere();
        unsigned int getId();
        std::string getnom() const;
        std::string getapogee() const;
        void setnometapogee(const std::string& n, const std::string& a);
        void setcoef(int c);

        void testRegression() const;

};

#endif