#include "equminer.h"







Stdin_format::Stdin_format() {}
Stdin_format::Stdin_format(int t, int l, int r) : type(t), left(l), right(r) {}
int Stdin_format::get_type() { return type; }
int Stdin_format::get_left() { return left; }
int Stdin_format::get_right() { return right; }

Equminer::Equminer() {}
Equminer::Equminer(Input *i1, Input *i2, Output *o, vector<Stdin_format *> &st) : input1(i1), input2(i2), output(o), stf(st) {}

string Equminer::random_case(int n)
{
    int t = stf[n]->get_type();
    int rand_num = stf[n]->get_left() + rand() % (stf[n]->get_right() - stf[n]->get_left() + 1);
    if (t == 1)
    {
        return to_string(rand_num);
    }
    else if (t == 2)
    {
        string str = "";
        char rand_char;
        for (int i = 0; i < rand_num; ++i)
        {
            rand_char = 'a' + rand() % 26;
            str += rand_char;
        }
        return str;
    }
}
void Equminer::case_compare()
{
    string c1 = "g++ -w -o a.out ";
    c1 += input1->get_apath();
    system(c1.c_str());

    string c2 = "g++ -w -o b.out ";
    c2 += input2->get_apath();
    system(c2.c_str());


    for (int i = 0; i < 10; ++i)
    {
        string inputfile_path = input1->get_dpath() + "/input.txt";
        string outputfile_path1 = input1->get_dpath() + "/output1.txt";
        string outputfile_path2 = input2->get_dpath() + "/output2.txt";

        ofstream out_file;
        out_file.open(inputfile_path);


        for (int j = 0; j < stf.size(); ++j)
        {
            out_file << random_case(j) << ' ';

        }

        out_file.close();


        char runner1[1001] = ".\\a.out <";
        strcat(runner1, inputfile_path.c_str());
        strcat(runner1, " >");
        strcat(runner1, outputfile_path1.c_str());
        int r1 = system(runner1);

        char runner2[1001] = ".\\b.out <";
        strcat(runner2, inputfile_path.c_str());
        strcat(runner2, " >");
        strcat(runner2, outputfile_path2.c_str());
        int r2 = system(runner2);


        if ((r1 != 0 && r2 == 0) || (r2 != 0 && r1 == 0))
        {
            output->set_equal(0);
            return;
        }
        if (r1 != 0 && r2 != 0)
        {
            continue;
        }


        fstream in_file1;
        vector<string> res1;
        in_file1.open(outputfile_path1);
        if (!in_file1)
        {
            cout << "Fail to open output1.txt!" << endl;
        }
        else
        {
            char buffer[51];
            while (in_file1 >> buffer)
            {
                string buf = buffer;
                res1.push_back(buf);
            }
        }
        in_file1.close();

        fstream in_file2;
        vector<string> res2;
        in_file2.open(outputfile_path2, ios::in);
        if (!in_file2)
        {
            cout << "Fail to open output2.txt!" << endl;
        }
        else
        {
            char buffer[51];
            while (in_file2 >> buffer)
            {
                string buf = buffer;
                res2.push_back(buf);
            }
        }
        in_file2.close();

        //compare
        if (res1.size() != res2.size())
        {
            output->set_equal(0);
            return;
        }
        else
        {
            for (int j = 0; j < res1.size(); ++j)
            {
                if (res1[j] != res2[j])
                {
                    output->set_equal(0);
                    return;
                }
            }
        }
    }
    //same output for ten times,then equal
    output->set_equal(1);
    //cout<<endl;
}



