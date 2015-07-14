void altroPlotMaker()
	{
	TFile * altro3 = new TFile("/gpfs01/star/pwg/iraklic/mySimulations/AuAu/Pico_generation/y2014_Altro_3_Out.rectree.root");
	TFile * altro4 = new TFile("/gpfs01/star/pwg/iraklic/mySimulations/AuAu/Pico_generation/y2014_Altro_4_Out.rectree.root");

	TH1D * hAltro3 = new TH1D ("altro3", "altro3", 50, 0, 50);
	TH1D * hAltro4 = new TH1D ("altro4", "altro4", 50, 0, 50);

	TTree * tAltro3 = (TTree *) altro3->Get("pTracks");
	TTree * tAltro4 = (TTree *) altro4->Get("pTracks");

	tAltro3->Draw("hits>>altro3");
	tAltro4->Draw("hits>>altro4");

	TCanvas * c = new TCanvas();
	hAltro4->SetLineColor(2);
	hAltro4->Draw();
	hAltro3->Draw("sames");

	char legend[200];
	TLegend * leg = new TLegend(0.1, 0.7, 0.5, 0.9);
	sprintf(legend, "Altro thr = 3; <N_{trk}> = %.2f", hAltro3->GetMean());
	leg->AddEntry(hAltro3, legend, "l");
	sprintf(legend, "Altro thr = 4; <N_{trk}> = %.2f", hAltro4->GetMean());
	leg->AddEntry(hAltro4, legend, "l");
	leg->Draw();

//	c->Close();
//	altro3->Close();
//	altro4->Close();
//	============================================================
	hAltro3->ComputeIntegral();
	double * integral3 = hAltro3->GetIntegral();
	hAltro3->SetContent(integral3);	

	hAltro4->ComputeIntegral();
	double * integral4 = hAltro4->GetIntegral();
	hAltro4->SetContent(integral4);	

	TH1F * h23 = hAltro3->Clone("h23");
	TH1F * h24 = hAltro4->Clone("h24");

	for (int i = 1; i <= hAltro3->GetNbinsX(); i++)
		{
		h23->SetBinContent(i, 1);
		h24->SetBinContent(i, 1);
		}
	h23->Add(hAltro3, -1);
	h24->Add(hAltro4, -1);
	h23->SetLineColor(2);
	h23->SetTitle("Inverse Cumulative Distribution");
	h23->Draw();
	h24->SetLineColor(4);
	h24->Draw("same");


	TLegend * leg1 = new TLegend(0.1, 0.1, 0.5, 0.3);
	leg1->AddEntry(h23, "Altro_thr = 3", "l");
	leg1->AddEntry(h24, "Altro_thr = 4", "l");
	leg1->Draw();

	}
