#include "bankomat.h"
#include "sml.hpp"
#include <iostream>
#include <queue>
#include "checkIfInt.h"

using namespace boost::sml;
using namespace std;

namespace bankomat
{
//states
auto machine_ready_state = "Automat_Bereit"_s;
auto pin_request_state = "PIN_Aufforderung"_s;
auto menue_state = "Menue"_s;
auto bank_balance_state = "Kontostand"_s;
auto money_selection_state = "Geldauswahl"_s;
auto confirm_selection_state = "Auswahl_Bestaetigen"_s;
auto money_output_state = "Geldausgabe"_s;
auto withdraw_card_state = "Karte_Einziehen"_s;
auto cancel_in_progress_state = "Abbrechen_In_Bearbeitung"_s;
auto operation_ended_state = "Vorgang_Beendet"_s;

const auto withdraw = []() {
    cout << "Bitte das Geld entnehmen. Um das Geld zu entnehmen Enter Taste drücken." 
         << endl;
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

struct Bankomat
{ //transition table in eigener klasse
    auto operator()()
    {
        using namespace boost::sml;

        //guards
        const auto right_PIN = [](PIN &pin) {
            return !pin.value.compare("1234");
        };

        return make_transition_table(
            *machine_ready_state + 
                event<input_card_event> 
                    / [] { cout << "Karte wird eingeführt" << endl; } 
                        = pin_request_state,

            pin_request_state + 
                event<input_pin_event>[right_PIN] 
                    / [] { cout << "PIN ist richtig" << endl; } 
                        = menue_state,

            pin_request_state + 
                event<input_pin_event>[!right_PIN] 
                    / [] { cout << "PIN ist falsch, Karte wird eingezogen" << endl; } 
                        = withdraw_card_state,

            pin_request_state +
                event<cancel_event> 
                = cancel_in_progress_state,

            menue_state + 
                event<select_balance_event> 
                    / [] { cout << "Option Kontostand wurde gewählt" << endl; } 
                        = bank_balance_state,

            menue_state 
                + event<select_withdraw_event> 
                    / [] { cout << "Option Abheben wurde gewählt" << endl; } 
                        = money_selection_state,

            menue_state 
                + event<cancel_event> 
                    / [] { cout << "Vorgang wird abgebrochen" << endl; } 
                        = cancel_in_progress_state,

            bank_balance_state 
                + event<confirm_event> 
                    / [] { cout << "Option wurde bestaetigt" << endl; } 
                        = operation_ended_state,

            money_selection_state 
                + event<confirm_amount_event> 
                    = confirm_selection_state,

            money_selection_state 
                + event<cancel_event> 
                    / [] { cout << "Vorgang wird abgebrochen" << endl; } 
                        = cancel_in_progress_state,

            confirm_selection_state 
                + event<confirm_event> 
                    / [] { cout << "Auswahl bestätigen" << endl; } 
                        = money_output_state,

            confirm_selection_state 
                + event<cancel_event> 
                    / [] { cout << "Vorgang wird abgebrochen" << endl; } 
                        = cancel_in_progress_state,

            money_output_state 
                + event<withdraw_money_event>[withdraw] 
                    / process(card_output_event{}) 
                        = operation_ended_state,

            operation_ended_state 
                + event<card_output_event> 
                    / [] { cout << "Karte wird ausgegeben" << endl; } 
                        = X,

            cancel_in_progress_state 
                + event<card_output_event> 
                    / [] { cout << "Karte wird ausgegeben" << endl; } 
                        = X
        );
    }
};

void start(){
    PIN p;
    //klasse dann verwenden um statemachine zu erstellen, parameter is einzugebender PIN
    boost::sml::sm<Bankomat, process_queue<std::queue>> sm{p}; 

    cout << "Automat ist bereit. PIN ist 1234" << endl;
    cout << "Um Karte einzuführen Enter Taste drücken." << endl;
    //Karte einführen
    cin.ignore(256, '\n');
    sm.process_event(input_card_event());

    cout << "Bitte geben Sie ihren PIN ein." 
         << "Um den Vorgang abzubrechen X eingeben." 
         << endl;

    //PIN Eingabe
    string input;

    cin.clear();
    cin >> input;
    if (input.compare("X") == 0 || input.compare("x") == 0)
    {
        sm.process_event(cancel_event());
        sm.process_event(card_output_event());
    }
    p.value = input; //change PIN set user input
    sm.process_event(input_pin_event());

    //Menü
    if (sm.is(menue_state))
    {
        cout << "Sie befinden sich im Menü" << endl;
        cout << "Um ihren Kontostand einzusehen geben Sie 1 ein." 
             << "Um Geld abzuheben geben Sie 2 ein."
             << "Um den Vorgang abzubrechen geben Sie X ein." 
             << endl;
        cin >> input;
        bool wrong_input{true};
        while (wrong_input)
        {
            if (input.compare("1") == 0)
            {
                sm.process_event(select_balance_event());
                wrong_input = false;
            }

            if (input.compare("2") == 0)
            {
                sm.process_event(select_withdraw_event());
                wrong_input = false;
            }

            if (input.compare("X") == 0 || input.compare("x") == 0)
            {
                sm.process_event(cancel_event());
                sm.process_event(card_output_event());
                wrong_input = false;
            }

            if (wrong_input)
            {
                cout << "Fehlerhafte Eingabe. Bitte erneut eingeben." << endl;
                cin >> input;
            }
        }

        //Kontostand
        if (sm.is(bank_balance_state))
        {
            cout << "Ihr Kontostand beträgt 132 €." << endl;
            cout << "Um den Vorgang zu beenden und zu bestätigen Enter Taste betätigen."
                 << endl;
            cin.clear();
            cin.ignore();
            cin.get();
            sm.process_event(confirm_event());
            sm.process_event(card_output_event());
        }

        //Geldauswahl
        if (sm.is(money_selection_state))
        {
            string input;
            bool wrong_input = true;
            cout << "Bitte geben Sie den gewünschten Betrag ein." 
                 << "Um Abzubrechen bitte X eingeben." 
                 << endl;
            while (wrong_input)
            {
                cin >> input;
                if (input.compare("X") == 0 || input.compare("x") == 0)
                {
                    sm.process_event(cancel_event{});
                    sm.process_event(card_output_event{});
                    wrong_input = false;
                }

                if (checkIfInt(input))
                {
                    if(stoi(input) > 0){
                        cout << "Folgender Betrag wurde ausgewählt: " << input << endl;
                        sm.process_event(confirm_amount_event{});
                        wrong_input = false;
                    }
                };

                if (wrong_input)
                {
                    cout << "Fehlerhafte Eingabe. Beachten Sie das der Beitrag positiv" 
                         << " sein muss und nur Zahlen enthalten darf." 
                         << endl;
                    cout << "Bitte erneut eingeben: ";
                }
            }

            if (sm.is(confirm_selection_state))
            {
                cout << "Um die Auswahl zu bestätigen bitte 1 eingeben." 
                     << "Um Abzubrechen bitte X eingeben" 
                     << endl;
                cin >> input;
                wrong_input = true;
                while (wrong_input)
                {
                    if (input.compare("1") == 0)
                    {
                        sm.process_event(confirm_event{});
                        wrong_input = false;
                    }

                    if (input.compare("X") == 0 || input.compare("x") == 0)
                    {
                        sm.process_event(cancel_event());
                        sm.process_event(card_output_event());
                        wrong_input = false;
                    }

                    if (wrong_input)
                    {
                        cout << "Fehlerhafte Eingabe. Bitte erneut eingeben." << endl;
                        cin >> input;
                    }
                }

                sm.process_event(withdraw_money_event{});
            }
        }
    }

    cout << "Vielen Dank für ihren Besuch" << endl;
}

} // namespace bankomat
