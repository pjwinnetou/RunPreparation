#include <iostream>
#include "Style_jaebeom.h"

using namespace std;

void makeCompPlot(const char *filename="output.root", bool setLogopt = true, double ymax = 1.09, double ymin =0.91)
{
  setTDRStyle();
  writeExtraText= true;
  int iPeriod = 22;
  int iPos = 33;
  
  string ResDirN = "CompPlotRes"; 
  const char* varn[4] = {"p_{T} (GeV/c)", "#eta", "#phi", "mass (GeV/c^{2})"};
  TFile* f1 = new TFile(filename,"read");
  TIter next(f1->GetListOfKeys()); TObject *obj;
  
  
  while ((obj = next()))
  {
    map<TString, TH1D*> hOnline;
    map<TString, TH1D*> hMisCal;
    obj = ((TKey*)obj)->ReadObj();
    cout << "OK" << endl;
    TDirectoryFile *tdir = dynamic_cast<TDirectoryFile*>(obj);
    tdir->cd();

    string st_name = tdir->GetName();
    st_name.erase(st_name.end()-1,st_name.end());
    cout << "st_name : " << st_name.c_str() << endl;
    
    int ivar=0;
    for(auto& var : {"pt", "eta", "phi", "mass"})
    {
      hOnline[ivar] = (TH1D*) gDirectory->FindObjectAny(Form("h_online_%s_%s",st_name.c_str(),var));
      hMisCal[ivar] = (TH1D*) gDirectory->FindObjectAny(Form("h_miscal_%s_%s",st_name.c_str(),var));
      
      TH1D *hrat = getHistRatio(hOnline[ivar], hMisCal[ivar]);
      hrat->GetXaxis()->SetTitle(varn[ivar]);
      hrat->GetYaxis()->SetTitle("ratio");
  
      hOnline[ivar]->GetYaxis()->SetTitle("Counts");

      TCanvas* cr = makeHistRatioCanvas(hrat, hOnline[ivar], hMisCal[ivar], ymax, ymin, setLogopt);
      TPad *pad1 = (TPad*) cr->GetPad(0); 
      ivar++;
     
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

