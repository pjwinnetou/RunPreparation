#include <iostream>
#include "Style_jaebeom.h"

using namespace std;

void makeCompPlot(const char *filename="output.root")
{
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
        
      TCanvas* cr = makeHistRatioCanvas(hOnline[ivar], hMisCal[ivar], 1.5, 0.5);
      ivar++;
      
      cr->SaveAs(Form("%s.pdf",cr->GetName()));
    }
  }
}

