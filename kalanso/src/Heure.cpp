#include "Heure.h"
#include <cassert>
#include <iostream>

Heure::Heure(): h(0),minute(0){}

Heure::Heure(unsigned int h, unsigned int minute) {
    this->h = h;
    this->minute = minute;
}

Heure::~Heure() {
}

std::string Heure::afficheHeure() {
    return std::to_string(h) + ":" + std::to_string(minute);
}

void Heure::testRegression() const {
    Heure h1;
    assert(h1.h == 0 && h1.minute == 0);

    Heure h2(10, 30);
    assert(h2.h == 10 && h2.minute == 30);

    assert(h2.afficheHeure() == "10:30");
    std::cout <<std::endl;
    std::cout<<"ok Heure"<<std::endl;
}
