#ifndef ABSENCE_H
#define ABSENCE_H

#include "Date.h"
#include "Heure.h"

#include <string>
#include <cassert>

/**
 * @brief Représente un enregistrement d'absence.
 */

class Absence{
    private:
        unsigned int idAb;  ///< Identifiant unique de l'absence
        Date Dateabsc;      ///< Date de l'absence
        Heure heureAbsc;    ///< Heure de l'absence
        std::string motif;  ///< Motif de l'absence
        bool justif;        ///< Indique si l'absence est justifiée
    public:
         /**
            * @brief Constructeur par défaut.
         */
         Absence();
         
        /**
            * @brief Destructeur.
         */
         ~Absence();
        
        /**
            * @brief Retourne la date de l'absence.
            * @return Référence vers l'objet Date.
         */
         const Date& getDate();
        
         /**
            * @brief Retourne le motif de l'absence.
            * @return Référence vers la chaîne motif.
         */
         const std::string& getMotif();

        /**
            * @brief Retourne le statut de justification.
            * @return Référence vers le booléen de justification.
         */
         const bool& getJustif();

        /**
            * @brief Retourne l'heure de l'absence.
            * @return Référence vers l'objet Heure.
         */
         Heure& getHeure();
        
        /**
            * @brief Retourne l'identifiant unique de l'absence.
            * @return Référence vers l'identifiant.
         */
         const unsigned int& getId();


         /**
            * @brief Définit tous les attributs de l'absence en une seule fois.
            * @param nIda Identifiant unique de l'absence.
            * @param d Date de l'absence.
            * @param he Heure de l'absence.
            * @param mot Motif de l'absence.
            * @param j Indique si l'absence est justifiée.
         */
         void setAbsc(const unsigned int nIda ,const Date d,const Heure he,const std::string mot,const bool j);
        
         /**
            * @brief Définit l'heure de l'absence.
            * @param he Nouvelle valeur de l'heure.
         */
         void setHeure (const Heure he);
         
         /**
            * @brief Définit le motif de l'absence.
            * @param mot Nouveau motif.
         */
        void setMotif(const std::string mot);
        
        /**
            * @brief Définit la date de l'absence.
            * @param d Nouvelle date.
         */
         void setDate(const Date d);
        
         /**
            * @brief Définit le statut de justification.
            * @param j Nouvelle valeur de justification.
         */
        void setJustif(const bool j);
        
         /**
            * @brief Définit l'identifiant unique de l'absence.
            * @param nIda Nouvel identifiant.
         */
         void setId(const unsigned int nIda);

         /**
            * @brief test de regression
          */
         void testRegression() const;

};

#endif