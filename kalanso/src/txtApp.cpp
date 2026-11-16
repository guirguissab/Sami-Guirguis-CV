#include "txtApp.h"
#include <vector>
#include <string>

enum Etat
{
    CONNEXION,
    MENUADMIN,
    MENUPROF,
    MENUETU,
    NOTES,
    MODIFIERNOTES,
    AJOUTNOTE,
    AJOUTABSENCE,
    INFOS,
    LISTE,
    COMPTESUPP,
    COMPTECREE,
    CLASSECREE,
    ASSIGNATION,
};

void txtBoucle(Context &ctx)
{
    WinTXT win(60, 20);
    Etat etat = CONNEXION;

    std::string login = "";
    std::string mdp = "";
    std::string input = "";
    int editMode = 0;
    int scroll_liste = 0;
    int scroll_note = 0;
    int i=3;
    std::string nom, prenom;
    int etape = 0;
    int choix_role = 0;
    int addStep = 0;
    std::string f1, f2, f3, f4, f5;
    bool passMode = false;
    bool loginError = false;

    bool redraw = true;

    while (ctx.running)
    {
        char c = win.getCh();

        //saisie
        if (c != 0)
        {
            redraw = true;

            // pour quand on veut quitter
            if (c == 'q')
            {
                ctx.running = false;
                break;
            }

    
            // CONNEXION
    
            if (etat == CONNEXION)
            {
                if (c == 10 || c == 13)
                {
                    if (!passMode)
                    {
                        passMode = true;
                    }
                    else
                    {
                    ctx.currentUser = ctx.bd.authentifier(login, mdp);

                    if (ctx.currentUser.id > 0)
                    {
                        ctx.logged = true;
                        ctx.chargerUtilisateurActif();

                        if (ctx.currentUser.role == "admin")
                            etat = MENUADMIN;
                        else if (ctx.currentUser.role =="professeur")
                            etat = MENUPROF;
                        else
                            etat = MENUETU;

                        login = "";
                        mdp = "";
                        passMode = false;

                        loginError = false;
                        i=3;
                    }
                    else
                    {
                        loginError = true;
                        login="";
                        mdp = "";
                        passMode = false;    // reste sur identifiant
                        i--;
                        if(i<=0)
                        ctx.running = false;
                    }
                }
            }
                else if (c == 127)
                {
                    if (!passMode && !login.empty()) //empty() vient de <string>, pour savoir si vide
                        login.pop_back(); // pareil, pour supprimer le dernier caractère de la chaîne
                    else if (passMode && !mdp.empty())
                        mdp.pop_back();
                }
                else
                {
                    if (!passMode)
                        login += c;
                    else
                        mdp += c;
                }
            }

    
            // MENUADMIN
    
            else if (etat == MENUADMIN)
            {
                if (c == '1')
                    etat = NOTES;
                if (c =='2')
                    etat = LISTE;
                if (c == '3') {
                    etat = ASSIGNATION;
                    addStep = 0;
                    f1 = f2 = f3 = f4 = f5 = "";
                }
                if (c == 'd')
                {
                    ctx.logged = false;
                    etat = CONNEXION;
                }
            }

    
            // MENUPROF
    
            else if (etat == MENUPROF)
            {
                if (c == '1')
                    etat = NOTES;
                if (c == '2')
                    etat = AJOUTNOTE;
                if (c == '3')
                    etat = AJOUTABSENCE;
                if (c == '4')
                    etat = INFOS;
                if (c == 'd')
                {
                    ctx.logged = false;
                    etat = CONNEXION;
                }
            }

    
            // MENUETU
    
            else if (etat == MENUETU)
            {
                if (c == '1')
                    etat = NOTES;
                if (c == '2')
                    etat = INFOS;
                if (c == 'd')
                {
                    ctx.logged = false;
                    etat = CONNEXION;
                }
            }

    
            // NOTES
    
            else if (etat == NOTES)
            {
                if (c == 'd') scroll_note++;
                if (c == 'm' && scroll_note > 0) scroll_note--;
                if (c == 'r'&& ctx.currentUser.role == "admin")
                   etat = MENUADMIN;
                    else if (c=='r' && ctx.currentUser.role =="professeur")
                        etat = MENUPROF;
                    else if (c=='r')
                        etat=MENUETU; 

                if (c == '2'&& ctx.currentUser.role == "admin")
                    etat = MODIFIERNOTES;
            }
    
            // MODIFIER NOTES
    
            else if (etat == MODIFIERNOTES)
            {
                if (c == 'r')
                    etat = NOTES;
                else if (c == 'm')
                {
                    editMode = 109;
                    input = "";
                }
                else if (c == 's')
                {
                    editMode = 115;
                    input = "";
                }
                else if (c == 10 || c == 13) // ENTER
                {
                    if (editMode == 109)
                    {
                        int id;
                        float val;
                        sscanf(input.c_str(), "%d %f", &id, &val);
                        ctx.adminActif.modifierNote(ctx.bd, id, val);
                    }
                    else if (editMode == 115)
                    {
                        int id = std::stoi(input);
                        ctx.adminActif.supprimerNote(ctx.bd, id);
                    }

                    input = "";
                    editMode = 0;
                    etat = NOTES;
                }
                else if (c == 127) // backspace
                {
                    if (!input.empty())
                        input.pop_back();
                }
                else if (editMode != 0)
                {
                    input += c;
                }
            }

            else if (etat == AJOUTNOTE)
            {
                if (c == 'r') {
                    etat = MENUPROF;
                    addStep = 0; f1 = f2 = f3 = f4 = f5 = "";
                } else if (c == 10 || c == 13) {
                    addStep++;
                    if (addStep > 4) {
                        ctx.professeurActif.saisirNote(ctx.bd, (unsigned int)(std::stoi(f1)),
                                                       (unsigned int)(std::stoi(f2)),
                                                       std::stof(f3), std::stof(f4), f5);
                        etat = MENUPROF;
                        addStep = 0; f1 = f2 = f3 = f4 = f5 = "";
                    }
                } else if (c == 127) {
                    if (addStep == 0 && !f1.empty()) f1.pop_back();
                    else if (addStep == 1 && !f2.empty()) f2.pop_back();
                    else if (addStep == 2 && !f3.empty()) f3.pop_back();
                    else if (addStep == 3 && !f4.empty()) f4.pop_back();
                    else if (addStep >= 4 && !f5.empty()) f5.pop_back();
                } else {
                    if (addStep == 0) f1 += c;
                    else if (addStep == 1) f2 += c;
                    else if (addStep == 2) f3 += c;
                    else if (addStep == 3) f4 += c;
                    else f5 += c;
                }
            }

            else if (etat == AJOUTABSENCE)
            {
                if (c == 'r') {
                    etat = MENUPROF;
                    addStep = 0; f1 = f2 = f3 = f4 = f5 = "";
                } else if (c == 10 || c == 13) {
                    addStep++;
                    if (addStep > 4) {
                        ctx.professeurActif.enregistrerAbsence(
                            ctx.bd,
                            (unsigned int)(std::stoi(f1)),
                            f2, f3, f4,
                            (f5 == "1" || f5 == "true"));
                        etat = MENUPROF;
                        addStep = 0; f1 = f2 = f3 = f4 = f5 = "";
                    }
                } else if (c == 127) {
                    if (addStep == 0 && !f1.empty()) f1.pop_back();
                    else if (addStep == 1 && !f2.empty()) f2.pop_back();
                    else if (addStep == 2 && !f3.empty()) f3.pop_back();
                    else if (addStep == 3 && !f4.empty()) f4.pop_back();
                    else if (addStep >= 4 && !f5.empty()) f5.pop_back();
                } else {
                    if (addStep == 0) f1 += c;
                    else if (addStep == 1) f2 += c;
                    else if (addStep == 2) f3 += c;
                    else if (addStep == 3) f4 += c;
                    else f5 += c;
                }
            }

            else if (etat == INFOS)
            {
                if ((ctx.currentUser.role == "etudiant" && c == 'r') ||
                    (ctx.currentUser.role == "professeur" && c == 'r')) {
                    etat = (ctx.currentUser.role == "etudiant") ? MENUETU : MENUPROF;
                }
            }
    
            // COMPTE SUPPRIMER
    
            else if (etat == COMPTESUPP)
            {

                if (c == 10 || c == 13)
                {
                    int id = std::stoi(input);
                    ctx.adminActif.supprimerCompte(ctx.bd, id);

                    input = "";
                    etat = LISTE;
                }
                else if (c == 127)
                {
                    if (!input.empty())
                        input.pop_back();
                }
                else
                {
                    input += c;
                }
                if (c == 'r')
                etat = MENUADMIN;
            }

            // COMPTE CRÉE
    
            else if (etat == COMPTECREE)
            {
                if (c == 10 || c == 13)
                {
                    if (etape == 0) etape = 1; //pour avancer quand appuie sur enter, pas step car pas booléen
                    else if (etape == 1) etape = 2;
                    else if (etape == 2) etape = 3;
                    else if (etape == 3)
                    {
                        if (choix_role == 1)
                            ctx.adminActif.creerCompte(ctx.bd, nom, prenom, mdp, 1);
                        else if (choix_role == 2)
                            ctx.adminActif.creerCompte(ctx.bd, nom, prenom, mdp, 2);
                        else if (choix_role == 3)
                            ctx.adminActif.creerCompte(ctx.bd, nom, prenom, mdp, 3);

                        nom = prenom = mdp = "";
                        etape = 0;
                        etat = LISTE;
                    }
                }
                else if (c==42)
                {
                    nom = prenom = mdp = "";
                    etape = 0;
                    etat = LISTE;   
                }
                else if (c == 127)
                {
                    if (etape == 0 && !nom.empty()) nom.pop_back();
                    else if (etape == 1 && !prenom.empty()) prenom.pop_back();
                    else if (etape == 2 && !mdp.empty()) mdp.pop_back();
                }
                else
                {
                    if (etape == 0) nom += c;
                    else if (etape == 1) prenom += c;
                    else if (etape == 2) mdp += c;
                    else if (etape == 3) choix_role = c - '0';
                }
            }
    
            // LISTE
    
            else if (etat == LISTE)
            {
                if (c == 'd') scroll_liste++;
                if (c == 'm' && scroll_liste > 0) scroll_liste--;
                if (c == 'r')
                    etat = MENUADMIN;
                if (c =='1')
                    etat=COMPTESUPP;
                if (c=='2')
                    etat=COMPTECREE;
                if (c=='3') {
                    etat = CLASSECREE;
                    addStep = 0;
                    f1 = f2 = f3 = "";
                }
            }

            else if (etat == CLASSECREE)
            {
                if (c == 'r') {
                    etat = LISTE;
                    addStep = 0;
                    f1 = f2 = f3 = "";
                } else if (c == 10 || c == 13) {
                    addStep++;
                    if (addStep > 2) {
                        ctx.adminActif.creerClasse(ctx.bd, f1, f2, f3);
                        etat = LISTE;
                        addStep = 0;
                        f1 = f2 = f3 = "";
                    }
                } else if (c == 127) {
                    if (addStep == 0 && !f1.empty()) f1.pop_back();
                    else if (addStep == 1 && !f2.empty()) f2.pop_back();
                    else if (addStep >= 2 && !f3.empty()) f3.pop_back();
                } else {
                    if (addStep == 0) f1 += c;
                    else if (addStep == 1) f2 += c;
                    else f3 += c;
                }
            }

            else if (etat == ASSIGNATION)
            {
                if (c == 'r') {
                    etat = MENUADMIN;
                    addStep = 0;
                    f1 = f2 = f3 = f4 = f5 = "";
                } else if (addStep == 0 && (c == '1' || c == '2' || c == '3' || c == '4' || c == '5' || c == '6')) {
                    f1 = "";
                    f2 = "";
                    f3 = "";
                    f4 = "";
                    f5 = "";
                    f1 += c;
                    addStep = 1;
                } else if (c == 127) {
                    if (addStep == 1 && !f2.empty()) f2.pop_back();
                    else if (addStep == 2 && !f3.empty()) f3.pop_back();
                    else if (addStep == 3 && !f4.empty()) f4.pop_back();
                    else if (addStep == 4 && !f5.empty()) f5.pop_back();
                } else if (c == 10 || c == 13) {
                    if (f1 == "1") {
                        if (addStep == 1) addStep = 2;
                        else if (addStep == 2) {
                            ctx.adminActif.assignerEtudiantClasse(ctx.bd, std::stoi(f2), std::stoi(f3));
                            addStep = 0;
                            f1 = f2 = f3 = f4 = f5 = "";
                        }
                    } else if (f1 == "2") {
                        if (addStep == 1) addStep = 2;
                        else if (addStep == 2) {
                            ctx.adminActif.assignerProfMatiere(ctx.bd, std::stoi(f2), std::stoi(f3));
                            addStep = 0;
                            f1 = f2 = f3 = f4 = f5 = "";
                        }
                    } else if (f1 == "3") {
                        if (addStep < 4) {
                            addStep++;
                        } else {
                            ctx.adminActif.affecterMatiereClasse(
                                ctx.bd,
                                std::stoi(f2),
                                std::stoi(f3),
                                std::stoi(f4),
                                (f5 == "1" || f5 == "true"));
                            addStep = 0;
                            f1 = f2 = f3 = f4 = f5 = "";
                        }
                    } else if (f1 == "4") {
                        if (addStep == 1) addStep = 2;
                        else if (addStep == 2) {
                            ctx.adminActif.assignerProfClasse(ctx.bd, std::stoi(f2), std::stoi(f3));
                            addStep = 0;
                            f1 = f2 = f3 = f4 = f5 = "";
                        }
                    } else if (f1 == "5") {
                        if (addStep < 7) {
                            addStep++;
                        } else {
                            ctx.adminActif.attribuerEDTProfesseur(
                                ctx.bd,
                                std::stoi(f2),
                                std::stoi(f3),
                                std::stoi(f4),
                                f5,
                                input,
                                nom,
                                prenom);
                            addStep = 0;
                            input = nom = prenom = "";
                            f1 = f2 = f3 = f4 = f5 = "";
                        }
                    } else if (f1 == "6") {
                        if (addStep == 1) addStep = 2;
                        else if (addStep == 2) {
                            ctx.adminActif.attribuerEDTEtudiant(ctx.bd, std::stoi(f2), std::stoi(f3));
                            addStep = 0;
                            f1 = f2 = f3 = f4 = f5 = "";
                        }
                    }
                } else {
                    if (addStep == 1) f2 += c;
                    else if (addStep == 2) f3 += c;
                    else if (addStep == 3) f4 += c;
                    else if (addStep == 4) f5 += c;
                    else if (addStep == 5) input += c;
                    else if (addStep == 6) nom += c;
                    else if (addStep == 7) prenom += c;
                }
            }
        }

        // pour pas que ça clignote
        if (!redraw)
            continue;

        redraw = false;

        win.clear();


        // PAGE CONNEXION

        if (etat == CONNEXION)
        {
            win.print(2, 2, "=== CONNEXION ===");

            win.print(2, 4, "Identifiant:");
            win.print(15, 4, login.c_str());

            win.print(2, 6, "Mot de passe:");

            std::string hidden(mdp.size(), '*');
            win.print(16, 6, hidden.c_str());
            win.print(2, 8, "=== COMMANDES ===");
            win.print(2, 10, passMode ? "Tape mot de passe + ENTER" : "Tape identifiant + ENTER");
            win.print(2, 12, "q - Quitter");
            std::string essais = std::to_string(i);
            if (loginError)
            {
                win.print(2, 14, "ERREUR : identifiant ou mot de passe invalide");
                win.print(2, 15, "Il vous reste : ");
                win.print(18, 15, essais.c_str());
                if(i==1)
                win.print(19, 15, " essai");
                else
                win.print(19, 15, " essais");
            }
        }


        // MENUADMIN

        else if (etat == MENUADMIN)
        {
            win.print(2, 2, "=== MENU ===");

            std::string user =
                "User ID: " + std::to_string(ctx.currentUser.id);

            win.print(2, 4, user.c_str());

            win.print(2, 6, "1 - Notes");
            win.print(2, 7, "2 - Liste");
            win.print(2, 8, "3 - Attributions au utilisateur");
            win.print(2, 9, "d - Deconnexion");
            win.print(2, 10, "q - Quitter");
        }

        // MENUPROF

        else if (etat == MENUPROF)
        {
            win.print(2, 2, "=== MENU ===");

            std::string user =
                "User ID: " + std::to_string(ctx.currentUser.id);

            win.print(2, 4, user.c_str());

            win.print(2, 6, "1 - Notes");
            win.print(2, 7, "2 - Ajouter une note");
            win.print(2, 8, "3 - Ajouter une absence");
            win.print(2, 9, "4 - Mes classes/EDT");
            win.print(2, 10, "d - Deconnexion");
            win.print(2, 11, "q - Quitter");
        }


        // MENUETU

        else if (etat == MENUETU)
        {
            win.print(2, 2, "=== MENU ===");

            std::string user =
                "User ID: " + std::to_string(ctx.currentUser.id);

            win.print(2, 4, user.c_str());

            win.print(2, 6, "1 - Notes");
            win.print(2, 7, "2 - Ma classe/EDT");
            win.print(2, 8, "d - Deconnexion");
            win.print(2, 9, "q - Quitter");
        }


        // MODIFIER NOTE

        else if (etat == MODIFIERNOTES)
        {
            win.print(2, 2, "=== MODIFICATION DES NOTES ===");

            win.print(2, 7, "m - Modifier une note");
            win.print(2, 8, "s - Supprimer une note");
            win.print(2, 9, "r - retour");
            win.print(2, 10, "d - Deconnexion");
            win.print(2, 11, "q - Quitter");
            if (editMode == 109)
            {
                win.print(2, 4, "Entrer: l'id et la valeur (ex: 3 15.5)");
                win.print(2, 5, input.c_str());
            }
            else if (editMode == 115)
            {
                win.print(2, 4, "Entrer: ID note");
                win.print(2, 5, input.c_str());
            }

        }

        // COMPTE SUPPRIMER

        else if (etat == COMPTESUPP)
        {
            win.print(2, 2, "=== SUPPRESSION COMPTE ===");

            win.print(2, 4, "Entrer ID utilisateur :");
            win.print(2, 6, input.c_str());
            win.print(2, 8, "ENTER = supprimer");
            win.print(2, 9, "r = retour");
        }

        // COMPTE CRÉE

        else if (etat == COMPTECREE)
        {
            win.print(2, 2, "=== CRÉATION COMPTE ===");

            win.print(2, 4, (std::string("Nom: ") + nom).c_str());
            win.print(2, 5, (std::string("Prénom: ") + prenom).c_str());
            win.print(2, 6, (std::string("MDP: ") + mdp).c_str());

            win.print(2, 8, "Rôle: 1 - Étudiant | 2 - Prof | 3 - Admin");

            win.print(2, 10, "ENTER pour valider et passer l'étape");
            win.print(2, 11, "* - retour");
        }

        else if (etat == CLASSECREE)
        {
            win.print(2, 2, "=== CREATION CLASSE ===");
            win.print(2, 4, "1) Nom classe:"); win.print(20, 4, f1.c_str());
            win.print(2, 5, "2) Niveau:"); win.print(20, 5, f2.c_str());
            win.print(2, 6, "3) Voie:"); win.print(20, 6, f3.c_str());
            win.print(2, 8, "ENTER etape suivante");
            win.print(2, 9, "r retour");
        }

        // NOTES

        else if (etat == NOTES)
        {
            win.print(2, 2, "=== COMMANDES ===");
            if(ctx.currentUser.role == "admin"){
            std::vector<NoteDB> notes = ctx.adminActif.getToutesNotes(ctx.bd);
            win.print(2, 10, "Nombre total : ");
            win.print(17, 10, std::to_string(notes.size()).c_str());
            
            int y = 11;
            int index=0;
            for (const auto &n : notes)
            {
                if (index++ < scroll_note)
                    continue;

                if (y > 20) // limite écran
                    break;
                std::string line =
                    std::to_string(n.id) + " - " + n.matiere + " : " + std::to_string(n.valeur); //création variable line pour contenir chaine

                win.print(2, y++, line.c_str()); //convertir line en const char* comme attendu dans winTXT
            }
            }
            else if (ctx.currentUser.role == "professeur"){
            std::vector<NoteDB> notes = ctx.professeurActif.consulterNotes(ctx.bd);
            win.print(2, 10, "Nombre total : ");
            win.print(17, 10, std::to_string(notes.size()).c_str());
            
            int y = 11;
            int index=0;
            for (const auto &n : notes)
            {
                if (index++ < scroll_note)
                    continue;

                if (y > 20) // limite écran
                    break;
                std::string line =
                    std::to_string(n.id) + " - " + n.matiere + " : " + std::to_string(n.valeur);

                win.print(2, y++, line.c_str());
            }
            }
            else{
            std::vector<Note> notes = ctx.etudiantActif.getNote();
            win.print(2, 10, "Nombre total : ");
            win.print(17, 10, std::to_string(notes.size()).c_str());
            
            int y = 11;
            int index=0;
            for (const auto &n : notes)
            {
                if (index++ < scroll_note)
                    continue;

                if (y > 20) // limite écran
                    break;
                std::string line =
                    std::to_string(n.getId()) + " - " + n.getAppr() + " : " + std::to_string(n.getValeur());

                win.print(2, y++, line.c_str());
            }

            }
            win.print(2, 4, "d/m - descendre/monter");
            win.print(2, 5, "r - retour");
            win.print(2, 6, "q - quitter");        
            if (ctx.currentUser.role == "admin")
                {win.print(2, 3, "2 - Modifier une note");}
            win.print(2, 9, "=== LISTE DES NOTES ==="); 
        }

        else if (etat == AJOUTNOTE)
        {
            win.print(2, 2, "=== AJOUT NOTE (PROF) ===");
            win.print(2, 4, "1) ID etudiant:"); win.print(20, 4, f1.c_str());
            win.print(2, 5, "2) ID matiere:"); win.print(20, 5, f2.c_str());
            win.print(2, 6, "3) Valeur:"); win.print(20, 6, f3.c_str());
            win.print(2, 7, "4) Coef:"); win.print(20, 7, f4.c_str());
            win.print(2, 8, "5) Appreciation:"); win.print(20, 8, f5.c_str());
            win.print(2, 10, "ENTER pour etape suivante");
            win.print(2, 11, "r retour");
        }

        else if (etat == AJOUTABSENCE)
        {
            win.print(2, 2, "=== AJOUT ABSENCE (PROF) ===");
            win.print(2, 4, "1) ID etudiant:"); win.print(20, 4, f1.c_str());
            win.print(2, 5, "2) Date YYYY-MM-DD:"); win.print(20, 5, f2.c_str());
            win.print(2, 6, "3) Heure HH:MM:"); win.print(20, 6, f3.c_str());
            win.print(2, 7, "4) Motif:"); win.print(20, 7, f4.c_str());
            win.print(2, 8, "5) Justifie 0/1:"); win.print(20, 8, f5.c_str());
            win.print(2, 10, "ENTER pour etape suivante");
            win.print(2, 11, "r retour");
        }

        else if (etat == INFOS)
        {
            win.print(2, 2, "=== INFOS ===");
            if (ctx.currentUser.role == "etudiant") {
                std::string cls = "Classe: " + (ctx.currentUser.classe_nom.empty() ? "-" : ctx.currentUser.classe_nom);
                win.print(2, 4, cls.c_str());
                win.print(2, 5, "Mon EDT:");
                std::vector<CoursDB> edt = ctx.bd.getEDTEtudiant(ctx.currentUser.id);
                int y = 6;
                for (int k = 0; k < int(edt.size()) && y < 18; ++k) {
                    std::string line = edt[k].jour + " " + edt[k].heure_debut + "-" + edt[k].heure_fin + " " + edt[k].matiere_nom;
                    win.print(2, y++, line.c_str());
                }
                win.print(2, 19, "r retour");
            } else {
                win.print(2, 4, "Mes classes:");
                std::vector<CoursDB> edt = ctx.bd.getEDTProfesseur(ctx.currentUser.id);
                int y = 5;
                std::vector<std::string> classes;
                for (int k = 0; k < int(edt.size()); ++k) {
                    bool seen = false;
                    for (int t = 0; t < int(classes.size()); ++t) if (classes[t] == edt[k].classe_nom) seen = true;
                    if (!seen) classes.push_back(edt[k].classe_nom);
                }
                for (int k = 0; k < int(classes.size()) && y < 10; ++k) win.print(2, y++, classes[k].c_str());
                win.print(2, 11, "Mon EDT:");
                y = 12;
                for (int k = 0; k < int(edt.size()) && y < 19; ++k) {
                    std::string line = edt[k].jour + " " + edt[k].heure_debut + "-" + edt[k].heure_fin + " " + edt[k].classe_nom;
                    win.print(2, y++, line.c_str());
                }
                win.print(2, 19, "r retour");
            }
        }

        // LISTE

        else if (etat == LISTE)
        {
            win.print(2, 2, "=== COMMANDES ===");

            std::vector<PersonneDB> users =
                ctx.adminActif.getTousUtilisateurs(ctx.bd);
            win.print(2, 10, "Nombre total : ");
            win.print(17, 10, std::to_string(users.size()).c_str());
            int y = 11;
            int index = 0;

            for (const auto &u : users)
            {
                if (index++ < scroll_liste)
                    continue;

                if (y > 20)
                    break;

                std::string line =
                    std::to_string(u.id) + " - " + u.identifiant + " - " + u.nom + " " + u.prenom +" (" + u.role + ")";

                win.print(2, y++, line.c_str());
            }

            win.print(2, 5, "d/m - descendre/monter");
            win.print(2, 6, "r - retour");
            win.print(2, 7, "q - quitter");        
            win.print(2, 3, "1 - Supprimer un compte");
            win.print(2, 4, "2 - Créer un compte"); 
            win.print(2, 5, "3 - Créer une classe");
            win.print(2, 9, "=== LISTE DES UTILISATEURS ==="); 
        }

        else if (etat == ASSIGNATION)
        {
            std::vector<PersonneDB> users = ctx.adminActif.getTousUtilisateurs(ctx.bd);
            std::vector<ClasseDB> classes = ctx.bd.getToutesLesClasses();
            std::vector<MatiereDB> matieres = ctx.bd.getToutesLesmatieres();
            win.print(2, 2, "=== ATTRIBUTIONS AU UTILISATEUR (ADMIN) ===");
            if (addStep == 0) {
                win.print(2, 4, "1 - Assigner etudiant -> classe");
                win.print(2, 5, "2 - Assigner professeur -> matiere");
                win.print(2, 6, "3 - Affecter matiere -> classe -> prof");
                win.print(2, 7, "4 - Assigner classe -> professeur");
                win.print(2, 8, "5 - Attribuer EDT -> professeur");
                win.print(2, 9, "6 - Attribuer EDT -> etudiant");
                win.print(2, 11, "Tape 1..6 puis ENTER");
                win.print(2, 9, "r retour");
            } else if (f1 == "1") {
                win.print(2, 4, "Action: Assigner etudiant -> classe");
                win.print(2, 5, "ID etudiant:"); win.print(25, 5, f2.c_str());
                win.print(2, 6, "ID classe:"); win.print(25, 6, f3.c_str());
                win.print(2, 8, "ENTER etape suivante");
                win.print(2, 9, "r retour");
            } else if (f1 == "2") {
                win.print(2, 4, "Action: Assigner professeur -> matiere");
                win.print(2, 5, "ID professeur:"); win.print(25, 5, f2.c_str());
                win.print(2, 6, "ID matiere:"); win.print(25, 6, f3.c_str());
                win.print(2, 8, "ENTER etape suivante");
                win.print(2, 9, "r retour");
            } else if (f1 == "3") {
                win.print(2, 4, "Action: Affecter matiere -> classe -> prof");
                win.print(2, 5, "ID classe:"); win.print(25, 5, f2.c_str());
                win.print(2, 6, "ID matiere:"); win.print(25, 6, f3.c_str());
                win.print(2, 7, "ID professeur:"); win.print(25, 7, f4.c_str());
                win.print(2, 8, "Obligatoire (0/1):"); win.print(25, 8, f5.c_str());
                win.print(2, 10, "ENTER etape suivante");
                win.print(2, 11, "r retour");
            } else if (f1 == "4") {
                win.print(2, 4, "Action: Assigner classe -> professeur");
                win.print(2, 5, "ID professeur:"); win.print(25, 5, f2.c_str());
                win.print(2, 6, "ID classe:"); win.print(25, 6, f3.c_str());
                win.print(2, 8, "ENTER etape suivante");
                win.print(2, 9, "r retour");
            } else if (f1 == "5") {
                win.print(2, 4, "Action: Attribuer EDT -> professeur");
                win.print(2, 5, "ID professeur:"); win.print(25, 5, f2.c_str());
                win.print(2, 6, "ID classe:"); win.print(25, 6, f3.c_str());
                win.print(2, 7, "ID matiere:"); win.print(25, 7, f4.c_str());
                win.print(2, 8, "Jour:"); win.print(25, 8, f5.c_str());
                win.print(2, 9, "Heure debut:"); win.print(25, 9, input.c_str());
                win.print(2, 10, "Heure fin:"); win.print(25, 10, nom.c_str());
                win.print(2, 11, "Salle:"); win.print(25, 11, prenom.c_str());
                win.print(2, 13, "ENTER etape suivante");
                win.print(2, 14, "r retour");
            } else if (f1 == "6") {
                win.print(2, 4, "Action: Attribuer EDT -> etudiant");
                win.print(2, 5, "ID etudiant:"); win.print(25, 5, f2.c_str());
                win.print(2, 6, "ID classe:"); win.print(25, 6, f3.c_str());
                win.print(2, 8, "ENTER etape suivante");
                win.print(2, 9, "r retour");
            }

            int yInfo = 12;
            if (f1 == "1" || f1 == "6") {
                win.print(2, yInfo++, "Etudiants (IDs):");
                for (int i = 0; i < int(users.size()) && yInfo < 16; ++i) {
                    if (users[i].role == "etudiant") {
                        std::string line = std::to_string(users[i].id) + " - " + users[i].prenom + " " + users[i].nom;
                        win.print(2, yInfo++, line.c_str());
                    }
                }
                win.print(2, yInfo++, "Classes (IDs):");
                for (int i = 0; i < int(classes.size()) && yInfo < 20; ++i) {
                    std::string cLine = std::to_string(classes[i].id) + " - " + classes[i].nom;
                    win.print(2, yInfo++, cLine.c_str());
                }
            } else if (f1 == "2" || f1 == "4" || f1 == "5") {
                win.print(2, yInfo++, "Professeurs (IDs):");
                for (int i = 0; i < int(users.size()) && yInfo < 16; ++i) {
                    if (users[i].role == "professeur") {
                        std::string line = std::to_string(users[i].id) + " - " + users[i].prenom + " " + users[i].nom;
                        win.print(2, yInfo++, line.c_str());
                    }
                }
                if (f1 == "2" || f1 == "5") {
                    win.print(2, yInfo++, "Matieres (IDs):");
                    for (int i = 0; i < int(matieres.size()) && yInfo < 20; ++i) {
                        std::string mLine = std::to_string(matieres[i].id) + " - " + matieres[i].nom;
                        win.print(2, yInfo++, mLine.c_str());
                    }
                } else {
                    win.print(2, yInfo++, "Classes (IDs):");
                    for (int i = 0; i < int(classes.size()) && yInfo < 20; ++i) {
                        std::string cLine = std::to_string(classes[i].id) + " - " + classes[i].nom;
                        win.print(2, yInfo++, cLine.c_str());
                    }
                }
            } else if (f1 == "3") {
                win.print(2, yInfo++, "Classes (IDs):");
                for (int i = 0; i < int(classes.size()) && yInfo < 15; ++i) {
                    std::string cLine = std::to_string(classes[i].id) + " - " + classes[i].nom;
                    win.print(2, yInfo++, cLine.c_str());
                }
                win.print(2, yInfo++, "Matieres (IDs):");
                for (int i = 0; i < int(matieres.size()) && yInfo < 18; ++i) {
                    std::string mLine = std::to_string(matieres[i].id) + " - " + matieres[i].nom;
                    win.print(2, yInfo++, mLine.c_str());
                }
                win.print(2, yInfo++, "Professeurs (IDs):");
                for (int i = 0; i < int(users.size()) && yInfo < 20; ++i) {
                    if (users[i].role == "professeur") {
                        std::string line = std::to_string(users[i].id) + " - " + users[i].prenom + " " + users[i].nom;
                        win.print(2, yInfo++, line.c_str());
                    }
                }
            }
        }
        win.draw();
    }
}