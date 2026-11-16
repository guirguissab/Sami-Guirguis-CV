#ifndef ACTUALITE_H
#define ACTUALITE_H


#include <string>

#include "Date.h"
#include <cassert>

/**
    * @brief Représente les actualite dans l'ecole
*/


class Actualite{
    private:
        unsigned int idA;       ///< Identifiant unique de l'article
        std::string titre;      ///< Le titre d'article
        std::string contenu;    ///< Le contenu de l'article
        Date datePublication;   ///< La date de publication de l'article
    public:
        /**
            * @brief Constructeur par défaut.
        */
        Actualite();
        /**
            * @brief Constructeur qui donne.
            * @param id Identifiant unique de l'actualite
            * @param tit Le titre de l'article
            * @param cont Le contenue de l'article
            * @param datepub est la date de publication de l'article
        */
        Actualite(const unsigned int& id,const std::string& tit,const std::string& cont,const Date& datepub);

        /**
            * @brief fonction qui retourn l'identifiant de l'article
            * @return l'identifiant unique de l'article
        */
        unsigned int getId() const;

        /**
            * @brief fonction qui retourn le titre de l'article
            * @return le titre l'article
        */
        std::string getTitre()const;
        
        /**
            * @brief fonction qui retourn le contenue de l'article
            * @return le contenue l'article
        */
        std::string getContenu()const;

        /**
            * @brief fonction qui retourn la date de l'article
            * @return la date l'article
        */
        Date getDate()const;


        /**
            * @brief fonction qui change la titre de l'article
            * @param tit la nouvelle titre de l'article
        */
        void setTitre(const std::string& tit);
        
        /**
            * @brief fonction qui change le contenue de l'article
            * @param cont le nouvel contenu de l'article
        */
        void setContenu(const std::string& cont);
        
        /**
            * @brief fonction qui change le contenue de l'article
            * @param cont le nouvel contenu de l'article
        */
        void setDate(const Date& datepub);

        /**
            * @brief test de regression
        */
        void testRegression() const;

};


#endif