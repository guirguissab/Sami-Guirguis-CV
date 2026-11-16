#ifndef SCHOOLFACADE_H
#define SCHOOLFACADE_H

#include <QObject>
#include <QString>
#include <QStringList>

#include "Database.h"
#include "Etudiant.h"
#include "Professeur.h"
#include "Administrateur.h"

class SchoolFacade : public QObject
{
    Q_OBJECT
    Q_PROPERTY(bool logged READ logged NOTIFY loggedChanged)
    Q_PROPERTY(QString role READ role NOTIFY userChanged)
    Q_PROPERTY(QString fullName READ fullName NOTIFY userChanged)
    Q_PROPERTY(QString identifiant READ identifiant NOTIFY userChanged)
    Q_PROPERTY(int userId READ userId NOTIFY userChanged)
    Q_PROPERTY(bool student READ isStudent NOTIFY userChanged)
    Q_PROPERTY(bool professeur READ isProfesseur NOTIFY userChanged)
    Q_PROPERTY(bool admin READ isAdmin NOTIFY userChanged)
    Q_PROPERTY(int notesCount READ notesCount NOTIFY notesChanged)
    Q_PROPERTY(QString statusMessage READ statusMessage NOTIFY statusMessageChanged)

public:
    explicit SchoolFacade(QObject* parent = nullptr);

    bool logged() const;
    QString role() const;
    QString fullName() const;
    QString identifiant() const;
    int userId() const;
    int notesCount() const;
    QString statusMessage() const;

    Q_INVOKABLE bool login(const QString& login, const QString& password);
    Q_INVOKABLE void logout();
    Q_INVOKABLE QStringList getNotesLines();
    Q_INVOKABLE QStringList getUsersLines();
    Q_INVOKABLE QStringList getUsersByRole(const QString& roleName);
    Q_INVOKABLE bool canManageNotes() const;
    Q_INVOKABLE bool canManageUsers() const;
    Q_INVOKABLE bool isStudent() const;
    Q_INVOKABLE bool isProfesseur() const;
    Q_INVOKABLE bool isAdmin() const;
    Q_INVOKABLE QStringList getMyClasses();
    Q_INVOKABLE QStringList getMyEDT();
    Q_INVOKABLE QStringList getMyNotes();
    Q_INVOKABLE bool modifyNote(int noteId, double valeur);
    Q_INVOKABLE bool deleteNote(int noteId);
    Q_INVOKABLE bool createUser(const QString& nom, const QString& prenom, const QString& mdp, int roleChoisi);
    Q_INVOKABLE bool deleteUser(int userId);
    Q_INVOKABLE void refreshData();
    Q_INVOKABLE bool changePassword(int userId, const QString& newPassword);
    Q_INVOKABLE bool setPhotoPath(int userId, const QString& photoPath);
    Q_INVOKABLE bool assignStudentToClass(int etudiantId, int classeId);
    Q_INVOKABLE bool assignProfToClass(int profId, int classeId);
    Q_INVOKABLE bool assignEDTToProf(int profId, int classeId, int matiereId,
                                     const QString& jour, const QString& heureDebut,
                                     const QString& heureFin, const QString& salle);
    Q_INVOKABLE bool assignEDTToStudent(int etudiantId, int classeId);

    Q_INVOKABLE bool addNote(int etudiantId, int matiereId, double valeur, double coef, const QString& appreciation);
    Q_INVOKABLE bool addNoteEtu(int classeId, int etudiantId, int matiereId, double valeur, double coef, const QString& appreciation);
    Q_INVOKABLE QStringList getNotesByStudent(int etudiantId);
    Q_INVOKABLE QStringList getNotesBySubject(int etudiantId, int matiereId);
    Q_INVOKABLE double getMoyenne(int etudiantId, int matiereId);

    Q_INVOKABLE bool addAbsence(int etudiantId, const QString& date, const QString& heure, const QString& motif, bool justifie);
    Q_INVOKABLE bool updateAbsence(int absenceId, bool justifie, const QString& motif);
    Q_INVOKABLE QStringList getAbsencesByStudent(int etudiantId);
    Q_INVOKABLE int getAbsenceCount(int etudiantId);

    Q_INVOKABLE bool addMatiere(const QString& nom, const QString& apogee, double coef);
    Q_INVOKABLE bool addClasse(const QString& nom, const QString& niveau, const QString& voie);
    Q_INVOKABLE QStringList getAllClasses();
    Q_INVOKABLE QStringList getAllMatieres();
    Q_INVOKABLE QStringList getMatieresProfesseur(int profId);
    Q_INVOKABLE QStringList getMatieresEtudiant(int etudiantId);
    Q_INVOKABLE QStringList getEtudiantsParMatiere(int matiereId);
    Q_INVOKABLE QStringList getEtudiantsParClasse(int classeId);
    Q_INVOKABLE QStringList getMatieresClasse(int classeId);
    Q_INVOKABLE bool assignProfMatiere(int profId, int matiereId);
    Q_INVOKABLE bool affectMatiereClasse(int classeId, int matiereId, int profId, bool obligatoire);
    Q_INVOKABLE bool inscrireEtudiantMatiere(int etudiantId, int matiereId);

    Q_INVOKABLE bool addCours(const QString& jour, const QString& heureDebut, const QString& heureFin,
                              const QString& salle, int classeId, int matiereId, int profId);
    Q_INVOKABLE bool updateCours(int coursId, const QString& jour, const QString& heureDebut,
                                 const QString& heureFin, const QString& salle);
    Q_INVOKABLE bool deleteCours(int coursId);
    Q_INVOKABLE QStringList getEDTEtudiant(int etudiantId);
    Q_INVOKABLE QStringList getEDTProfesseur(int profId);
    Q_INVOKABLE QStringList getEDTGlobal();

    Q_INVOKABLE bool publishActualite(const QString& titre, const QString& contenu, const QString& datePublication);
    Q_INVOKABLE bool deleteActualite(int actualiteId);
    Q_INVOKABLE QStringList getActualites();

signals:
    void loggedChanged();
    void userChanged();
    void notesChanged();
    void statusMessageChanged();

private:
    void refreshRoleObjects();
    void refreshNotesCount();
    void setStatusMessage(const QString& message);

    Database m_bd;
    PersonneDB m_user;
    Etudiant m_etudiant;
    Professeur m_professeur;
    Administrateur m_admin;
    int m_notesCount;
    QString m_statusMessage;
};

#endif
