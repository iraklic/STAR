// RUN LIKE THIS
// root -q -b 'adcComparator("file1","file2","legeng1","legend2")'


#include <iostream>
#include "TFile.h"
#include "TTree.h"
#include "TH1.h"
#include "TCanvas.h"
#include "TLegend.h"
#include "TROOT.h"
#include "TDirectory.h"
#include "TKey.h"


using namespace std;

void adcComparator(const char * f1, const char * f2, const char * legend1, const char * legend2) {
	TFile * file1 = new TFile(f1);
	TFile * file2 = new TFile(f2);
	
	TIter next(file1->GetListOfKeys());
	TKey *key;
	TCanvas * c1 = new TCanvas();

	char outName[200];
	string name[2] = {"Inner", "Outer"};

	while ((key = (TKey*)next())) {
		TClass *cl = gROOT->GetClass(key->GetClassName());
		if (!cl->InheritsFrom("TH1")) continue;
		TH1 *h = (TH1*)key->ReadObj();
		TString histName = h->GetName();
//		cout << "Working with : " << h->GetName() << endl;

		for (int i = 0; i < 2; i++) 
			if (histName.Contains(name[i])) { 
				if (histName.Contains("TimeRc")) { 
					if (histName.Contains("X"))	sprintf(outName, "/afs/rhic.bnl.gov/star/users/iraklic/WWW/iTPC/SAMPA/iTPC/%s/TimeRC/%s.gif", name[i].c_str(), h->GetName());
					else				sprintf(outName, "/afs/rhic.bnl.gov/star/users/iraklic/WWW/iTPC/SAMPA/TPC/%s/TimeRC/%s.gif", name[i].c_str(), h->GetName());
				}
				if (histName.Contains("PadRc")) { 
					if (histName.Contains("X"))	sprintf(outName, "/afs/rhic.bnl.gov/star/users/iraklic/WWW/iTPC/SAMPA/iTPC/%s/PadRC/%s.gif", name[i].c_str(), h->GetName());
					else				sprintf(outName, "/afs/rhic.bnl.gov/star/users/iraklic/WWW/iTPC/SAMPA/TPC/%s/PadRC/%s.gif", name[i].c_str(), h->GetName());
				}
			break;
			}
		cout << outName << endl;
				

		h->SetMarkerColor(6);
		h->Draw();
		TH1F * h2 = (TH1F *) file2->Get(histName);
		if (!h2) {
			cout << "No histogram with name : " << histName << endl;
			continue;
		}
		h2->SetMarkerColor(4);
		h2->Draw("SAME");

		TLegend * leg = new TLegend(0.7, 0.1, 0.9, 0.3);
		leg->AddEntry(h2, legend2, "pl");
		leg->AddEntry(h, legend1, "pl");
		leg->Draw();

		c1->SaveAs(outName);
	}
	cout << "=========================" << endl;
	cout << "====    D O N E !   =====" << endl;
	cout << "=========================" << endl;

	delete file1;
	delete file2;
}
