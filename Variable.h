#ifndef VARIABLE_H
#define VARIABLE_H
struct Variable {
    Variable() {};
    Variable(std::string value) : value(value) {};
    std::string value;
};
#endif
