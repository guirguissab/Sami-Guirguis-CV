#include "SchoolFacade.h"

#include "Date.h"
#include "Note.h"
#include <sstream>
#include <set>

namespace {
QString formatNote(const NoteDB& n) {
    return QString("#%1 | etu:%2 | mat:%3 | %4 (coef %5)")
        .arg(n.id).arg(n.etudiant_id)
        .arg(QString::fromStdString(n.matiere))
        .arg(n.valeur, 0, 'f', 2).arg(n.coef, 0, 'f', 2);
}

QString formatPersonne(const PersonneDB& p) {
    return QString("#%1 | %2 | %3 %4 | %5 | classe:%6")
        .arg(p.id)
        .arg(QString::fromStdString(p.identifiant))
        .arg(QString::fromStdString(p.prenom))
        .arg(QString::fromStdString(p.nom))
        .arg(QString::fromStdString(p.role))
        .arg(p.classe_id);
}

QString formatAbsence(const AbsenceDB& a) {
    return QString("#%1 | %2 %3 | %4 | %5")
        .arg(a.id)
        .arg(QString::fromStdString(a.date))
        .arg(QString::fromStdString(a.heure))
        .arg(QString::fromStdString(a.motif))
        .arg(a.justifie ? "justifiee" : "non justifiee");
}

QString formatMatiere(const MatiereDB& m) {
    return QString("#%1 | %2 | %3 | coef %4")
        .arg(m.id)
        .arg(QString::fromStdString(m.nom))
        .arg(QString::fromStdString(m.apogee))
        .arg(m.coef, 0, 'f', 2);
}

QString formatClasse(const ClasseDB& c) {
    return QString("#%1 | %2 | %3 | %4")
        .arg(c.id)
        .arg(QString::fromStdString(c.nom))
        .arg(QString::fromStdString(c.niveau))
        .arg(QString::fromStdString(c.voie));
}

QString formatCours(const CoursDB& c) {
    return QString("#%1 | %2 %3-%4 | %5 | %6 | %7 | %8")
        .arg(c.id)
        .arg(QString::fromStdString(c.jour))
        .arg(QString::fromStdString(c.heure_debut))
        .arg(QString::fromStdString(c.heure_fin))
        .arg(QString::fromStdString(c.salle))
        .arg(QString::fromStdString(c.classe_nom))
        .arg(QString::fromStdString(c.matiere_nom))
        .arg(QString::fromStdString(c.prof_nom));
}
} // namespace

SchoolFacade::SchoolFacade(QObject* parent)
    : QObject(parent), m_notesCount(0)
{
    m_bd.init();
    m_user = PersonneDB{-1, "", "", "", "", "", "", -1, ""};
}

bool SchoolFacade::logged() const { return m_user.id > 0; }
QString SchoolFacade::role() const { return QString::fromStdString(m_user.role); }
QString SchoolFacade::fullName() const { return QString::fromStdString(m_user.prenom + " " + m_user.nom); }
QString SchoolFacade::identifiant() const { return QString::fromStdString(m_user.identifiant); }
int SchoolFacade::userId() const { return m_user.id; }
int SchoolFacade::notesCount() const { return m_notesCount; }
QString SchoolFacade::statusMessage() const { return m_statusMessage; }

bool SchoolFacade::login(const QString& login, const QString& password)
{
    PersonneDB user = m_bd.authentifier(login.toStdString(), password.toStdString());
    if (user.id <= 0) {
        setStatusMessage("Echec connexion: identifiant ou mot de passe invalide.");
        return false;
    }

    m_user = user;
    refreshRoleObjects();
    refreshNotesCount();
    setStatusMessage("Connexion reussie.");

    emit loggedChanged();
    emit userChanged();
    emit notesChanged();
    return true;
}

void SchoolFacade::logout()
{
    m_user = PersonneDB{-1, "", "", "", "", "", "", -1, ""};
    m_notesCount = 0;
    setStatusMessage("Deconnecte.");
    emit loggedChanged();
    emit userChanged();
    emit notesChanged();
}

