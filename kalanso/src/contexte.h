#ifndef CONTEXT_H
#define CONTEXT_H

#include "Database.h"
#include "Personne.h"
#include "Etudiant.h"
#include "Professeur.h"
#include "Administrateur.h"

struct Context
{
    Database bd;
    PersonneDB currentUser;
    Etudiant etudiantActif;
    Professeur professeurActif;
    Administrateur adminActif;

    bool logged = false;
    bool running = true;

    Context()
    {
        bd.init();
        adminActif.synchroniserBD(bd);
        currentUser.id = 0;
    }

    void chargerUtilisateurActif()
    {
        Date d(1, 1, 2000);
        Personne profil(currentUser.id, currentUser.nom, currentUser.prenom, "", d);
        if (currentUser.role == "etudiant")
        {
            etudiantActif = Etudiant(profil, nullptr);
            etudiantActif.chargerDepuisDatabase(bd);
        }
        else if (currentUser.role == "professeur")
        {
            professeurActif = Professeur(profil);
            professeurActif.chargerDepuisDatabase(bd);
        }
        else if (currentUser.role == "admin")
        {
            adminActif = Administrateur(profil);
        }
    }
};

#endif