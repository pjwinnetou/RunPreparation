#ifndef HLT_comp_h
#define HLT_comp_h

#include <TF1.h>
#include <TCut.h>
#include <TChain.h>
#include <TGraphAsymmErrors.h>
#include <TCanvas.h>
#include <TNtuple.h>
#include <iostream>
#include <TLine.h>
#include <TMath.h>
#include <TTree.h>
#include <math.h>
#include <TVector3.h>
#include <TLorentzVector.h>
#include <TROOT.h>
#include <TClonesArray.h>
#include <TFile.h>
#include <TH1D.h>
#include <TH2D.h>
#include <TLegend.h>
#include <iostream>
#include <vector>

using namespace std;


string triglist = "triggerList.txt";
ifstream triglistfile;
vector<string> TRIGLIST;
string line;
  
map<TString, map<TString,TH1D*>> histonline;
map<TString, map<TString,TH1D*>> histmiscal;

std::map<std::string, std::map<std::string, std::unique_ptr<TTreeReaderValue<std::vector<double> > > > > obj_online;
std::map<std::string, std::map<std::string, std::unique_ptr<TTreeReaderValue<std::vector<double> > > > > obj_miscal;


const int nVar=4;
const int nBins[nVar]={14, 20, 11, 3};
double ptbins[nBins[0]+1] = {0, 1, 2, 3, 4, 5, 6, 8, 10, 14, 18, 23, 30, 40, 50};
double etabins[nBins[1]+1] = {-2.4, -2.1, -1.8, -1.5, -1.2, -1.0, -0.8, -0.6, -0.4, -0.2, 0.0, 0.2, 0.4, 0.6, 0.8, 1.0, 1.2, 1.5, 1.8, 2.1, 2.4};
double phibins[nBins[2]+1] = {0, 0.3, 0.6, 0.9, 1.2, 1.5, 1.8, 2.1, 2.4, 2.7, 3.0, 3.2};
double massbins[nBins[3]+1] = {0, 0.1, 0.2, 0.3};

double* bins;
void setbins(int ivar){
  if(ivar==0) bins = ptbins;
  else if(ivar==1) bins=etabins;
  else if(ivar==2) bins=phibins;
  else if(ivar==3) bins=massbins;
};


#endif