QStringList SchoolFacade::getNotesLines()
{
    QStringList lines;
    if (m_user.id <= 0) {
        return lines;
    }

    if (m_user.role == "etudiant") {
        std::vector<Note> notes = m_etudiant.getNote();
        for (int i = 0; i < int(notes.size()); ++i) {
            lines << QString("#%1 | etu:%2 | %3 | %4")
                         .arg(notes[i].getId())
                         .arg(m_user.id)
                         .arg(QString::fromStdString(notes[i].getAppr()))
                         .arg(notes[i].getValeur(), 0, 'f', 2);
        }
    } else if (m_user.role == "professeur") {
        std::vector<NoteDB> notes = m_professeur.consulterNotes(m_bd);
        for (int i = 0; i < int(notes.size()); ++i) {
            lines << QString("#%1 | etu:%2 | %3 | %4")
                         .arg(notes[i].id)
                         .arg(notes[i].etudiant_id)
                         .arg(QString::fromStdString(notes[i].matiere))
                         .arg(notes[i].valeur, 0, 'f', 2);
        }
    } else if (m_user.role == "admin") {
        std::vector<NoteDB> notes = m_admin.getToutesNotes(m_bd);
        for (int i = 0; i < int(notes.size()); ++i) {
            lines << QString("#%1 | etu:%2 | %3 | %4")
                         .arg(notes[i].id)
                         .arg(notes[i].etudiant_id)
                         .arg(QString::fromStdString(notes[i].matiere))
                         .arg(notes[i].valeur, 0, 'f', 2);
        }
    }
    return lines;
}

QStringList SchoolFacade::getUsersLines()
{
    QStringList lines;
    if (m_user.role != "admin") {
        return lines;
    }
    std::vector<PersonneDB> users = m_admin.getTousUtilisateurs(m_bd);
    for (int i = 0; i < int(users.size()); ++i) {
        lines << QString("#%1 | %2 | %3 %4 | %5")
                     .arg(users[i].id)
                     .arg(QString::fromStdString(users[i].identifiant))
                     .arg(QString::fromStdString(users[i].prenom))
                     .arg(QString::fromStdString(users[i].nom))
                     .arg(QString::fromStdString(users[i].role));
    }
    return lines;
}

QStringList SchoolFacade::getUsersByRole(const QString& roleName)
{
    QStringList lines;
    if (m_user.role != "admin") {
        return lines;
    }
    std::vector<PersonneDB> users = m_admin.getTousUtilisateurs(m_bd);
    const std::string roleFilter = roleName.toStdString();
    for (int i = 0; i < int(users.size()); ++i) {
        if (users[i].role == roleFilter) {
            lines << QString("#%1 | %2 %3 | %4")
                         .arg(users[i].id)
                         .arg(QString::fromStdString(users[i].prenom))
                         .arg(QString::fromStdString(users[i].nom))
                         .arg(QString::fromStdString(users[i].identifiant));
        }
    }
    return lines;
}

bool SchoolFacade::canManageNotes() const
{
    return m_user.role == "admin";
}

bool SchoolFacade::canManageUsers() const
{
    return m_user.role == "admin";
}

bool SchoolFacade::isStudent() const { return m_user.role == "etudiant"; }
bool SchoolFacade::isProfesseur() const { return m_user.role == "professeur"; }
bool SchoolFacade::isAdmin() const { return m_user.role == "admin"; }

QStringList SchoolFacade::getMyClasses()
{
    QStringList lines;
    if (isStudent()) {
        if (m_user.classe_id > 0) {
            lines << QString("#%1 | %2").arg(m_user.classe_id).arg(QString::fromStdString(m_user.classe_nom));
        }
        return lines;
    }
    if (isProfesseur()) {
        std::vector<CoursDB> cours = m_bd.getEDTProfesseur(m_user.id);
        std::set<int> seen;
        for (int i = 0; i < int(cours.size()); ++i) {
            if (cours[i].classe_id > 0 && seen.insert(cours[i].classe_id).second) {
                lines << QString("#%1 | %2").arg(cours[i].classe_id).arg(QString::fromStdString(cours[i].classe_nom));
            }
        }
    }
    return lines;
}

