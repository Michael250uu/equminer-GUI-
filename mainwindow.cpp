#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <dirent.h>
#include"input.h"
#include<fstream>
#include"equminer.h"
#include<vector>
#include<QPushButton>
#include<QMessageBox>
#include<output.h>
#include<QString>
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setWindowTitle("Equminer");
setFixedSize(900, 500);
    ui->lineEdit->setPlaceholderText("请在这里输入INPUT文件夹的绝对路径");
    connect(ui->compareButton,&QPushButton::clicked,[&](){
        //read path of INPUT
        input_path =  ui->lineEdit->text().toStdString();
        const char* input_path_name = input_path.c_str();
        DIR* input_dir = opendir(input_path_name);

        if(input_dir == NULL){
             QMessageBox::information(this, "Error", "找不到input文件夹！",QStringLiteral("返回"));
        }
        else{
            dirent* curdir;
            //Traverse subfolders of INPUT
            while((curdir = readdir(input_dir)) != NULL){
               dir_name = curdir->d_name;
                if(dir_name != ".vscode" && dir_name != "." && dir_name != ".."){
                    //generate Input and Stdin_format objects
                    dir_path = input_path + "\\" + dir_name;
                    //cout<<curdir_path<<endl;
                    generate_input();
                    generate_stdin();

                    //generate Equminer and Output objects
                    //then compare each Input object, get results
                    equminer_test();

                    //empty input and stdin vector, for next directory
                    inputs.clear();
                    stds.clear();
                }
            }
            closedir(input_dir);
        }

        //output results
        generate_output();

        //compareFinished = true;

        manual_judge();

        connect(ui->equalButton,&QPushButton::clicked,[&](){
            outputs[cur_num]->set_equal(1);
            generate_output();
            manual_judge();
        });

        connect(ui->inequalButton,&QPushButton::clicked,[&](){
            outputs[cur_num]->set_equal(0);
            generate_output();
            manual_judge();
        });

        connect(ui->doubtButton,&QPushButton::clicked,[&](){
            outputs[cur_num]->set_equal(2);
            generate_output();
            manual_judge();
        });
    });

}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::generate_input()
{
    const char *dir_path_name = dir_path.c_str();
    DIR *curdir_dir = opendir(dir_path_name);
    if (curdir_dir == NULL)
    {
        cout << "Fail to open current file directory!" << endl;
    }
    else
    {
        dirent *curfile;

        while ((curfile = readdir(curdir_dir)) != NULL)
        {
            file_name = curfile->d_name;
            if (file_name != "." && file_name != ".." && file_name != "stdin_format.txt" && file_name != "input.txt" &&  file_name != "output1.txt" && file_name != "output2.txt" && file_name != "a.out" && file_name != "b.out")
            {
                file_apath = dir_path + "//" + file_name;
                file_rpath = "input//" + dir_name + "//" + file_name;
                Input *curInput = new Input(file_name, file_apath, file_rpath, dir_path);
                inputs.push_back(curInput);

            }
        }
        closedir(curdir_dir);
    }
}


void MainWindow::generate_stdin()
{
    file_apath = dir_path + "//stdin_format.txt";
    ifstream in_file(file_apath);

    if (!in_file)
    {
        cout << "Fail to open stdin_format.txt!" << endl;
    }
    else
    {
        char buffer[51];
        while (in_file >> buffer)
        {
            int t, l, r;

            if(buffer[0]=='i')
            {
                t = 1;
                if (buffer[3] == '(')
                {
                    string left_num = "", right_num = "";
                    for (int i = 4; i <= 50; ++i)
                    {
                        if (buffer[i] == ',')
                            break;
                        else
                            left_num += buffer[i];
                    }
                    l = stoi(left_num);
                    for (int i = 4 + left_num.size() + 1; i <= 50; ++i)
                    {
                        if (buffer[i] == ')')
                            break;
                        else
                            right_num += buffer[i];
                    }
                    r = stoi(right_num);
                }
                else
                    cout << "Error " << endl;


            }
            else if(buffer[0]=='c')
            {
                t = 2;
                l = 1;
                r = 1;
            }
            else if(buffer[0]=='s')
            {
                t = 2;
                if (buffer[6] == '(')
                {
                    string left_num = "", right_num = "";
                    for (int i = 7; i <= 50; ++i)
                    {
                        if (buffer[i] == ',')
                            break;
                        else
                            left_num += buffer[i];
                    }

                    l = stoi(left_num);
                    for (int i = 7 + left_num.size() + 1; i <= 50; ++i)
                    {
                        if (buffer[i] == ')')
                            break;
                        else
                            right_num += buffer[i];
                    }
                    r = stoi(right_num);
                }
                else
                    cout << "Error in stdin_format.txt!" << endl;


            }
            else
            {
                cout << "Error in stdin_format.txt!" << endl;

            }

            if (l <= r)
            {
                Stdin_format *sf = new Stdin_format(t, l, r);
                stds.push_back(sf);
            }
            else
            {
                cout << "Error in stdin_format.txt!" << endl;
            }
        }
    }
    in_file.close();
}


