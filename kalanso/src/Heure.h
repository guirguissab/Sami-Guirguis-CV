#ifndef HEURE_H
#define HEURE_H
#include <string>
#include <cassert>
class Heure {
    private:
        unsigned int h;
        unsigned int minute;

    public:
        Heure();
        Heure(unsigned int h, unsigned int minute);

        ~Heure();

        std::string afficheHeure();

        void testRegression() const;
    };

#endif
