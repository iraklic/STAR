#include <iostream>
#include <vector>
#include <algorithm>
#include <TROOT.h>
#include <TStyle.h>
#include <TChain.h>
#include <TFile.h>
#include "TH2.h"
#include "TH1.h"
#include "TCanvas.h"
#include "TLeaf.h"
#include "TEfficiency.h"
#include "TLegend.h"
#include "TFile.h"
#include <fstream>
#include "TF1.h"
#include "TGraph.h"

using namespace std;

struct var
	{
	float hits;
	float pid;
	float eta;
	float pt;
	float rapidity;
	};

struct myEffSet
	{
	TEfficiency * etaEff;
	TEfficiency * ptEffCentral;
	TEfficiency * ptEffForward;
	TEfficiency * ptEffFull;
	};

void drawHists(vector<myEffSet> vec, vector<string> name)
	{
	TFile * myFile = new TFile("altro.root", "RECREATE");
	char outName[100];
	TLegend * leg = new TLegend(0.1, 0.1, 0.4, 0.6);
	TCanvas * cEta = new TCanvas();
	TCanvas * cPtFull = new TCanvas();
	TCanvas * cPtCentral = new TCanvas();
	TCanvas * cPtForward = new TCanvas();

	TF1 * fit = new TF1("fit", "pol0", 1, 3);

	double yPt[vec.size()];
	double xPt[vec.size()];

	double fitPars[1];

	for (int i = 0; i < vec.size(); i++)
		{
		cEta->cd();
		if (i == 0) vec[i].etaEff->Draw();
		else
			{
			vec[i].etaEff->SetMarkerColor(i+1);
			vec[i].etaEff->SetLineColor(i+1);
			vec[i].etaEff->Draw("same");
			}
		cPtFull->cd();

		fit->SetLineColor(i+1);
		vec[i].ptEffFull->Fit(fit, "QR");
		yPt[i] = fit->GetParameter(0);
		xPt[i] = i + 3;
		if (i == 0) vec[i].ptEffFull->Draw();
		else
			{
			vec[i].ptEffFull->SetMarkerColor(i+1);
			vec[i].ptEffFull->SetLineColor(i+1);
			vec[i].ptEffFull->Draw("same");
			}
		cPtCentral->cd();

		if (i == 0) vec[i].ptEffCentral->Draw();
		else
			{
			vec[i].ptEffCentral->SetMarkerColor(i+1);
			vec[i].ptEffCentral->SetLineColor(i+1);
			vec[i].ptEffCentral->Draw("same");
			}
		cPtForward->cd();

		if (i == 0) vec[i].ptEffForward->Draw();
		else
			{
			vec[i].ptEffForward->SetMarkerColor(i+1);
			vec[i].ptEffForward->SetLineColor(i+1);
			vec[i].ptEffForward->Draw("same");
			}

		leg->AddEntry(vec[i].etaEff, name[i].c_str(), "pl");
		vec[i].etaEff->Write();
		}
	cEta->cd();
	leg->Draw();
	sprintf(outName, "effEta.C");
	cEta->SaveAs(outName);

	cPtFull->cd();
	leg->Draw();
	sprintf(outName, "effPtFull.C");
	cPtFull->SaveAs(outName);

	cPtCentral->cd();
	leg->Draw();
	sprintf(outName, "effPtCentral.gif");
	cPtCentral->SaveAs(outName);

	cPtForward->cd();
	leg->Draw();
	sprintf(outName, "effPtForward.pdf");
	cPtForward->SaveAs(outName);

	TCanvas * c  = new TCanvas();
	TGraph * gr = new TGraph(vec.size(), xPt, yPt);
	gr->Draw("apl");
	gr->GetXaxis()->SetTitle("Altro Threshold");
	gr->GetYaxis()->SetTitle("Efficiency");
	c->SaveAs("efficiencies.C");

	delete cEta;
	delete cPtFull;
	delete cPtCentral;
	delete cPtForward;

	myFile->Close();
	}

vector<var> branchToVec(const char * file, const char * branch)
	{
	vector<var> vec;
	var myVar;
	TFile * myFile = new TFile(file);

	TTree * tree = (TTree *) myFile->Get(branch);

	float eta, pt;
	float pid, hits;
	float rapidity;

	tree->SetBranchAddress("eta", &eta);
	tree->SetBranchAddress("pt", &pt);
	tree->SetBranchAddress("hits", &hits);
	tree->SetBranchAddress("pid", &pid);
	tree->SetBranchAddress("rapidity", &rapidity);

	cout << "Getting Entries from " << file << " branch " << branch << " : " << tree->GetEntries() << endl;

	for (int i = 0; i < tree->GetEntries(); i++)
		{
		tree->GetEntry(i);
		myVar.hits = hits;
		myVar.eta = eta;
		myVar.pt = pt;
		myVar.pid = pid;
		myVar.rapidity = rapidity;
		vec.push_back(myVar);
		}

	delete tree;
	myFile->Close();
	return vec;
	}

