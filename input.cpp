#include"input.h"
Input::Input(){}
Input::Input(string n, string ap, string rp, string dp):name(n),apath(ap),rpath(rp),dpath(dp){}
string Input::get_name(){return name;}
string Input::get_apath(){return apath;}
string Input::get_rpath(){return rpath;}
string Input::get_dpath(){return dpath;}
