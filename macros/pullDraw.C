void pullDraw(const char * fileName) {
	TFile * f = new TFile(fileName);

	TTree * myStiPulls = (TTree *) f->Get("StiPulls");


	myStiPulls->SetAlias("sector","((mHitsG.mHardwarePosition-(mHitsG.mHardwarePosition%16))/16)%32");
	myStiPulls->SetAlias("padrow","((mHitsG.mHardwarePosition-(mHitsG.mHardwarePosition%512))/512)%128");
	myStiPulls->SetAlias("localresid","((mHitsG.lYHit-mHitsG.lYFit)*sign(mHitsG.lZHit))");
	myStiPulls->SetAlias("resid","(mHitsG.lYFit-mHitsG.lYHit)");
	myStiPulls->SetAlias("residZ","(mHitsG.lZFit-mHitsG.lZHit)");
	myStiPulls->SetAlias("residT","((mHitsG.lZFit-mHitsG.lZHit)*sign(mHitsG.lZHit)/5.5)");

	myStiPulls->SetAlias("psector","((mHitsP.mHardwarePosition-(mHitsP.mHardwarePosition%16))/16)%32");
	myStiPulls->SetAlias("ppadrow","((mHitsP.mHardwarePosition-(mHitsP.mHardwarePosition%512))/512)%128");
	myStiPulls->SetAlias("plocalresid","((mHitsP.lYHit-mHitsP.lYFit)*sign(mHitsG.lZHit))");
	myStiPulls->SetAlias("presid","(mHitsP.lYFit-mHitsG.lYHit)");
	myStiPulls->SetAlias("presidZ","(mHitsP.lZFit-mHitsP.lZHit)");
	myStiPulls->SetAlias("presidT","((mHitsP.lZFit-mHitsP.lZHit)*sign(mHitsP.lZHit)/5.5)");


	TCanvas * c = new TCanvas();
	map<int, TH1D *> histoMap;

	for (int i = 1; i < 25; i++) {
		cout << "psector " << i << endl;

		char query[200], name[200], plot[200], plotName[200], sliceName[200];

		sprintf (query, "psector == %d", i);
		sprintf (name, "psector%d.gif", i);
		sprintf (sliceName, "h%d_1", i);
		sprintf (plotName, "h%d", i);
		sprintf (plot, "presid:padrow>>%s", plotName);

		TH2F * h = new TH2F(plotName, plotName, 72, 1, 73, 200, -2, 2);

		myStiPulls->Draw(plot, query, "colz");

		h->FitSlicesY();
		histoMap[i] = (TH1D *) gDirectory->Get(sliceName);

		delete h;
	}

	FILE * resid;
	resid = fopen("presid.csv", "w");
	fprintf(resid, "sector, padrow, presidZ\n");
	for (map<int, TH1D*>::iterator mit = histoMap.begin(); mit != histoMap.end(); mit++) {
		for (int i = 1; i < 73; i++)
			fprintf(resid, "%d, %d, %.4f\n", mit->first, i, mit->second->GetBinContent(i));
		mit->second->SetMarkerColor(mit->first);
		if (mit->first == 1) mit->second->Draw();
		else mit->second->Draw("SAME");
	}
	c->SaveAs("residBySector.svg");
	fclose(resid);
}
