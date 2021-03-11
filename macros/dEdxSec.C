void dEdxSec(const char * inFile, const char * outFileName) {
	FILE * outFile;
	outFile = fopen(outFileName, "w");
	fprintf(outFile, "sector, padrow, dEdx, dEdxE\n");


	TFile * f = new TFile(inFile);
	TTree * t = (TTree *) f->Get("FitP");

	Float_t sector;
	Float_t padrow;
	Float_t mu;
	Float_t sigma;
	t->SetBranchAddress("x", &sector);
	t->SetBranchAddress("y", &padrow);
	t->SetBranchAddress("mu", &mu);
	t->SetBranchAddress("sigma", &sigma);

	cout << t->GetEntries() << " entries!" << endl;

	for (int i = 0; i < t->GetEntries(); i++) {
		t->GetEntry(i);
		printf("%.0f, %.0f, %.3f, %.3f\n", sector, padrow, mu, sigma);
		if (sector == 0 || padrow == 0) continue;
		fprintf(outFile, "%.0f, %.0f, %.3f, %.3f\n", sector, padrow, mu, sigma);
	}
	fclose(outFile);
}
