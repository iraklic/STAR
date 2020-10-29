TH1D * fluxMaker(const char * file)
	{
	gStyle->SetOptStat(0);

	TFile * myFile = new TFile(file);

	TH2F *ion_xyL = (TH2F *) myFile->Get("ion_xyL");

	if (! ion_xyL) return;
	Double_t xmin = ion_xyL->GetXaxis()->GetXmin();
	Double_t xmax = ion_xyL->GetXaxis()->GetXmax();
	TH1F *eX = (TH1F *) myFile->Get("eX");
	Double_t N = 5000;
	if (eX) N = eX->GetEntries();
	Double_t elFlux = N/(xmax - xmin);
	Int_t    nx   = ion_xyL->GetXaxis()->GetNbins();
	Int_t    ny   = ion_xyL->GetYaxis()->GetNbins();
	Double_t ymin = ion_xyL->GetYaxis()->GetXmin();
	Double_t ymax = ion_xyL->GetYaxis()->GetXmax();
	Double_t scale = elFlux*(xmax - xmin)/nx*(ymax - ymin)/ny;
	cout << "scale = " << scale << endl;
	ion_xyL->Scale(1./scale);

	TCanvas *c2 = new TCanvas("Projection","Projection");
	TH1D *proj = ion_xyL->ProjectionX("proj",11,40);
	proj->Scale(1./30);
	proj->SetTitle("Flux (ion/cm) at -0.25 cm from GG");
	proj->Draw();

	return proj;
}


void fluxDraw()
	{
	char legName[100];

	TH1D * h1 = fluxMaker("/gpfs01/star/pwg/iraklic/GARFIELD/WorkingSimulation/myGeo_V03/V03.root");
	TH1D * h2 = fluxMaker("/gpfs01/star/pwg/iraklic/GARFIELD/WorkingSimulation/myGeo_V08/V08.root");
	TH1D * h3 = fluxMaker("/gpfs01/star/pwg/iraklic/GARFIELD/WorkingSimulation/myGeo_V09/V09.root");
	TH1D * h4 = fluxMaker("/gpfs01/star/pwg/iraklic/GARFIELD/WorkingSimulation/TPC/TPC.root");

	TCanvas * c = new TCanvas();
	c->SetLogy();
	c->cd();
	h4->SetLineColor(1);
	h4->Draw();
	h2->SetLineColor(2);
	h2->Draw("same");
	h3->SetLineColor(3);
	h3->Draw("same");
	h1->SetLineColor(4);
	h1->Draw("same");

	TLegend * leg = new TLegend(0.1, 0.1, 0.2, 0.2);
	sprintf(legName, "L-Shape Beyond GG : %.2f", h1->Integral());
	leg->AddEntry(h1, legName, "l");
	sprintf(legName, "Wall with potential -690V : %.2f", h2->Integral());
	leg->AddEntry(h2, legName, "l");
	sprintf(legName, "L-Shape Between GG and FG : %.2f", h3->Integral());
	leg->AddEntry(h3, legName, "l");
	sprintf(legName, "Current TPC : %.2f", h4->Integral());
	leg->AddEntry(h4, legName, "l");
	leg->Draw();

	c->SaveAs("projection.gif");
	}