QStringList SchoolFacade::getMyEDT()
{
    if (isStudent()) return getEDTEtudiant(m_user.id);
    if (isProfesseur()) return getEDTProfesseur(m_user.id);
    return QStringList();
}

QStringList SchoolFacade::getMyNotes()
{
    if (isStudent()) return getNotesByStudent(m_user.id);
    return QStringList();
}

bool SchoolFacade::modifyNote(int noteId, double valeur)
{
    if (!(isAdmin() || isProfesseur())) {
        setStatusMessage("Action non autorisee.");
        return false;
    }
    if (isAdmin()) {
        m_admin.modifierNote(m_bd, noteId, static_cast<float>(valeur));
    } else {
        m_professeur.modifierNote(m_bd, (unsigned int)(noteId), static_cast<float>(valeur));
    }
    refreshData();
    setStatusMessage("Note modifiee.");
    return true;
}

bool SchoolFacade::deleteNote(int noteId)
{
    if (!isAdmin()) {
        setStatusMessage("Action non autorisee.");
        return false;
    }
    m_admin.supprimerNote(m_bd, noteId);
    refreshData();
    setStatusMessage("Note supprimee.");
    return true;
}

bool SchoolFacade::createUser(const QString& nom, const QString& prenom, const QString& mdp, int roleChoisi)
{
    if (!canManageUsers()) {
        setStatusMessage("Action non autorisee.");
        return false;
    }
    const int id = m_admin.creerCompte(m_bd, nom.toStdString(), prenom.toStdString(), mdp.toStdString(), roleChoisi);
    if (id <= 0) {
        setStatusMessage("Echec de creation du compte.");
        return false;
    }
    refreshData();
    setStatusMessage("Compte cree.");
    return true;
}

bool SchoolFacade::deleteUser(int userId)
{
    if (!canManageUsers()) {
        setStatusMessage("Action non autorisee.");
        return false;
    }
    m_admin.supprimerCompte(m_bd, userId);
    refreshData();
    setStatusMessage("Compte supprime.");
    return true;
}

bool SchoolFacade::changePassword(int userId, const QString& newPassword)
{
    if (!isAdmin()) {
        setStatusMessage("Action non autorisee.");
        return false;
    }
    const bool ok = m_bd.modifierMotDePasse(userId, newPassword.toStdString());
    setStatusMessage(ok ? "Mot de passe modifie." : "Echec modification mot de passe.");
    return ok;
}

bool SchoolFacade::setPhotoPath(int userId, const QString& photoPath)
{
    if (!isAdmin()) {
        setStatusMessage("Action non autorisee.");
        return false;
    }
    const bool ok = m_bd.modifierPhotoProfil(userId, photoPath.toStdString());
    setStatusMessage(ok ? "Photo mise a jour." : "Echec mise a jour photo.");
    return ok;
}

bool SchoolFacade::assignStudentToClass(int etudiantId, int classeId)
{
    if (!isAdmin()) {
        setStatusMessage("Action non autorisee.");
        return false;
    }
    const bool ok = m_admin.assignerEtudiantClasse(m_bd, etudiantId, classeId);
    setStatusMessage(ok ? "Etudiant assigne a la classe." : "Echec assignation classe.");
    refreshData();
    return ok;
}

bool SchoolFacade::assignProfToClass(int profId, int classeId)
{
    if (!isAdmin()) {
        setStatusMessage("Action non autorisee.");
        return false;
    }
    const bool ok = m_admin.assignerProfClasse(m_bd, profId, classeId);
    setStatusMessage(ok ? "Classe assignee au professeur." : "Echec assignation classe-professeur.");
    refreshData();
    return ok;
}

