#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include<string>
#include <QMainWindow>
#include<string.h>
#include<iostream>
#include<vector>
#include <stdlib.h>
using namespace std;
class Input;
class Output;
class Stdin_format;
namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    void generate_input();
    void generate_stdin();
    void equminer_test();
    void generate_output();
    void manual_judge();

private:
    Ui::MainWindow *ui;
    string input_path;
    string dir_path;
    string dir_name;
    string file_apath;
    string file_rpath;
    string file_name;
    vector<Input *> inputs;
    vector<Output *> outputs;
    vector<Stdin_format *> stds;
    int cur_num=-1;

};

#endif // MAINWINDOW_H
