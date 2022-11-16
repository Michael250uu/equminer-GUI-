#pragma once
#include <string.h>
#include <iostream>
#include"input.h"
using namespace std;
class Output
{
private:
    Input* input1 = NULL;
    Input* input2 = NULL;
    bool equal = 0;                 //0 = inequal, 1 = equal
public:
    Output();
    Output(Input* i1,Input* i2);

    Input* get_input1();
    Input* get_input2();
    bool get_equal();
    void set_equal(bool r);
};