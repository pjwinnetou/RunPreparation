#ifndef HLT_comp_evtMatch_h 
#define HLT_comp_evtMatch_h

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
//#include <math.h>
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
#include <stdio.h>
#include <string.h>
#include <TTreeReaderValue.h>
#include <TTreeReader.h>

using namespace std;

string triglist = "triggerList.txt";
ifstream triglistfile;
vector<string> TRIGLIST;
string line;

map<TString, map<TString,TH1D*>> histonline;
map<TString, map<TString,TH1D*>> histmiscal;

std::map<std::string, std::map<std::string, std::unique_ptr<TTreeReaderValue<std::vector<double> > > > > obj_online;
std::map<std::string, std::map<std::string, std::unique_ptr<TTreeReaderValue<std::vector<double> > > > > obj_miscal;



double* ptbins; 
double* etabins; 
double* phibins; 
int nBins = -1;

double ptbins_mu[] = {0, 0.5, 1, 1.5, 2, 2.5, 3, 3.5, 4, 5, 6, 7, 8, 9, 10, 12, 14, 16, 20, 25, 30, 35, 40, 50};
double etabins_mu[] = {-2.4, -2.1, -1.8, -1.5, -1.2, -1.0, -0.8, -0.6, -0.4, -0.2, 0.0, 0.2, 0.4, 0.6, 0.8, 1.0, 1.2, 1.5, 1.8, 2.1, 2.4};
double phibins_mu[] = {0, 0.3, 0.6, 0.9, 1.2, 1.5, 1.8, 2.1, 2.4, 2.7, 3.0, 3.2};

double ptbins_pho[] = {0, 1, 2, 3, 4, 5, 6, 8, 10, 13, 16, 20, 25, 30, 35, 40, 45, 50, 60, 80, 120, 200};
double etabins_pho[] = {-2.4, -2.1, -1.8, -1.5, -1.2, -1.0, -0.8, -0.6, -0.4, -0.2, 0.0, 0.2, 0.4, 0.6, 0.8, 1.0, 1.2, 1.5, 1.8, 2.1, 2.4};
double phibins_pho[] = {0, 0.3, 0.6, 0.9, 1.2, 1.5, 1.8, 2.1, 2.4, 2.7, 3.0, 3.2};

double ptbins_ele[] = {0, 1, 2, 3, 4, 5, 6, 8, 10, 14, 18, 23, 30, 40, 50, 60, 80, 120, 200};
double etabins_ele[] = {-2.4, -2.1, -1.8, -1.5, -1.2, -1.0, -0.8, -0.6, -0.4, -0.2, 0.0, 0.2, 0.4, 0.6, 0.8, 1.0, 1.2, 1.5, 1.8, 2.1, 2.4};
double phibins_ele[] = {0, 0.3, 0.6, 0.9, 1.2, 1.5, 1.8, 2.1, 2.4, 2.7, 3.0, 3.2};

double massbins[] = {0, 0.1, 0.2, 0.3};

double* bins;
void setbins(const string trgStr, int ivar){
  const char * ptrPho = strstr(trgStr.c_str(),"Photon");
  const char * ptrEle = strstr(trgStr.c_str(),"Ele");
  const char * ptrMu = strstr(trgStr.c_str(),"Mu");

  if( (ptrPho!=NULL && ptrEle!=NULL) || (ptrPho!=NULL && ptrMu!=NULL) || (ptrEle!=NULL && ptrMu!=NULL) ){cout << "Trigger Double Matching!!! ERROR!! " << endl; return;}

  if(ptrPho!=NULL){
    ptbins = ptbins_pho;
    etabins = etabins_pho;
    phibins = phibins_pho;
  }
  else if(ptrEle!=NULL){
    ptbins = ptbins_pho;
    etabins = etabins_pho;
    phibins = phibins_pho;
  }
  else if(ptrMu!=NULL){
    ptbins = ptbins_pho;
    etabins = etabins_pho;
    phibins = phibins_pho;
  }
  else{ cout << "ERROR!!!! ::: No Matched Trigger Particle Selection!!" << endl; return;}
  
  if(ivar==0) bins = ptbins;
  else if(ivar==1) bins = etabins;
  else if(ivar==2) bins = phibins;
  else if(ivar==3) bins = massbins;
  nBins = sizeof(bins)/sizeof(bins[0]);
};

struct hltTreeBranch {
  unsigned long long event;
  int lumi;
  int runN;
};


map<unsigned long long, int> runLumiEvtToEntryMap;
const unsigned long long runMult  = 1;
const unsigned long long lumiMult = 1000000;
const unsigned long long evtMult  = 10000000000;
const unsigned long long evtLimit = 10000000000;

unsigned long long keyFromRunLumiEvent(unsigned int run, unsigned int lumi, unsigned long long event){

  unsigned long long key = 0;
  if(event >= evtLimit){
    cout << "RUNLUMIEVENTKEY WARNING : \'" << event << "\' is greated that event limit 10^10. returning key 0" << endl;
    return key;
  }

  key += runMult*((unsigned long long)run);
  key += lumiMult*((unsigned long long)lumi);
  key += evtMult*event;

  return key;

};


#endif