void MainWindow::equminer_test(){
    for(int i = 0;i < inputs.size()-1;++i){
        for(int j = i+1;j < inputs.size();++j){
            //generate Output objects
            Output* op = new Output(inputs[i],inputs[j]);
            outputs.push_back(op);

            //generate Equminer objects
            Equminer* eq = new Equminer(inputs[i],inputs[j],op,stds);
            //equ_vec.push_back(eq);

            //random test
            eq->case_compare();
        }
    }
}

void MainWindow::generate_output()
{
    //output results
    ofstream out_file_equal;
    out_file_equal.open("equal.csv");
    out_file_equal<<"file1"<<','<<"file2"<<endl;

    //inequal
    ofstream out_file_inequal;
    out_file_inequal.open("inequal.csv");
    out_file_inequal<<"file1"<<','<<"file2"<<endl;

    ofstream out_file_doubt;
    out_file_doubt.open("doubt.csv");
    out_file_doubt<<"file1"<<','<<"file2"<<endl;


    for(int i = 0;i < outputs.size();i++)
    {
        if(outputs[i]->get_equal() == 0)

            out_file_inequal<<outputs[i]->get_input1()->get_rpath()<<','<<outputs[i]->get_input2()->get_rpath()<<endl;

        else if(outputs[i]->get_equal() == 1)


            out_file_equal<<outputs[i]->get_input1()->get_rpath()<<','<<outputs[i]->get_input2()->get_rpath()<<endl;

        else
            out_file_doubt<<outputs[i]->get_input1()->get_rpath()<<','<<outputs[i]->get_input2()->get_rpath()<<endl;

    }

    out_file_equal.close();
    out_file_inequal.close();
    out_file_doubt.close();
}

void MainWindow::manual_judge()
{

    ui->listWidget->clear();
    ui->listWidget_2->clear();
    ui->label->setText("");
    ui->label_2->setText("");

    bool flag = false;
    for(int i = cur_num+1;i<outputs.size();++i){
        if(outputs[i]->get_equal() == 1){
            flag = true;
            cur_num = i;
            break;
        }
    }

    if(!flag){

            connect(ui->equalButton,&QPushButton::clicked,[&](){
                QMessageBox::information(this, "Tips", "您已完成对所有文件的人工确认！",QStringLiteral("确定"));
            });

            connect(ui->inequalButton,&QPushButton::clicked,[&](){
                QMessageBox::information(this, "Tips", "您已完成对所有文件的人工确认！",QStringLiteral("确定"));
            });

            connect(ui->doubtButton,&QPushButton::clicked,[&](){
                QMessageBox::information(this, "Tips", "您已完成对所有文件的人工确认！",QStringLiteral("确定"));
            });



        return;
    }

    fstream in_file1;
    QStringList qsl;
    in_file1.open(outputs[cur_num]->get_input1()->get_apath(),ios::in);
    if(!in_file1){
        cerr<<"Error to open file!"<<endl;
        return;
    }
    else{
        string line;
        while(getline(in_file1, line))
        {
            QString l = QString::fromStdString(line);
            qsl.append(l);

        }
        QString p = QString::fromStdString(outputs[cur_num]->get_input1()->get_rpath());
        ui->label->setText(p);
    }
    in_file1.close();
    //cout<<endl;

    fstream in_file2;
    QStringList qsl2;
    in_file2.open(outputs[cur_num]->get_input2()->get_apath(),ios::in);
    if(!in_file2){
        cerr<<"Error to open file!"<<endl;
        return;
    }
    else{
        string line;
        while(getline(in_file2, line)){
            QString l = QString::fromStdString(line);
            qsl2.append(l);
            //cout<<buf<<' ';
        }
        QString p = QString::fromStdString(outputs[cur_num]->get_input2()->get_rpath());
        ui->label_2->setText(p);
    }
    in_file2.close();

    QVector<pair<int,int>> same;
    int cur = 0;
    for(int i = 0;i<qsl.size();++i){
        for(int j = cur;j<qsl2.size();++j){
            if(qsl[i] == qsl2[j]){
                cur = j+1;
                same.append(make_pair(i,j));
                continue;
            }
        }
    }

    int curline = 0;
    QVector<int> redl,redr;
    for(int i = 0;i<same.size();++i)
    {
        int gapl = 0,gapr = 0;
        if(i == 0){
            gapl = same[i].second;
            gapr = same[i].first;
        }
        else{
            gapl = same[i].second - same[i-1].second -1;
            gapr = same[i].first - same[i-1].first -1;
        }
        for(int j = curline;j<curline+gapr;++j){
            qsl2.insert(j," ");
            redl.append(j);
        }
        for(int j = curline;j<curline+gapl;++j){
            qsl.insert(j+gapr," ");
            redr.append(j+gapr);
        }
        curline += (gapl+gapr+1);
    }
    int sizel = qsl.size(),sizer = qsl2.size();
    for(int i = curline;i<sizel;++i){
        qsl2.insert(i," ");
        redl.append(i);
    }
    for(int i = curline;i<sizer;++i){
        qsl.insert(i+sizel-curline," ");
        redr.append(i+sizel-curline);
    }

    ui->listWidget->insertItems(0,qsl);
    ui->listWidget_2->insertItems(0,qsl2);
    for(int i = 0;i<redl.size();++i)
        ui->listWidget->item(redl[i])->setForeground(QColor("red"));
    for(int i = 0;i<redr.size();++i)
        ui->listWidget_2->item(redr[i])->setForeground(QColor("red"));
}
