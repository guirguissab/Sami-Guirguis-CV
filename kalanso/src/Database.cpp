#include "Database.h"

#include <algorithm>
#include <cctype>
#include <iomanip>
#include <iostream>
#include <sstream>

using namespace std;

namespace {

constexpr const char* ROLE_ETUDIANT   = "etudiant";
constexpr const char* ROLE_PROFESSEUR = "professeur";
constexpr const char* ROLE_ADMIN      = "admin";

string uppercaseCopy(string value) {
    transform(value.begin(), value.end(), value.begin(), [](unsigned char c) {
        return static_cast<char>(toupper(c));
    });
    return value;
}

string textColumn(sqlite3_stmt* stmt, int index) {
    const unsigned char* value = sqlite3_column_text(stmt, index);
    return value ? reinterpret_cast<const char*>(value) : "";
}

const char* ordreJourSQL() {
    return
        "CASE LOWER(c.jour) "
        "WHEN 'lundi' THEN 1 "
        "WHEN 'mardi' THEN 2 "
        "WHEN 'mercredi' THEN 3 "
        "WHEN 'jeudi' THEN 4 "
        "WHEN 'vendredi' THEN 5 "
        "WHEN 'samedi' THEN 6 "
        "WHEN 'dimanche' THEN 7 "
        "ELSE 8 END";
}

} // namespace

Database::Database() : db(nullptr) {}

Database::~Database() {
    close();
}

bool Database::execSQL(const string& sql) {
    if (!ensureOpen("execSQL")) {
        return false;
    }

    char* errMsg = nullptr;
    int rc = sqlite3_exec(db, sql.c_str(), nullptr, nullptr, &errMsg);
    if (rc != SQLITE_OK) {
        cerr << "[DB ERREUR] " << (errMsg ? errMsg : sqlite3_errmsg(db)) << "\n";
        cerr << "[DB SQL]    " << sql << "\n";
        sqlite3_free(errMsg);
        return false;
    }
    return true;
}

bool Database::ensureOpen(const string& contexte) const {
    if (db != nullptr) {
        return true;
    }

    cerr << "[DB ERREUR] Connexion non initialisee pour " << contexte << ".\n";
    return false;
}

bool Database::prepareStatement(const string& sql, sqlite3_stmt** stmt) const {
    if (!ensureOpen("prepareStatement")) {
        return false;
    }

    *stmt = nullptr;
    int rc = sqlite3_prepare_v2(db, sql.c_str(), -1, stmt, nullptr);
    if (rc != SQLITE_OK) {
        cerr << "[DB ERREUR] Echec prepare : " << sqlite3_errmsg(db) << "\n";
        cerr << "[DB SQL]    " << sql << "\n";
        return false;
    }

    return true;
}

bool Database::executeNonQuery(sqlite3_stmt* stmt, const string& contexte) const {
    int rc = sqlite3_step(stmt);
    if (rc != SQLITE_DONE) {
        cerr << "[DB ERREUR] Echec " << contexte << " : " << sqlite3_errmsg(db) << "\n";
        return false;
    }

    return true;
}

bool Database::tableHasColumn(const string& tableName, const string& columnName) const {
    sqlite3_stmt* stmt = nullptr;
    if (!prepareStatement("PRAGMA table_info(" + tableName + ");", &stmt)) {
        return false;
    }

    bool found = false;
    while (sqlite3_step(stmt) == SQLITE_ROW) {
        if (textColumn(stmt, 1) == columnName) {
            found = true;
            break;
        }
    }

    sqlite3_finalize(stmt);
    return found;
}

string Database::construireIdentifiant(const string& role, int numero) const {
    char prefix = 'U';
    if (role == ROLE_ETUDIANT) {
        prefix = 'E';
    } else if (role == ROLE_PROFESSEUR) {
        prefix = 'P';
    } else if (role == ROLE_ADMIN) {
        prefix = 'A';
    }

    ostringstream stream;
    stream << prefix << setw(5) << setfill('0') << numero;
    return stream.str();
}

int Database::prochainNumeroIdentifiant(const string& role) const {
    sqlite3_stmt* stmt = nullptr;
    if (prepareStatement(
            "SELECT next_value FROM identifiant_sequences WHERE role = ?;",
            &stmt)) {
        sqlite3_bind_text(stmt, 1, role.c_str(), -1, SQLITE_TRANSIENT);
        if (sqlite3_step(stmt) == SQLITE_ROW) {
            const int nextValue = sqlite3_column_int(stmt, 0);
            sqlite3_finalize(stmt);
            return nextValue;
        }
        sqlite3_finalize(stmt);
    }

    if (!prepareStatement(
            "SELECT COALESCE(MAX(CAST(SUBSTR(identifiant, 2) AS INTEGER)), 0) + 1 "
            "FROM personnes WHERE role = ?;",
            &stmt)) {
        return 1;
    }

    sqlite3_bind_text(stmt, 1, role.c_str(), -1, SQLITE_TRANSIENT);
    int nextValue = 1;
    if (sqlite3_step(stmt) == SQLITE_ROW) {
        nextValue = sqlite3_column_int(stmt, 0);
    }
    sqlite3_finalize(stmt);
    return max(nextValue, 1);
}

