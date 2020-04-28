#include <TROOT.h>
#include <TSystem.h>
#include <TH1.h>
#include <TCanvas.h>
#include <TMath.h>
#include <TFile.h>
#include "Style_jaebeom.h"
#include "SetHLT.C"

using namespace std;
void makeRatePlot(const char* filename = "openHLT",int isel=0)
{
 
  TFile* rf = new TFile(Form("%s.root",filename),"read");
  TTree* tree = (TTree*) rf->Get("hltbitanalysis/HltTree"); 
  SetHLT(tree,isel);
  cout << "# of events : " << tree->GetEntries() << endl;
  
  TFile* wf = new TFile(Form("Rate_HLT_%sObject.root",selPO[isel].Data()),"recreate");
  const int nTrig = trigname.size();
  cout << "nTrig : " << nTrig << endl; 

  map<TString,TH1D*> hist;
  int trigNum=0;
  for(const auto& triggerStr : trigname){
    hist[trigNum] = new TH1D(Form("h_%s",triggerStr.c_str()),";;;",5,0,5);
    trigNum++;
    cout << "trigNum : " << trigNum << endl;
  }
  
  for(int itrig=0;itrig<nTrig;itrig++){
    for(int ievt = 0; ievt<tree->GetEntries();ievt++){
    tree->GetEntry(ievt);
      if(trig[itrig]==1){
        hist[itrig] -> Fill(0);
        if(trig[MBTrigline]==1){hist[itrig]->Fill(1);}
      }
    }
    if(hist[itrig]->GetBinContent(1)!=0){
      hist[itrig]->SetBinContent(3,(double)hist[itrig]->GetBinContent(1)/tree->GetEntries());
      if(hist[itrig]->GetBinContent(2)!=0) hist[itrig]->SetBinContent(4,(double)hist[itrig]->GetBinContent(1)/hist[itrig]->GetBinContent(2));
      else hist[itrig]->SetBinContent(4,0);
    }
    else{hist[itrig]->SetBinContent(3,0);}
    hist[itrig]->Write();
  }
    
}

  

