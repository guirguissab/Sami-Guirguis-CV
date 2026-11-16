#include"Note.h"
#include"cour.h"
#include"Heure.h"
#include"EDT.h"
#include"ClassEtu.h"
#include"Etudiant.h"
#include"Professeur.h"
#include"Absence.h"
#include "Actualite.h"
#include "Administrateur.h"
#include "Database.h"

#include <iostream>
#include <cassert>

using namespace std;

int main()
{
    Database bd;
    bd.init();
    cout<<endl;
    Etudiant e;
    
    cout <<endl;

    e.testRegression();

    Professeur p;
    cout <<endl;
    p.testRegression();
    Personne pe;
    cout <<endl;
    pe.testRegression();
    Date d;
    cout <<endl;
    d.testRegression();
    Note n;
    cout <<endl;
    n.testRegression();
    Absence ab;
    cout <<endl;
    ab.testRegression();
    EDT edt;
    cout <<endl;
    edt.testRegression();
    ClassEtu classetu;
    cout <<endl;
    classetu.testRegression();
    Administrateur admin;
    cout <<endl;
    admin.testRegression(bd);


    cout <<endl;
    cout <<endl;
    /*
    g++ $(ls src/*.cpp | grep -v main_txt.cpp) -o build/testregression -lsqlite3
    */

    return 0;
}
