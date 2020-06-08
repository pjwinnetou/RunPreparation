#include <iostream>
#include "Style_jaebeom.h"
#include <stdio.h>
#include <string.h>


using namespace std;

double getErrorPropaDivide(double a, double da, double b, double db){
  double err = a/b*TMath::Sqrt((da/a)*(da/a) + (db/b)*(db/b));
  return err;
}

void makeEffPlot(const char *filename="outputEff.root", bool setLogopt = false, double ymax = 1.09, double ymin =0.91)
{
  setTDRStyle();
  writeExtraText= true;
  int iPeriod = 22;
  int iPos = 33;

  double posx1 = 0.5;
  double posx2 = 0.7;
  double posy1 = 0.78;
  double posy2 = 0.90;
  
  
  string ResDirN = "EffPlotRes"; 
  const char* varn[3] = {"p_{T} (GeV/c)", "#eta", "y"};
  TFile* f1 = new TFile(filename,"read");
  TIter next(f1->GetListOfKeys()); TObject *obj;
  
  while ((obj = next()))
  {
    TGraphAsymmErrors* hOnline;
    TGraphAsymmErrors* hMisCal;
    obj = ((TKey*)obj)->ReadObj();
    TDirectoryFile *tdir = dynamic_cast<TDirectoryFile*>(obj);
    tdir->cd();

    string st_name = tdir->GetName();
    
    cout << "stname : " << st_name.c_str() << endl;
    const char * ObjDouble = strstr(st_name.c_str(),"Double");
    const char * ObjSingle = strstr(st_name.c_str(),"Single");
    bool isSingle = false; bool isPt = true;
    
    if(ObjDouble!=NULL && ObjSingle==NULL) isSingle = false;
    else if(ObjDouble==NULL && ObjSingle!=NULL) isSingle = true;
    else {cout << "ERROR no matched Single OR Double" << endl;break;}

    if(!isSingle){
      const char * ObjPt = strstr(st_name.c_str(),"Pt");
      const char * ObjRap = strstr(st_name.c_str(),"Rapidity");
      if(ObjPt!=NULL && ObjRap == NULL) isPt = true;
      else if(ObjPt == NULL && ObjRap != NULL) isPt = false;
      else {cout << "ERROR no matched Pt or Rapidity in DoubleMuon Eff Plots" << endl;break;}
      
      if(isPt){
        hOnline = (TGraphAsymmErrors*) gDirectory->FindObjectAny(Form("%s_online",st_name.c_str()));
        hMisCal = (TGraphAsymmErrors*) gDirectory->FindObjectAny(Form("%s_miscal",st_name.c_str()));

        if(hOnline->GetN()!=hMisCal->GetN()){cout << "ERROR!! Inconsistent Bin number b/w two histograms!! " << endl;break;}
        
        double px,py,px_,py_,ex,eyh,eyl,ex_,eyh_,eyl_;

        TGraphAsymmErrors* gRatio = new TGraphAsymmErrors();
        
        for(int ip=0; ip<hOnline->GetN(); ip++){
          hOnline->GetPoint(ip,px,py);
          hMisCal->GetPoint(ip,px_,py_);
          
          ex = hOnline->GetErrorXlow(ip);
          eyh = hOnline->GetErrorYhigh(ip);
          eyl = hOnline->GetErrorYlow(ip);
          ex_ = hMisCal->GetErrorXlow(ip);
          eyh_ = hMisCal->GetErrorYhigh(ip);
          eyl_ = hMisCal->GetErrorYlow(ip);
   
          
          double eyl_r = getErrorPropaDivide(py,eyl,py_,eyl_);
          double eyh_r = getErrorPropaDivide(py,eyl,py_,eyl_);
          double pr = py/py_;
          gRatio->SetPoint(ip,px,pr);
          gRatio->SetPointError(ip,ex,ex,eyl_r,eyh_r);

        }


        gRatio->GetXaxis()->SetTitle("p_{T} (GeV/c)");
        gRatio->GetYaxis()->SetTitle("#frac{Online}{MisCalibrated}");
        gRatio->GetXaxis()->SetLimits(hOnline->GetXaxis()->GetXmin(),hOnline->GetXaxis()->GetXmax());
        gRatio->SetName(Form("%s",st_name.c_str())); 
        TCanvas* cr = makeGraphRatioCanvas(gRatio, hOnline, hMisCal, ymax, ymin, setLogopt);

        TPad *pad1 = (TPad*) cr->GetPad(0); 
        pad1->Draw();
        pad1->cd();

        TLegend* leg = new TLegend(posx1, posy1, posx2, posy2);
        SetLegendStyle(leg);
        leg->AddEntry(hOnline,"Online","pe");
        leg->AddEntry(hMisCal,"MisCal","pe");
        leg->Draw("same"); 

        drawGlobText(st_name.c_str(), 0.2,0.55,1,0.034);
        pad1->Update();
        cr->Update();


        CMS_lumi_square( pad1, iPeriod, iPos,1);
        cr->Modified();
        cr->Update();

        string savedir = ResDirN+"/"+st_name;
        void * dirf = gSystem->OpenDirectory(savedir.c_str());
        if(dirf) gSystem->FreeDirectory(dirf);
        else {gSystem->mkdir(savedir.c_str(), kTRUE);}

        cr->SaveAs(Form("%s/%s.pdf",savedir.c_str(),cr->GetName()));
      }
      
      else if(!isPt){
        hOnline = (TGraphAsymmErrors*) gDirectory->FindObjectAny(Form("%s_online",st_name.c_str()));
        hMisCal = (TGraphAsymmErrors*) gDirectory->FindObjectAny(Form("%s_miscal",st_name.c_str()));

        
        double px,py,px_,py_,ex,eyh,eyl,ex_,eyh_,eyl_;

        TGraphAsymmErrors* gRatio = new TGraphAsymmErrors();
        
        for(int ip=0; ip<hOnline->GetN(); ip++){
          hOnline->GetPoint(ip,px,py);
          hMisCal->GetPoint(ip,px_,py_);
          
          ex = hOnline->GetErrorXlow(ip);
          eyh = hOnline->GetErrorYhigh(ip);
          eyl = hOnline->GetErrorYlow(ip);
          ex_ = hMisCal->GetErrorXlow(ip);
          eyh_ = hMisCal->GetErrorYhigh(ip);
          eyl_ = hMisCal->GetErrorYlow(ip);
    
          
          double eyl_r = getErrorPropaDivide(py_,eyl_,py,eyl);
          double eyh_r = getErrorPropaDivide(py_,eyl_,py,eyl);
          double pr = py_/py;
          gRatio->SetPoint(ip,px,pr);
          gRatio->SetPointError(ip,ex,ex,eyl_r,eyh_r);
        }
        
        gRatio->GetYaxis()->SetTitle("#frac{Online}{MisCalibrated}");
        gRatio->GetXaxis()->SetTitle("y");
        gRatio->GetXaxis()->SetLimits(hOnline->GetXaxis()->GetXmin(),hOnline->GetXaxis()->GetXmax());
       
        gRatio->SetName(Form("%s",st_name.c_str())); 
        TCanvas* cr = makeGraphRatioCanvas(gRatio, hOnline, hMisCal, ymax, ymin, setLogopt);

        TPad *pad1 = (TPad*) cr->GetPad(0); 
        pad1->Draw();
        pad1->cd();

        TLegend* leg = new TLegend(posx1, posy1, posx2, posy2);
        SetLegendStyle(leg);
        leg->AddEntry(hOnline,"Online","pe");
        leg->AddEntry(hMisCal,"MisCal","pe");
        leg->Draw("same"); 

        drawGlobText(st_name.c_str(), 0.2,0.55,1,0.034);
        pad1->Update();
        cr->Update();


        CMS_lumi_square( pad1, iPeriod, iPos,1);
        cr->Modified();
        cr->Update();

        string savedir = ResDirN+"/"+st_name;
        void * dirf = gSystem->OpenDirectory(savedir.c_str());
        if(dirf) gSystem->FreeDirectory(dirf);
        else {gSystem->mkdir(savedir.c_str(), kTRUE);}

        cr->SaveAs(Form("%s/%s.pdf",savedir.c_str(),cr->GetName()));
      }
    }
    else if(isSingle){
      const char * ObjPt = strstr(st_name.c_str(),"Pt");
      const char * ObjRap = strstr(st_name.c_str(),"Eta");
      if(ObjPt!=NULL && ObjRap == NULL) isPt = true;
      else if(ObjPt == NULL && ObjRap != NULL) isPt = false;
      else {cout << "ERROR no matched Pt or Rapidity in DoubleMuon Eff Plots" << endl;break;}
      
      if(isPt){
        hOnline = (TGraphAsymmErrors*) gDirectory->FindObjectAny(Form("%s_online",st_name.c_str()));
        hMisCal = (TGraphAsymmErrors*) gDirectory->FindObjectAny(Form("%s_miscal",st_name.c_str()));

        if(hOnline->GetN()!=hMisCal->GetN()){cout << "ERROR!! Inconsistent Bin number b/w two histograms!! " << endl;break;}
        
        double px,py,px_,py_,ex,eyh,eyl,ex_,eyh_,eyl_;

        TGraphAsymmErrors* gRatio = new TGraphAsymmErrors();
        
        for(int ip=0; ip<hOnline->GetN(); ip++){
          hOnline->GetPoint(ip,px,py);
          hMisCal->GetPoint(ip,px_,py_);
          
          ex = hOnline->GetErrorXlow(ip);
          eyh = hOnline->GetErrorYhigh(ip);
          eyl = hOnline->GetErrorYlow(ip);
          ex_ = hMisCal->GetErrorXlow(ip);
          eyh_ = hMisCal->GetErrorYhigh(ip);
          eyl_ = hMisCal->GetErrorYlow(ip);
    
          
          double eyl_r = getErrorPropaDivide(py_,eyl_,py,eyl);
          double eyh_r = getErrorPropaDivide(py_,eyl_,py,eyl);
          double pr = py_/py;
          gRatio->SetPoint(ip,px,pr);
          gRatio->SetPointError(ip,ex,ex,eyl_r,eyh_r);
        }
        gRatio->GetXaxis()->SetLimits(hOnline->GetXaxis()->GetXmin(),hOnline->GetXaxis()->GetXmax());
        
        gRatio->GetXaxis()->SetTitle("p_{T} (GeV/c)");
        gRatio->GetYaxis()->SetTitle("#frac{Online}{MisCalibrated}");
       
        gRatio->SetName(Form("%s",st_name.c_str())); 
        TCanvas* cr = makeGraphRatioCanvas(gRatio, hOnline, hMisCal, ymax, ymin, setLogopt);

        TPad *pad1 = (TPad*) cr->GetPad(0); 
        pad1->Draw();
        pad1->cd();

        TLegend* leg = new TLegend(posx1, posy1, posx2, posy2);
        SetLegendStyle(leg);
        leg->AddEntry(hOnline,"Online","pe");
        leg->AddEntry(hMisCal,"MisCal","pe");
        leg->Draw("same"); 

        drawGlobText(st_name.c_str(), 0.2,0.55,1,0.034);
        pad1->Update();
        cr->Update();


        CMS_lumi_square( pad1, iPeriod, iPos,1);
        cr->Modified();
        cr->Update();

        string savedir = ResDirN+"/"+st_name;
        void * dirf = gSystem->OpenDirectory(savedir.c_str());
        if(dirf) gSystem->FreeDirectory(dirf);
        else {gSystem->mkdir(savedir.c_str(), kTRUE);}

        cr->SaveAs(Form("%s/%s.pdf",savedir.c_str(),cr->GetName()));
      }
      else if(!isPt){
        hOnline = (TGraphAsymmErrors*) gDirectory->FindObjectAny(Form("%s_online",st_name.c_str()));
        hMisCal = (TGraphAsymmErrors*) gDirectory->FindObjectAny(Form("%s_miscal",st_name.c_str()));

        if(hOnline->GetN()!=hMisCal->GetN()){cout << "ERROR!! Inconsistent Bin number b/w two histograms!! " << endl;break;}
        
        double px,py,px_,py_,ex,eyh,eyl,ex_,eyh_,eyl_;

        TGraphAsymmErrors* gRatio = new TGraphAsymmErrors();
        
        for(int ip=0; ip<hOnline->GetN(); ip++){
          hOnline->GetPoint(ip,px,py);
          hMisCal->GetPoint(ip,px_,py_);
          
          ex = hOnline->GetErrorXlow(ip);
          eyh = hOnline->GetErrorYhigh(ip);
          eyl = hOnline->GetErrorYlow(ip);
          ex_ = hMisCal->GetErrorXlow(ip);
          eyh_ = hMisCal->GetErrorYhigh(ip);
          eyl_ = hMisCal->GetErrorYlow(ip);
    
          
          double eyl_r = getErrorPropaDivide(py_,eyl_,py,eyl);
          double eyh_r = getErrorPropaDivide(py_,eyl_,py,eyl);
          double pr = py_/py;
          gRatio->SetPoint(ip,px,pr);
          gRatio->SetPointError(ip,ex,ex,eyl_r,eyh_r);
        }
        
        gRatio->GetXaxis()->SetTitle("#eta");
        gRatio->GetYaxis()->SetTitle("#frac{Online}{MisCalibrated}");
        gRatio->GetXaxis()->SetLimits(hOnline->GetXaxis()->GetXmin(),hOnline->GetXaxis()->GetXmax());
       
        gRatio->SetName(Form("%s",st_name.c_str())); 
        TCanvas* cr = makeGraphRatioCanvas(gRatio, hOnline, hMisCal, ymax, ymin, setLogopt);

        TPad *pad1 = (TPad*) cr->GetPad(0); 
        pad1->Draw();
        pad1->cd();

        TLegend* leg = new TLegend(posx1, posy1, posx2, posy2);
        SetLegendStyle(leg);
        leg->AddEntry(hOnline,"Online","pe");
        leg->AddEntry(hMisCal,"MisCal","pe");
        leg->Draw("same"); 

        drawGlobText(st_name.c_str(), 0.2,0.55,1,0.034);
        pad1->Update();
        cr->Update();


        CMS_lumi_square( pad1, iPeriod, iPos,1);
        cr->Modified();
        cr->Update();

        string savedir = ResDirN+"/"+st_name;
        void * dirf = gSystem->OpenDirectory(savedir.c_str());
        if(dirf) gSystem->FreeDirectory(dirf);
        else {gSystem->mkdir(savedir.c_str(), kTRUE);}

        cr->SaveAs(Form("%s/%s.pdf",savedir.c_str(),cr->GetName()));
      }
    }
  }
}