bool SchoolFacade::assignEDTToProf(int profId, int classeId, int matiereId,
                                   const QString& jour, const QString& heureDebut,
                                   const QString& heureFin, const QString& salle)
{
    if (!isAdmin()) {
        setStatusMessage("Action non autorisee.");
        return false;
    }
    const bool ok = m_admin.attribuerEDTProfesseur(
        m_bd, profId, classeId, matiereId,
        jour.toStdString(), heureDebut.toStdString(), heureFin.toStdString(), salle.toStdString());
    setStatusMessage(ok ? "EDT attribue au professeur." : "Echec attribution EDT professeur.");
    refreshData();
    return ok;
}

bool SchoolFacade::assignEDTToStudent(int etudiantId, int classeId)
{
    if (!isAdmin()) {
        setStatusMessage("Action non autorisee.");
        return false;
    }
    const bool ok = m_admin.attribuerEDTEtudiant(m_bd, etudiantId, classeId);
    setStatusMessage(ok ? "EDT attribue a l'etudiant via sa classe." : "Echec attribution EDT etudiant.");
    refreshData();
    return ok;
}

bool SchoolFacade::addNote(int etudiantId, int matiereId, double valeur, double coef, const QString& appreciation)
{
    if (!(isAdmin() || isProfesseur())) {
        setStatusMessage("Action non autorisee.");
        return false;
    }
    if (isProfesseur()) {
        m_professeur.saisirNote(m_bd, (unsigned int)(etudiantId), (unsigned int)(matiereId),
                                static_cast<float>(valeur), static_cast<float>(coef), appreciation.toStdString());
    } else {
        m_bd.ajouterNote(etudiantId, matiereId, static_cast<float>(valeur), static_cast<float>(coef), appreciation.toStdString());
    }
    refreshData();
    setStatusMessage("Tentative d'ajout de note effectuee.");
    return true;
}

bool SchoolFacade::addNoteEtu(int classeId, int etudiantId, int matiereId, double valeur, double coef, const QString& appreciation)
{
    if (!isProfesseur()) {
        setStatusMessage("Action reservee au professeur.");
        return false;
    }
    if (classeId <= 0 || etudiantId <= 0 || matiereId <= 0) {
        setStatusMessage("Parametres invalides pour addNoteEtu.");
        return false;
    }

    refreshRoleObjects();

    std::vector<MatiereDB> matieres = m_bd.getToutesLesmatieres();
    Matiere matiere("Matiere", "MAT", int(coef), unsigned(matiereId));
    for (int i = 0; i < int(matieres.size()); ++i) {
        if (matieres[i].id == matiereId) {
            matiere = Matiere(
                matieres[i].nom,
                matieres[i].apogee,
                int(matieres[i].coef),
                unsigned(matieres[i].id));
            break;
        }
    }

    Note note(static_cast<float>(valeur), static_cast<float>(coef), appreciation.toStdString(), matiere, 0);
    const bool addedInClass = m_professeur.addNoteEtu(unsigned(classeId), unsigned(etudiantId), note);
    if (!addedInClass) {
        setStatusMessage("Echec addNoteEtu: classe ou etudiant introuvable.");
        return false;
    }

    // Persist also in DB so the note appears in standard listings.
    m_professeur.saisirNote(
        m_bd,
        unsigned(etudiantId),
        unsigned(matiereId),
        static_cast<float>(valeur),
        static_cast<float>(coef),
        appreciation.toStdString());

    refreshData();
    setStatusMessage("Note ajoutee via addNoteEtu.");
    return true;
}

QStringList SchoolFacade::getNotesByStudent(int etudiantId)
{
    QStringList lines;
    std::vector<NoteDB> notes = m_bd.getNotes(etudiantId);
    for (int i = 0; i < int(notes.size()); ++i) {
        lines << formatNote(notes[i]);
    }
    return lines;
}

