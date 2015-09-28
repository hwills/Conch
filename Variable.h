//
//  Variable.h
//  ShellFish
//
//  Created by Morgan Redding on 9/24/15.
//  Copyright © 2015 Morgan Redding. All rights reserved.
//

#ifndef Variable_h
#define Variable_h

#include <assert.h>

enum VariableType {
    var_none,
    var_integer,
    var_string
};

struct Variable {
    
    Variable() : type(var_none) {};
    Variable(std::string value) : string_value(value), int_value(0), type(var_string) {};
    Variable(int value) : string_value(""), int_value(value), type(var_integer) {};
    
    Variable operator+(const Variable& b) const {
        assert(this->type == b.type);
        if(type == var_integer) {
            return Variable(int_value + b.int_value);
        }
        else if(type == var_string) {
            return Variable(string_value + b.string_value);
        }
        else {
            return Variable();
        }
    }
    
    std::string value_as_string() const {
        if(type == var_integer) {
            return std::to_string(int_value);
        }
        else if(type == var_string) {
            return "\"" + string_value + "\"";
        }
        return "";
    }
    
    int int_value;
    std::string string_value;
    VariableType type;
};

#endif /* Variable_h */
