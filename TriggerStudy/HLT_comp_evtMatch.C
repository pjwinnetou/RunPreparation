#include <TSystem.h>
#include <TFile.h>
#include <TH1.h>
#include <TEfficiency.h>
#include <TTreeReader.h>
#include <TTreeReaderValue.h>
#include <TTreeReaderArray.h>
#include <TLorentzVector.h>
#include <TGraphAsymmErrors.h>
#include <TCanvas.h>
#include <TPad.h>
#include <TAxis.h>
#include <TLine.h>
#include <TLegend.h>
#include <TLegendEntry.h>

#include <iostream>
#include <map>
#include <set>
#include <vector>
#include <string>
#include <dirent.h>
#include <memory>
#include "HLT_comp_evtMatch.h"

using namespace std;


void SetHLTBitTreeKeyRLE(TTree *tree,  const int nEntries){
  hltTreeBranch hltval;
  tree->SetBranchAddress("Event",&hltval.event);
  tree->SetBranchAddress("LumiBlock",&hltval.lumi);
  tree->SetBranchAddress("Run",&hltval.runN);

  for(long int i = 0; i < nEntries; i++){
    tree->GetEntry(i);
    unsigned long long key0 = keyFromRunLumiEvent(hltval.runN, hltval.lumi, hltval.event);
    runLumiEvtToEntryMap[key0] = i;
//    if(i%100000==0){ cout << "SetHLTBitTreeKeyRLE entry = " << i << " / " << nEntries << " (" << (int) (100*i/nEntries) << " %)" << endl;}
  }
};

void makeDir(TFile* f1, const string dir)
{
  TDirectory* subdir = f1->mkdir(dir.c_str());
    std::cout << "[INFO] DataSet directory: " << dir << endl;
};

void HLT_comp_evtMatch(){

  TFile* f1 = new TFile("/eos/cms/store/group/phys_heavyions/anstahll/HLT2021/TREE/OpenHLTTree_HIMinBias_103X_dataRun2_HLT_NoZS_20200521.root","OPEN");
  TFile* f2 = new TFile("/eos/cms/store/group/phys_heavyions/anstahll/HLT2021/TREE/OpenHLTTree_HIMinBias_103X_dataRun2_HLT_ForHITestsV3_NoZS_20200521.root","OPEN");
  TFile* wf = new TFile("output.root","RECREATE");

  triglistfile.open(Form("%s",triglist.c_str()));

  while (getline(triglistfile, line))
  {
    TRIGLIST.push_back(line);
  }
    
  const int sizelist = TRIGLIST.size();
  TTree* OnlineTR[sizelist];
  TTree* MisCalTR[sizelist];
  
  map<TString,map<TString,vector<double>*>> valVectOnline;
  map<TString,map<TString,vector<double>*>> valVectMisCal;
  
  int itree=0;
  for(auto& t : TRIGLIST){
    const string tt = t.substr(0, t.size()-1);
    OnlineTR[itree] = (TTree*) f1->Get(Form("hltobject/%s",tt.c_str()));
    MisCalTR[itree] = (TTree*) f2->Get(Form("hltobject/%s",tt.c_str()));

    int ivar=0;
    for( const auto& var : {"pt", "eta", "phi", "mass"}){     
      valVectOnline[itree][ivar] = 0;
      valVectMisCal[itree][ivar] = 0;

      OnlineTR[itree] -> SetBranchAddress(var,&valVectOnline[itree][ivar]);
      MisCalTR[itree] -> SetBranchAddress(var,&valVectMisCal[itree][ivar]);
      setbins(ivar);
      histonline[itree][ivar] = new TH1D(Form("h_online_%s_%s",tt.c_str(),var),Form(";%s;",var),nBins[ivar],bins);
      histmiscal[itree][ivar] = new TH1D(Form("h_miscal_%s_%s",tt.c_str(),var),Form(";%s;",var),nBins[ivar],bins);
      ivar++;
    }
    makeDir(wf,tt.c_str());
    itree++;
  } 

  TTree* hlt_online_tree = (TTree*) f1->Get("hltanalysis/HltTree");
  TTree* hlt_miscal_tree = (TTree*) f2->Get("hltanalysis/HltTree");

 
  int run_misc;
  int lumi_misc;
  unsigned long long event_misc;
  hlt_miscal_tree->SetBranchAddress("Event",&event_misc);
  hlt_miscal_tree->SetBranchAddress("LumiBlock",&lumi_misc);
  hlt_miscal_tree->SetBranchAddress("Run",&run_misc);

  const int NEntries = hlt_online_tree->GetEntries();
  SetHLTBitTreeKeyRLE(hlt_online_tree, NEntries);

  cout << "SetHLTBitTreeKeyRLE Done" << endl;

  for(int i = 0; i < NEntries; i++){
    hlt_miscal_tree->GetEntry(i);
    unsigned long long key1 = keyFromRunLumiEvent(run_misc, lumi_misc, event_misc);

    int entry = -1; 
    if(runLumiEvtToEntryMap.count(key1) == 0){
      cout << "NoMatch ::: runLumiEvtToEntryMap[key1] = " << runLumiEvtToEntryMap.count(key1) << ", key1 : " << key1 << ", runLumiEvtToEntryMap[key1] : " << runLumiEvtToEntryMap[key1] << endl;
      continue;}
    else entry = runLumiEvtToEntryMap[key1];
  
    int itree_=0;
    for(auto& t : TRIGLIST){
      const string tt = t.substr(0, t.size()-1);
      OnlineTR[itree_] ->GetEntry(entry);
      MisCalTR[itree_] ->GetEntry(entry);

      int ivar=0;
      for( const auto& var : {"pt", "eta", "phi", "mass"}){     
        for(int it=0;it<valVectOnline[itree_][ivar]->size(); it++){
          histonline[itree_][ivar]->Fill(valVectOnline[itree_][ivar]->at(it));
          histmiscal[itree_][ivar]->Fill(valVectMisCal[itree_][ivar]->at(it));
        }
        ivar++;
      }
      itree_++;
    } 
    if(entry%1000==0){ cout << "Done for entry = " << entry << " / " << NEntries << " (" << (int) (100*entry/NEntries)<< " %)" << endl;}
  }

    cout << "OKDONE" << endl;
  wf->cd();
  itree=0;
  for(auto& t : TRIGLIST){
    const string tt = t.substr(0, t.size()-1);
    TDirectory* subdir_ = wf->GetDirectory(tt.c_str());
    subdir_->cd();
    int ivar=0;
    for( const auto& var : {"pt", "eta", "phi", "mass"}){     
      histonline[itree][ivar]->Write();
      histmiscal[itree][ivar]->Write();
      ivar++;
    }
    itree++;
  } 


}

