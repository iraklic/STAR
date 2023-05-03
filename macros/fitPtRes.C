void fitPtRes(const char * inFileName) {
	FILE * myOutFile;
	char outFileName[200];
	sprintf(outFileName, "params_%s.txt", inFileName);
	myOutFile = fopen(outFileName, "w");
	TFile *f1 = TFile::Open(inFileName);
	TIter next(f1->GetListOfKeys());
	TKey *key;
	TCanvas c1;
	c1.Print("hsimple.ps[");

	while ((key = (TKey*)next())) {
		TClass *cl = gROOT->GetClass(key->GetClassName());
		if (!cl->InheritsFrom("TH1")) continue;
		TH1 *h = (TH1*)key->ReadObj();
		if (! ((TString) h->GetTitle()).BeginsWith("resSec")) continue;
		TF1 * myFit = new TF1("myGaus", "gaus");
		TFitResultPtr fitResults = h->Fit("myGaus");
		fprintf(myOutFile, "%.4f, %.4f\n", myFit->GetParameter(1), myFit->GetParameter(2));
		h->Draw();
		c1.Print("hsimple.ps");
	}

	c1.Print("hsimple.ps]");
	f1->Close();
	fclose(myOutFile);
}
