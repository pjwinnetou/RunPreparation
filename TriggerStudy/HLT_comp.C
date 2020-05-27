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
#include "HLT_comp.h"

using namespace std;

void makeDir(TFile* f1, const string dir)
{
  TDirectory* subdir = f1->mkdir(dir.c_str());
    std::cout << "[INFO] DataSet directory: " << dir << endl;
};

void HLT_comp(){

  TFile* f1 = new TFile("/eos/cms/store/group/phys_heavyions/anstahll/HLT2021/TREE/OpenHLTTree_HIMinBias_103X_dataRun2_HLT_NoZS_20200521.root","OPEN");
  TFile* f2 = new TFile("/eos/cms/store/group/phys_heavyions/anstahll/HLT2021/TREE/OpenHLTTree_HIMinBias_103X_dataRun2_HLT_ForHITestsV3_NoZS_20200521.root","OPEN");

  triglistfile.open(Form("%s",triglist.c_str()));

  while (getline(triglistfile, line))
  {
    TRIGLIST.push_back(line);
  }
  
  TFile* wf = new TFile("output.root","RECREATE");
  wf->cd();

  int trigNum=0;
  for(auto& t : TRIGLIST){
    makeDir(wf, t.c_str());
    const string tt = t.substr(0, t.size()-1);
    TTreeReader* OnlineTR = new TTreeReader(Form("hltobject/%s",tt.c_str()), f1);
    TTreeReader* MiscalTR = new TTreeReader(Form("hltobject/%s",tt.c_str()), f2);

    int varNum=0;
    for( const auto& var : {"pt", "eta", "phi", "mass"}){
      setbins(varNum);
      histonline[trigNum][varNum] = new TH1D(Form("h_online_%s_%s",tt.c_str(),var),Form(";%s;",var),nBins[varNum],bins);
      histmiscal[trigNum][varNum] = new TH1D(Form("h_miscal_%s_%s",tt.c_str(),var),Form(";%s;",var),nBins[varNum],bins);
      obj_online[tt][var].reset(new TTreeReaderValue<std::vector<double> >(*OnlineTR,var));
      obj_miscal[tt][var].reset(new TTreeReaderValue<std::vector<double> >(*MiscalTR,var));
      varNum++;
    }

    int ivar=0;
    TDirectory* subdir_ = wf->GetDirectory(t.c_str());
    subdir_->cd();
    while(OnlineTR->Next()){
      ivar=0;
      for( const auto& var : {"pt", "eta", "phi", "mass"}){
        for(auto& value : *obj_online.at(tt).at(var)->Get()){
          histonline[trigNum][ivar]->Fill(value);
        }
        ivar++;
      }
    }
    while(MiscalTR->Next()){
      ivar=0;
      for( const auto& var : {"pt", "eta", "phi", "mass"}){
        for(auto& value : *obj_miscal.at(tt).at(var)->Get()){
          histmiscal[trigNum][ivar]->Fill(value);
        }
        ivar++;
      }
    }
    for(int is=0;is<4;is++){
    histonline[trigNum][is]->Write(); 
    histmiscal[trigNum][is]->Write(); 
    }
    trigNum++;
  }
}