QStringList SchoolFacade::getNotesBySubject(int etudiantId, int matiereId)
{
    QStringList lines;
    std::vector<NoteDB> notes = m_bd.getNotesByMatiere(etudiantId, matiereId);
    for (int i = 0; i < int(notes.size()); ++i) {
        lines << formatNote(notes[i]);
    }
    return lines;
}

double SchoolFacade::getMoyenne(int etudiantId, int matiereId)
{
    return m_bd.getMoyenne(etudiantId, matiereId);
}

bool SchoolFacade::addAbsence(int etudiantId, const QString& date, const QString& heure, const QString& motif, bool justifie)
{
    if (!(isAdmin() || isProfesseur())) {
        setStatusMessage("Action non autorisee.");
        return false;
    }
    if (isProfesseur()) {
        m_professeur.enregistrerAbsence(m_bd, (unsigned int)(etudiantId), date.toStdString(),
                                        heure.toStdString(), motif.toStdString(), justifie);
    } else {
        m_bd.ajouterAbsence(etudiantId, date.toStdString(), heure.toStdString(), motif.toStdString(), justifie);
    }
    setStatusMessage("Tentative d'ajout d'absence effectuee.");
    return true;
}

bool SchoolFacade::updateAbsence(int absenceId, bool justifie, const QString& motif)
{
    if (!isAdmin()) {
        setStatusMessage("Action non autorisee.");
        return false;
    }
    m_bd.modifierAbsence(absenceId, justifie, motif.toStdString());
    setStatusMessage("Tentative de modification d'absence effectuee.");
    return true;
}

QStringList SchoolFacade::getAbsencesByStudent(int etudiantId)
{
    QStringList lines;
    std::vector<AbsenceDB> absences = m_bd.getAbsences(etudiantId);
    for (int i = 0; i < int(absences.size()); ++i) {
        lines << formatAbsence(absences[i]);
    }
    return lines;
}

int SchoolFacade::getAbsenceCount(int etudiantId)
{
    return m_bd.getNbAbsences(etudiantId);
}

bool SchoolFacade::addMatiere(const QString& nom, const QString& apogee, double coef)
{
    if (!isAdmin()) {
        setStatusMessage("Action non autorisee.");
        return false;
    }
    const int id = m_admin.creerMatiere(m_bd, nom.toStdString(), apogee.toStdString(), static_cast<float>(coef));
    setStatusMessage(id > 0 ? "Matiere ajoutee." : "Echec ajout matiere.");
    return id > 0;
}

bool SchoolFacade::addClasse(const QString& nom, const QString& niveau, const QString& voie)
{
    if (!isAdmin()) {
        setStatusMessage("Action non autorisee.");
        return false;
    }
    const int id = m_admin.creerClasse(m_bd, nom.toStdString(), niveau.toStdString(), voie.toStdString());
    setStatusMessage(id > 0 ? "Classe ajoutee." : "Echec ajout classe.");
    return id > 0;
}

QStringList SchoolFacade::getAllClasses()
{
    QStringList lines;
    std::vector<ClasseDB> classes = m_bd.getToutesLesClasses();
    for (int i = 0; i < int(classes.size()); ++i) {
        lines << formatClasse(classes[i]);
    }
    return lines;
}

QStringList SchoolFacade::getAllMatieres()
{
    QStringList lines;
    std::vector<MatiereDB> matieres = m_bd.getToutesLesmatieres();
    for (int i = 0; i < int(matieres.size()); ++i) {
        lines << formatMatiere(matieres[i]);
    }
    return lines;
}

QStringList SchoolFacade::getMatieresProfesseur(int profId)
{
    QStringList lines;
    std::vector<MatiereDB> matieres = m_bd.getMatieresProfesseur(profId);
    for (int i = 0; i < int(matieres.size()); ++i) {
        lines << formatMatiere(matieres[i]);
    }
    return lines;
}

