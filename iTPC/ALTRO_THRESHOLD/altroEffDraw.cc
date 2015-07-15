#include <iostream>
#include <vector>
#include <algorithm>
#include <TROOT.h>
#include <TChain.h>
#include <TFile.h>
#include "TH2.h"
#include "TH1.h"
#include "TCanvas.h"
#include "TLeaf.h"
#include "TEfficiency.h"
#include "TLegend.h"

using namespace std;

struct var
	{
	float hits;
	float pid;
	float eta;
	float pt;
	float rapidity;
	};

vector<var> altro3;
vector<var> mcaltro3;
vector<var> altro4;
vector<var> mcaltro4;

template <class histObject>
void drawHists(histObject h1, histObject h2, const char * name, const char * titles)
	{
	char outName[100];
	TCanvas * c = new TCanvas();
	c->Clear();
	h1->Draw();
	h1->SetTitle(titles);
	h2->SetLineColor(2);
	h2->Draw("same");

	TLegend * leg = new TLegend(0.1, 0.7, 0.3, 0.9);
	leg->AddEntry(h1, "Altro thr = 3", "pl");
	leg->AddEntry(h2, "Altro thr = 4", "pl");
	leg->Draw();

	sprintf(outName, "%s.gif", name);
	c->SaveAs(outName);
	delete c;
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

void efficiency()
	{
	double ptCut = 0.1;
	char partName [200];

//	altro3
	TH2F * ptEta = new TH2F ("ptEtaaltro3", "altro3 p_{T} vs. #eta", 100, -3, 3, 100, 0, 3);
	TH2F * mcptEta = new TH2F ("mcptEtaaltro3", "altro3 MC p_{T} vs. #eta", 100, -3, 3, 100, 0, 3);

	for (vector<var>::iterator it = altro3.begin(); it != altro3.end(); it++)
		ptEta->Fill(it->eta, it->pt);

	for (vector<var>::iterator it = mcaltro3.begin(); it != mcaltro3.end(); it++)
		mcptEta->Fill(it->eta, it->pt);

	TH1D * h_eta = ptEta->ProjectionX("etaaltro3", ptEta->ProjectionY()->FindBin(ptCut), 100);
	TH1D * mch_eta = mcptEta->ProjectionX("mcetaaltro3", mcptEta->ProjectionY()->FindBin(ptCut), 100);

	TH1D * h_pt1 = ptEta->ProjectionY("ptaltro3_1", ptEta->ProjectionX()->FindBin(-0.5), ptEta->ProjectionX()->FindBin(0.5));
	TH1D * mch_pt1 = mcptEta->ProjectionY("mcptaltro3_1", mcptEta->ProjectionX()->FindBin(-0.5), mcptEta->ProjectionX()->FindBin(0.5));

	TH1D * h_pt2 = ptEta->ProjectionY("ptaltro3_2", ptEta->ProjectionX()->FindBin(-1.0), ptEta->ProjectionX()->FindBin(-0.5));
	TH1D * mch_pt2 = mcptEta->ProjectionY("mcptaltro3_2", mcptEta->ProjectionX()->FindBin(-1.0), mcptEta->ProjectionX()->FindBin(-0.5));

	TH1D * h_pt3 = ptEta->ProjectionY("ptaltro3_3", ptEta->ProjectionX()->FindBin(0.5), ptEta->ProjectionX()->FindBin(1.0));
	TH1D * mch_pt3 = mcptEta->ProjectionY("mcptaltro3_3", mcptEta->ProjectionX()->FindBin(0.5), ptEta->ProjectionX()->FindBin(1.0));

	h_pt2->Add(h_pt3);
	mch_pt2->Add(h_pt3);

//	altro4
	TH2F * ptEtad = new TH2F ("ptEtaaltro4", "altro4 p_{T} vs. #eta", 100, -3, 3, 100, 0, 3);
	TH2F * mcptEtad = new TH2F ("mcptEtaaltro4", "altro4 MC p_{T} vs. #eta", 100, -3, 3, 100, 0, 3);

	for (vector<var>::iterator it = altro4.begin(); it != altro4.end(); it++)
			ptEtad->Fill(it->eta, it->pt);

	for (vector<var>::iterator it = mcaltro4.begin(); it != mcaltro4.end(); it++)
			mcptEtad->Fill(it->eta, it->pt);

	TH1D * h_etad = ptEtad->ProjectionX("etaaltro4", ptEtad->ProjectionY()->FindBin(ptCut), 100);
	TH1D * mch_etad = mcptEtad->ProjectionX("mcetaaltro4", mcptEtad->ProjectionY()->FindBin(ptCut), 100);

	TH1D * h_pt1d = ptEtad->ProjectionY("ptaltro4_1", ptEtad->ProjectionX()->FindBin(-0.5), ptEtad->ProjectionX()->FindBin(0.5));
	TH1D * mch_pt1d = mcptEtad->ProjectionY("mcptaltro4_1", mcptEtad->ProjectionX()->FindBin(-0.5), mcptEtad->ProjectionX()->FindBin(0.5));

	TH1D * h_pt2d = ptEtad->ProjectionY("ptaltro4_2", ptEtad->ProjectionX()->FindBin(-1.0), ptEtad->ProjectionX()->FindBin(-0.5));
	TH1D * mch_pt2d = mcptEtad->ProjectionY("mcptaltro4_2", mcptEtad->ProjectionX()->FindBin(-1.0), mcptEtad->ProjectionX()->FindBin(-0.5));

	TH1D * h_pt3d = ptEtad->ProjectionY("ptaltro4_3", ptEtad->ProjectionX()->FindBin(0.5), ptEtad->ProjectionX()->FindBin(1.0));
	TH1D * mch_pt3d = mcptEtad->ProjectionY("mcptaltro4_3", mcptEtad->ProjectionX()->FindBin(0.5), mcptEtad->ProjectionX()->FindBin(1.0));

	h_pt2d->Add(h_pt3d);
	mch_pt2d->Add(h_pt3d);

	char effName[200];

	TEfficiency * effEta = new TEfficiency (*h_eta, *mch_eta);
	sprintf(effName, "altro3_effvsY_%s", partName);
	effEta->SetName(effName);
//	effEta->SetTitle("p_{T} > 0.1 GeV; y; Efficiency");

	TEfficiency * effPt = new TEfficiency (*h_pt1, *mch_pt1);
	sprintf(effName, "altro3_effvsPt_Y1_%s", partName);
	effPt->SetName(effName);
//	effEta->SetTitle("|y| < 0.5; p_{T}; Efficiency");

	TEfficiency * effPt1 = new TEfficiency (*h_pt2, *mch_pt2);
	sprintf(effName, "altro3_effvsPt_Y2_%s", partName);
	effPt1->SetName(effName);
//	effEta->SetTitle("0.5 < |y| < 1; p_{T}; Efficiency");

	TEfficiency * effEtad = new TEfficiency (*h_etad, *mch_etad);
	sprintf(effName, "altro4_effvsY_%s", partName);
	effEtad->SetName(effName);
//	effEta->SetTitle("p_{T} > 0.1 GeV; y; Efficiency");

	TEfficiency * effPtd = new TEfficiency (*h_pt1d, *mch_pt1d);
	sprintf(effName, "altro4_effvsPt_Y1_%s", partName);
	effPtd->SetName(effName);
//	effEta->SetTitle("|y| < 0.5; p_{T}; Efficiency");

	TEfficiency * effPt1d = new TEfficiency (*h_pt2d, *mch_pt2d);
	sprintf(effName, "altro4_effvsPt_Y2_%s", partName);
	effPt1d->SetName(effName);
//	effEta->SetTitle("0.5 < |y| < 1; p_{T}; Efficiency");


//	-------------------------------------------------------------------
	drawHists(effEtad, effEta, "effEta", "p_{T} > 0.1 GeV/c;#eta;Efficiency");
	drawHists(effPtd, effPt, "effPt", "|#eta| < 0.5;p_{T};Efficiency");
	drawHists(effPt1d, effPt1, "effPt1", "0.5 < |#eta| < 1;p_{T};Efficiency");
//	-------------------------------------------------------------------
	}

int main()
	{
	altro3 = branchToVec("/gpfs01/star/pwg/iraklic/mySimulations/AuAu/Pico_generation/y2014_Altro_3_Out.rectree.root", "pTracks");
	mcaltro3 = branchToVec("/gpfs01/star/pwg/iraklic/mySimulations/AuAu/Pico_generation/y2014_Altro_3_Out.rectree.root", "mcTracks");

	altro4 = branchToVec("/gpfs01/star/pwg/iraklic/mySimulations/AuAu/Pico_generation/y2014_Altro_4_Out.rectree.root", "pTracks");
	mcaltro4 = branchToVec("/gpfs01/star/pwg/iraklic/mySimulations/AuAu/Pico_generation/y2014_Altro_4_Out.rectree.root", "mcTracks");

	efficiency();
	return 1;
	}
