#include <iostream>
#include <string>
#include <iomanip>
#include <ctime>
#include <cstring>
#include <fstream>
#include <sys/stat.h>
#include <vector>
#include <algorithm>
#include <stdlib.h>
//#include <sstring.h>

using namespace std;

ofstream out;
string newsymbol;
float bid, ask;
int ttq, pbid, pask, seq, prevseq,ltp;
string bidq, askq, lrt, t_lrt, lastprint, prevstr;
vector<int> trade_seq, processed_seq;


vector<string> split (string s, string delimiter) {
    size_t pos_start = 0, pos_end, delim_len = delimiter.length();
    string token;
    vector<string> res;

    while ((pos_end = s.find (delimiter, pos_start)) != string::npos) {

        token = s.substr (pos_start, pos_end - pos_start);
        pos_start = pos_end + delim_len;
        res.push_back (token);
    }

    res.push_back (s.substr (pos_start));
    return res;
}


string gettlrt(){
        return lastprint;
}

string ro(float var){
        float value = (int)(var * 100);
        return to_string((float)value / 100).substr(0,7);

}

void writeline(){

        if(ttq > 0){
                if(prevseq != 0)
                        out << prevstr;
                prevseq = seq;
                prevstr = gettlrt() + "," + newsymbol + "," + bidq + "," + ro(bid)  + "," + askq + "," + ro(ask) + "," + to_string(ltp) + "," + to_string(ttq)  +"\n";
        }
}

int main(int argc, char *argv[]){
        string symbol = argv[1];
        string subsymbol = argv[2];
        //cout << symbol << " " <<subsymbol << endl;

        //string folder = "/mnt_data/arvin/interaday/";
        //string folder = "home/cpptest/Akash/Merge/";
        string folder = "data/";
        newsymbol = subsymbol + "1cmerevnew2_Comdty";

        time_t now = time(0);
        struct tm *tm;
        tm = localtime(&now);
        char cdt[32], pdt[32];
        // Current Date --------------------------------------------
        strftime(cdt, 32, "%Y%m%d", tm);
        string cds(cdt);

        tm-> tm_mday--;
        mktime(tm);
        // Previous Date --------------------------------------------
        strftime(pdt, 32, "%Y%m%d", tm);
        string pds(pdt);

        //cout << cds << " " << pds<<endl;
        string file_name = folder + cds + "/" + subsymbol + "Pricelog.txt";
        //string file_name = cds + "/" + subsymbol + "Pricelog.txt";
        cout << file_name << endl;

        ifstream in;
        in.open(file_name.c_str());
        if(in.is_open()){
                string f= folder + cds;

                /*
                 if(not IsPathExist(folder + cds))
                        if(mkdir(f.c_str(),0777) == -1)
                                cout << "File Cannot be created";
                */

                bid = 0.0, ask = 0.0, ltp = 0.0;
                ttq = 0, pbid = 0, pask = 0, seq = 0, prevseq = 0;
                bidq = "0", askq = "0", lrt = "", t_lrt="", lastprint = "", prevstr = "";
                string ofn = folder + cds + "/" + newsymbol + ".csv";

                out.open(ofn.c_str());
                string line;
                getline(in , line);
                vector <string> larr = split(line , ",");

                pds = "20210328";
                if(larr[0] == pds){
                        while(!in.eof()){
                                vector<string> lr = split(line, ",");
                                if(lr.size() > 5 && lr[6] == symbol){
                                        if(lr[9] != "A" and  lr[9] != "B"){
                                                ttq += atoi(lr[10].c_str());
                                                seq = atoi(lr[4].c_str());
                                                ltp = atof(lr[9].c_str())* atof(lr[13].c_str());
                                                t_lrt = lr[2] +  lr[3];
                                                trade_seq.push_back(atoi(lr[4].c_str()));

                                                if(bid < ask){
                                                        lastprint = t_lrt.substr(8);
                                                        int co = count(processed_seq.begin(), processed_seq.end(), seq);
                                                        if(co ==0 )
                                                                processed_seq.push_back(seq);
                                                        writeline();
                                                        pbid = bid;
                                                        pask = ask;

                                                }
                                        }
                                        if(lr[9] == "A" or  lr[9] == "B"){
                                                seq = atoi(lr[4].c_str());
                                                lrt = lr[2] +  lr[3];
                                                string line2;

                                                getline(in, line2);
                                                vector<string> lrpair = split(line2, ",");
                                                //cout << lrpair[2]+lrpair[3] <<  t_lrt << endl;
                                                if((lrpair.size() > 5 && lrpair[6] == symbol) &&  lrpair[2]+lrpair[3] == lrt){
                                                        //cout << bid << " " << ask <<"______________________\n";
                                                        if(lr[9]=="A" && lrpair[9]== "B"){
                                                                bid = atoi(lrpair[10].c_str())* atof(lrpair[12].c_str());
                                                                bidq = lrpair[11];
                                                                ask =  atoi(lr[10].c_str())* atof(lr[12].c_str());
                                                                askq =lr[11];
                                                        //      cout << bid << " " << ask <<"______________________\n";
                                                        }
                                                         else if(lr[9]=="B" && lrpair[9]== "A"){
                                                                ask = atoi(lrpair[10].c_str())* atof(lrpair[12].c_str());
                                                                askq = lrpair[11];
                                                                bid =  atoi(lr[10].c_str())* atof(lr[12].c_str());
                                                                bidq =lr[11];
                                                        //      cout << bid << " " << ask <<"______________________\n";
                                                        }

                                                        if((pbid != bid or pask != ask) && bid < ask){
                                                                lastprint = lrt.substr(8);
                                                                int co = count(processed_seq.begin(), processed_seq.end(), seq);
                                                                if(co == 0 )
                                                                        processed_seq.push_back(seq);
                                                                writeline();
                                                                pbid = bid;
                                                                pask = ask;
                                                        }
                                                }
                                        }
                                        getline(in, line);
                                }

                        }
                        out << prevstr ;
                }


        }
        else
                cout << "Input file not found for merge \n";

}