QStringList SchoolFacade::getMatieresEtudiant(int etudiantId)
{
    QStringList lines;
    std::vector<MatiereDB> matieres = m_bd.getMatieresEtudiant(etudiantId);
    for (int i = 0; i < int(matieres.size()); ++i) {
        lines << formatMatiere(matieres[i]);
    }
    return lines;
}

QStringList SchoolFacade::getEtudiantsParMatiere(int matiereId)
{
    QStringList lines;
    std::vector<PersonneDB> etudiants = m_bd.getEtudiantsParMatiere(matiereId);
    for (int i = 0; i < int(etudiants.size()); ++i) {
        lines << formatPersonne(etudiants[i]);
    }
    return lines;
}

QStringList SchoolFacade::getEtudiantsParClasse(int classeId)
{
    QStringList lines;
    std::vector<PersonneDB> etudiants = m_bd.getEtudiantsClasse(classeId);
    for (int i = 0; i < int(etudiants.size()); ++i) {
        lines << formatPersonne(etudiants[i]);
    }
    return lines;
}

QStringList SchoolFacade::getMatieresClasse(int classeId)
{
    QStringList lines;
    std::vector<ClasseMatiereDB> matieres = m_bd.getMatieresClasse(classeId);
    for (int i = 0; i < int(matieres.size()); ++i) {
        lines << QString("classe:%1 | mat:%2 | prof:%3 | %4")
                     .arg(QString::fromStdString(matieres[i].classe_nom))
                     .arg(QString::fromStdString(matieres[i].matiere_nom))
                     .arg(QString::fromStdString(matieres[i].prof_nom))
                     .arg(QString::fromStdString(matieres[i].type_matiere));
    }
    return lines;
}

bool SchoolFacade::assignProfMatiere(int profId, int matiereId)
{
    if (!isAdmin()) {
        setStatusMessage("Action non autorisee.");
        return false;
    }
    m_admin.assignerProfMatiere(m_bd, profId, matiereId);
    setStatusMessage("Affectation professeur-matiere effectuee.");
    return true;
}

bool SchoolFacade::affectMatiereClasse(int classeId, int matiereId, int profId, bool obligatoire)
{
    if (!isAdmin()) {
        setStatusMessage("Action non autorisee.");
        return false;
    }
    const bool ok = m_admin.affecterMatiereClasse(m_bd, classeId, matiereId, profId, obligatoire);
    setStatusMessage(ok ? "Matiere affectee a la classe." : "Echec affectation matiere-classe.");
    return ok;
}

bool SchoolFacade::inscrireEtudiantMatiere(int etudiantId, int matiereId)
{
    if (!isAdmin()) {
        setStatusMessage("Action non autorisee.");
        return false;
    }
    m_admin.inscrireEtudiantMatiere(m_bd, etudiantId, matiereId);
    setStatusMessage("Inscription etudiant-matiere effectuee.");
    return true;
}

bool SchoolFacade::addCours(const QString& jour, const QString& heureDebut, const QString& heureFin,
                            const QString& salle, int classeId, int matiereId, int profId)
{
    if (!isAdmin()) {
        setStatusMessage("Action non autorisee.");
        return false;
    }
    const int id = m_admin.ajouterCours(m_bd, jour.toStdString(), heureDebut.toStdString(), heureFin.toStdString(),
                                        salle.toStdString(), classeId, matiereId, profId);
    setStatusMessage(id > 0 ? "Cours ajoute." : "Echec ajout cours.");
    return id > 0;
}

bool SchoolFacade::updateCours(int coursId, const QString& jour, const QString& heureDebut,
                               const QString& heureFin, const QString& salle)
{
    if (!isAdmin()) {
        setStatusMessage("Action non autorisee.");
        return false;
    }
    m_admin.modifierCours(m_bd, coursId, jour.toStdString(), heureDebut.toStdString(), heureFin.toStdString(), salle.toStdString());
    setStatusMessage("Tentative de modification de cours effectuee.");
    return true;
}

