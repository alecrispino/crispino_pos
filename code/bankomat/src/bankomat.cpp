#include <iostream>
#include "sml.hpp"
#include "bankomat.h"
#include "checkIfInt.h"
#include <queue>

using namespace boost::sml;
using namespace std;

namespace bankomat
{
//states
auto Automat_Bereit_state = "Automat_Bereit"_s;
auto PIN_Aufforderung_state = "PIN_Aufforderung"_s;
auto Menue_state = "Menue"_s;
auto Kontostand_state = "Kontostand"_s;
auto Geldauswahl_state = "Geldauswahl"_s;
auto Auswahl_Bestaetigen_state = "Auswahl_Bestaetigen"_s;
auto Geldausgabe_state = "Geldausgabe"_s;
auto Karte_Einziehen_state = "Karte_Einziehen"_s;
auto Abbrechen_In_Bearbeitung_state = "Abbrechen_In_Bearbeitung"_s;
auto Vorgang_Beendet_state = "Vorgang_Beendet"_s;

const auto geldentnahme = []() {
    cout << "Bitte das Geld entnehmen. Um das Geld zu entnehmen Enter Taste drücken." << endl;
    cin.clear();
    cin.ignore();
    cin.get();
    cout << "Geld entnommen." << endl;
    return true;
};

//PIN
struct PIN{
    std::string value{};
};

struct bk
{ //transition table in eigener klasse
    auto operator()()
    {
        using namespace boost::sml;

        //guards
        const auto right_PIN = [](PIN &pin) {
            return !pin.value.compare("1234");
        };

        return make_transition_table(
            *Automat_Bereit_state + event<Karte_Einfuehren_event> / [] { cout << "Karte wird eingeführt" << endl; } = PIN_Aufforderung_state,
            PIN_Aufforderung_state + event<PIN_Eingabe_event>[right_PIN] / [] { cout << "PIN ist richtig" << endl; } = Menue_state,
            PIN_Aufforderung_state + event<PIN_Eingabe_event>[!right_PIN] / [] { cout << "PIN ist falsch, Karte wird eingezogen" << endl; } = Karte_Einziehen_state,
            PIN_Aufforderung_state + event<Abbruch_event> = Abbrechen_In_Bearbeitung_state,
            Menue_state + event<Kontostand_Auswaehlen_event> / [] { cout << "Option Kontostand wurde gewählt" << endl; } = Kontostand_state,
            Menue_state + event<Abheben_Auswaehlen_event> / [] { cout << "Option Abheben wurde gewählt" << endl; } = Geldauswahl_state,
            Menue_state + event<Abbruch_event> / [] { cout << "Vorgang wird abgebrochen" << endl; } = Abbrechen_In_Bearbeitung_state,
            Kontostand_state + event<Bestaetigung_event> / [] { cout << "Option wurde bestaetigt" << endl; } = Vorgang_Beendet_state,
            Geldauswahl_state + event<Betrag_Auswaehlen_event> /*abheben*/ = Auswahl_Bestaetigen_state,
            Geldauswahl_state + event<Abbruch_event> / [] { cout << "Vorgang wird abgebrochen" << endl; } = Abbrechen_In_Bearbeitung_state,
            Auswahl_Bestaetigen_state + event<Bestaetigung_event> / [] { cout << "Auswahl bestätigen" << endl; } = Geldausgabe_state,
            Auswahl_Bestaetigen_state + event<Abbruch_event> / [] { cout << "Vorgang wird abgebrochen" << endl; } = Abbrechen_In_Bearbeitung_state,
            Geldausgabe_state + event<Geldentnahme_event>[geldentnahme] / process(Karte_Ausgeben_event{}) = Vorgang_Beendet_state,
            Vorgang_Beendet_state + event<Karte_Ausgeben_event> / [] { cout << "Karte wird ausgegeben" << endl; } = X,
            Abbrechen_In_Bearbeitung_state + event<Karte_Ausgeben_event> / [] { cout << "Karte wird ausgegeben" << endl; } = X);
    }
};

void start()
{
    PIN p;
    boost::sml::sm<bk, process_queue<std::queue>> sm{p}; //klasse dann verwenden um statemachine zu erstellen, parameter is einzugebender PIN

    cout << "Automat ist bereit. PIN ist 1234" << endl;
    cout << "Um Karte einzuführen Enter Taste drücken." << endl;
    //Karte einführen
    cin.ignore(256, '\n');
    sm.process_event(Karte_Einfuehren_event());

    cout << "Bitte geben Sie ihren PIN ein. Um den Vorgang abzubrechen X eingeben." << endl;

    //PIN Eingabe
    string input;

    cin.clear();
    cin >> input;
    if (input.compare("X") == 0 || input.compare("x") == 0)
    {
        sm.process_event(Abbruch_event());
        sm.process_event(Karte_Ausgeben_event());
    }
    p.value = input; //change PIN set user input
    sm.process_event(PIN_Eingabe_event());

    //Menü
    if (sm.is(Menue_state))
    {
        cout << "Sie befinden sich im Menü" << endl;
        cout << "Um ihren Kontostand einzusehen geben Sie 1 ein. Um Geld abzuheben geben Sie 2 ein. Um den Vorgang abzubrechen geben Sie X ein." << endl;
        cin >> input;
        bool wrongInput{true};
        while (wrongInput)
        {
            if (input.compare("1") == 0)
            {
                sm.process_event(Kontostand_Auswaehlen_event());
                wrongInput = false;
            }

            if (input.compare("2") == 0)
            {
                sm.process_event(Abheben_Auswaehlen_event());
                wrongInput = false;
            }

            if (input.compare("X") == 0 || input.compare("x") == 0)
            {
                sm.process_event(Abbruch_event());
                sm.process_event(Karte_Ausgeben_event());
                wrongInput = false;
            }

            if (wrongInput)
            {
                cout << "Fehlerhafte Eingabe. Bitte erneut eingeben." << endl;
                cin >> input;
            }
        }

        //Kontostand
        if (sm.is(Kontostand_state))
        {
            cout << "Ihr Kontostand beträgt 132 €." << endl;
            cout << "Um den Vorgang zu beenden und zu bestätigen bitte Enter Taste betätigen." << endl;
            cin.clear();
            cin.ignore();
            cin.get();
            sm.process_event(Bestaetigung_event());
            sm.process_event(Karte_Ausgeben_event());
        }

        //Geldauswahl
        if (sm.is(Geldauswahl_state))
        {
            string input;
            bool wrongInput = true;
            cout << "Bitte geben Sie den gewünschten Betrag ein. Um Abzubrechen bitte X eingeben." << endl;
            while (wrongInput)
            {
                cin >> input;
                if (input.compare("X") == 0 || input.compare("x") == 0)
                { // Abbruch noch machen
                    sm.process_event(Abbruch_event{});
                    sm.process_event(Karte_Ausgeben_event{});
                    wrongInput = false;
                }

                if (checkIfInt(input))
                {
                    if(stoi(input) > 0){
                        cout << "Folgender Betrag wurde ausgewählt: " << input << endl;
                        sm.process_event(Betrag_Auswaehlen_event{});
                        wrongInput = false;
                    }
                };

                if (wrongInput)
                {
                    cout << "Fehlerhafte Eingabe. Beachten Sie das der Beitrag positiv sein muss und nur Zahlen enthalten darf." << endl;
                    cout << "Bitte erneut eingeben: ";
                }
            }

            if (sm.is(Auswahl_Bestaetigen_state))
            {
                cout << "Um die Auswahl zu bestätigen bitte 1 eingeben. Um Abzubrechen bitte X eingeben" << endl;
                cin >> input;
                wrongInput = true;
                while (wrongInput)
                {
                    if (input.compare("1") == 0)
                    {
                        sm.process_event(Bestaetigung_event{});
                        wrongInput = false;
                    }

                    if (input.compare("X") == 0 || input.compare("x") == 0)
                    {
                        sm.process_event(Abbruch_event());
                        sm.process_event(Karte_Ausgeben_event());
                        wrongInput = false;
                    }

                    if (wrongInput)
                    {
                        cout << "Fehlerhafte Eingabe. Bitte erneut eingeben." << endl;
                        cin >> input;
                    }
                }

                sm.process_event(Geldentnahme_event{});
            }
        }
    }

    cout << "Vielen Dank für ihren Besuch" << endl;
}

} // namespace bankomat
