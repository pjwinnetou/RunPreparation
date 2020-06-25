#include <iostream>
#include "Style_jaebeom.h"
#include <stdio.h>
#include <string.h>
#include "TRIGLIST.h"

using namespace std;

double getErrorPropaDivide(double a, double da, double b, double db){
  double err = a/b*TMath::Sqrt((da/a)*(da/a) + (db/b)*(db/b));
  return err;
}


void test2_makeEffPlot_threeGT(TString tst_name = "HLT_HIL3DoubleMuOpen_v1", bool isPt=true, bool setLogopt = false, double ymax = 1.09, double ymin =0.91){

  TFile* f1 = new TFile("/Users/soohwanlee/RunPreparation/TriggerStudy/outputTree/outputEff_HLT_Track_miscal.root","read");
  TFile* f2 = new TFile("/Users/soohwanlee/RunPreparation/TriggerStudy/outputTree/outputEff_HLT_ECAL_Track_miscal.root","read");

  for( auto& t : TRIGLIST){
    for( auto& st_name : t.second){
  setTDRStyle();
  writeExtraText = true;
  int iPeriod = 22;
  int iPos = 33;

  double posx1 = 0.5;
  double posx2 = 0.7;
  double posy1 = 0.78;
  double posy2 = 0.90;
  
  string ResDirN = "EffPlotRes_three"; 
  const char* varn[3] = {"p_{T} (GeV/c)", "#eta", "y"};

  TString dirName = "";
    if(isPt){dirName = "eff"+t.first +"_"+ st_name + "_Pt";}
    else if(!isPt){dirName = "eff"+t.first +"_"+ st_name + "_Eta";}
/*  if(strstr(st_name.Data(),"GED")){
    if(isPt){dirName = "effSinglePhoton_" + st_name + "_Pt";}
    else if(!isPt){dirName = "effSinglePhoton_" + st_name + "_Eta";}
  }
  else if(strstr(st_name.Data(),"DoubleMu")){
    if(isPt){dirName = "effDoubleMuon_" + st_name + "_Pt";}
    else if(!isPt){dirName = "effDoubleMuon_" + st_name + "_Rapidity";}
  }
  else if(strstr(st_name.Data(),"L3Mu")){
    if(isPt){dirName = "effSingleMuon_" + st_name + "_Pt";}
    else if(!isPt){dirName = "effSingleMuon_" + st_name + "_Eta";}
  }
  else if(strstr(st_name.Data(),"HIL1MuOpen")){
    if(isPt){dirName = "effSingleMuon_" + st_name + "_Pt";}
    else if(!isPt){dirName = "effSingleMuon_" + st_name + "_Eta";}
  }
  else if(strstr(st_name.Data(),"Gsf")){
    if(isPt){dirName = "effSingleElectron_" + st_name + "_Pt";}
    else if(!isPt){dirName = "effSingleElectron_" + st_name + "_Eta";}
  }
*/

  TGraphAsymmErrors* gonl = (TGraphAsymmErrors*) f1 -> Get(Form("%s/%s_online",dirName.Data(),dirName.Data()));
  TGraphAsymmErrors* gtrk = (TGraphAsymmErrors*) f1 -> Get(Form("%s/%s_miscal",dirName.Data(),dirName.Data()));
  TGraphAsymmErrors* gtrkcal = (TGraphAsymmErrors*) f2 -> Get(Form("%s/%s_miscal",dirName.Data(),dirName.Data()));

  SetGraphStyle(gonl, 0, 0);
  SetGraphStyle(gtrk, 1, 1);
  SetGraphStyle(gtrkcal, 2, 2);

  TGraphAsymmErrors* grtrk = new TGraphAsymmErrors();
  TGraphAsymmErrors* grtrkcal = new TGraphAsymmErrors();
        
  double px,py,px_,py_,ex,eyh,eyl,ex_,eyh_,eyl_;
  for(int ip=0; ip<gonl->GetN(); ip++)
  {
    gonl->GetPoint(ip,px,py);
    gtrk->GetPoint(ip,px_,py_);

    ex = gonl->GetErrorXlow(ip);
    eyh = gonl->GetErrorYhigh(ip);
    eyl = gonl->GetErrorYlow(ip);
    ex_ = gtrk->GetErrorXlow(ip);
    eyh_ = gtrk->GetErrorYhigh(ip);
    eyl_ = gtrk->GetErrorYlow(ip);


    double eyl_r = getErrorPropaDivide(py,eyl,py_,eyl_);
    double eyh_r = getErrorPropaDivide(py,eyl,py_,eyl_);
    double pr = py/py_;
    grtrk->SetPoint(ip,px,pr);
    grtrk->SetPointError(ip,ex,ex,eyl_r,eyh_r);
  }

  for(int ip=0; ip<gonl->GetN(); ip++)
  {
    gonl->GetPoint(ip,px,py);
    gtrkcal->GetPoint(ip,px_,py_);

    ex = gonl->GetErrorXlow(ip);
    eyh = gonl->GetErrorYhigh(ip);
    eyl = gonl->GetErrorYlow(ip);
    ex_ = gtrkcal->GetErrorXlow(ip);
    eyh_ = gtrkcal->GetErrorYhigh(ip);
    eyl_ = gtrkcal->GetErrorYlow(ip);


    double eyl_r = getErrorPropaDivide(py,eyl,py_,eyl_);
    double eyh_r = getErrorPropaDivide(py,eyl,py_,eyl_);
    double pr = py/py_;
    grtrkcal->SetPoint(ip,px,pr);
    grtrkcal->SetPointError(ip,ex,ex,eyl_r,eyh_r);
  }
  
  SetGraphStyle(grtrkcal, 1, 1);
  SetGraphStyle(grtrk, 2, 2);

  TCanvas* c1 = new TCanvas("c1","",700,700);
  c1->cd();

  gonl->GetXaxis()->SetTitle(" ");
  gonl->GetXaxis()->SetTitleSize(0);
  gonl->GetXaxis()->SetLabelSize(0);
  gonl->GetYaxis()->SetTitleOffset(0.9);
  gonl->GetYaxis()->SetTitleSize(0.07);
  gonl->GetYaxis()->SetLabelSize(0.06);
  
  gonl->GetXaxis()->SetNdivisions(510);

  grtrk->GetXaxis()->SetNdivisions(510);
  grtrk->GetYaxis()->SetLimits(ymin,ymax);
  grtrk->GetYaxis()->SetRangeUser(ymin,ymax);
  grtrk->GetXaxis()->SetTitleOffset(1.2);
  grtrk->GetXaxis()->SetTitleSize(0.1);
  grtrk->GetYaxis()->SetTitleOffset(0.62);
  grtrk->GetYaxis()->SetTitleSize(0.1);
  grtrk->GetXaxis()->SetLabelSize(0.1);
  grtrk->GetYaxis()->SetLabelSize(0.085);
  grtrk->GetYaxis()->SetNdivisions(505);
  grtrk->GetYaxis()->SetTickSize(0.04);
  grtrk->GetXaxis()->SetTickSize(0.06);
  grtrk->GetXaxis()->SetTitle("p_{T} (GeV/c)");
  grtrk->GetYaxis()->SetTitle("#frac{Online}{MisCalibrated}");
  grtrk->GetXaxis()->SetLimits(gonl->GetXaxis()->GetXmin(),gonl->GetXaxis()->GetXmax());

  TPad* pad1 = new TPad("pad1", "",0, 0.4, 1.0, 1.0); 
//  pad1->SetRightMargin(0.05);
  pad1->SetTopMargin(0.090);
  pad1->SetBottomMargin(0);
  pad1->SetLeftMargin(0.14);
  

  TPad* pad2 = new TPad("pad2","",0, 0, 1.0,0.4);
  pad2->SetTopMargin(0);
  pad2->SetBottomMargin(0.3);
  pad2->SetLeftMargin(0.14);
//  pad2->SetRightMargin(0.05);


  c1->cd();
  pad1->Draw();
  pad1->cd();
  if(setLogopt)pad1->SetLogy();
  gonl->Draw("AP");
  gtrk->Draw("P");
  gtrkcal->Draw("P");
  TLegend* leg = new TLegend(posx1, posy1, posx2, posy2);
  SetLegendStyle(leg);
  leg->AddEntry(gonl,"Online","pe");
  leg->AddEntry(gtrk,"Track Miscal","pe");
  leg->AddEntry(gtrkcal,"Calo+Track Miscal","pe");
  leg->Draw("same"); 
  drawGlobText(st_name.Data(), 0.2,0.55,1,0.034);
        
  
  c1->cd();
  pad2->Draw();
  pad2->cd();
  grtrk->Draw("AP");
  dashedLine(gonl->GetXaxis()->GetXmin(),1,gonl->GetXaxis()->GetXmax(),1,1,1);
//  grtrk->SetName(Form("%s",st_name.c_str())); 
  pad2->Update();
  c1->Modified();
  c1->Update();
  

  pad1->Update();
  c1->Update();


  CMS_lumi_square( pad1, iPeriod, iPos,1);
  c1->Modified();
  c1->Update();

  string savedir = ResDirN+"/"+st_name.Data();
  void * dirf = gSystem->OpenDirectory(savedir.c_str());
  if(dirf) gSystem->FreeDirectory(dirf);
  else {gSystem->mkdir(savedir.c_str(), kTRUE);}

  string pdfFileName = gonl->GetName();
  const string pdfsave = pdfFileName.substr(0, pdfFileName.size()-7);

  c1->SaveAs(Form("%s/%s.pdf",savedir.c_str(),pdfsave.c_str()));
  c1->Close();
  }
  }
}



