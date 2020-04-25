#include <iostream>
#include "sml.hpp"
#include "bankomat.h"

using namespace boost::sml;
using namespace std;

//states
auto PIN_Aufforderung_state = "PIN_Aufforderung"_s;
auto Menue_state = "Menue"_s;
auto Kontostand_state = "Kontostand"_s;
auto Geldauswahl_state = "Geldauswahl"_s;
auto Auswahl_Bestaetigen_state = "Auswahl_Bestaetigen"_s;
auto Geldausgabe_state = "Geldausgabe"_s;
auto Karte_Einziehen_state = "Karte_Einziehen"_s;
auto Karte_Ausgeben_state = "Karte_Ausgeben"_s;

//events
auto Karte_Einfuehren_event = "Karte_Einfuehren"_e;
auto PIN_Eingabe_event = "PIN_Eingabe"_e;
auto PIN_Falsch_event = "PIN_Falsch"_e;
auto PIN_Richtig_event = "PIN_Richtig"_e;
auto Kontostand_Auswaehlen_event =  "Kontostand_Auswahlen"_e; 
auto Abheben_Auswaehlen_event = "Abheben_Auswaehlen"_e;
auto Kontostand_Bestaetigung_event = "Kontostand_Bestaetigung"_e; //maybe just one for both?
auto Auswahl_Bestaetigung_event = "Auswahl_Bestaetigung"_e; //maybe just one for both?
auto Betrag_Auswaehlen_event = "Betrag_Auswaehlen"_e;
auto Geldentnahme_event = "Geldentnahme"_e;
auto Abbruch_event = "Abbruch"_e;

//guards
const auto right_PIN = [](int pin){ if(pin == 0000){
                                        return true;
                                    }else{
                                        return false;
                                    }
                                }

//actions
const auto check_PIN = [] { std::cout << "checking PIN" << std::endl; };


