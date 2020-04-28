#ifndef SetHLT_C
#define SetHLT_C

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

using namespace std;
const int nPO = 2;
const int MAXBRANCH = 100;
vector<string> trigname;
string line;
vector<string> trglist = {"triggerList_mu.txt","triggerList_eg.txt"};
TString selPO[nPO] = {"Muon", "E/Gamma"};
vector<int> trig;
TBranch *b_trig[MAXBRANCH];
int MBTrigline = -1;
const char* MBTrigName = "HLT_HIMinimumBias_SinglePixelTrack_NpixBypass_part0_v1";

void SetHLT(TTree *tree = 0, int isel = 0){
  cout << endl;
  cout << "=======================================" << endl;
  cout << "==========Begin HLT setting============" << endl;
  cout << "=======================================" << endl;
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
  int linecounter=0;
  while (getline(trgfile, line))
  {
    trigname.push_back(line);
    if(line.find(Form("%s",MBTrigName)) !=string::npos){MBTrigline = linecounter;}
    linecounter++;
  }
  cout << endl; cout << endl;
  cout << "Done" << endl;
  cout << "MB Trigger line number : " << MBTrigline << endl;
  if(MBTrigline==-1){cout << "ERROR!! No matching MB trigger in the list" << endl;}
  
  cout << "Resize trigger int variable" << endl;
  trig.resize(trigname.size(),0);
  cout << "Done" << endl;

  cout << "Size of trig vector : " << trigname.size() << " // Size of trigname int vector : " << trig.size() << endl;
  cout << endl;

  if(trigname.size() != trig.size()){cout << "ERORR!!! Inconsistent vector size!!! " << endl;}
  cout << "SetBranchAddress ..." <<endl;
  for(int itrig=0; itrig<trig.size(); itrig++){
    tree->SetBranchAddress(Form("%s",trigname[itrig].c_str()),&trig[itrig],&b_trig[itrig]);
  }
  cout << "Done" << endl;
  cout << endl;
  cout << "HLT setting complete!" << endl;
}

#endif


