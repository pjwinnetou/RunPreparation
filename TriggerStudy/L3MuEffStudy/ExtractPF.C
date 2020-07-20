#include <iostream>
#include <fstream>
#include <string>
#include <sstream>

void ExtractPF(){

string fname = "Exported_L3MuEffTest_v1.py";
std::ifstream infile(fname.c_str());
std::ofstream out1;
std::ofstream out2;

out1.open("List_EDFilter.txt");
out2.open("List_EDProducer.txt");

std::string line; 

while(std::getline(infile, line)){ 
  if(line.size()>20 && line.find("cms.EDFilter") != std::string::npos){
    std::string tline(line);
    std::cout << tline << std::endl;
    std::size_t pos = tline.find("( \"");
    std::size_t bpos = tline.find("\",");
    std::string pname = tline.substr(pos+3,pos+3-bpos);
    pname.pop_back();
    pname.pop_back();
    out1 << pname <<endl;
  }
  
  if(line.size()>20 && line.find("cms.EDProducer") != std::string::npos){
    std::string tline(line);
    std::cout << tline << std::endl;
    std::size_t pos = tline.find("( \"");
    std::size_t bpos = tline.find("\",");
    std::string pname = tline.substr(pos+3,pos+3-bpos);
    pname.pop_back();
    pname.pop_back();
    out2 << pname <<endl;
  }
}
out1.close();
out2.close();
}


