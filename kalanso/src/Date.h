#ifndef _Date_h_
#define _Date_h_
#include <cassert>

class Date {
    private:
        unsigned int j,m,a;
    public :
        Date();

        Date(const unsigned int& jj,const unsigned int& mm, const unsigned int& aa);


        unsigned int getjour() const;

        unsigned int getmois() const;

        unsigned int getanne() const;

        void affichedate() const;

        ~Date();

        void testRegression() const;

};
#endif