myEffSet efficiency(vector<var> tracks, vector<var> mctracks, string name)
	{
	myEffSet effs;
	double ptCut = 0.1;

//	altro3
	TH2F * ptEta = new TH2F ("ptEtaaltro3", "altro3 p_{T} vs. #eta", 100, -2, 2, 100, 0, 3);
	TH2F * mcptEta = new TH2F ("mcptEtaaltro3", "altro3 MC p_{T} vs. #eta", 100, -2, 2, 100, 0, 3);

	for (vector<var>::iterator it = tracks.begin(); it != tracks.end(); it++)
		if (it->pid == 7 || it->pid == 8) ptEta->Fill(it->eta, it->pt);

	for (vector<var>::iterator it = mctracks.begin(); it != mctracks.end(); it++)
		if (it->pid == 7 || it->pid == 8) mcptEta->Fill(it->eta, it->pt);

	TH1D * h_eta = ptEta->ProjectionX("tracksEta", ptEta->ProjectionY()->FindBin(ptCut), 100);
	TH1D * mch_eta = mcptEta->ProjectionX("mcetracksEta", mcptEta->ProjectionY()->FindBin(ptCut), 100);

	TH1D * h_pt_Full = ptEta->ProjectionY("tracksPt_Full", ptEta->ProjectionX()->FindBin(-1.0), ptEta->ProjectionX()->FindBin(1.0));
	TH1D * mch_pt_Full = mcptEta->ProjectionY("mctracks_pt_Full", mcptEta->ProjectionX()->FindBin(-1.0), mcptEta->ProjectionX()->FindBin(1.0));

	TH1D * h_pt = ptEta->ProjectionY("tracksPt", ptEta->ProjectionX()->FindBin(-0.5), ptEta->ProjectionX()->FindBin(0.5));
	TH1D * mch_pt = mcptEta->ProjectionY("mctracksPt", mcptEta->ProjectionX()->FindBin(-0.5), mcptEta->ProjectionX()->FindBin(0.5));

	TH1D * h_pt1 = ptEta->ProjectionY("tracksPt_1", ptEta->ProjectionX()->FindBin(-1.0), ptEta->ProjectionX()->FindBin(-0.5));
	TH1D * mch_pt1 = mcptEta->ProjectionY("mctracksPt_1", mcptEta->ProjectionX()->FindBin(-1.0), mcptEta->ProjectionX()->FindBin(-0.5));

	TH1D * h_pt2 = ptEta->ProjectionY("tracksPt_2", ptEta->ProjectionX()->FindBin(0.5), ptEta->ProjectionX()->FindBin(1.0));
	TH1D * mch_pt2 = mcptEta->ProjectionY("mctracksPt_2", mcptEta->ProjectionX()->FindBin(0.5), ptEta->ProjectionX()->FindBin(1.0));

	h_pt1->Add(h_pt2);
	mch_pt1->Add(h_pt2);

	char effName[200];

	effs.etaEff = new TEfficiency (*h_eta, *mch_eta);
	sprintf(effName, "%s_effvsY", name.c_str());
	effs.etaEff->SetName(effName);

	effs.ptEffFull = new TEfficiency (*h_pt, *mch_pt);
	sprintf(effName, "%s_effvsPt_Full", name.c_str());
	effs.ptEffFull->SetName(effName);


	effs.ptEffCentral = new TEfficiency (*h_pt1, *mch_pt1);
	sprintf(effName, "%s_effvsPt_Central", name.c_str());
	effs.ptEffCentral->SetName(effName);

	effs.ptEffForward = new TEfficiency (*h_pt2, *mch_pt2);
	sprintf(effName, "%s_effvsPt_Forward", name.c_str());
	effs.ptEffForward->SetName(effName);

	return effs;
	}
int main(int argc, char * argv[])
	{
	gStyle->SetOptDate(0);
	ifstream infile(argv[1]);
	char file[200];
	char type[200];

	vector<vector<var> > inData;
	vector<vector<var> > mcInData;
	vector<string> types;

	while (infile >> file >> type)
		{
		inData.push_back(branchToVec(file, "pTracks"));
		mcInData.push_back(branchToVec(file, "mcTracks"));
		types.push_back(type);
		}

	vector<myEffSet> effVec;

	for (int i = 0; i < types.size(); i++)
		effVec.push_back(efficiency(inData[i], mcInData[i], types[i]));

	drawHists(effVec, types);
	return 1;
	}
