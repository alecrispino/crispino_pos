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

    //PIN
    struct PIN {
        int value{};
    };

    struct bk { //transition table in eigener klasse
    auto operator()() {
        using namespace boost::sml;

        //guards
        const auto right_PIN = [](PIN& pin){ cout << "PIN VALUE: " << pin.value << endl;
                                        return pin.value == 1234;
                                    };

        return make_transition_table(
                * Automat_Bereit_state         + Karte_Einfuehren_event / []{ cout << "karte einfuehren" << endl; }            = PIN_Aufforderung_state,
                /* PIN Stuff, Guard, Action */
                PIN_Aufforderung_state         + PIN_Eingabe_event [right_PIN]                                     = Menue_state,
                PIN_Aufforderung_state         + PIN_Eingabe_event [!right_PIN]                                    = Karte_Einziehen_state,
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
        PIN p;
        boost::sml::sm<bk> sm{p}; //klasse dann verwenden um statemachine zu erstellen, parameter is einzugebender PIN

        sm.process_event(Karte_Einfuehren_event());
        sm.visit_current_states([](auto state) { std::cout << state.c_str() << std::endl; });
        //assert(sm.is(Automat_Bereit_state));
        p.value = 1234; //channge PIN set user input
        sm.process_event(PIN_Eingabe_event());
        sm.visit_current_states([](auto state) { std::cout << state.c_str() << std::endl; });
    }

} // namespace bankomat
