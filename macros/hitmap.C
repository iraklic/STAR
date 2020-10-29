TH1D* mapS = 0;
void hitmap() {
  TFile* f = (TFile*) (gROOT->GetListOfFiles()->At(0));
  TTree* MuDst = (TTree*) (f->GetKey("MuDst")->ReadObj());
  MuDst->SetAlias("map0","PrimaryTracks.mTopologyMap.mMap0");
  MuDst->SetAlias("map1","PrimaryTracks.mTopologyMap.mMap1");
  mapS = new TH1D("mapS","mapS",52,0.5,52.5);
  for (int ii=1;ii<=52;ii++) {
    int jj = (ii<32 ? ii : ii-32);
    TString ss = Form("(map%d&(1<<%d))/(1<<%d)>>hh%d(2,-0.5,1.5)",(ii<32 ? 0 : 1),jj,jj,ii);
//    TCut cut = "PrimaryTracks.mIdTruth < 10000 && abs(PrimaryTracks.mEta) < 0.5 && PrimaryTracks.mEta > 0"; // IRAKLI : just one side - this way I removed sector 20 in Run-18
    TCut cut = "PrimaryTracks.mIdTruth<10000&&abs(PrimaryTracks.mEta)<0.5";
    int ent = MuDst->Draw(ss.Data(),cut,"goff", 100);
    TH1* h0 = (TH1*) (gROOT->FindObject(Form("hh%d",ii)));
    mapS->SetBinContent(ii,h0->GetMean());
  }
	FILE * outFile;
	outFile = fopen("hitMap.csv", "w");
	fprintf(outFile, "detId, nHits\n");
	for (int bin = 0; bin < mapS->GetNbinsX(); bin++) {
		fprintf(outFile, "%d, %.3f\n", bin , mapS->GetBinContent(bin));
		cout << bin << ", " << mapS->GetBinContent(bin) << endl;
	}
	mapS->Draw();
	fclose(outFile);

}
