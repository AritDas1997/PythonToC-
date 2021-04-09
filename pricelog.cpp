#include<stdio.h>
#include<iostream>
#include<fstream>
#include<sstream>
#include<string>
#include<bits/stdc++.h>
#include<ostream>
#include<stdlib.h>
#include<string.h>
#include<stdint.h>
using namespace std;
void tokenizer(string const &str, const char delim,vector<string> &tokens)
{
    int i=0;
    stringstream ss(str);
    string s;
    while (getline(ss, s, delim)) {
        if(i==0 || i==1)
     {
        tokens.push_back(s);
        }
        i++;

    }
}

double time_sec(string time_part){
                vector<string> time;
                double test;
                stringstream check1(time_part);
                string time_slice;

                 while (getline(check1, time_slice, ':') )
                        {

                                time.push_back(time_slice);

                        }
                double h=atof(time[0].c_str());
                double m=atof(time[1].c_str());
                double s=atof(time[2].c_str());
                double t_present=h*3600+m*60+s;
                sscanf(to_string(t_present).c_str(),"%12lf",&test);
                return test;
}

int main(){
        string FirstFile,output,myText,myText1;
        int j=0,count=0,n;
	double t_prev,t_present;
        list<string> m;
        const char delim=',';
        cout<<"Enter first file name\n";
        cin>>FirstFile;
        cout<<"Enter output file name\n";
        cin>>output;
        ifstream MyReadFile(FirstFile.c_str());
        ifstream MyReadFile1(FirstFile.c_str());
        ofstream ob(output.c_str());
        m.push_back("0");
        while(1){
                int i=0;
                vector<string> token;
                getline(MyReadFile, myText);
                j++;
                tokenizer(myText,delim,token);
                if(MyReadFile.eof()){
                        break;
                }
                if(token.size()<2)
                        continue;
                const bool is_in = find(m.begin(), m.end(),token[1]) != m.end();
                if(is_in==1){
                        continue;
                }
                else{
                        m.push_back(token[1]);
                        ob<<myText;
                        ob<<"\n";
                        t_prev=time_sec(token[0]);
                        t_present=time_sec(token[0]);
                        int v=0;
                        while(v<j-1){
        			getline(MyReadFile1, myText1);
                                v++;
                        }
                        while(1){
                                int l=0;
                                vector<string> token1;
                                getline(MyReadFile1, myText1);
                                tokenizer(myText1,delim,token1);
                                 if(MyReadFile1.eof()){
                                           break;
                                }
                                 if(token1[1].compare(m.back())==0){
                                         t_present=time_sec(token1[0]);
                                         if(t_present-t_prev>=1){
                                                   		ob<<myText1;
                                                        	ob<<"\n";
                                          }
                                }
                                else{
                                     continue;
                                }
                                t_prev=t_present;
                                token1.clear();
                        }
                        MyReadFile1.clear();
                        MyReadFile1.seekg(0);
                }
                token.clear();
        }
        ob.close();
        return 0;
}
