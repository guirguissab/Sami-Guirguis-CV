#ifndef DATABASE_H
#define DATABASE_H

#include <sqlite3.h>
#include <string>
#include <vector>

struct PersonneDB {
    int    id;
    std::string identifiant;
    std::string nom;
    std::string prenom;
    std::string mot_de_passe;
    std::string role; // "etudiant", "professeur", "admin"
    std::string photo_path;
    int    classe_id;
    std::string classe_nom;
};

struct NoteDB {
    int    id;
    float  valeur;
    float  coef;
    std::string appreciation;
    std::string matiere;
    int    etudiant_id;
    int    matiere_id;
};


struct AbsenceDB {
    int    id;
    std::string date;
    std::string motif;
    bool   justifie;
    std::string heure;
    int    etudiant_id;
};

struct MatiereDB {
    int    id;
    std::string nom;
    std::string apogee;
    float  coef;
};

struct ClasseDB {
    int    id;
    std::string nom;
    std::string niveau;
    std::string voie;
};

struct ClasseMatiereDB {
    int    classe_id;
    int    matiere_id;
    int    prof_id;
    bool   obligatoire;
    std::string classe_nom;
    std::string matiere_nom;
    std::string prof_nom;
    std::string type_matiere;
};

struct CoursDB {
    int    id;
    std::string jour;
    std::string heure_debut;
    std::string heure_fin;
    std::string salle;
    std::string classe_nom;
    std::string matiere_nom;
    std::string prof_nom;
    int    classe_id;
    int    matiere_id;
    int    prof_id;
};

struct ActualiteDB {
    int    id;
    std::string titre;
    std::string contenu;
    std::string date_publication;
};




class Database{
    private:
        sqlite3* db; 
        bool execSQL(const std::string& sql);
        bool ensureOpen(const std::string& contexte) const;
        bool prepareStatement(const std::string& sql, sqlite3_stmt** stmt) const;
        bool executeNonQuery(sqlite3_stmt* stmt, const std::string& contexte) const;
        bool tableHasColumn(const std::string& tableName, const std::string& columnName) const;
        bool personneExiste(int personneId) const;
        bool personneAvecRoleExiste(int personneId, const std::string& role) const;
        bool classeExiste(int classeId) const;
        bool matiereExiste(int matiereId) const;
        bool profEnseigneMatiere(int profId, int matiereId) const;
        bool matiereAffecteeClasse(int classeId, int matiereId) const;
        bool profAffecteClasseMatiere(int classeId, int matiereId, int profId) const;
        bool etudiantInscritMatiere(int etudiantId, int matiereId) const;
        bool ensureIdentifiantsPersonnes();
        std::string construireIdentifiant(const std::string& role, int numero) const;
        int prochainNumeroIdentifiant(const std::string& role) const;
        bool definirProchainNumeroIdentifiant(const std::string& role, int nextValue) const;
        void synchroniserMatieresEtudiantDepuisClasse(int etudiantId, int classeId);
        void initialiserDonneesCoherentes();
        int insererPersonne(const std::string& nom, const std::string& prenom,
                            const std::string& mdp, const std::string& role);
        
    public:
        Database();

        ~Database();

        void init();

        void close();

        PersonneDB authentifier(const std::string& identifiant, const std::string& mdp);

        int creerEtudiant(const std::string& nom, const std::string& prenom, const std::string& mdp);
        
        int creerProfesseur(const std::string& nom, const std::string& prenom, const std::string& mdp);

    
        int creerAdmin(const std::string& nom, const std::string& prenom, const std::string& mdp);

        
        void supprimerCompte(int personneId);

        
        std::vector<PersonneDB> getTousEtudiants();

        
        std::vector<PersonneDB> getTousProfesseurs();

        
        std::vector<PersonneDB> getTousUtilisateurs();

        PersonneDB getPersonneParId(int personneId);

        bool modifierPhotoProfil(int personneId, const std::string& photoPath);

        
        bool assignerEtudiantClasse(int etudiantId, int classeId);

        
        int getPersonneIdParIdentifiant(const std::string& identifiant);

        
        std::string getIdentifiantParId(int personneId);

        
        bool modifierMotDePasse(int personneId, const std::string& nouveauMdp);

        
        void ajouterNote(int etudiantId, int matiereId, float valeur, float coef, const std::string& appreciation);

        void modifierNote(int noteId, float nouvelleValeur);

	void modifierAbsence(int absenceId, bool justifie, const std::string& motif);   
        void supprimerNote(int noteId);

        
        std::vector<NoteDB> getNotes(int etudiantId);

        std::vector<NoteDB> getAllNotes();

        
        std::vector<NoteDB> getNotesByMatiere(int etudiantId, int matiereId);



        float getMoyenne(int etudiantId, int matiereId);

    

        
        void ajouterAbsence(int etudiantId, const std::string& date, const std::string& heure,
                            const std::string& motif, bool justifie);

        
        std::vector<AbsenceDB> getAbsences(int etudiantId);

        
        int getNbAbsences(int etudiantId);

    
        int ajouterMatiere(const std::string& nom, const std::string& apogee, float coef);

        
        int ajouterClasse(const std::string& nom, const std::string& niveau, const std::string& voie);

        std::vector<ClasseDB> getToutesLesClasses();

    
        std::vector<MatiereDB> getToutesLesmatieres();

    
        std::vector<MatiereDB> getMatieresProfesseur(int profId);

        std::vector<MatiereDB> getMatieresEtudiant(int etudiantId);

    
        std::vector<PersonneDB> getEtudiantsParMatiere(int matiereId);

        std::vector<PersonneDB> getEtudiantsClasse(int classeId);

        
        std::vector<ClasseMatiereDB> getMatieresClasse(int classeId);

        
        void assignerProfMatiere(int profId, int matiereId);

        
        bool affecterMatiereClasse(int classeId, int matiereId, int profId, bool obligatoire);


        void inscrireEtudiantMatiere(int etudiantId, int matiereId);

    
        int ajouterCours(const std::string& jour, const std::string& heureDebut,
                        const std::string& heureFin, const std::string& salle,
                        int classeId, int matiereId, int profId);

        
        void modifierCours(int coursId, const std::string& jour, const std::string& heureDebut,
                        const std::string& heureFin, const std::string& salle);

        
        void supprimerCours(int coursId);

        
        std::vector<CoursDB> getEDTEtudiant(int etudiantId);



        std::vector<CoursDB> getEDTProfesseur(int profId);

    
        
        std::vector<CoursDB> getToutEDT();


        int publierActualite(const std::string& titre, const std::string& contenu,
                            const std::string& datePublication);

        std::vector<ActualiteDB> getToutesActualites();

    
        void supprimerActualite(int actualiteId);
        

};


#endif // DATABASE_H