bool Database::definirProchainNumeroIdentifiant(const string& role, int nextValue) const {
    sqlite3_stmt* stmt = nullptr;
    if (!prepareStatement(
            "INSERT INTO identifiant_sequences (role, next_value) VALUES (?, ?) "
            "ON CONFLICT(role) DO UPDATE SET next_value = excluded.next_value;",
            &stmt)) {
        return false;
    }

    sqlite3_bind_text(stmt, 1, role.c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_int(stmt, 2, max(nextValue, 1));

    const bool ok = executeNonQuery(stmt, "mise a jour sequence identifiant");
    sqlite3_finalize(stmt);
    return ok;
}

bool Database::ensureIdentifiantsPersonnes() {
    if (!tableHasColumn("personnes", "identifiant")) {
        if (!execSQL("ALTER TABLE personnes ADD COLUMN identifiant TEXT;")) {
            return false;
        }
    }

    if (!execSQL(
            "CREATE TABLE IF NOT EXISTS identifiant_sequences ("
            "  role TEXT PRIMARY KEY CHECK(role IN ('etudiant', 'professeur', 'admin')),"
            "  next_value INTEGER NOT NULL CHECK(next_value >= 1)"
            ");")) {
        return false;
    }

    sqlite3_stmt* countStmt = nullptr;
    if (!prepareStatement(
            "SELECT COUNT(*) FROM identifiant_sequences;",
            &countStmt)) {
        return false;
    }

    int sequenceCount = 0;
    if (sqlite3_step(countStmt) == SQLITE_ROW) {
        sequenceCount = sqlite3_column_int(countStmt, 0);
    }
    sqlite3_finalize(countStmt);

    vector<string> roles = {ROLE_ETUDIANT, ROLE_PROFESSEUR, ROLE_ADMIN};

    if (sequenceCount == 0) {
        for (int roleIndex = 0; roleIndex < int(roles.size()); ++roleIndex) {
            sqlite3_stmt* selectStmt = nullptr;
            if (!prepareStatement(
                    "SELECT id FROM personnes WHERE role = ? ORDER BY id;",
                    &selectStmt)) {
                return false;
            }

            sqlite3_bind_text(selectStmt, 1, roles[roleIndex].c_str(), -1, SQLITE_TRANSIENT);

            vector<int> ids;
            while (sqlite3_step(selectStmt) == SQLITE_ROW) {
                ids.push_back(sqlite3_column_int(selectStmt, 0));
            }
            sqlite3_finalize(selectStmt);

            sqlite3_stmt* updateStmt = nullptr;
            if (!prepareStatement(
                    "UPDATE personnes SET identifiant = ? WHERE id = ?;",
                    &updateStmt)) {
                return false;
            }

            for (int i = 0; i < int(ids.size()); ++i) {
                const string identifiant = construireIdentifiant(roles[roleIndex], int(i) + 1);
                sqlite3_bind_text(updateStmt, 1, identifiant.c_str(), -1, SQLITE_TRANSIENT);
                sqlite3_bind_int(updateStmt, 2, ids[i]);

                if (!executeNonQuery(updateStmt, "normalisation identifiant")) {
                    sqlite3_finalize(updateStmt);
                    return false;
                }

                sqlite3_reset(updateStmt);
                sqlite3_clear_bindings(updateStmt);
            }

            sqlite3_finalize(updateStmt);

            if (!definirProchainNumeroIdentifiant(roles[roleIndex], int(ids.size()) + 1)) {
                return false;
            }
        }
    }

    sqlite3_stmt* selectStmt = nullptr;
    if (!prepareStatement(
            "SELECT id, role FROM personnes "
            "WHERE identifiant IS NULL OR TRIM(identifiant) = '';",
            &selectStmt)) {
        return false;
    }

    vector<pair<int, string>> missingPersons;
    while (sqlite3_step(selectStmt) == SQLITE_ROW) {
        missingPersons.push_back({sqlite3_column_int(selectStmt, 0), textColumn(selectStmt, 1)});
    }
    sqlite3_finalize(selectStmt);

    if (!missingPersons.empty()) {
        sqlite3_stmt* updateStmt = nullptr;
        if (!prepareStatement(
                "UPDATE personnes SET identifiant = ? WHERE id = ?;",
                &updateStmt)) {
            return false;
        }

        for (int i = 0; i < int(missingPersons.size()); ++i) {
            const int nextValue = prochainNumeroIdentifiant(missingPersons[i].second);
            const string identifiant = construireIdentifiant(missingPersons[i].second, nextValue);

            sqlite3_bind_text(updateStmt, 1, identifiant.c_str(), -1, SQLITE_TRANSIENT);
            sqlite3_bind_int(updateStmt, 2, missingPersons[i].first);

            if (!executeNonQuery(updateStmt, "mise a jour identifiant")) {
                sqlite3_finalize(updateStmt);
                return false;
            }

            if (!definirProchainNumeroIdentifiant(missingPersons[i].second, nextValue + 1)) {
                sqlite3_finalize(updateStmt);
                return false;
            }

            sqlite3_reset(updateStmt);
            sqlite3_clear_bindings(updateStmt);
        }

        sqlite3_finalize(updateStmt);
    }

    for (int roleIndex = 0; roleIndex < int(roles.size()); ++roleIndex) {
        if (!definirProchainNumeroIdentifiant(roles[roleIndex], prochainNumeroIdentifiant(roles[roleIndex]))) {
            return false;
        }
    }

    return execSQL(
        "CREATE UNIQUE INDEX IF NOT EXISTS idx_personnes_identifiant "
        "ON personnes(identifiant);"
    );
}

bool Database::personneExiste(int personneId) const {
    sqlite3_stmt* stmt = nullptr;
    if (!prepareStatement("SELECT 1 FROM personnes WHERE id = ?;", &stmt)) {
        return false;
    }

    sqlite3_bind_int(stmt, 1, personneId);
    int rc = sqlite3_step(stmt);
    bool existe = (rc == SQLITE_ROW);
    sqlite3_finalize(stmt);
    return existe;
}

bool Database::personneAvecRoleExiste(int personneId, const string& role) const {
    sqlite3_stmt* stmt = nullptr;
    if (!prepareStatement(
            "SELECT 1 FROM personnes WHERE id = ? AND role = ?;",
            &stmt)) {
        return false;
    }

    sqlite3_bind_int(stmt, 1, personneId);
    sqlite3_bind_text(stmt, 2, role.c_str(), -1, SQLITE_TRANSIENT);
    int rc = sqlite3_step(stmt);
    bool existe = (rc == SQLITE_ROW);
    sqlite3_finalize(stmt);
    return existe;
}

bool Database::classeExiste(int classeId) const {
    sqlite3_stmt* stmt = nullptr;
    if (!prepareStatement("SELECT 1 FROM classes WHERE id = ?;", &stmt)) {
        return false;
    }

    sqlite3_bind_int(stmt, 1, classeId);
    const bool existe = (sqlite3_step(stmt) == SQLITE_ROW);
    sqlite3_finalize(stmt);
    return existe;
}

bool Database::matiereExiste(int matiereId) const {
    sqlite3_stmt* stmt = nullptr;
    if (!prepareStatement("SELECT 1 FROM matieres WHERE id = ?;", &stmt)) {
        return false;
    }

    sqlite3_bind_int(stmt, 1, matiereId);
    int rc = sqlite3_step(stmt);
    bool existe = (rc == SQLITE_ROW);
    sqlite3_finalize(stmt);
    return existe;
}

bool Database::profEnseigneMatiere(int profId, int matiereId) const {
    sqlite3_stmt* stmt = nullptr;
    if (!prepareStatement(
            "SELECT 1 FROM prof_matiere WHERE prof_id = ? AND matiere_id = ?;",
            &stmt)) {
        return false;
    }

    sqlite3_bind_int(stmt, 1, profId);
    sqlite3_bind_int(stmt, 2, matiereId);
    int rc = sqlite3_step(stmt);
    bool existe = (rc == SQLITE_ROW);
    sqlite3_finalize(stmt);
    return existe;
}

bool Database::matiereAffecteeClasse(int classeId, int matiereId) const {
    sqlite3_stmt* stmt = nullptr;
    if (!prepareStatement(
            "SELECT 1 FROM classe_matieres WHERE classe_id = ? AND matiere_id = ?;",
            &stmt)) {
        return false;
    }

    sqlite3_bind_int(stmt, 1, classeId);
    sqlite3_bind_int(stmt, 2, matiereId);
    const bool existe = (sqlite3_step(stmt) == SQLITE_ROW);
    sqlite3_finalize(stmt);
    return existe;
}

bool Database::profAffecteClasseMatiere(int classeId, int matiereId, int profId) const {
    sqlite3_stmt* stmt = nullptr;
    if (!prepareStatement(
            "SELECT 1 FROM classe_matieres "
            "WHERE classe_id = ? AND matiere_id = ? AND prof_id = ?;",
            &stmt)) {
        return false;
    }

    sqlite3_bind_int(stmt, 1, classeId);
    sqlite3_bind_int(stmt, 2, matiereId);
    sqlite3_bind_int(stmt, 3, profId);
    const bool existe = (sqlite3_step(stmt) == SQLITE_ROW);
    sqlite3_finalize(stmt);
    return existe;
}

bool Database::etudiantInscritMatiere(int etudiantId, int matiereId) const {
    sqlite3_stmt* stmt = nullptr;
    if (!prepareStatement(
            "SELECT 1 FROM etudiant_matiere WHERE etudiant_id = ? AND matiere_id = ?;",
            &stmt)) {
        return false;
    }

    sqlite3_bind_int(stmt, 1, etudiantId);
    sqlite3_bind_int(stmt, 2, matiereId);
    int rc = sqlite3_step(stmt);
    bool existe = (rc == SQLITE_ROW);
    sqlite3_finalize(stmt);
    return existe;
}

int Database::insererPersonne(const string& nom, const string& prenom,
                              const string& mdp, const string& role) {
    if (!ensureOpen("insererPersonne")) {
        return -1;
    }
    if (nom.empty() || prenom.empty() || mdp.empty()) {
        cerr << "[DB ERREUR] Nom, prenom et mot de passe sont obligatoires.\n";
        return -1;
    }

    sqlite3_stmt* stmt = nullptr;
    const string sql =
        "INSERT INTO personnes (identifiant, nom, prenom, mot_de_passe, role) "
        "VALUES (NULL, ?, ?, ?, ?);";

    if (!prepareStatement(sql, &stmt)) {
        return -1;
    }

    sqlite3_bind_text(stmt, 1, nom.c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt, 2, prenom.c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt, 3, mdp.c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt, 4, role.c_str(), -1, SQLITE_TRANSIENT);

    bool ok = executeNonQuery(stmt, "insertion personne");
    sqlite3_finalize(stmt);
    if (!ok) {
        return -1;
    }

    const int newId = int(sqlite3_last_insert_rowid(db));
    const int numeroIdentifiant = prochainNumeroIdentifiant(role);
    const string identifiant = construireIdentifiant(role, numeroIdentifiant);

    sqlite3_stmt* updateStmt = nullptr;
    if (!prepareStatement(
            "UPDATE personnes SET identifiant = ? WHERE id = ?;",
            &updateStmt)) {
        return -1;
    }

    sqlite3_bind_text(updateStmt, 1, identifiant.c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_int(updateStmt, 2, newId);

    ok = executeNonQuery(updateStmt, "generation identifiant");
    sqlite3_finalize(updateStmt);
    if (!ok) {
        return -1;
    }

    return definirProchainNumeroIdentifiant(role, numeroIdentifiant + 1) ? newId : -1;
}

void Database::synchroniserMatieresEtudiantDepuisClasse(int etudiantId, int classeId) {
    if (!personneAvecRoleExiste(etudiantId, ROLE_ETUDIANT) || !classeExiste(classeId)) {
        return;
    }

    sqlite3_stmt* deleteStmt = nullptr;
    if (prepareStatement("DELETE FROM etudiant_matiere WHERE etudiant_id = ?;", &deleteStmt)) {
        sqlite3_bind_int(deleteStmt, 1, etudiantId);
        executeNonQuery(deleteStmt, "reset matieres etudiant");
        sqlite3_finalize(deleteStmt);
    }

    sqlite3_stmt* insertStmt = nullptr;
    if (prepareStatement(
            "INSERT OR IGNORE INTO etudiant_matiere (etudiant_id, matiere_id) "
            "SELECT ?, matiere_id FROM classe_matieres "
            "WHERE classe_id = ? AND obligatoire = 1;",
            &insertStmt)) {
        sqlite3_bind_int(insertStmt, 1, etudiantId);
        sqlite3_bind_int(insertStmt, 2, classeId);
        executeNonQuery(insertStmt, "synchronisation matieres obligatoires");
        sqlite3_finalize(insertStmt);
    }
}

void Database::initialiserDonneesCoherentes() {
    const auto ensureMatiere = [this](const string& nom, const string& apogee, float coef) {
        vector<MatiereDB> matieres = getToutesLesmatieres();
        for (int i = 0; i < int(matieres.size()); ++i) {
            if (matieres[i].nom == nom) {
                return matieres[i].id;
            }
        }
        return ajouterMatiere(nom, apogee, coef);
    };

    const auto ensureProf = [this](const string& nom, const string& prenom, const string& mdp) {
        vector<PersonneDB> profs = getTousProfesseurs();
        for (int i = 0; i < int(profs.size()); ++i) {
            if (profs[i].nom == nom && profs[i].prenom == prenom) {
                return profs[i].id;
            }
        }
        return creerProfesseur(nom, prenom, mdp);
    };

    const auto ensureEtudiant = [this](const string& nom, const string& prenom, const string& mdp) {
        vector<PersonneDB> etudiants = getTousEtudiants();
        for (int i = 0; i < int(etudiants.size()); ++i) {
            if (etudiants[i].nom == nom && etudiants[i].prenom == prenom) {
                return etudiants[i].id;
            }
        }
        return creerEtudiant(nom, prenom, mdp);
    };

    const auto ensureClasse = [this](const string& nom, const string& niveau, const string& voie) {
        vector<ClasseDB> classes = getToutesLesClasses();
        for (int i = 0; i < int(classes.size()); ++i) {
            if (classes[i].nom == nom) {
                return classes[i].id;
            }
        }
        return ajouterClasse(nom, niveau, voie);
    };

    const auto ensureCours = [this](const string& jour, const string& heureDebut,
                                    const string& heureFin, const string& salle,
                                    int classeId, int matiereId, int profId) {
        vector<CoursDB> cours = getToutEDT();
        for (int i = 0; i < int(cours.size()); ++i) {
            if (cours[i].jour == jour &&
                cours[i].heure_debut == heureDebut &&
                cours[i].heure_fin == heureFin &&
                cours[i].salle == salle &&
                cours[i].classe_id == classeId &&
                cours[i].matiere_id == matiereId &&
                cours[i].prof_id == profId) {
                return cours[i].id;
            }
        }
        return ajouterCours(jour, heureDebut, heureFin, salle, classeId, matiereId, profId);
    };

    const auto ensureActualite = [this](const string& titre, const string& contenu,
                                        const string& datePublication) {
        vector<ActualiteDB> actualites = getToutesActualites();
        for (int i = 0; i < int(actualites.size()); ++i) {
            if (actualites[i].titre == titre &&
                actualites[i].date_publication == datePublication) {
                return actualites[i].id;
            }
        }
        return publierActualite(titre, contenu, datePublication);
    };

    const auto ensureNote = [this](int etudiantId, int matiereId, float valeur,
                                   float coef, const string& appreciation) {
        vector<NoteDB> notes = getNotesByMatiere(etudiantId, matiereId);
        for (int i = 0; i < int(notes.size()); ++i) {
            if (notes[i].valeur == valeur &&
                notes[i].coef == coef &&
                notes[i].appreciation == appreciation) {
                return;
            }
        }
        ajouterNote(etudiantId, matiereId, valeur, coef, appreciation);
    };

    const auto ensureAbsence = [this](int etudiantId, const string& date,
                                      const string& heure, const string& motif,
                                      bool justifie) {
        vector<AbsenceDB> absences = getAbsences(etudiantId);
        for (int i = 0; i < int(absences.size()); ++i) {
            if (absences[i].date == date &&
                absences[i].heure == heure &&
                absences[i].motif == motif &&
                absences[i].justifie == justifie) {
                return;
            }
        }
        ajouterAbsence(etudiantId, date, heure, motif, justifie);
    };

    const int matFrancais = ensureMatiere("Francais", "FRA101", 3.0f);
    const int matHgg = ensureMatiere("Histoire-Geographie", "HGG101", 3.0f);
    const int matAnglais = ensureMatiere("Anglais", "ANG101", 2.0f);
    const int matEps = ensureMatiere("EPS", "EPS101", 1.0f);
    const int matEmc = ensureMatiere("EMC", "EMC101", 0.5f);
    const int matMaths = ensureMatiere("Mathematiques", "MAT301", 4.0f);
    const int matPc = ensureMatiere("Physique-Chimie", "PHY301", 4.0f);
    const int matNsi = ensureMatiere("NSI", "NSI301", 4.0f);
    const int matPhilo = ensureMatiere("Philosophie", "PHI401", 4.0f);
    const int matMathsExpertes = ensureMatiere("Maths expertes", "MEX401", 2.0f);
    const int matDgemc = ensureMatiere("DGEMC", "DGM401", 2.0f);
    const int matLatin = ensureMatiere("Latin", "LAT201", 1.0f);

    const int profMaths = ensureProf("Durand", "Lea", "abcd");
    const int profPc = ensureProf("Benali", "Karim", "123456");
    const int profHgg = ensureProf("Martin", "Claire", "hgg123");
    const int profFrancais = ensureProf("Laurent", "Sophie", "fra123");
    const int profAnglais = ensureProf("Diallo", "Nora", "ang123");
    const int profNsi = ensureProf("Riviere", "Julien", "nsi123");
    const int profPhilo = ensureProf("Petit", "Camille", "phi123");
    const int profLatin = ensureProf("Lefevre", "Alice", "lat123");

    const int classeSeconde = ensureClasse("2nde Generale A", "Seconde", "Generale");
    const int classePremiere = ensureClasse("1re Generale B", "Premiere", "Generale");
    const int classeTerminale = ensureClasse("Tle Generale C", "Terminale", "Generale");

    affecterMatiereClasse(classeSeconde, matFrancais, profFrancais, true);
    affecterMatiereClasse(classeSeconde, matHgg, profHgg, true);
    affecterMatiereClasse(classeSeconde, matAnglais, profAnglais, true);
    affecterMatiereClasse(classeSeconde, matEps, profHgg, true);
    affecterMatiereClasse(classeSeconde, matMaths, profMaths, true);
    affecterMatiereClasse(classeSeconde, matPc, profPc, true);
    affecterMatiereClasse(classeSeconde, matLatin, profLatin, false);

    affecterMatiereClasse(classePremiere, matFrancais, profFrancais, true);
    affecterMatiereClasse(classePremiere, matHgg, profHgg, true);
    affecterMatiereClasse(classePremiere, matAnglais, profAnglais, true);
    affecterMatiereClasse(classePremiere, matEps, profHgg, true);
    affecterMatiereClasse(classePremiere, matEmc, profHgg, true);
    affecterMatiereClasse(classePremiere, matMaths, profMaths, true);
    affecterMatiereClasse(classePremiere, matPc, profPc, true);
    affecterMatiereClasse(classePremiere, matNsi, profNsi, true);
    affecterMatiereClasse(classePremiere, matLatin, profLatin, false);

    affecterMatiereClasse(classeTerminale, matPhilo, profPhilo, true);
    affecterMatiereClasse(classeTerminale, matHgg, profHgg, true);
    affecterMatiereClasse(classeTerminale, matAnglais, profAnglais, true);
    affecterMatiereClasse(classeTerminale, matEps, profHgg, true);
    affecterMatiereClasse(classeTerminale, matEmc, profHgg, true);
    affecterMatiereClasse(classeTerminale, matMaths, profMaths, true);
    affecterMatiereClasse(classeTerminale, matPc, profPc, true);
    affecterMatiereClasse(classeTerminale, matMathsExpertes, profMaths, false);
    affecterMatiereClasse(classeTerminale, matDgemc, profPhilo, false);

    const int etuSeydina = ensureEtudiant("Diarra", "Seydina", "123456");
    const int etuInes = ensureEtudiant("Bernard", "Ines", "eleve123");
    const int etuLucas = ensureEtudiant("Moreau", "Lucas", "eleve123");
    const int etuSarah = ensureEtudiant("Diallo", "Sarah", "eleve123");
    const int etuEmma = ensureEtudiant("Lefort", "Emma", "eleve123");
    const int etuYanis = ensureEtudiant("Petit", "Yanis", "eleve123");
    const int etuNina = ensureEtudiant("Garcia", "Nina", "eleve123");
    const int etuTheo = ensureEtudiant("Roux", "Theo", "eleve123");

    assignerEtudiantClasse(etuNina, classeSeconde);
    assignerEtudiantClasse(etuTheo, classeSeconde);
    assignerEtudiantClasse(etuSeydina, classePremiere);
    assignerEtudiantClasse(etuInes, classePremiere);
    assignerEtudiantClasse(etuLucas, classePremiere);
    assignerEtudiantClasse(etuSarah, classeTerminale);
    assignerEtudiantClasse(etuEmma, classeTerminale);
    assignerEtudiantClasse(etuYanis, classeTerminale);

    inscrireEtudiantMatiere(etuNina, matLatin);
    inscrireEtudiantMatiere(etuInes, matLatin);
    inscrireEtudiantMatiere(etuSarah, matMathsExpertes);
    inscrireEtudiantMatiere(etuEmma, matDgemc);

    ensureCours("lundi", "08:00", "10:00", "A101", classeSeconde, matFrancais, profFrancais);
    ensureCours("mardi", "10:15", "12:15", "B12", classeSeconde, matMaths, profMaths);
    ensureCours("jeudi", "14:00", "15:30", "Gymnase", classeSeconde, matEps, profHgg);
    ensureCours("vendredi", "15:45", "17:00", "B104", classeSeconde, matLatin, profLatin);

    ensureCours("lundi", "08:00", "10:00", "B12", classePremiere, matMaths, profMaths);
    ensureCours("lundi", "10:15", "12:15", "Labo 2", classePremiere, matPc, profPc);
    ensureCours("mardi", "09:00", "11:00", "C201", classePremiere, matFrancais, profFrancais);
    ensureCours("jeudi", "14:00", "16:00", "Info 1", classePremiere, matNsi, profNsi);
    ensureCours("vendredi", "14:00", "15:30", "B104", classePremiere, matLatin, profLatin);

    ensureCours("lundi", "13:00", "15:00", "A203", classeTerminale, matPhilo, profPhilo);
    ensureCours("mardi", "08:00", "10:00", "B12", classeTerminale, matMaths, profMaths);
    ensureCours("mardi", "10:15", "12:15", "Labo 2", classeTerminale, matPc, profPc);
    ensureCours("jeudi", "09:00", "11:00", "C105", classeTerminale, matHgg, profHgg);
    ensureCours("vendredi", "14:00", "16:00", "B12", classeTerminale, matMathsExpertes, profMaths);

    ensureActualite(
        "Rentree 2026",
        "Bienvenue au lycee. Les emplois du temps des classes sont disponibles dans Kalanso.",
        "2026-09-02"
    );
    ensureActualite(
        "Orientation",
        "Une reunion d'information sur Parcoursup et l'orientation est prevue pour les classes de Terminale.",
        "2026-09-10"
    );
    ensureActualite(
        "Conseils de classe",
        "Les conseils de classe de Premieres et Terminales auront lieu la semaine du 5 octobre.",
        "2026-10-01"
    );

    ensureNote(etuNina, matFrancais, 15.0f, 1.0f, "Bon demarrage");
    ensureNote(etuSeydina, matMaths, 14.5f, 1.0f, "Resultat serieux");
    ensureNote(etuSeydina, matPc, 13.0f, 1.0f, "Bon raisonnement");
    ensureNote(etuInes, matFrancais, 16.0f, 1.0f, "Expression solide");
    ensureNote(etuSarah, matPhilo, 15.0f, 1.0f, "Analyse pertinente");
    ensureNote(etuEmma, matMaths, 12.5f, 1.0f, "Peut gagner en rigueur");
    ensureNote(etuYanis, matAnglais, 11.5f, 1.0f, "Participation reguliere");

    ensureAbsence(etuLucas, "2026-09-12", "10:15", "Rendez-vous medical", true);
    ensureAbsence(etuYanis, "2026-09-15", "08:00", "Retard transport", false);
    ensureAbsence(etuTheo, "2026-09-18", "15:45", "Absence non justifiee", false);
}

void Database::init() {
    if (db != nullptr) {
        close();
    }

    int rc = sqlite3_open("kalanso.db", &db);
    if (rc != SQLITE_OK) {
        cerr << "[DB ERREUR] Impossible d'ouvrir kalanso.db : "
             << sqlite3_errmsg(db) << "\n";
        if (db != nullptr) {
            sqlite3_close(db);
            db = nullptr;
        }
        return;
    }

    cout << "[DB] kalanso.db ouvert avec succes.\n";

    bool ok = true;
    ok = execSQL("PRAGMA foreign_keys = ON;") && ok;

    ok = execSQL(
        "CREATE TABLE IF NOT EXISTS personnes ("
        "  id           INTEGER PRIMARY KEY AUTOINCREMENT,"
        "  identifiant  TEXT UNIQUE,"
        "  nom          TEXT NOT NULL,"
        "  prenom       TEXT NOT NULL,"
        "  mot_de_passe TEXT NOT NULL,"
        "  role         TEXT NOT NULL CHECK(role IN ('etudiant', 'professeur', 'admin')),"
        "  photo_path   TEXT,"
        "  classe_id    INTEGER,"
        "  FOREIGN KEY (classe_id) REFERENCES classes(id) ON DELETE SET NULL"
        ");"
    ) && ok;

    ok = execSQL(
        "CREATE TABLE IF NOT EXISTS classes ("
        "  id     INTEGER PRIMARY KEY AUTOINCREMENT,"
        "  nom    TEXT NOT NULL UNIQUE,"
        "  niveau TEXT NOT NULL,"
        "  voie   TEXT NOT NULL"
        ");"
    ) && ok;

    if (!tableHasColumn("personnes", "photo_path")) {
        ok = execSQL("ALTER TABLE personnes ADD COLUMN photo_path TEXT;") && ok;
    }
    if (!tableHasColumn("personnes", "classe_id")) {
        ok = execSQL("ALTER TABLE personnes ADD COLUMN classe_id INTEGER;") && ok;
    }

    ok = execSQL(
        "CREATE TABLE IF NOT EXISTS matieres ("
        "  id     INTEGER PRIMARY KEY AUTOINCREMENT,"
        "  nom    TEXT NOT NULL,"
        "  apogee TEXT,"
        "  coef   REAL DEFAULT 1.0 CHECK(coef > 0)"
        ");"
    ) && ok;

    ok = execSQL(
        "CREATE TABLE IF NOT EXISTS prof_matiere ("
        "  prof_id    INTEGER NOT NULL,"
        "  matiere_id INTEGER NOT NULL,"
        "  PRIMARY KEY (prof_id, matiere_id),"
        "  FOREIGN KEY (prof_id) REFERENCES personnes(id) ON DELETE CASCADE,"
        "  FOREIGN KEY (matiere_id) REFERENCES matieres(id) ON DELETE CASCADE"
        ");"
    ) && ok;

    ok = execSQL(
        "CREATE TABLE IF NOT EXISTS etudiant_matiere ("
        "  etudiant_id INTEGER NOT NULL,"
        "  matiere_id  INTEGER NOT NULL,"
        "  PRIMARY KEY (etudiant_id, matiere_id),"
        "  FOREIGN KEY (etudiant_id) REFERENCES personnes(id) ON DELETE CASCADE,"
        "  FOREIGN KEY (matiere_id) REFERENCES matieres(id) ON DELETE CASCADE"
        ");"
    ) && ok;

    ok = execSQL(
        "CREATE TABLE IF NOT EXISTS classe_matieres ("
        "  classe_id    INTEGER NOT NULL,"
        "  matiere_id   INTEGER NOT NULL,"
        "  prof_id      INTEGER NOT NULL,"
        "  obligatoire  INTEGER NOT NULL DEFAULT 1 CHECK(obligatoire IN (0, 1)),"
        "  PRIMARY KEY (classe_id, matiere_id),"
        "  FOREIGN KEY (classe_id) REFERENCES classes(id) ON DELETE CASCADE,"
        "  FOREIGN KEY (matiere_id) REFERENCES matieres(id) ON DELETE CASCADE,"
        "  FOREIGN KEY (prof_id) REFERENCES personnes(id) ON DELETE CASCADE"
        ");"
    ) && ok;

    ok = execSQL(
        "CREATE TABLE IF NOT EXISTS notes ("
        "  id           INTEGER PRIMARY KEY AUTOINCREMENT,"
        "  valeur       REAL NOT NULL CHECK(valeur >= 0 AND valeur <= 20),"
        "  coef         REAL DEFAULT 1.0 CHECK(coef > 0),"
        "  appreciation TEXT,"
        "  etudiant_id  INTEGER NOT NULL,"
        "  matiere_id   INTEGER NOT NULL,"
        "  FOREIGN KEY (etudiant_id) REFERENCES personnes(id) ON DELETE CASCADE,"
        "  FOREIGN KEY (matiere_id) REFERENCES matieres(id) ON DELETE CASCADE"
        ");"
    ) && ok;

    ok = execSQL(
        "CREATE TABLE IF NOT EXISTS absences ("
        "  id          INTEGER PRIMARY KEY AUTOINCREMENT,"
        "  date        TEXT NOT NULL,"
        "  heure       TEXT,"
        "  motif       TEXT,"
        "  justifie    INTEGER DEFAULT 0 CHECK(justifie IN (0, 1)),"
        "  etudiant_id INTEGER NOT NULL,"
        "  FOREIGN KEY (etudiant_id) REFERENCES personnes(id) ON DELETE CASCADE"
        ");"
    ) && ok;

    ok = execSQL(
        "CREATE TABLE IF NOT EXISTS cours ("
        "  id          INTEGER PRIMARY KEY AUTOINCREMENT,"
        "  jour        TEXT NOT NULL,"
        "  heure_debut TEXT NOT NULL,"
        "  heure_fin   TEXT NOT NULL,"
        "  salle       TEXT,"
        "  classe_id   INTEGER,"
        "  matiere_id  INTEGER NOT NULL,"
        "  prof_id     INTEGER NOT NULL,"
        "  FOREIGN KEY (classe_id) REFERENCES classes(id) ON DELETE CASCADE,"
        "  FOREIGN KEY (matiere_id) REFERENCES matieres(id) ON DELETE CASCADE,"
        "  FOREIGN KEY (prof_id) REFERENCES personnes(id) ON DELETE CASCADE"
        ");"
    ) && ok;

    if (!tableHasColumn("cours", "classe_id")) {
        ok = execSQL("ALTER TABLE cours ADD COLUMN classe_id INTEGER;") && ok;
    }

    ok = execSQL(
        "CREATE TABLE IF NOT EXISTS actualites ("
        "  id               INTEGER PRIMARY KEY AUTOINCREMENT,"
        "  titre            TEXT NOT NULL,"
        "  contenu          TEXT,"
        "  date_publication TEXT"
        ");"
    ) && ok;

    ok = execSQL(
        "CREATE INDEX IF NOT EXISTS idx_notes_etudiant ON notes(etudiant_id);"
    ) && ok;
    ok = execSQL(
        "CREATE INDEX IF NOT EXISTS idx_notes_matiere ON notes(matiere_id);"
    ) && ok;
    ok = execSQL(
        "CREATE INDEX IF NOT EXISTS idx_absences_etudiant ON absences(etudiant_id);"
    ) && ok;
    ok = execSQL(
        "CREATE INDEX IF NOT EXISTS idx_cours_prof ON cours(prof_id);"
    ) && ok;
    ok = execSQL(
        "CREATE INDEX IF NOT EXISTS idx_cours_matiere ON cours(matiere_id);"
    ) && ok;
    ok = execSQL(
        "CREATE INDEX IF NOT EXISTS idx_cours_classe ON cours(classe_id);"
    ) && ok;
    ok = execSQL(
        "CREATE INDEX IF NOT EXISTS idx_cours_jour_heure ON cours(jour, heure_debut);"
    ) && ok;
    ok = execSQL(
        "CREATE INDEX IF NOT EXISTS idx_personnes_classe ON personnes(classe_id);"
    ) && ok;
    ok = ensureIdentifiantsPersonnes() && ok;

    if (ok) {
        sqlite3_stmt* stmt = nullptr;
        if (prepareStatement(
                "SELECT COUNT(*) FROM personnes WHERE role = 'admin';",
                &stmt)) {
            int adminCount = 0;
            if (sqlite3_step(stmt) == SQLITE_ROW) {
                adminCount = sqlite3_column_int(stmt, 0);
            }
            sqlite3_finalize(stmt);

            if (adminCount == 0) {
                const int adminId = insererPersonne("Systeme", "Admin", "admin", ROLE_ADMIN);
                if (adminId > 0) {
                    cout << "[DB] Compte admin par defaut cree : "
                         << getIdentifiantParId(adminId) << " / admin\n";
                } else {
                    ok = false;
                }
            }
        } else {
            ok = false;
        }
    }

    if (ok) {
        initialiserDonneesCoherentes();
    }

    if (ok) {
        cout << "[DB] Tables initialisees.\n";
    } else {
        cerr << "[DB ERREUR] Initialisation incomplete de la base.\n";
    }
    sqlite3_exec(db,"INSERT OR IGNORE INTO personnes (identifiant, nom, prenom, mot_de_passe, role) ""VALUES ('testadmin', 'Martin', 'Jean', '1234', 'admin');",nullptr, nullptr, nullptr);
    sqlite3_exec(db,"INSERT OR IGNORE INTO personnes (identifiant, nom, prenom, mot_de_passe, role) ""VALUES ('testprof', 'Harald', 'Matthiews', '12345', 'professeur');",nullptr, nullptr, nullptr);
    sqlite3_exec(db,"INSERT OR IGNORE INTO personnes (identifiant, nom, prenom, mot_de_passe, role) ""VALUES ('testetudiant', 'Morne', 'Silver', '123456', 'etudiant');",nullptr, nullptr, nullptr);
    sqlite3_exec(db, "INSERT INTO notes (valeur, coef, appreciation, etudiant_id, matiere_id) " "VALUES (18, 2, 'très bien', 7, 1);", nullptr, nullptr, nullptr);
}

void Database::close() {
    if (db == nullptr) {
        return;
    }

    int rc = sqlite3_close(db);
    if (rc != SQLITE_OK) {
        cerr << "[DB ERREUR] Impossible de fermer la connexion : "
             << sqlite3_errmsg(db) << "\n";
        return;
    }

    db = nullptr;
    cout << "[DB] Connexion fermee.\n";
}

PersonneDB Database::authentifier(const string& identifiant, const string& mdp) {
    PersonneDB p {-1, "", "", "", "", "", "", -1, ""};

    sqlite3_stmt* stmt = nullptr;
    const string sql =
        "SELECT p.id, p.identifiant, p.nom, p.prenom, p.role, p.photo_path, "
        "       COALESCE(p.classe_id, -1), COALESCE(c.nom, '') "
        "FROM personnes p "
        "LEFT JOIN classes c ON c.id = p.classe_id "
        "WHERE UPPER(identifiant) = UPPER(?) AND mot_de_passe = ?;";

    if (!prepareStatement(sql, &stmt)) {
        return p;
    }

    sqlite3_bind_text(stmt, 1, identifiant.c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt, 2, mdp.c_str(), -1, SQLITE_TRANSIENT);

    if (sqlite3_step(stmt) == SQLITE_ROW) {
        p.id           = sqlite3_column_int(stmt, 0);
        p.identifiant  = uppercaseCopy(textColumn(stmt, 1));
        p.nom          = textColumn(stmt, 2);
        p.prenom       = textColumn(stmt, 3);
        p.mot_de_passe = "";
        p.role         = textColumn(stmt, 4);
        p.photo_path   = textColumn(stmt, 5);
        p.classe_id    = sqlite3_column_int(stmt, 6);
        p.classe_nom   = textColumn(stmt, 7);
    }

    sqlite3_finalize(stmt);
    return p;
}

int Database::creerEtudiant(const string& nom, const string& prenom, const string& mdp) {
    return insererPersonne(nom, prenom, mdp, ROLE_ETUDIANT);
}

int Database::creerProfesseur(const string& nom, const string& prenom, const string& mdp) {
    return insererPersonne(nom, prenom, mdp, ROLE_PROFESSEUR);
}

int Database::creerAdmin(const string& nom, const string& prenom, const string& mdp) {
    return insererPersonne(nom, prenom, mdp, ROLE_ADMIN);
}

void Database::supprimerCompte(int personneId) {
    if (!ensureOpen("supprimerCompte")) {
        return;
    }
    if (!personneExiste(personneId)) {
        cerr << "[DB ERREUR] Aucun utilisateur avec l'id " << personneId << ".\n";
        return;
    }

    if (!execSQL("BEGIN TRANSACTION;")) {
        return;
    }

    auto deleteByPersonId = [&](const string& sql, const string& contexte) -> bool {
        sqlite3_stmt* stmt = nullptr;
        if (!prepareStatement(sql, &stmt)) {
            return false;
        }
        sqlite3_bind_int(stmt, 1, personneId);
        bool ok = executeNonQuery(stmt, contexte);
        sqlite3_finalize(stmt);
        return ok;
    };

    bool ok = true;
    ok = deleteByPersonId(
             "DELETE FROM notes WHERE etudiant_id = ?;",
             "suppression notes utilisateur") && ok;
    ok = deleteByPersonId(
             "DELETE FROM absences WHERE etudiant_id = ?;",
             "suppression absences utilisateur") && ok;
    ok = deleteByPersonId(
             "DELETE FROM cours WHERE prof_id = ?;",
             "suppression cours professeur") && ok;
    ok = deleteByPersonId(
             "DELETE FROM etudiant_matiere WHERE etudiant_id = ?;",
             "suppression inscriptions etudiant") && ok;
    ok = deleteByPersonId(
             "DELETE FROM prof_matiere WHERE prof_id = ?;",
             "suppression affectations professeur") && ok;
    ok = deleteByPersonId(
             "DELETE FROM personnes WHERE id = ?;",
             "suppression utilisateur") && ok;

    if (ok) {
        execSQL("COMMIT;");
    } else {
        execSQL("ROLLBACK;");
    }
}

vector<PersonneDB> Database::getTousEtudiants() {
    vector<PersonneDB> liste;
    sqlite3_stmt* stmt = nullptr;
    const string sql =
        "SELECT p.id, p.identifiant, p.nom, p.prenom, p.role, p.photo_path, "
        "       COALESCE(p.classe_id, -1), COALESCE(c.nom, '') "
        "FROM personnes p "
        "LEFT JOIN classes c ON c.id = p.classe_id "
        "WHERE p.role = 'etudiant' "
        "ORDER BY p.nom, p.prenom;";

    if (!prepareStatement(sql, &stmt)) {
        return liste;
    }

    while (sqlite3_step(stmt) == SQLITE_ROW) {
        PersonneDB p;
        p.id           = sqlite3_column_int(stmt, 0);
        p.identifiant  = uppercaseCopy(textColumn(stmt, 1));
        p.nom          = textColumn(stmt, 2);
        p.prenom       = textColumn(stmt, 3);
        p.mot_de_passe = "";
        p.role         = textColumn(stmt, 4);
        p.photo_path   = textColumn(stmt, 5);
        p.classe_id    = sqlite3_column_int(stmt, 6);
        p.classe_nom   = textColumn(stmt, 7);
        liste.push_back(p);
    }

    sqlite3_finalize(stmt);
    return liste;
}

vector<PersonneDB> Database::getTousProfesseurs() {
    vector<PersonneDB> liste;
    sqlite3_stmt* stmt = nullptr;
    const string sql =
        "SELECT p.id, p.identifiant, p.nom, p.prenom, p.role, p.photo_path, "
        "       COALESCE(p.classe_id, -1), COALESCE(c.nom, '') "
        "FROM personnes p "
        "LEFT JOIN classes c ON c.id = p.classe_id "
        "WHERE p.role = 'professeur' "
        "ORDER BY p.nom, p.prenom;";

    if (!prepareStatement(sql, &stmt)) {
        return liste;
    }

    while (sqlite3_step(stmt) == SQLITE_ROW) {
        PersonneDB p;
        p.id           = sqlite3_column_int(stmt, 0);
        p.identifiant  = uppercaseCopy(textColumn(stmt, 1));
        p.nom          = textColumn(stmt, 2);
        p.prenom       = textColumn(stmt, 3);
        p.mot_de_passe = "";
        p.role         = textColumn(stmt, 4);
        p.photo_path   = textColumn(stmt, 5);
        p.classe_id    = sqlite3_column_int(stmt, 6);
        p.classe_nom   = textColumn(stmt, 7);
        liste.push_back(p);
    }

    sqlite3_finalize(stmt);
    return liste;
}

vector<PersonneDB> Database::getTousUtilisateurs() {
    vector<PersonneDB> liste;
    sqlite3_stmt* stmt = nullptr;
    const string sql =
        "SELECT p.id, p.identifiant, p.nom, p.prenom, p.role, p.photo_path, "
        "       COALESCE(p.classe_id, -1), COALESCE(c.nom, '') "
        "FROM personnes p "
        "LEFT JOIN classes c ON c.id = p.classe_id "
        "ORDER BY p.role, p.nom, p.prenom;";

    if (!prepareStatement(sql, &stmt)) {
        return liste;
    }

    while (sqlite3_step(stmt) == SQLITE_ROW) {
        PersonneDB p;
        p.id           = sqlite3_column_int(stmt, 0);
        p.identifiant  = uppercaseCopy(textColumn(stmt, 1));
        p.nom          = textColumn(stmt, 2);
        p.prenom       = textColumn(stmt, 3);
        p.mot_de_passe = "";
        p.role         = textColumn(stmt, 4);
        p.photo_path   = textColumn(stmt, 5);
        p.classe_id    = sqlite3_column_int(stmt, 6);
        p.classe_nom   = textColumn(stmt, 7);
        liste.push_back(p);
    }

    sqlite3_finalize(stmt);
    return liste;
}

PersonneDB Database::getPersonneParId(int personneId) {
    PersonneDB personne {-1, "", "", "", "", "", "", -1, ""};

    sqlite3_stmt* stmt = nullptr;
    if (!prepareStatement(
            "SELECT p.id, p.identifiant, p.nom, p.prenom, p.role, p.photo_path, "
            "       COALESCE(p.classe_id, -1), COALESCE(c.nom, '') "
            "FROM personnes p "
            "LEFT JOIN classes c ON c.id = p.classe_id "
            "WHERE p.id = ?;",
            &stmt)) {
        return personne;
    }

    sqlite3_bind_int(stmt, 1, personneId);

    if (sqlite3_step(stmt) == SQLITE_ROW) {
        personne.id           = sqlite3_column_int(stmt, 0);
        personne.identifiant  = uppercaseCopy(textColumn(stmt, 1));
        personne.nom          = textColumn(stmt, 2);
        personne.prenom       = textColumn(stmt, 3);
        personne.mot_de_passe = "";
        personne.role         = textColumn(stmt, 4);
        personne.photo_path   = textColumn(stmt, 5);
        personne.classe_id    = sqlite3_column_int(stmt, 6);
        personne.classe_nom   = textColumn(stmt, 7);
    }

    sqlite3_finalize(stmt);
    return personne;
}

bool Database::modifierPhotoProfil(int personneId, const string& photoPath) {
    if (!ensureOpen("modifierPhotoProfil") || !personneExiste(personneId)) {
        return false;
    }

    sqlite3_stmt* stmt = nullptr;
    if (!prepareStatement(
            "UPDATE personnes SET photo_path = ? WHERE id = ?;",
            &stmt)) {
        return false;
    }

    sqlite3_bind_text(stmt, 1, photoPath.c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_int(stmt, 2, personneId);

    const bool ok = executeNonQuery(stmt, "mise a jour photo profil");
    sqlite3_finalize(stmt);
    return ok;
}

bool Database::assignerEtudiantClasse(int etudiantId, int classeId) {
    if (!personneAvecRoleExiste(etudiantId, ROLE_ETUDIANT) || !classeExiste(classeId)) {
        return false;
    }

    sqlite3_stmt* stmt = nullptr;
    if (!prepareStatement(
            "UPDATE personnes SET classe_id = ? WHERE id = ?;",
            &stmt)) {
        return false;
    }

    sqlite3_bind_int(stmt, 1, classeId);
    sqlite3_bind_int(stmt, 2, etudiantId);

    const bool ok = executeNonQuery(stmt, "assignation etudiant-classe");
    sqlite3_finalize(stmt);
    if (ok) {
        synchroniserMatieresEtudiantDepuisClasse(etudiantId, classeId);
    }
    return ok;
}

int Database::getPersonneIdParIdentifiant(const string& identifiant) {
    sqlite3_stmt* stmt = nullptr;
    if (!prepareStatement(
            "SELECT id FROM personnes WHERE UPPER(identifiant) = UPPER(?);",
            &stmt)) {
        return -1;
    }

    sqlite3_bind_text(stmt, 1, identifiant.c_str(), -1, SQLITE_TRANSIENT);

    int personneId = -1;
    if (sqlite3_step(stmt) == SQLITE_ROW) {
        personneId = sqlite3_column_int(stmt, 0);
    }

    sqlite3_finalize(stmt);
    return personneId;
}

string Database::getIdentifiantParId(int personneId) {
    sqlite3_stmt* stmt = nullptr;
    if (!prepareStatement(
            "SELECT identifiant FROM personnes WHERE id = ?;",
            &stmt)) {
        return "";
    }

    sqlite3_bind_int(stmt, 1, personneId);

    string identifiant;
    if (sqlite3_step(stmt) == SQLITE_ROW) {
        identifiant = uppercaseCopy(textColumn(stmt, 0));
    }

    sqlite3_finalize(stmt);
    return identifiant;
}

bool Database::modifierMotDePasse(int personneId, const string& nouveauMdp) {
    if (!ensureOpen("modifierMotDePasse")) {
        return false;
    }
    if (!personneExiste(personneId)) {
        cerr << "[DB ERREUR] Aucun utilisateur avec l'id " << personneId << ".\n";
        return false;
    }
    if (nouveauMdp.empty()) {
        cerr << "[DB ERREUR] Le mot de passe ne peut pas etre vide.\n";
        return false;
    }

    sqlite3_stmt* stmt = nullptr;
    if (!prepareStatement(
            "UPDATE personnes SET mot_de_passe = ? WHERE id = ?;",
            &stmt)) {
        return false;
    }

    sqlite3_bind_text(stmt, 1, nouveauMdp.c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_int(stmt, 2, personneId);

    bool ok = executeNonQuery(stmt, "mise a jour mot de passe");
    sqlite3_finalize(stmt);
    return ok;
}

void Database::ajouterNote(int etudiantId, int matiereId, float valeur,
                           float coef, const string& appreciation) {
    if (!personneAvecRoleExiste(etudiantId, ROLE_ETUDIANT)) {
        cerr << "[DB ERREUR] L'id " << etudiantId << " n'est pas un etudiant.\n";
        return;
    }
    if (!matiereExiste(matiereId)) {
        cerr << "[DB ERREUR] La matiere " << matiereId << " n'existe pas.\n";
        return;
    }
    if (!etudiantInscritMatiere(etudiantId, matiereId)) {
        cerr << "[DB ERREUR] Etudiant non inscrit a cette matiere.\n";
        return;
    }
    if (valeur < 0.0f || valeur > 20.0f) {
        cerr << "[DB ERREUR] La note doit etre comprise entre 0 et 20.\n";
        return;
    }
    if (coef <= 0.0f) {
        cerr << "[DB ERREUR] Le coefficient doit etre strictement positif.\n";
        return;
    }

    sqlite3_stmt* stmt = nullptr;
    const string sql =
        "INSERT INTO notes (valeur, coef, appreciation, etudiant_id, matiere_id) "
        "VALUES (?, ?, ?, ?, ?);";

    if (!prepareStatement(sql, &stmt)) {
        return;
    }

    sqlite3_bind_double(stmt, 1, valeur);
    sqlite3_bind_double(stmt, 2, coef);
    sqlite3_bind_text(stmt, 3, appreciation.c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_int(stmt, 4, etudiantId);
    sqlite3_bind_int(stmt, 5, matiereId);

    executeNonQuery(stmt, "ajout note");
    sqlite3_finalize(stmt);
}

void Database::modifierNote(int noteId, float nouvelleValeur) {
    if (nouvelleValeur < 0.0f || nouvelleValeur > 20.0f) {
        cerr << "[DB ERREUR] La note doit etre comprise entre 0 et 20.\n";
        return;
    }

    sqlite3_stmt* stmt = nullptr;
    if (!prepareStatement(
            "UPDATE notes SET valeur = ? WHERE id = ?;",
            &stmt)) {
        return;
    }

    sqlite3_bind_double(stmt, 1, nouvelleValeur);
    sqlite3_bind_int(stmt, 2, noteId);

    bool ok = executeNonQuery(stmt, "modification note");
    int changes = ok ? sqlite3_changes(db) : 0;
    sqlite3_finalize(stmt);

    if (ok && changes == 0) {
        cerr << "[DB ERREUR] Aucune note avec l'id " << noteId << ".\n";
    }
}

void Database::supprimerNote(int noteId) {
    sqlite3_stmt* stmt = nullptr;
    if (!prepareStatement("DELETE FROM notes WHERE id = ?;", &stmt)) {
        return;
    }

    sqlite3_bind_int(stmt, 1, noteId);
    bool ok = executeNonQuery(stmt, "suppression note");
    int changes = ok ? sqlite3_changes(db) : 0;
    sqlite3_finalize(stmt);

    if (ok && changes == 0) {
        cerr << "[DB ERREUR] Aucune note avec l'id " << noteId << ".\n";
    }
}

vector<NoteDB> Database::getNotes(int etudiantId) {
    vector<NoteDB> liste;
    if (!personneAvecRoleExiste(etudiantId, ROLE_ETUDIANT)) {
        return liste;
    }

    sqlite3_stmt* stmt = nullptr;
    const string sql =
        "SELECT n.id, n.valeur, n.coef, n.appreciation, m.nom, n.etudiant_id, n.matiere_id "
        "FROM notes n "
        "JOIN matieres m ON n.matiere_id = m.id "
        "WHERE n.etudiant_id = ? "
        "ORDER BY m.nom, n.id;";

    if (!prepareStatement(sql, &stmt)) {
        return liste;
    }

    sqlite3_bind_int(stmt, 1, etudiantId);

    while (sqlite3_step(stmt) == SQLITE_ROW) {
        NoteDB n;
        n.id           = sqlite3_column_int(stmt, 0);
        n.valeur       = static_cast<float>(sqlite3_column_double(stmt, 1));
        n.coef         = static_cast<float>(sqlite3_column_double(stmt, 2));
        n.appreciation = textColumn(stmt, 3);
        n.matiere      = textColumn(stmt, 4);
        n.etudiant_id  = sqlite3_column_int(stmt, 5);
        n.matiere_id   = sqlite3_column_int(stmt, 6);
        liste.push_back(n);
    }

    sqlite3_finalize(stmt);
    return liste;
}

vector<NoteDB> Database::getAllNotes()
{
    vector<NoteDB> liste;

    sqlite3_stmt* stmt = nullptr;

    const string sql =
        "SELECT n.id, n.valeur, n.coef, n.appreciation, m.nom, n.etudiant_id, n.matiere_id "
        "FROM notes n "
        "JOIN matieres m ON n.matiere_id = m.id "
        "ORDER BY m.nom, n.id;";

    if (!prepareStatement(sql, &stmt))
        return liste;

    while (sqlite3_step(stmt) == SQLITE_ROW)
    {
        NoteDB n;
        n.id           = sqlite3_column_int(stmt, 0);
        n.valeur       = sqlite3_column_double(stmt, 1);
        n.coef         = sqlite3_column_double(stmt, 2);
        n.appreciation = textColumn(stmt, 3);
        n.matiere      = textColumn(stmt, 4);
        n.etudiant_id  = sqlite3_column_int(stmt, 5);
        n.matiere_id   = sqlite3_column_int(stmt, 6);

        liste.push_back(n);
    }

    sqlite3_finalize(stmt);
    return liste;
}

vector<NoteDB> Database::getNotesByMatiere(int etudiantId, int matiereId) {
    vector<NoteDB> liste;
    if (!personneAvecRoleExiste(etudiantId, ROLE_ETUDIANT) || !matiereExiste(matiereId)) {
        return liste;
    }

    sqlite3_stmt* stmt = nullptr;
    const string sql =
        "SELECT n.id, n.valeur, n.coef, n.appreciation, m.nom, n.etudiant_id, n.matiere_id "
        "FROM notes n "
        "JOIN matieres m ON n.matiere_id = m.id "
        "WHERE n.etudiant_id = ? AND n.matiere_id = ? "
        "ORDER BY n.id;";

    if (!prepareStatement(sql, &stmt)) {
        return liste;
    }

    sqlite3_bind_int(stmt, 1, etudiantId);
    sqlite3_bind_int(stmt, 2, matiereId);

    while (sqlite3_step(stmt) == SQLITE_ROW) {
        NoteDB n;
        n.id           = sqlite3_column_int(stmt, 0);
        n.valeur       = static_cast<float>(sqlite3_column_double(stmt, 1));
        n.coef         = static_cast<float>(sqlite3_column_double(stmt, 2));
        n.appreciation = textColumn(stmt, 3);
        n.matiere      = textColumn(stmt, 4);
        n.etudiant_id  = sqlite3_column_int(stmt, 5);
        n.matiere_id   = sqlite3_column_int(stmt, 6);
        liste.push_back(n);
    }

    sqlite3_finalize(stmt);
    return liste;
}

float Database::getMoyenne(int etudiantId, int matiereId) {
    if (!personneAvecRoleExiste(etudiantId, ROLE_ETUDIANT) || !matiereExiste(matiereId)) {
        return -1.0f;
    }

    sqlite3_stmt* stmt = nullptr;
    const string sql =
        "SELECT SUM(valeur * coef) / SUM(coef) "
        "FROM notes "
        "WHERE etudiant_id = ? AND matiere_id = ?;";

    if (!prepareStatement(sql, &stmt)) {
        return -1.0f;
    }

    sqlite3_bind_int(stmt, 1, etudiantId);
    sqlite3_bind_int(stmt, 2, matiereId);

    float moyenne = -1.0f;
    if (sqlite3_step(stmt) == SQLITE_ROW && sqlite3_column_type(stmt, 0) != SQLITE_NULL) {
        moyenne = static_cast<float>(sqlite3_column_double(stmt, 0));
    }

    sqlite3_finalize(stmt);
    return moyenne;
}

void Database::ajouterAbsence(int etudiantId, const string& date,
                              const string& heure, const string& motif,
                              bool justifie) {
    if (!personneAvecRoleExiste(etudiantId, ROLE_ETUDIANT)) {
        cerr << "[DB ERREUR] L'id " << etudiantId << " n'est pas un etudiant.\n";
        return;
    }
    if (date.empty()) {
        cerr << "[DB ERREUR] La date d'absence est obligatoire.\n";
        return;
    }

    sqlite3_stmt* stmt = nullptr;
    const string sql =
        "INSERT INTO absences (date, heure, motif, justifie, etudiant_id) "
        "VALUES (?, ?, ?, ?, ?);";

    if (!prepareStatement(sql, &stmt)) {
        return;
    }

    sqlite3_bind_text(stmt, 1, date.c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt, 2, heure.c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt, 3, motif.c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_int(stmt, 4, justifie ? 1 : 0);
    sqlite3_bind_int(stmt, 5, etudiantId);

    executeNonQuery(stmt, "ajout absence");
    sqlite3_finalize(stmt);
}

void Database::modifierAbsence(int absenceId, bool justifie, const string& motif) {
    sqlite3_stmt* stmt = nullptr;
    const string sql =
        "UPDATE absences SET justifie = ?, motif = ? WHERE id = ?;";

    if (!prepareStatement(sql, &stmt)) {
        return;
    }

    sqlite3_bind_int(stmt, 1, justifie ? 1 : 0);
    sqlite3_bind_text(stmt, 2, motif.c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_int(stmt, 3, absenceId);

    bool ok = executeNonQuery(stmt, "modification absence");
    int changes = ok ? sqlite3_changes(db) : 0;
    sqlite3_finalize(stmt);

    if (ok && changes == 0) {
        cerr << "[DB ERREUR] Aucune absence avec l'id " << absenceId << ".\n";
    }
}

vector<AbsenceDB> Database::getAbsences(int etudiantId) {
    vector<AbsenceDB> liste;
    if (!personneAvecRoleExiste(etudiantId, ROLE_ETUDIANT)) {
        return liste;
    }

    sqlite3_stmt* stmt = nullptr;
    const string sql =
        "SELECT id, date, heure, motif, justifie, etudiant_id "
        "FROM absences WHERE etudiant_id = ? "
        "ORDER BY date DESC, heure DESC;";

    if (!prepareStatement(sql, &stmt)) {
        return liste;
    }

    sqlite3_bind_int(stmt, 1, etudiantId);

    while (sqlite3_step(stmt) == SQLITE_ROW) {
        AbsenceDB a;
        a.id          = sqlite3_column_int(stmt, 0);
        a.date        = textColumn(stmt, 1);
        a.heure       = textColumn(stmt, 2);
        a.motif       = textColumn(stmt, 3);
        a.justifie    = sqlite3_column_int(stmt, 4) == 1;
        a.etudiant_id = sqlite3_column_int(stmt, 5);
        liste.push_back(a);
    }

    sqlite3_finalize(stmt);
    return liste;
}

int Database::getNbAbsences(int etudiantId) {
    if (!personneAvecRoleExiste(etudiantId, ROLE_ETUDIANT)) {
        return 0;
    }

    sqlite3_stmt* stmt = nullptr;
    if (!prepareStatement(
            "SELECT COUNT(*) FROM absences WHERE etudiant_id = ?;",
            &stmt)) {
        return 0;
    }

    sqlite3_bind_int(stmt, 1, etudiantId);

    int count = 0;
    if (sqlite3_step(stmt) == SQLITE_ROW) {
        count = sqlite3_column_int(stmt, 0);
    }

    sqlite3_finalize(stmt);
    return count;
}

int Database::ajouterMatiere(const string& nom, const string& apogee, float coef) {
    if (!ensureOpen("ajouterMatiere")) {
        return -1;
    }
    if (nom.empty()) {
        cerr << "[DB ERREUR] Le nom de la matiere est obligatoire.\n";
        return -1;
    }
    if (coef <= 0.0f) {
        cerr << "[DB ERREUR] Le coefficient de la matiere doit etre positif.\n";
        return -1;
    }

    sqlite3_stmt* stmt = nullptr;
    const string sql =
        "INSERT INTO matieres (nom, apogee, coef) VALUES (?, ?, ?);";

    if (!prepareStatement(sql, &stmt)) {
        return -1;
    }

    sqlite3_bind_text(stmt, 1, nom.c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt, 2, apogee.c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_double(stmt, 3, coef);

    bool ok = executeNonQuery(stmt, "ajout matiere");
    sqlite3_finalize(stmt);

    return ok ? int(sqlite3_last_insert_rowid(db)) : -1;
}

int Database::ajouterClasse(const string& nom, const string& niveau, const string& voie) {
    if (!ensureOpen("ajouterClasse")) {
        return -1;
    }
    if (nom.empty() || niveau.empty() || voie.empty()) {
        cerr << "[DB ERREUR] Nom, niveau et voie de la classe sont obligatoires.\n";
        return -1;
    }

    sqlite3_stmt* stmt = nullptr;
    const string sql =
        "INSERT INTO classes (nom, niveau, voie) VALUES (?, ?, ?);";

    if (!prepareStatement(sql, &stmt)) {
        return -1;
    }

    sqlite3_bind_text(stmt, 1, nom.c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt, 2, niveau.c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt, 3, voie.c_str(), -1, SQLITE_TRANSIENT);

    const bool ok = executeNonQuery(stmt, "ajout classe");
    sqlite3_finalize(stmt);
    return ok ? int(sqlite3_last_insert_rowid(db)) : -1;
}

vector<ClasseDB> Database::getToutesLesClasses() {
    vector<ClasseDB> liste;
    sqlite3_stmt* stmt = nullptr;
    if (!prepareStatement(
            "SELECT id, nom, niveau, voie FROM classes ORDER BY niveau, nom;",
            &stmt)) {
        return liste;
    }

    while (sqlite3_step(stmt) == SQLITE_ROW) {
        ClasseDB classe;
        classe.id     = sqlite3_column_int(stmt, 0);
        classe.nom    = textColumn(stmt, 1);
        classe.niveau = textColumn(stmt, 2);
        classe.voie   = textColumn(stmt, 3);
        liste.push_back(classe);
    }

    sqlite3_finalize(stmt);
    return liste;
}

vector<MatiereDB> Database::getToutesLesmatieres() {
    vector<MatiereDB> liste;
    sqlite3_stmt* stmt = nullptr;

    if (!prepareStatement(
            "SELECT id, nom, apogee, coef FROM matieres ORDER BY nom;",
            &stmt)) {
        return liste;
    }

    while (sqlite3_step(stmt) == SQLITE_ROW) {
        MatiereDB m;
        m.id     = sqlite3_column_int(stmt, 0);
        m.nom    = textColumn(stmt, 1);
        m.apogee = textColumn(stmt, 2);
        m.coef   = static_cast<float>(sqlite3_column_double(stmt, 3));
        liste.push_back(m);
    }

    sqlite3_finalize(stmt);
    return liste;
}

vector<MatiereDB> Database::getMatieresProfesseur(int profId) {
    vector<MatiereDB> liste;
    if (!personneAvecRoleExiste(profId, ROLE_PROFESSEUR)) {
        return liste;
    }

    sqlite3_stmt* stmt = nullptr;
    const string sql =
        "SELECT m.id, m.nom, m.apogee, m.coef "
        "FROM matieres m "
        "JOIN prof_matiere pm ON m.id = pm.matiere_id "
        "WHERE pm.prof_id = ? "
        "ORDER BY m.nom;";

    if (!prepareStatement(sql, &stmt)) {
        return liste;
    }

    sqlite3_bind_int(stmt, 1, profId);

    while (sqlite3_step(stmt) == SQLITE_ROW) {
        MatiereDB m;
        m.id     = sqlite3_column_int(stmt, 0);
        m.nom    = textColumn(stmt, 1);
        m.apogee = textColumn(stmt, 2);
        m.coef   = static_cast<float>(sqlite3_column_double(stmt, 3));
        liste.push_back(m);
    }

    sqlite3_finalize(stmt);
    return liste;
}

vector<MatiereDB> Database::getMatieresEtudiant(int etudiantId) {
    vector<MatiereDB> liste;
    if (!personneAvecRoleExiste(etudiantId, ROLE_ETUDIANT)) {
        return liste;
    }

    sqlite3_stmt* stmt = nullptr;
    const string sql =
        "SELECT m.id, m.nom, m.apogee, m.coef "
        "FROM matieres m "
        "JOIN etudiant_matiere em ON m.id = em.matiere_id "
        "WHERE em.etudiant_id = ? "
        "ORDER BY m.nom;";

    if (!prepareStatement(sql, &stmt)) {
        return liste;
    }

    sqlite3_bind_int(stmt, 1, etudiantId);

    while (sqlite3_step(stmt) == SQLITE_ROW) {
        MatiereDB m;
        m.id     = sqlite3_column_int(stmt, 0);
        m.nom    = textColumn(stmt, 1);
        m.apogee = textColumn(stmt, 2);
        m.coef   = static_cast<float>(sqlite3_column_double(stmt, 3));
        liste.push_back(m);
    }

    sqlite3_finalize(stmt);
    return liste;
}

vector<PersonneDB> Database::getEtudiantsClasse(int classeId) {
    vector<PersonneDB> liste;
    if (!classeExiste(classeId)) {
        return liste;
    }

    sqlite3_stmt* stmt = nullptr;
    const string sql =
        "SELECT p.id, p.identifiant, p.nom, p.prenom, p.role, p.photo_path, "
        "       COALESCE(p.classe_id, -1), COALESCE(c.nom, '') "
        "FROM personnes p "
        "LEFT JOIN classes c ON c.id = p.classe_id "
        "WHERE p.role = 'etudiant' AND p.classe_id = ? "
        "ORDER BY p.nom, p.prenom;";

    if (!prepareStatement(sql, &stmt)) {
        return liste;
    }

    sqlite3_bind_int(stmt, 1, classeId);

    while (sqlite3_step(stmt) == SQLITE_ROW) {
        PersonneDB p;
        p.id           = sqlite3_column_int(stmt, 0);
        p.identifiant  = uppercaseCopy(textColumn(stmt, 1));
        p.nom          = textColumn(stmt, 2);
        p.prenom       = textColumn(stmt, 3);
        p.mot_de_passe = "";
        p.role         = textColumn(stmt, 4);
        p.photo_path   = textColumn(stmt, 5);
        p.classe_id    = sqlite3_column_int(stmt, 6);
        p.classe_nom   = textColumn(stmt, 7);
        liste.push_back(p);
    }

    sqlite3_finalize(stmt);
    return liste;
}

vector<ClasseMatiereDB> Database::getMatieresClasse(int classeId) {
    vector<ClasseMatiereDB> liste;
    if (!classeExiste(classeId)) {
        return liste;
    }

    sqlite3_stmt* stmt = nullptr;
    const string sql =
        "SELECT cm.classe_id, cm.matiere_id, cm.prof_id, cm.obligatoire, "
        "       c.nom, m.nom, p.nom || ' ' || p.prenom "
        "FROM classe_matieres cm "
        "JOIN classes c ON c.id = cm.classe_id "
        "JOIN matieres m ON m.id = cm.matiere_id "
        "JOIN personnes p ON p.id = cm.prof_id "
        "WHERE cm.classe_id = ? "
        "ORDER BY cm.obligatoire DESC, m.nom;";

    if (!prepareStatement(sql, &stmt)) {
        return liste;
    }

    sqlite3_bind_int(stmt, 1, classeId);

    while (sqlite3_step(stmt) == SQLITE_ROW) {
        ClasseMatiereDB liaison;
        liaison.classe_id   = sqlite3_column_int(stmt, 0);
        liaison.matiere_id  = sqlite3_column_int(stmt, 1);
        liaison.prof_id     = sqlite3_column_int(stmt, 2);
        liaison.obligatoire = sqlite3_column_int(stmt, 3) == 1;
        liaison.classe_nom  = textColumn(stmt, 4);
        liaison.matiere_nom = textColumn(stmt, 5);
        liaison.prof_nom    = textColumn(stmt, 6);
        liaison.type_matiere = liaison.obligatoire ? "Obligatoire" : "Optionnelle";
        liste.push_back(liaison);
    }

    sqlite3_finalize(stmt);
    return liste;
}

vector<PersonneDB> Database::getEtudiantsParMatiere(int matiereId) {
    vector<PersonneDB> liste;
    if (!matiereExiste(matiereId)) {
        return liste;
    }

    sqlite3_stmt* stmt = nullptr;
    const string sql =
        "SELECT p.id, p.identifiant, p.nom, p.prenom, p.role, p.photo_path, "
        "       COALESCE(p.classe_id, -1), COALESCE(c.nom, '') "
        "FROM personnes p "
        "JOIN etudiant_matiere em ON em.etudiant_id = p.id "
        "LEFT JOIN classes c ON c.id = p.classe_id "
        "WHERE em.matiere_id = ? AND p.role = 'etudiant' "
        "ORDER BY p.nom, p.prenom;";

    if (!prepareStatement(sql, &stmt)) {
        return liste;
    }

    sqlite3_bind_int(stmt, 1, matiereId);

    while (sqlite3_step(stmt) == SQLITE_ROW) {
        PersonneDB p;
        p.id           = sqlite3_column_int(stmt, 0);
        p.identifiant  = uppercaseCopy(textColumn(stmt, 1));
        p.nom          = textColumn(stmt, 2);
        p.prenom       = textColumn(stmt, 3);
        p.mot_de_passe = "";
        p.role         = textColumn(stmt, 4);
        p.photo_path   = textColumn(stmt, 5);
        p.classe_id    = sqlite3_column_int(stmt, 6);
        p.classe_nom   = textColumn(stmt, 7);
        liste.push_back(p);
    }

    sqlite3_finalize(stmt);
    return liste;
}

void Database::assignerProfMatiere(int profId, int matiereId) {
    if (!personneAvecRoleExiste(profId, ROLE_PROFESSEUR)) {
        cerr << "[DB ERREUR] L'id " << profId << " n'est pas un professeur.\n";
        return;
    }
    if (!matiereExiste(matiereId)) {
        cerr << "[DB ERREUR] La matiere " << matiereId << " n'existe pas.\n";
        return;
    }

    sqlite3_stmt* stmt = nullptr;
    if (!prepareStatement(
            "INSERT OR IGNORE INTO prof_matiere (prof_id, matiere_id) VALUES (?, ?);",
            &stmt)) {
        return;
    }

    sqlite3_bind_int(stmt, 1, profId);
    sqlite3_bind_int(stmt, 2, matiereId);

    executeNonQuery(stmt, "affectation professeur-matiere");
    sqlite3_finalize(stmt);
}

bool Database::affecterMatiereClasse(int classeId, int matiereId, int profId, bool obligatoire) {
    if (!classeExiste(classeId) || !matiereExiste(matiereId)) {
        return false;
    }
    if (!personneAvecRoleExiste(profId, ROLE_PROFESSEUR)) {
        return false;
    }

    assignerProfMatiere(profId, matiereId);

    sqlite3_stmt* stmt = nullptr;
    if (!prepareStatement(
            "INSERT INTO classe_matieres (classe_id, matiere_id, prof_id, obligatoire) "
            "VALUES (?, ?, ?, ?) "
            "ON CONFLICT(classe_id, matiere_id) DO UPDATE SET "
            "prof_id = excluded.prof_id, obligatoire = excluded.obligatoire;",
            &stmt)) {
        return false;
    }

    sqlite3_bind_int(stmt, 1, classeId);
    sqlite3_bind_int(stmt, 2, matiereId);
    sqlite3_bind_int(stmt, 3, profId);
    sqlite3_bind_int(stmt, 4, obligatoire ? 1 : 0);

    bool ok = executeNonQuery(stmt, "affectation matiere-classe");
    sqlite3_finalize(stmt);
    if (!ok) {
        return false;
    }

    if (obligatoire) {
        vector<PersonneDB> etudiants = getEtudiantsClasse(classeId);
        for (int i = 0; i < int(etudiants.size()); ++i) {
            inscrireEtudiantMatiere(etudiants[i].id, matiereId);
        }
    }

    return true;
}

void Database::inscrireEtudiantMatiere(int etudiantId, int matiereId) {
    if (!personneAvecRoleExiste(etudiantId, ROLE_ETUDIANT)) {
        cerr << "[DB ERREUR] L'id " << etudiantId << " n'est pas un etudiant.\n";
        return;
    }
    if (!matiereExiste(matiereId)) {
        cerr << "[DB ERREUR] La matiere " << matiereId << " n'existe pas.\n";
        return;
    }

    sqlite3_stmt* stmt = nullptr;
    if (!prepareStatement(
            "INSERT OR IGNORE INTO etudiant_matiere (etudiant_id, matiere_id) VALUES (?, ?);",
            &stmt)) {
        return;
    }

    sqlite3_bind_int(stmt, 1, etudiantId);
    sqlite3_bind_int(stmt, 2, matiereId);

    executeNonQuery(stmt, "inscription etudiant-matiere");
    sqlite3_finalize(stmt);
}

int Database::ajouterCours(const string& jour, const string& heureDebut,
                           const string& heureFin, const string& salle,
                           int classeId, int matiereId, int profId) {
    if (!ensureOpen("ajouterCours")) {
        return -1;
    }
    if (jour.empty() || heureDebut.empty() || heureFin.empty()) {
        cerr << "[DB ERREUR] Jour, heure de debut et heure de fin sont obligatoires.\n";
        return -1;
    }
    if (heureDebut >= heureFin) {
        cerr << "[DB ERREUR] L'heure de debut doit etre avant l'heure de fin.\n";
        return -1;
    }
    if (!matiereExiste(matiereId)) {
        cerr << "[DB ERREUR] La matiere " << matiereId << " n'existe pas.\n";
        return -1;
    }
    if (!classeExiste(classeId)) {
        cerr << "[DB ERREUR] La classe " << classeId << " n'existe pas.\n";
        return -1;
    }
    if (!personneAvecRoleExiste(profId, ROLE_PROFESSEUR)) {
        cerr << "[DB ERREUR] L'id " << profId << " n'est pas un professeur.\n";
        return -1;
    }
    if (!matiereAffecteeClasse(classeId, matiereId)) {
        cerr << "[DB ERREUR] Cette matiere n'est pas affectee a cette classe.\n";
        return -1;
    }
    if (!profAffecteClasseMatiere(classeId, matiereId, profId)) {
        cerr << "[DB ERREUR] Ce professeur n'est pas affecte a cette matiere pour cette classe.\n";
        return -1;
    }

    sqlite3_stmt* stmt = nullptr;
    const string sql =
        "INSERT INTO cours (jour, heure_debut, heure_fin, salle, classe_id, matiere_id, prof_id) "
        "VALUES (?, ?, ?, ?, ?, ?, ?);";

    if (!prepareStatement(sql, &stmt)) {
        return -1;
    }

    sqlite3_bind_text(stmt, 1, jour.c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt, 2, heureDebut.c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt, 3, heureFin.c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt, 4, salle.c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_int(stmt, 5, classeId);
    sqlite3_bind_int(stmt, 6, matiereId);
    sqlite3_bind_int(stmt, 7, profId);

    bool ok = executeNonQuery(stmt, "ajout cours");
    sqlite3_finalize(stmt);

    return ok ? int(sqlite3_last_insert_rowid(db)) : -1;
}

void Database::modifierCours(int coursId, const string& jour, const string& heureDebut,
                             const string& heureFin, const string& salle) {
    if (jour.empty() || heureDebut.empty() || heureFin.empty()) {
        cerr << "[DB ERREUR] Jour, heure de debut et heure de fin sont obligatoires.\n";
        return;
    }
    if (heureDebut >= heureFin) {
        cerr << "[DB ERREUR] L'heure de debut doit etre avant l'heure de fin.\n";
        return;
    }

    sqlite3_stmt* stmt = nullptr;
    const string sql =
        "UPDATE cours SET jour = ?, heure_debut = ?, heure_fin = ?, salle = ? WHERE id = ?;";

    if (!prepareStatement(sql, &stmt)) {
        return;
    }

    sqlite3_bind_text(stmt, 1, jour.c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt, 2, heureDebut.c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt, 3, heureFin.c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt, 4, salle.c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_int(stmt, 5, coursId);

    bool ok = executeNonQuery(stmt, "modification cours");
    int changes = ok ? sqlite3_changes(db) : 0;
    sqlite3_finalize(stmt);

    if (ok && changes == 0) {
        cerr << "[DB ERREUR] Aucun cours avec l'id " << coursId << ".\n";
    }
}

void Database::supprimerCours(int coursId) {
    sqlite3_stmt* stmt = nullptr;
    if (!prepareStatement("DELETE FROM cours WHERE id = ?;", &stmt)) {
        return;
    }

    sqlite3_bind_int(stmt, 1, coursId);
    bool ok = executeNonQuery(stmt, "suppression cours");
    int changes = ok ? sqlite3_changes(db) : 0;
    sqlite3_finalize(stmt);

    if (ok && changes == 0) {
        cerr << "[DB ERREUR] Aucun cours avec l'id " << coursId << ".\n";
    }
}

vector<CoursDB> Database::getEDTEtudiant(int etudiantId) {
    vector<CoursDB> liste;
    if (!personneAvecRoleExiste(etudiantId, ROLE_ETUDIANT)) {
        return liste;
    }

    sqlite3_stmt* stmt = nullptr;
    const string sql =
        "SELECT c.id, c.jour, c.heure_debut, c.heure_fin, c.salle, "
        "       COALESCE(cl.nom, ''), m.nom, p.nom || ' ' || p.prenom, "
        "       COALESCE(c.classe_id, -1), c.matiere_id, c.prof_id "
        "FROM cours c "
        "JOIN personnes e ON e.id = ? "
        "JOIN matieres m ON c.matiere_id = m.id "
        "JOIN personnes p ON c.prof_id = p.id "
        "LEFT JOIN classes cl ON cl.id = c.classe_id "
        "JOIN etudiant_matiere em ON em.matiere_id = c.matiere_id AND em.etudiant_id = e.id "
        "WHERE e.id = ? AND (c.classe_id = e.classe_id OR c.classe_id IS NULL) "
        "ORDER BY " + string(ordreJourSQL()) + ", c.heure_debut;";

    if (!prepareStatement(sql, &stmt)) {
        return liste;
    }

    sqlite3_bind_int(stmt, 1, etudiantId);
    sqlite3_bind_int(stmt, 2, etudiantId);

    while (sqlite3_step(stmt) == SQLITE_ROW) {
        CoursDB c;
        c.id          = sqlite3_column_int(stmt, 0);
        c.jour        = textColumn(stmt, 1);
        c.heure_debut = textColumn(stmt, 2);
        c.heure_fin   = textColumn(stmt, 3);
        c.salle       = textColumn(stmt, 4);
        c.classe_nom  = textColumn(stmt, 5);
        c.matiere_nom = textColumn(stmt, 6);
        c.prof_nom    = textColumn(stmt, 7);
        c.classe_id   = sqlite3_column_int(stmt, 8);
        c.matiere_id  = sqlite3_column_int(stmt, 9);
        c.prof_id     = sqlite3_column_int(stmt, 10);
        liste.push_back(c);
    }

    sqlite3_finalize(stmt);
    return liste;
}

vector<CoursDB> Database::getEDTProfesseur(int profId) {
    vector<CoursDB> liste;
    if (!personneAvecRoleExiste(profId, ROLE_PROFESSEUR)) {
        return liste;
    }

    sqlite3_stmt* stmt = nullptr;
    const string sql =
        "SELECT c.id, c.jour, c.heure_debut, c.heure_fin, c.salle, "
        "       COALESCE(cl.nom, ''), m.nom, p.nom || ' ' || p.prenom, "
        "       COALESCE(c.classe_id, -1), c.matiere_id, c.prof_id "
        "FROM cours c "
        "JOIN matieres m ON c.matiere_id = m.id "
        "JOIN personnes p ON c.prof_id = p.id "
        "LEFT JOIN classes cl ON cl.id = c.classe_id "
        "WHERE c.prof_id = ? "
        "ORDER BY " + string(ordreJourSQL()) + ", c.heure_debut;";

    if (!prepareStatement(sql, &stmt)) {
        return liste;
    }

    sqlite3_bind_int(stmt, 1, profId);

    while (sqlite3_step(stmt) == SQLITE_ROW) {
        CoursDB c;
        c.id          = sqlite3_column_int(stmt, 0);
        c.jour        = textColumn(stmt, 1);
        c.heure_debut = textColumn(stmt, 2);
        c.heure_fin   = textColumn(stmt, 3);
        c.salle       = textColumn(stmt, 4);
        c.classe_nom  = textColumn(stmt, 5);
        c.matiere_nom = textColumn(stmt, 6);
        c.prof_nom    = textColumn(stmt, 7);
        c.classe_id   = sqlite3_column_int(stmt, 8);
        c.matiere_id  = sqlite3_column_int(stmt, 9);
        c.prof_id     = sqlite3_column_int(stmt, 10);
        liste.push_back(c);
    }

    sqlite3_finalize(stmt);
    return liste;
}

vector<CoursDB> Database::getToutEDT() {
    vector<CoursDB> liste;
    sqlite3_stmt* stmt = nullptr;
    const string sql =
        "SELECT c.id, c.jour, c.heure_debut, c.heure_fin, c.salle, "
        "       COALESCE(cl.nom, ''), m.nom, p.nom || ' ' || p.prenom, "
        "       COALESCE(c.classe_id, -1), c.matiere_id, c.prof_id "
        "FROM cours c "
        "JOIN matieres m ON c.matiere_id = m.id "
        "JOIN personnes p ON c.prof_id = p.id "
        "LEFT JOIN classes cl ON cl.id = c.classe_id "
        "ORDER BY " + string(ordreJourSQL()) + ", c.heure_debut;";

    if (!prepareStatement(sql, &stmt)) {
        return liste;
    }

    while (sqlite3_step(stmt) == SQLITE_ROW) {
        CoursDB c;
        c.id          = sqlite3_column_int(stmt, 0);
        c.jour        = textColumn(stmt, 1);
        c.heure_debut = textColumn(stmt, 2);
        c.heure_fin   = textColumn(stmt, 3);
        c.salle       = textColumn(stmt, 4);
        c.classe_nom  = textColumn(stmt, 5);
        c.matiere_nom = textColumn(stmt, 6);
        c.prof_nom    = textColumn(stmt, 7);
        c.classe_id   = sqlite3_column_int(stmt, 8);
        c.matiere_id  = sqlite3_column_int(stmt, 9);
        c.prof_id     = sqlite3_column_int(stmt, 10);
        liste.push_back(c);
    }

    sqlite3_finalize(stmt);
    return liste;
}

int Database::publierActualite(const string& titre, const string& contenu,
                               const string& datePublication) {
    if (!ensureOpen("publierActualite")) {
        return -1;
    }
    if (titre.empty()) {
        cerr << "[DB ERREUR] Le titre de l'actualite est obligatoire.\n";
        return -1;
    }

    sqlite3_stmt* stmt = nullptr;
    const string sql =
        "INSERT INTO actualites (titre, contenu, date_publication) VALUES (?, ?, ?);";

    if (!prepareStatement(sql, &stmt)) {
        return -1;
    }

    sqlite3_bind_text(stmt, 1, titre.c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt, 2, contenu.c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt, 3, datePublication.c_str(), -1, SQLITE_TRANSIENT);

    bool ok = executeNonQuery(stmt, "publication actualite");
    sqlite3_finalize(stmt);

    return ok ? int(sqlite3_last_insert_rowid(db)) : -1;
}

vector<ActualiteDB> Database::getToutesActualites() {
    vector<ActualiteDB> liste;
    sqlite3_stmt* stmt = nullptr;

    if (!prepareStatement(
            "SELECT id, titre, contenu, date_publication FROM actualites "
            "ORDER BY date_publication DESC, id DESC;",
            &stmt)) {
        return liste;
    }

    while (sqlite3_step(stmt) == SQLITE_ROW) {
        ActualiteDB a;
        a.id               = sqlite3_column_int(stmt, 0);
        a.titre            = textColumn(stmt, 1);
        a.contenu          = textColumn(stmt, 2);
        a.date_publication = textColumn(stmt, 3);
        liste.push_back(a);
    }

    sqlite3_finalize(stmt);
    return liste;
}

void Database::supprimerActualite(int actualiteId) {
    sqlite3_stmt* stmt = nullptr;
    if (!prepareStatement("DELETE FROM actualites WHERE id = ?;", &stmt)) {
        return;
    }

    sqlite3_bind_int(stmt, 1, actualiteId);
    bool ok = executeNonQuery(stmt, "suppression actualite");
    int changes = ok ? sqlite3_changes(db) : 0;
    sqlite3_finalize(stmt);

    if (ok && changes == 0) {
        cerr << "[DB ERREUR] Aucune actualite avec l'id " << actualiteId << ".\n";
    }
}


