#include <iostream>
#include "sml.hpp"
#include "bankomat.h"

using namespace boost::sml;
using namespace std;

namespace bankomat{
    //states
    auto Automat_Bereit_state = "Automat_Bereit"_s;
    auto PIN_Aufforderung_state = "PIN_Aufforderung"_s;
    auto Menue_state = "Menue"_s;
    auto Kontostand_state = "Kontostand"_s;
    auto Geldauswahl_state = "Geldauswahl"_s;
    auto Auswahl_Bestaetigen_state = "Auswahl_Bestaetigen"_s;
    auto Geldausgabe_state = "Geldausgabe"_s;
    auto Karte_Einziehen_state = "Karte_Einziehen"_s;
    auto Karte_Ausgegeben_state = "Karte_Ausgeben"_s;
    auto Abbrechen_In_Bearbeitung_state = "Abbrechen_In_Bearbeitung"_s;
    auto Vorgang_Beendet_state = "Vorgang_Beendet"_s;

    //events
    auto Karte_Einfuehren_event = "Karte_Einfuehren"_e;
    auto PIN_Eingabe_event = "PIN_Eingabe"_e;
    auto PIN_Falsch_event = "PIN_Falsch"_e;
    auto PIN_Richtig_event = "PIN_Richtig"_e;
    auto Kontostand_Auswaehlen_event = "Kontostand_Auswahlen"_e;
    auto Abheben_Auswaehlen_event = "Abheben_Auswaehlen"_e;
    //auto Kontostand_Bestaetigung_event = "Kontostand_Bestaetigung"_e; //maybe just one for both?
    //auto Auswahl_Bestaetigung_event = "Auswahl_Bestaetigung"_e;       //maybe just one for both?
    auto Bestaetigung_event = "Bestaetigung"_e;
    auto Betrag_Auswaehlen_event = "Betrag_Auswaehlen"_e;
    auto Geldentnahme_event = "Geldentnahme"_e;
    auto Abbruch_event = "Abbruch"_e;
    auto Karte_Ausgeben_event = "Karte_Ausgeben"_e;
    //struct Abbruch_Ev{};
    //auto Abbruch_event = sml::event<Abbruch_Ev>;

    //guards
    /*const auto right_PIN = [](int pin){ if(pin == 0000){
                                            return true;
                                        }else{
                                            return false;
                                        }
                                    };

    //actions
    const auto check_PIN = [] { std::cout << "checking PIN" << std::endl; };*/

    class bk { //transition table in eigener klasse
    public:
    auto operator()() {
        using namespace boost::sml;
        return make_transition_table(
                * Automat_Bereit_state         + Karte_Einfuehren_event            = PIN_Aufforderung_state,
                /* PIN Stuff, Guard, Action */
                Menue_state                    + Kontostand_Auswaehlen_event       = Kontostand_state,
                Menue_state                    + Abheben_Auswaehlen_event          = Geldauswahl_state,
                Menue_state                    + Abbruch_event                     = Abbrechen_In_Bearbeitung_state,
                Kontostand_state               + Bestaetigung_event                = Vorgang_Beendet_state,
                Geldauswahl_state              + Betrag_Auswaehlen_event           = Auswahl_Bestaetigen_state,
                Geldauswahl_state              + Abbruch_event                     = Abbrechen_In_Bearbeitung_state,
                Auswahl_Bestaetigen_state      + Bestaetigung_event                = Geldausgabe_state,
                Auswahl_Bestaetigen_state      + Abbruch_event                     = Abbrechen_In_Bearbeitung_state,
                Geldausgabe_state              + Geldentnahme_event                = Vorgang_Beendet_state,
                Vorgang_Beendet_state          + Karte_Ausgeben_event              = Karte_Ausgegeben_state,
                Abbrechen_In_Bearbeitung_state + Karte_Ausgeben_event              = Karte_Ausgegeben_state
        );
    }
};

    void start()
    {
        cout << "start" << endl;

        boost::sml::sm<bk> sm; //klasse dann verwenden um statemachine zu erstellen

        sm.process_event(Karte_Einfuehren_event());
        sm.visit_current_states([](auto state) { std::cout << state.c_str() << std::endl; });
        assert(sm.is(Automat_Bereit_state));
    }

} // namespace bankomat
