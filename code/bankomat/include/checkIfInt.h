#pragma once

bool checkIfInt(std::string value){
    try{
        std::stoi(value);
    }catch(std::exception&){
        return false;
    }
    return true;
}