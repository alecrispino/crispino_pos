#pragma once

namespace bankomat{
    //events
    struct Karte_Einfuehren_event{};
    struct PIN_Eingabe_event{};
    struct PIN_Falsch_event{};
    struct PIN_Richtig_event{};
    struct Kontostand_Auswaehlen_event{};
    struct Abheben_Auswaehlen_event{};
    struct Bestaetigung_event{};
    struct Betrag_Auswaehlen_event{};
    struct Geldentnahme_event{};
    struct Abbruch_event{};
    struct Karte_Ausgeben_event{};

    void start();
}