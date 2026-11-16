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


#include "WinTXT.h"
#include "txtApp.h"
#include "contexte.h"

int main()
{
    Context ctx;

    termClear();
    txtBoucle(ctx);
    Context ctx2;

    termClear();
    txtBoucle(ctx2);
    termClear();

    termClear();

    return 0;
}
/*0 g++ src/*.cpp -o mon_programme
g++ src/*.cpp -o mon_programme -lsqlite3
./mon_programme 0*/
