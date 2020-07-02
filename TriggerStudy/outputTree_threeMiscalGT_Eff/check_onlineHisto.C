#include <TFile.h>
#include <TH1.h>
#include <TROOT.h>
#include <iostream>
#include <string>
#include "../RunPreparation/TriggerStudy/TRIGLIST.h"
#include <TString.h>

void check_onlineHisto(bool isPt=false){


  TFile* f1 = new TFile("/Users/soohwanlee/RunPreparation/TriggerStudy/outputTree/outputEff_HLT_Track_miscal.root","read");
  TFile* f2 = new TFile("/Users/soohwanlee/RunPreparation/TriggerStudy/outputTree/outputEff_HLT_ECAL_Track_miscal.root","read");

  for( auto& t : TRIGLIST){
    for( auto& st_name : t.second){

  TString dirName = "";
    if(isPt){dirName = "eff"+t.first +"_"+ st_name + "_Pt";}
    else if(!isPt){
        if(strstr(t.first.Data(),"Double")){dirName = "eff"+t.first+"_"+st_name+"_Rapidity";}
        else dirName = "eff"+t.first +"_"+ st_name + "_Eta";
      }
  std::cout << "getting plot " + dirName << std::endl;

  TGraphAsymmErrors* gonl1 = (TGraphAsymmErrors*) f1 -> Get(Form("%s/%s_online",dirName.Data(),dirName.Data()));
  TGraphAsymmErrors* gonl2 = (TGraphAsymmErrors*) f2 -> Get(Form("%s/%s_online",dirName.Data(),dirName.Data()));

  TCanvas *c1 = new TCanvas("c1","Compare Plot");
  c1->cd();
  gonl1->Draw("AP");
  gonl2->Draw("same");

  string savedir = dirName.Data();
  void * dirf = gSystem->OpenDirectory("png");
  if(dirf) gSystem->FreeDirectory(dirf);
  else {gSystem->mkdir("png", kTRUE);}


  string saveform = dirName.Data();
  c1->SaveAs(Form("png/%s.png", saveform.c_str()));
  c1->Close();
  }
}
}
