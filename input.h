#ifndef INPUT_H
#define INPUT_H
#include <string.h>
#include <iostream>
using namespace std;

class Input
{
private:
    string name;                   //name of file
    string apath;          //absolute path
    string rpath; //relative path
    string dpath;               //directory path
public:
    Input();
    Input(string n, string ap, string rp, string dp);

    string get_name();
    string get_apath();
    string get_rpath();
    string get_dpath();
};
#endif // INPUT_H
