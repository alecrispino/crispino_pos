#pragma once

namespace bankomat{
    //events
    struct input_card_event{};
    struct input_pin_event{};
    struct wrong_pin_event{};
    struct right_pin_event{};
    struct select_balance_event{};
    struct select_withdraw_event{};
    struct confirm_event{};
    struct confirm_amount_event{};
    struct withdraw_money_event{};
    struct cancel_event{};
    struct card_output_event{};

    struct PIN;
    struct Bankomat;

    void start();
}