#define cosmicReader_cxx
#include "cosmicReader.h"
#include <TH2.h>
#include <TStyle.h>
#include <TCanvas.h>

void cosmicReader::Loop()
{
//   In a ROOT session, you can do:
//      root> .L cosmicReader.C
//      root> cosmicReader t
//      root> t.GetEntry(12); // Fill t data members with entry number 12
//      root> t.Show();       // Show values of entry 12
//      root> t.Show(16);     // Read and show values of entry 16
//      root> t.Loop();       // Loop on all entries
//

//     This is the loop skeleton where:
//    jentry is the global entry number in the chain
//    ientry is the entry number in the current Tree
//  Note that the argument to GetEntry must be:
//    jentry for TChain::GetEntry
//    ientry for TTree::GetEntry and TBranch::GetEntry
//
//       To read only selected branches, Insert statements like:
// METHOD1:
//    fChain->SetBranchStatus("*",0);  // disable all branches
//    fChain->SetBranchStatus("branchname",1);  // activate branchname
// METHOD2: replace line
//    fChain->GetEntry(jentry);       //read all branches
//by  b_branchname->GetEntry(ientry); //read only this branch
	TFile * fOut = new TFile("cosmicPlots.root", "RECREATE");
	if (fChain == 0) return;

	Long64_t nentries = fChain->GetEntriesFast();

	Long64_t nbytes = 0, nb = 0;

	map<int, TH2F*> ptResMap;
	map<int, TH1F*> ptMap, resMap;

	char ptResMapName[100];
	char ptMapName[100];
	char resMapName[100];

	for (int i = 1; i < 25; i++) {
		sprintf(ptResMapName, "ptResSec%d", i);
		sprintf(ptMapName, "ptSec%d", i);
		sprintf(resMapName, "resSec%d", i);
		ptResMap[i] = new TH2F(ptResMapName, ptResMapName, 50, 0, 5, 100, -0.2, 0.2);
		ptMap[i] = new TH1F(ptMapName, ptMapName, 50, 0, 5);
		resMap[i] = new TH1F(resMapName, resMapName, 100, -0.2, 0.2);
	}

	int counter = 0;

	for (Long64_t jentry = 0; jentry < nentries; jentry++) {
		if(jentry % 1000 == 0) cout << "Working on event " << jentry << " out of " << nentries << endl;
		Long64_t ientry = LoadTree(jentry);
		if (ientry < 0) break;
		nb = fChain->GetEntry(jentry);   nbytes += nb;
/*
		if (nTracks > 1000) {
			cout << "Event Info : " << runId << " : " << eventId << endl;
			cout << "Pt : " << 1/K_mPti << " : " << 1/L_mPti << endl;
			cout << "Sectors : " << sectorK << " : " << sectorL << endl;
			cout << "chi2 : " << chi2 << endl;
			counter++;
			if (counter > 1) return 0;
		}
*/
		if (nTracks != 2) continue;

		if (nTracks > 50) {
			cout << runId << " - " << eventId << "has more than 100 tracks" << endl;
//		return;
		}

		if ((sectorK == 19 && noFitpK > 70) || (sectorL == 19 && noFitpL > 70)) {
//			cout << runId << " - " << eventId << endl;
//			return;
		}

		if (sectorK > 0) {
			ptResMap[sectorK]->Fill(TMath::Abs((K_mPti - L_mPti)/2), K_mPti + K_mPti);
			// ptResMap[sectorK]->Fill(TMath::Abs((K_mPti - L_mPti)/2), K_mPti + K_mPti);
			ptMap[sectorK]->Fill(TMath::Abs((K_mPti - L_mPti)/2));
			resMap[sectorK]->Fill(K_mPti + L_mPti);
		}
		if (sectorL > 0) {
			ptResMap[sectorL]->Fill(TMath::Abs((K_mPti - L_mPti)/2), K_mPti + K_mPti);
			// ptResMap[sectorL]->Fill(TMath::Abs((K_mPti - L_mPti)/2), K_mPti + K_mPti);
			ptMap[sectorL]->Fill(TMath::Abs((K_mPti - L_mPti)/2));
			resMap[sectorL]->Fill(K_mPti + L_mPti);
		}

// if (Cut(ientry) < 0) continue;
	}
	fOut->Write();
	fOut->Close();
}
