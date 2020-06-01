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
}

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

/*void SetHLTObjectTree(TFile* file, int isel, string TrigPathName){
  cout << endl;
  cout << "===----======================-----------=====$$$==" << endl;
  cout << "==*****===Begin HLTObject setting=================" << endl;
  cout << "=======----=========================----^^^----===" << endl;
  cout << endl;
  cout << endl;

  cout << Form("Object selection :: %s ",selPO[isel].Data()) << endl;
  cout << endl;

  ifstream trgfile;
  trgfile.open(Form("%s",trglist[isel].c_str()));
  if(!trgfile){
  cout << "Error in opening trigger list txt file!!!" << endl;
  }

  cout << "Reading trigger list line" << endl;
  cout << "..." << endl;
  bool isTrigList=false;
  while (getline(trgfile, line))
  {
  trigname.push_back(line);
  if(trigname[line].compare(TrigPathName)==0){cout << "Trigger path exist in the list!" << endl; cout << endl; isTrigList=true;}
  }
  if(!isTrigList){cout << "ERROR!! Requested Trigger don't exist in the list!!" << endl; break}
  cout << "..." << endl;
  cout << "Done" << endl;
  cout << endl;

  TrigPathName.erase (TrigPathName.end());
  cout << "Trigger Path : " << TrigPathName << endl;
  cout << endl;

  cout << "Reading File.." << endl;
  hltobjectree = (TTree*) file->Get(Form("hltobject/%s",TrigPathName.c_str());
  cout << "Done" << endl;
  cout << endl;

  cout << "SetBranchAddress ..." <<endl;
  hltobjectree->SetBranchAddress("pt",&pt,&b_pt);
  hltobjectree->SetBranchAddress("eta",&eta,&b_eta);
  hltobjectree->SetBranchAddress("phi",&phi,&b_phi);
  hltobjectree->SetBranchAddress("mass",&mass,&b_mass);

  cout << "Done" << endl;
  cout << endl;
  cout << "Open HLT setting complete!" << endl;
  }*/

#endif
