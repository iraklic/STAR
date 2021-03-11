void TSec(const char * inFile, const char * outFileName) {
	FILE * outFile;
	outFile = fopen(outFileName, "w");
	fprintf(outFile, "sector, z, dT, dTE, section\n");


	TFile * f = new TFile(inFile);
	TH3D * hI = (TH3D *) f->Get("TSecI");
	TH3D * hO = (TH3D *) f->Get("TSecO");


	TProfile2D * pI = hI->Project3DProfile("xz");
	TProfile2D * pO = hO->Project3DProfile("xz");

	for (int i = 1; i <= pI->GetNbinsX(); i++) 
		for (int j = 8; j <= pI->GetNbinsY() - 7; j++) {
			int sector = pI->GetXaxis()->GetBinCenter(i);
			int zI = pI->GetYaxis()->GetBinCenter(j);
			int zO = pO->GetYaxis()->GetBinCenter(j);
			if(sector > 12 && zI > 0) continue; 
			if(sector < 13 && zI < 0) continue; 
			fprintf(outFile, "%d, %d, %.3f, %.3f, inner\n", sector, zI, pI->GetBinContent(i, j), pI->GetBinError(i, j));
			fprintf(outFile, "%d, %d, %.3f, %.3f, outer\n", sector, zO, pO->GetBinContent(i, j), pO->GetBinError(i, j));
		}
	fclose(outFile);
}
