// Dumps the content of the root file into gif files

#include <TFile.h>
#include <TKey.h>
#include <TCanvas.h>
#include <TH1.h>
#include <TH2.h>

#include <string>
#include <iostream>

using namespace std;

void plotReader()
	{
	TFile * myFile = new TFile("/gpfs01/star/pwg/iraklic/mySimulations/AuAu/TPC_reconstruction/y2014_Altro_thr_3/V02/MuMc.root");
	TIter next(myFile->GetListOfKeys());
	TKey * key;
	while ((key = (TKey *) next()))
		{
		char name[200];
		sprintf(name, "%s_new.gif", key->GetName());
		TCanvas * c = new TCanvas();
		string className = key->GetClassName();

		if (className.find("TH2") != std::string::npos)
			{
			TH2D * h = (TH2D *) key->ReadObj();
			h->Draw("colz");
			}
		else
			key->ReadObj()->Draw();
		c->SaveAs(name);
		}
	}
