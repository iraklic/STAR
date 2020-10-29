void printHist(const TH1 * hist = 0, const char * axis = "", const char * part = "") {
	int nx = hist->GetNbinsX();
	printf("Double_t corr%s[24] = {", axis);
	for (int ix = 1; ix <= nx; ix++) {
		double cont = hist->GetBinContent(ix);
		double err = hist->GetBinError(ix);
//		printf("%.4f, %.4f, %s, %s\n", cont, err, axis, part);
		if (ix == nx) printf("%.4f};\n", cont);
		else printf("%.4f, ", cont);
	}
}


void dumpSSCalibParams(const char * fileName, const char * part = "") {
	TFile * f = new TFile(fileName);
	TH3F * dXS = (TH3F *) f->Get("dXS");
	TH3F * dYS = (TH3F *) f->Get("dYS");
	TH3F * dZS = (TH3F *) f->Get("dZS");

	TH2F * dXS_zx = (TH2F *) dXS->Project3D("zx");
	TH2F * dYS_zx = (TH2F *) dYS->Project3D("zx");
	TH2F * dZS_zx = (TH2F *) dZS->Project3D("zx");

	TF1 * fit = new TF1("f", "gaus", -0.1, 0.1);

	dXS_zx->FitSlicesY(fit); TH1F * dXS_zx_1 = (TH1F *) gDirectory->Get("dXS_zx_1"); printHist(dXS_zx_1, "X", part);
	dYS_zx->FitSlicesY(fit); TH1F * dYS_zx_1 = (TH1F *) gDirectory->Get("dYS_zx_1"); printHist(dYS_zx_1, "Y", part);
	dZS_zx->FitSlicesY(fit); TH1F * dZS_zx_1 = (TH1F *) gDirectory->Get("dZS_zx_1"); printHist(dZS_zx_1, "Z", part);

	TCanvas * c = new TCanvas();
	for (int i = 1; i <= 24; i++) {
		dXS_zx->ProjectionY("_py", i, i);
		dYS_zx->ProjectionY("_py", i, i);
		dZS_zx->ProjectionY("_py", i, i);
		TH1F * dXS_zx_py = (TH1F *) gDirectory->Get("dXS_zx_py");
		TH1F * dYS_zx_py = (TH1F *) gDirectory->Get("dYS_zx_py");
		TH1F * dZS_zx_py = (TH1F *) gDirectory->Get("dZS_zx_py");
		dXS_zx_py->Fit(fit, "R");
		dYS_zx_py->Fit(fit, "R");
		dZS_zx_py->Fit(fit, "R");
		char nameIt[200];
		sprintf(nameIt, "/afs/rhic.bnl.gov/star/users/iraklic/WWW/Alignment/dXS_fit_%d.gif", i);
		dXS_zx_py->Draw();
		c->SaveAs(nameIt);
		sprintf(nameIt, "/afs/rhic.bnl.gov/star/users/iraklic/WWW/Alignment/dYS_fit_%d.gif", i);
		dYS_zx_py->Draw();
		c->SaveAs(nameIt);
		sprintf(nameIt, "/afs/rhic.bnl.gov/star/users/iraklic/WWW/Alignment/dZS_fit_%d.gif", i);
		dZS_zx_py->Draw();
		c->SaveAs(nameIt);
	}


	TCanvas * c = new TCanvas();
	c->Divide(3, 1);
	c->cd(1);
	dXS_zx->Draw("colz");
	c->cd(2);
	dYS_zx->Draw("colz");
	c->cd(3);
	dZS_zx->Draw("colz");
	}

