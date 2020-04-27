#pragma once

bool checkIfInt(std::string value){
    int tmp;
    try{
        tmp = std::stoi(value);
    }catch(std::exception&){
        return false;
    }
    return true;
}