void AlignSec(const char * inFile, const char * outFileName) {
	FILE * outFile;
	outFile = fopen(outFileName, "w");
	fprintf(outFile, "sector, z, dz, dzE\n");


	TFile * f = new TFile(inFile);
	TH3D * h = (TH3D *) f->Get("dZ");

	TProfile2D * p = h->Project3DProfile("yx");

	for (int i = 1; i <= p->GetNbinsX(); i++) 
		for (int j = 8; j <= p->GetNbinsY(); j++) {
			int sector = p->GetXaxis()->GetBinCenter(i);
			int z = p->GetYaxis()->GetBinCenter(j);

			fprintf(outFile, "%d, %d, %.3f, %.3f\n", sector, z, p->GetBinContent(i, j), p->GetBinError(i, j));
		}
	fclose(outFile);
}
