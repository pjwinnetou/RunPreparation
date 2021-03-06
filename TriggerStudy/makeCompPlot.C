#include <iostream>
#include "Style_jaebeom.h"

using namespace std;

void makeCompPlot(const char *filename="output.root", bool setLogopt = true, double ymax = 1.09, double ymin =0.91)
{
  setTDRStyle();
  writeExtraText= true;
  int iPeriod = 22;
  int iPos = 33;

  double posx1 = 0.5;
  double posx2 = 0.7;
  double posy1 = 0.78;
  double posy2 = 0.90;
  
  
  string ResDirN = "CompPlotRes"; 
  const char* varn[4] = {"p_{T} (GeV/c)", "#eta", "#phi", "mass (GeV/c^{2})"};
  TFile* f1 = new TFile(filename,"read");
  TIter next(f1->GetListOfKeys()); TObject *obj;
  
  while ((obj = next()))
  {
    map<TString, TH1D*> hOnline;
    map<TString, TH1D*> hMisCal;
    obj = ((TKey*)obj)->ReadObj();
    TDirectoryFile *tdir = dynamic_cast<TDirectoryFile*>(obj);
    tdir->cd();

    string st_name = tdir->GetName();
    st_name.erase(st_name.end()-1,st_name.end());
    
    int ivar=0;
    for(auto& var : {"pt", "eta", "phi", "mass"})
    {
      hOnline[ivar] = (TH1D*) gDirectory->FindObjectAny(Form("h_online_%s_%s",st_name.c_str(),var));
      hMisCal[ivar] = (TH1D*) gDirectory->FindObjectAny(Form("h_miscal_%s_%s",st_name.c_str(),var));
      
      TH1D *hrat = getHistRatio(hOnline[ivar], hMisCal[ivar]);
      hrat->GetXaxis()->SetTitle(varn[ivar]);
      hrat->GetYaxis()->SetTitle("ratio");
  
      hOnline[ivar]->GetYaxis()->SetTitle("Counts");
      
      if(ivar!=0) setLogopt = false;
      TCanvas* cr = makeHistRatioCanvas(hrat, hOnline[ivar], hMisCal[ivar], ymax, ymin, setLogopt);
      
      TPad *pad1 = (TPad*) cr->GetPad(0); 
      pad1->Draw();
      pad1->cd();
      
      TLegend* leg = new TLegend(posx1, posy1, posx2, posy2);
      SetLegendStyle(leg);
      leg->AddEntry(hOnline[ivar],"Online","pe");
      leg->AddEntry(hMisCal[ivar],"MisCal","pe");
      leg->Draw("same"); 
      ivar++;

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

