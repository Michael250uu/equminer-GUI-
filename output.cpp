#include "output.h"
Output::Output() {}
Output::Output(Input *i1, Input *i2) : input1(i1), input2(i2) {}
Input *Output::get_input1() { return input1; }
Input *Output::get_input2() { return input2; }
bool Output::get_equal() { return equal; };
void Output::set_equal(bool r) { equal = r; };




