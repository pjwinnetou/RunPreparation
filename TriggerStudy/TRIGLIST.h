#ifndef TRIGLIST_h
#define TRIGLIST_h

#include <TString.h>
#include <map>
#include <vector>

const std::map<TString, std::vector<TString> > TRIGLIST = {
    { "SingleMuon" ,
                      {
                        "HLT_HIL1MuOpen_Centrality_70_100_v1",
                        "HLT_HIL1MuOpen_Centrality_80_100_v1",
                        "HLT_HIL2Mu3_NHitQ15_v1",
                        "HLT_HIL2Mu5_NHitQ15_v1",
                        "HLT_HIL2Mu7_NHitQ15_v1",
                        "HLT_HIL3Mu12_v1",
                        "HLT_HIL3Mu15_v1",
                        "HLT_HIL3Mu20_v1",
                        "HLT_HIL3Mu3_NHitQ10_v1",
                        "HLT_HIL3Mu5_NHitQ10_v1",
                        "HLT_HIL3Mu7_NHitQ10_v1"
                     }
    },
    { "DoubleMuon" ,
                     {
                        "HLT_HIL1DoubleMu10_v1",
                        "HLT_HIL1DoubleMuOpen_v1",
                        "HLT_HIL2DoubleMuOpen_v1",
                        "HLT_HIL2_L1DoubleMu10_v1",
                        "HLT_HIL3DoubleMuOpen_M60120_v1",
                        "HLT_HIL3DoubleMuOpen_Upsi_v1",
                        "HLT_HIL3DoubleMuOpen_v1",
                        "HLT_HIL3Mu0_L2Mu0_v1",
                        "HLT_HIL3Mu2p5NHitQ10_L2Mu2_M7toinf_v1",
                        "HLT_HIL3_L1DoubleMu10_v1",
                        "HLT_HIL1DoubleMuOpen_Centrality_50_100_v1",
                        "HLT_HIL1DoubleMuOpen_OS_Centrality_40_100_v1",
                        "HLT_HIL3DoubleMuOpen_JpsiPsi_v1",
                        "HLT_HIL3Mu0NHitQ10_L2Mu0_MAXdR3p5_M1to5_v1"
                     }
    },
    { "SingleElectron" ,
                     {
                        "HLT_HIEle10Gsf_v1",
                        "HLT_HIEle15Gsf_v1",
                        "HLT_HIEle20Gsf_v1",
                        "HLT_HIEle30Gsf_v1",
                        "HLT_HIEle40Gsf_v1",
                        "HLT_HIEle50Gsf_v1"
                      }
    },
    { "DoubleElectron" ,
                     {
                        "HLT_HIDoubleEle10GsfMass50_v1",
                        "HLT_HIDoubleEle10Gsf_v1",
                        "HLT_HIDoubleEle15GsfMass50_v1",
                        "HLT_HIDoubleEle15Gsf_v1",
                        "HLT_HIEle15Ele10GsfMass50_v1",
                        "HLT_HIEle15Ele10Gsf_v1"
                     }
    },
    { "SinglePhoton" ,
                     {
                        "HLT_HIGEDPhoton30_EB_v1",
                        "HLT_HIGEDPhoton30_v1",
                        "HLT_HIGEDPhoton40_EB_v1",
                        "HLT_HIGEDPhoton40_v1",
                        "HLT_HIGEDPhoton50_EB_v1",
                        "HLT_HIGEDPhoton50_v1",
                        "HLT_HIGEDPhoton60_EB_v1",
                        "HLT_HIGEDPhoton60_v1",
                        "HLT_HIGEDPhoton10_EB_v1",
                        "HLT_HIGEDPhoton10_v1",
                        "HLT_HIGEDPhoton20_EB_v1",
                        "HLT_HIGEDPhoton20_v1"
                     }
    }
};

#endif
