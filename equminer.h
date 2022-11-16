#pragma once
#include"output.h"
#include<vector>
#include <dirent.h>
#include<fstream>
#include<iostream>
using namespace std;
class Stdin_format
{
private:
    int type;       //int = 1, string = 2, char = string(1,1)
    int left;
    int right;
public:
    Stdin_format();
    Stdin_format(int t,int l,int r);

    int get_type();
    int get_left();
    int get_right();
};

class Equminer{
private:
    Input* input1 = NULL;
    Input* input2 = NULL;
    Output* output = NULL;
    vector<Stdin_format*> stf;
public:
    Equminer();
    Equminer(Input* i1,Input* i2,Output* o,vector<Stdin_format*> &st);

    string random_case(int n);      //Generate random test samples
    void case_compare();            //Compare using test samples
};