bool SchoolFacade::deleteCours(int coursId)
{
    if (!isAdmin()) {
        setStatusMessage("Action non autorisee.");
        return false;
    }
    m_admin.supprimerCours(m_bd, coursId);
    setStatusMessage("Tentative de suppression de cours effectuee.");
    return true;
}

QStringList SchoolFacade::getEDTEtudiant(int etudiantId)
{
    QStringList lines;
    std::vector<CoursDB> cours = m_bd.getEDTEtudiant(etudiantId);
    for (int i = 0; i < int(cours.size()); ++i) {
        lines << formatCours(cours[i]);
    }
    return lines;
}

QStringList SchoolFacade::getEDTProfesseur(int profId)
{
    QStringList lines;
    std::vector<CoursDB> cours = m_bd.getEDTProfesseur(profId);
    for (int i = 0; i < int(cours.size()); ++i) {
        lines << formatCours(cours[i]);
    }
    return lines;
}

QStringList SchoolFacade::getEDTGlobal()
{
    QStringList lines;
    std::vector<CoursDB> cours = m_bd.getToutEDT();
    for (int i = 0; i < int(cours.size()); ++i) {
        lines << formatCours(cours[i]);
    }
    return lines;
}

bool SchoolFacade::publishActualite(const QString& titre, const QString& contenu, const QString& datePublication)
{
    if (!isAdmin()) {
        setStatusMessage("Action non autorisee.");
        return false;
    }
    const int id = m_admin.publierActualite(m_bd, titre.toStdString(), contenu.toStdString(), datePublication.toStdString());
    setStatusMessage(id > 0 ? "Actualite publiee." : "Echec publication actualite.");
    return id > 0;
}

bool SchoolFacade::deleteActualite(int actualiteId)
{
    if (!isAdmin()) {
        setStatusMessage("Action non autorisee.");
        return false;
    }
    m_admin.supprimerActualite(m_bd, actualiteId);
    setStatusMessage("Tentative de suppression actualite effectuee.");
    return true;
}

QStringList SchoolFacade::getActualites()
{
    QStringList lines;
    std::vector<ActualiteDB> actualites = m_bd.getToutesActualites();
    for (int i = 0; i < int(actualites.size()); ++i) {
        lines << QString("#%1 | %2 | %3 | %4")
                     .arg(actualites[i].id)
                     .arg(QString::fromStdString(actualites[i].date_publication))
                     .arg(QString::fromStdString(actualites[i].titre))
                     .arg(QString::fromStdString(actualites[i].contenu));
    }
    return lines;
}

void SchoolFacade::refreshData()
{
    refreshRoleObjects();
    refreshNotesCount();
    emit notesChanged();
    emit userChanged();
}

void SchoolFacade::refreshRoleObjects()
{
    Date d(1, 1, 2000);
    Personne profil(m_user.id, m_user.nom, m_user.prenom, "", d);
    if (m_user.role == "etudiant") {
        m_etudiant = Etudiant(profil, nullptr);
        m_etudiant.chargerDepuisDatabase(m_bd);
    } else if (m_user.role == "professeur") {
        m_professeur = Professeur(profil);
        m_professeur.chargerDepuisDatabase(m_bd);
    } else if (m_user.role == "admin") {
        m_admin = Administrateur(profil);
    }
}

void SchoolFacade::refreshNotesCount()
{
    if (m_user.role == "admin") {
        m_notesCount = int(m_admin.getToutesNotes(m_bd).size());
    } else if (m_user.role == "professeur") {
        m_notesCount = int(m_professeur.consulterNotes(m_bd).size());
    } else if (m_user.role == "etudiant") {
        m_notesCount = int(m_etudiant.getNote().size());
    } else {
        m_notesCount = 0;
    }
}

void SchoolFacade::setStatusMessage(const QString& message)
{
    if (m_statusMessage == message) {
        return;
    }
    m_statusMessage = message;
    emit statusMessageChanged();
}
