//Modified from StMiniMcMaker.cxx
///////////////////////////////////////////////////////////
#include "StMuDSTMaker/COMMON/StMuDstMaker.h"
#include "StMuDSTMaker/COMMON/StMuEvent.h"
#include "StMuDSTMaker/COMMON/StMuDst.h"
#include "StMuDSTMaker/COMMON/StMuTrack.h"
#include "StMuDSTMaker/COMMON/StMuPrimaryVertex.h"
#include "StMuDSTMaker/COMMON/StMuMcVertex.h"
#include "StMuDSTMaker/COMMON/StMuMcTrack.h"

#include "StRecTreeMaker.h"
#include "StEventTypes.h"
#include "StEvent.h"
#include "StParticleTable.hh"
#include "StParticleDefinition.hh"
#include "TNtuple.h"
#include "TFile.h"
#include "TTree.h"
#include "TBranch.h"
#include "TMath.h"

#include "StMcEventMaker/StMcEventMaker.h"
#include "StMcEventTypes.hh"
#include "StMcEvent.hh"
#include "StMcEvent/StMcTrack.hh"
#include "StMcEvent/StMcTpcHitCollection.hh"
#include "StMcEvent/StMcTpcHit.hh"
#include "StMcEvent/StMcEmcHitCollection.hh"
#include "StMcEvent/StMcCalorimeterHit.hh"
#include "StMcEvent/StMcTofHitCollection.hh"
#include "StMcEvent/StMcTofHit.hh"
#include "StEvent/StTpcHitCollection.h"
#include "StEvent/StTpcHit.h"
#include "StEvent/StTrackPidTraits.h"
#include "StEvent/StDedxPidTraits.h"
#include "StEventUtilities/StuRefMult.hh"
#include "StBFChain.h"
#include "StMessMgr.h"

#include "StChain.h"
#include "St_DataSet.h"
#include "St_DataSetIter.h"
#include "StEventTypes.h"
#include "StTpcDedxPidAlgorithm.h"
#include "StParticleDefinition.hh"
#include "StPhysicalHelix.hh"
//#include "StMuProbPidTraits.h"

#include "StBichsel/Bichsel.h"
#include "../../Names.h"
/*
#include "StStrangeMuDstMaker/StStrangeMuDstMaker.h"
#include "StStrangeMuDstMaker/StStrangeEvMuDst.hh"
#include "StStrangeMuDstMaker/StV0MuDst.hh"
#include "StStrangeMuDstMaker/StV0Mc.hh"
#include "StStrangeMuDstMaker/StXiMuDst.hh"
#include "StStrangeMuDstMaker/StXiMc.hh"
#include "StStrangeMuDstMaker/StKinkMuDst.hh"
#include "StStrangeMuDstMaker/StKinkMc.hh"
*/

#include "RecTree.h"

ClassImp(StRecTreeMaker)	// this a macro, no ";" here

StRecTreeMaker::StRecTreeMaker(StMuDstMaker* maker, const Char_t *outname="") : 
	mTupFile(0),
	trackTree(0),
	pTrackTree(0),
	mcTrackTree(0)
	{
	mMuDstMaker = maker;
	outName = outname;

	return;
	}

StRecTreeMaker::~StRecTreeMaker() { return; }

void StRecTreeMaker::InitHisto()
	{
	mcCounter = 0;
	rcCounter = 0;

	myVarMap["length"];
	myVarMap["pt"];
	myVarMap["eta"];
	myVarMap["hits"];
	myVarMap["pid"];
	myVarMap["rapidity"];
	myVarMap["lasthitz"];
	myVarMap["idTruth"];

	myMCVarMap["pt"];
	myMCVarMap["eta"];
	myMCVarMap["hits"];
	myMCVarMap["pid"];
	myMCVarMap["rapidity"];
	myMCVarMap["idTruth"];

//	For track length in STAR TPC volume
	myHisto   = new TH2F("TPoints70B", "dEdx(I70) / Pion versus Length in Tpc  for All", 210, 10, 500., 500, -1., 4.);
	myHistoLess1   = new TH2F("TPoints70BLess1", "dEdx(I70) / Pion versus Length in Tpc for |eta| < 1", 210, 10, 500., 500, -1., 4.);
	myHistoMore1   = new TH2F("TPoints70BMore1", "dEdx(I70) / Pion versus Length in Tpc for |eta| > 1", 210, 10, 500., 500, -1., 4.);

	mcEtavsEta = new TH2F("mcEtavsEta", "mcEtavsEta", 100, -2, 2, 100, -2, 2);

//	For !!!ACTIVE!!! track length in STAR TPC volume
	myHistoA   = new TH2F("TPoints70BA", "dEdx(I70) / Pion vs. ACTIVE Length in Tpc  for All", 210, 10, 500., 500, -1., 4.);
	myHistoLess1A   = new TH2F("TPoints70BLess1A", "dEdx(I70) / Pion vs. ACTIVE Length in Tpc for |eta| < 1", 210, 10, 500., 500, -1., 4.);
	myHistoMore1A   = new TH2F("TPoints70BMore1A", "dEdx(I70) / Pion vs. ACTIVE Length in Tpc for |eta| > 1", 210, 10, 500., 500, -1., 4.);


	I_pTrackLength = new TH1F ("pTrackLength", "pTrackLength", 100, 0, 300);

	I_LengthvsEtaP = new TH2F ("LengthvsEtaP", "Length vs. eta passed", 100, -3, 3, 100, 0, 300);
	I_LengthvsEtaF = new TH2F ("LengthvsEtaF", "Length vs. eta failed", 100, -3, 3, 100, 0, 300);

	I_MCHitsvsEta = new TH2F ("MCHitsvsEta", "MC Hits vs. eta", 100, -3, 3, 100, 0, 300);

	I_HitsvsEtaP = new TH2F ("HitsvsEtaP", "Hits vs. eta passed", 100, -3, 3, 100, 0, 300);
	I_HitsvsEtaF = new TH2F ("HitsvsEtaF", "Hits vs. eta failed", 100, -3, 3, 100, 0, 300);

	I_HitsvsLength = new TH2F ("HitsvsLength", "Hits vs Length", 100, 0, 200, 100, 0, 100);

	I_eff15 = new TH1F("eff15", "Track reconstruction efficiency (nHits > 15)" , 100, -3, 3);
	I_eff30 = new TH1F("eff30", "Track reconstruction efficiency (trackLength > 30)" , 100, -3, 3);

	I_MCTrackPtvsEta = new TH2F("MCPtvsEta", "MCPtvsEta", 100, -3, 3, 100, 0, 3);
	I_MCTrackPtvsEta15 = new TH2F("MCPtvsEta15", "MCPtvsEta15", 100, -3, 3, 100, 0, 3);

	I_gTrackPtvsEta = new TH2F("PtvsEta", "PtvsEta", 100, -3, 3, 100, 0, 3);
	I_gTrackPtvsEta15 = new TH2F("PtvsEta15", "PtvsEta", 100, -3, 3, 100, 0, 3);
	I_gTrackPtvsEta30 = new TH2F("PtvsEta30", "PtvsEta", 100, -3, 3, 100, 0, 3);

//	IRAKLI
	hLastHitZ = new TH1F("lastHitZ", "lastHitZ", 1000, -300, 300);

	h_hitsTimesLength = new TH1F("hitsTimesLength", "hitsTimesLength", 100, 0, 50);
	h_hitsTimesLengthNoClone = new TH1F("hitsTimesLengthNoClone", "hitsTimesLengthNoClone", 100, 0, 50);
	h_hitsTimesLengthClone = new TH1F("hitsTimesLengthClone", "hitsTimesLengthClone", 100, 0, 50);
	h_mcHits_mcTrackPt = new TH2D ("mcHits_mcTrackPt", "mcHits_mcTrackPt", 30, 0, 3, 30, 0, 300);
	h_mcHits = new TH1D("mcHits", "mcHits", 300, 0, 300);

	h_gTrackHits = new TH1D("gTrackHits", "gTrackHits", 100, 0, 100);
	h_gTrackHitsClone = new TH1D("gTrackHitsClone", "gTrackHitsClone", 100, 0, 100);
	h_gTrackHitsNoGhost = new TH1D("gTrackHitsNoGhost", "gTrackHitsNoGhost", 100, 0, 100);
	h_gTrackHitsCloneNoGhost = new TH1D("gTrackHitsCloneNoGhost", "gTrackHitsCloneNoGhost", 100, 0, 100);
	h_gTrackHitsNoClone = new TH1D("gTrackHitsNoClone", "gTrackHitsNoClone", 100, 0, 100);
	h_gTrackHitsNoCloneNoGhost = new TH1D("gTrackHitsNoCloneNoGhost", "gTrackHitsNoCloneNoGhost", 100, 0, 100);

	h_mcEta = new TH1F("mcEta", "mcEta", 300, -3, 3);
	h_gTrackEta = new TH1D("gTrackEta", "gTrackEta", 300, -3, 3);
	h_gTrackEtaNoGhost = new TH1D("gTrackEtaNoGhost", "gTrackEtaNoGhost", 300, -3, 3);
	h_gTrackEtaNoClone = new TH1D("gTrackEtaNoClone", "gTrackEtaNoClone", 300, -3, 3);

	h_Eff = new TH1D("Efficiency", "Efficiency", 300, -3, 3);
	h_EffNoGhost = new TH1D("EfficiencyNoGhost", "EfficiencyNoGhost", 300, -3, 3);
	h_EffNoClone = new TH1D("EfficiencyNoClone", "EfficiencyNoClone", 300, -3, 3);
	}

Int_t StRecTreeMaker::Init()
	{
	for (int i = 0; i < 302; i++)
		{
		mcEtaMap[i] = 0;
		gTrackEtaMap[i] = 0;
		gTrackEtaMapClone[i] = 0;
		gTrackEtaMapCloneNoGhost[i] = 0;
		gTrackEtaMapNoGhost[i] = 0;
		gTrackEtaMapNoClone[i] = 0;
		gTrackEtaMapNoCloneNoGhost[i] = 0;
		}

	outName.ReplaceAll("root","rectree.root");
	mTupFile = new TFile(outName, "RECREATE");
	mTupFile->SetCompressionLevel(9);
	trackTree= new TTree("tracks", "tracks");
	pTrackTree= new TTree("pTracks", "pTracks");
	mcTrackTree= new TTree("mcTracks", "mcTracks");
	InitHisto();
	trackTree->SetAutoSave(1000000);
	pTrackTree->SetAutoSave(1000000);
	mcTrackTree->SetAutoSave(1000000);

	cout << "Initialize the recotree ... " << endl;

	for (map<string, float>::iterator mit = myVarMap.begin(); mit != myVarMap.end(); mit++)
		{
		string name = mit->first;
		string leafList = name + "/F";
		pTrackTree->Branch(name.c_str(), &(mit->second), leafList.c_str());
		}
	for (map<string, float>::iterator mit = myMCVarMap.begin(); mit != myMCVarMap.end(); mit++)
		{
		string name = mit->first;
		string leafList = name + "/F";
		mcTrackTree->Branch(name.c_str(), &(mit->second), leafList.c_str());
		}


/*
	trackTree->Branch("mIEvt",&mTrackEvent.mIEvt,"mIEvt/I");
	trackTree->Branch("OriginMult",&mTrackEvent.OriginMult,"OriginMult/I");
	trackTree->Branch("McMult",&mTrackEvent.McMult,"McMult/I");
	trackTree->Branch("mRcVertexX",&mTrackEvent.mRcVertexX,"mRcVertexX/F");
	trackTree->Branch("mRcVertexY",&mTrackEvent.mRcVertexY,"mRcVertexY/F");
	trackTree->Branch("mRcVertexZ",&mTrackEvent.mRcVertexZ,"mRcVertexZ/F");
	trackTree->Branch("mMcVertexX",&mTrackEvent.mMcVertexX,"mMcVertexX/F");
	trackTree->Branch("mMcVertexY",&mTrackEvent.mMcVertexY,"mMcVertexY/F");
	trackTree->Branch("mMcVertexZ",&mTrackEvent.mMcVertexZ,"mMcVertexZ/F");
	trackTree->Branch("MagField",&mTrackEvent.MagField,"MagField/F");

	trackTree->Branch("mNMcTrk",&mTrackEvent.mNMcTrk,"mNMcTrk/I");
	trackTree->Branch("mMcId",mTrackEvent.mMcId,"mMcId[mNMcTrk]/I");
	trackTree->Branch("mGeantId",mTrackEvent.mGeantId,"mGeantId[mNMcTrk]/I");
	trackTree->Branch("mMcPt",mTrackEvent.mMcPt,"mMcPt[mNMcTrk]/F");
	trackTree->Branch("mMcPx",mTrackEvent.mMcPx,"mMcPx[mNMcTrk]/F");
	trackTree->Branch("mMcPy",mTrackEvent.mMcPy,"mMcPy[mNMcTrk]/F");
	trackTree->Branch("mMcPz",mTrackEvent.mMcPz,"mMcPz[mNMcTrk]/F");
	trackTree->Branch("mMcEta",mTrackEvent.mMcEta,"mMcEta[mNMcTrk]/F");
	trackTree->Branch("mMcPhi",mTrackEvent.mMcPhi,"mMcPhi[mNMcTrk]/F");

	trackTree->Branch("mNRcTrk",&mTrackEvent.mNRcTrk,"mNRcTrk/I");
	trackTree->Branch("mRcId",mTrackEvent.mRcId,"mRcId[mNRcTrk]/I");
	trackTree->Branch("mRcAssoId",mTrackEvent.mRcAssoId,"mRcAssoId[mNRcTrk]/I");
	trackTree->Branch("mRcAssoGId",mTrackEvent.mRcAssoGId,"mRcAssoGId[mNRcTrk]/I");
	trackTree->Branch("RcMult",&mTrackEvent.RcMult,"RcMult/I");
*/

	return StMaker::Init();
	} // \StMaker::Init

void StRecTreeMaker::Clear(Option_t *opt)
	{
	StMaker::Clear();
	}

Int_t StRecTreeMaker::Finish()
	{
	cout << " *** === *** " << mcCounter << " : " << rcCounter << endl;

//	IRAKLI
	int total_gTracks = 0;
	int total_gTracksNoGhost = 0;
	int total_mcTracks = 0;
	for (map<int, int>::iterator mit = mcEtaMap.begin(); mit != mcEtaMap.end(); mit++)
		{
		h_mcEta->SetBinContent(mit->first, mit->second);
		total_mcTracks+=mit->second;
		}
	for (map<int, int>::iterator mit = gTrackEtaMap.begin(); mit != gTrackEtaMap.end(); mit++)
		{
		h_gTrackEta->SetBinContent(mit->first, mit->second);
		total_gTracks+=mit->second;
		}
	for (map<int, int>::iterator mit = gTrackEtaMapNoGhost.begin(); mit != gTrackEtaMapNoGhost.end(); mit++)
		{
		h_gTrackEtaNoGhost->SetBinContent(mit->first, mit->second);
		total_gTracksNoGhost+=mit->second;
		}
	for (map<int, int>::iterator mit = gTrackEtaMapNoClone.begin(); mit != gTrackEtaMapNoClone.end(); mit++)
		h_gTrackEtaNoClone->SetBinContent(mit->first, mit->second);

	for (int i = 0; i < 302; i ++)
		{
		double eff = 0;
		double effNoGhost = 0;
		double effNoClone = 0;
		if (mcEtaMap[i] != 0)
			{
			eff		= (double) gTrackEtaMap[i] / (double) mcEtaMap[i];
			effNoGhost	= (double) gTrackEtaMapNoGhost[i] / (double) mcEtaMap[i];
			effNoClone	= (double) gTrackEtaMapNoClone[i] / (double) mcEtaMap[i];
			}
		h_Eff->SetBinContent(i, eff);
		h_EffNoGhost->SetBinContent(i, effNoGhost);
		h_EffNoClone->SetBinContent(i, effNoClone);
		}

	cout << "Number of MC Tracks = " << total_mcTracks << "\nNumber of gTracks = " << total_gTracks << "\nNumber of gTracksNoGhost = " << total_gTracksNoGhost << endl;

	hLastHitZ->Write();

	myHistoA->Write();
	myHistoLess1A->Write();
	myHistoMore1A->Write();

	myHisto->Write();
	myHistoLess1->Write();
	myHistoMore1->Write();

	mcEtavsEta->Write();

	pTrackTree->Write();
	mcTrackTree->Write();
	trackTree->Write();

	mTupFile->Write();
	mTupFile->Close();

	return kStOK;
	}

Int_t StRecTreeMaker::Make()
	{
	mcIdVec.clear();

//	StMuDst*	muDst	= mMuDstMaker->muDst();
	StMuEvent*	muEvent = mMuDstMaker->muDst()->event();

	if(!muEvent )
		{
		cout << "xuyifei: ------------muEvent" << endl;
		return kStOK;
		}
	int mRcMult = muEvent->refMult();

//	*******************************************************************
	double mMassProton = 0.93827;
	double mMassPion = 0.13957;
	double mMassLambda = 1.11568;
//	*******************************************************************
//	event level
	tb_mIEvt = muEvent->eventId();
	tb_MagField = mMuDstMaker->muDst()->event()->magneticField()/10.;

	double magn = muEvent->magneticField();

	StThreeVectorF pv;
//	StThreeVectorF pvx = muEvent->primaryVertexPosition();

//	*******************************************************************
	TClonesArray *PrimaryVertices = mMuDstMaker->muDst()->primaryVertices();
	Int_t NoPrimaryVertices = PrimaryVertices->GetEntriesFast();

	TClonesArray *GlobalTracks = mMuDstMaker->muDst()->array(muGlobal);
	Int_t NoGlobalTracks = GlobalTracks->GetEntriesFast(); 

	TClonesArray * PrimaryTracks =  mMuDstMaker->muDst()->array(muPrimary);  
	int NoPrimaryTracks = PrimaryTracks->GetEntriesFast();

	TClonesArray *MuMcVertices = mMuDstMaker->muDst()->mcArray(0);
	Int_t NoMuMcVertices = MuMcVertices->GetEntriesFast();

	TClonesArray *MuMcTracks = mMuDstMaker->muDst()->mcArray(1);
	Int_t NoMuMcTracks = MuMcTracks->GetEntriesFast();

//	=================================================================================================
	for(Int_t m = 0; m < NoMuMcTracks; m++)
		{
		StMuMcTrack *mcTrack = (StMuMcTrack *) MuMcTracks->UncheckedAt(m);
		if(! mcTrack) continue;

		if (mcTrack->Charge() == 0) continue;
		if (mcTrack->IdVx() != 1) continue;

		I_MCTrackPtvsEta->Fill(mcTrack->Pxyz().pseudoRapidity(), mcTrack->Pxyz().perp());
		I_MCHitsvsEta->Fill(mcTrack->Pxyz().pseudoRapidity(), mcTrack->No_tpc_hit());

		myMCVarMap["hits"] = mcTrack->No_tpc_hit();
		myMCVarMap["eta"] = mcTrack->Pxyz().pseudoRapidity();
		myMCVarMap["rapidity"] = mcTrack->Rapidity();
		myMCVarMap["pt"] =  mcTrack->Pxyz().perp();
		myMCVarMap["pid"] =  mcTrack->GePid();
		myMCVarMap["idTruth"] =  mcTrack->Id();
		mcTrackTree->Fill();

		mcIdVec[mcTrack->Id()] = mcTrack->Pxyz().pseudoRapidity();

		if (mcTrack->Pxyz().pseudoRapidity() > -1.58 && mcTrack->Pxyz().pseudoRapidity() < -1.578 && mcTrack->GePid() == 14)
			{
			cout << "MC : " << mcTrack->Id() << endl;
			mcCounter++;
			}
		}
//	-------------------------------------------------------------------------------------------------
	multimap<Int_t, Int_t> Mc2RcTracks;
	int cloneCounter = -999;
//	vector <int> cloneCheck;
	cloneCheck.clear();

	Bichsel  * m_Bichsel = Bichsel::Instance();
/*
//	Global Tracks ----------------------------------------------------------
	for (int g = 0; g < NoGlobalTracks; g++)
		{
		StMuTrack * gTrack = (StMuTrack *) GlobalTracks->UncheckedAt(g);
		StMuTrack * temppTrack = (StMuTrack *) gTrack->primaryTrack();
		if (!temppTrack) continue;
		StMuProbPidTraits gTraits = gTrack->probPidTraits();

		if (gTrack->idTruth() < 0 || gTrack->idTruth() > NoMuMcTracks)
			{
			cout << "Illegal idTruth " << gTrack->idTruth() << " The track is ignored" << endl;
			continue;
			}
		StMuMcTrack *mcTrack = (StMuMcTrack *) MuMcTracks->UncheckedAt(gTrack->idTruth()-1);
		if (mcTrack->Id() != gTrack->idTruth())
			{
			cout << "Mismatched idTruth " << gTrack->idTruth() << " and mcTrack Id " <<  mcTrack->Id() << " The track is ignored" << endl;
			continue;
			}

		if (! gTrack) continue;
		if (find(cloneCheck.begin(), cloneCheck.end(), mcTrack->Id()) == cloneCheck.end())
			{
			if (	gTrack				&&
				gTrack->idTruth()		&&
				gTrack->charge()		&&
				gTrack->flag() >= 100	 	&&
				gTrack->flag() % 100 != 11	&&	// bad fit or short track pointing to EEMC
				gTrack->flag() <= 1000		&&	// pile up track in TPC
				gTrack->nHitsFit() >= 5)
				{
				double bgL10;
				double zPred[KPidParticles];
				double dEdx[2] = {1e6 * gTraits.dEdxTruncated(), 1e6 * gTraits.dEdxFit()};
				if (dEdx[0] <= 0 || dEdx[1] <= 0) continue;
				double p = gTrack->pt() * TMath::CosH(gTrack->eta());

				for (int l = 0; l < KPidParticles; l++)
					{
					bgL10 = TMath::Log10(p / Masses[l]);
					zPred[l] = m_Bichsel->I70Trs(l, bgL10);
					}
				if (fabs(gTrack->eta()) < 1) myHistoLess1->Fill(gTrack->length(), TMath::Log(dEdx[0]) - zPred[kPidPion]);
				else myHistoMore1->Fill(gTrack->length(), TMath::Log(dEdx[0]) - zPred[kPidPion]);
				myHisto->Fill(gTrack->length(), TMath::Log(dEdx[0]) - zPred[kPidPion]);
				}
			cloneCheck.push_back(mcTrack->Id());
			}
		}
*/
//	------------------------------------------------------------------------

	cloneCheck.clear();
	for (int l = 0; l < NoPrimaryVertices; l++)
		{
		StMuPrimaryVertex *Vtx = (StMuPrimaryVertex *) PrimaryVertices->UncheckedAt(l);
		if(!Vtx)
			{
			cout << "no vertex pointer! " << endl;
			continue;
			}
		if(!Vtx->idTruth() || Vtx->idTruth() < 0 || Vtx->idTruth() > NoMuMcVertices)
			{
//			cout << "Illegal idTruth " << Vtx->idTruth() << " The vertex is ignored" << endl;
			continue;
			}
		StMuMcVertex *mcVertex = (StMuMcVertex *) MuMcVertices->UncheckedAt(Vtx->idTruth()-1);
		if(mcVertex->Id() != Vtx->idTruth())
			{
			cout << "Mismatched idTruth " << Vtx->idTruth() << " and mcVertex Id " <<mcVertex->Id() << " The vertex is ignored" <<endl;
			continue;
			}

		if (Vtx->idTruth() != 1) continue;

		for (Int_t k = 0; k < NoPrimaryTracks; k++)
			{
			StMuTrack *pTrack = (StMuTrack *) PrimaryTracks->UncheckedAt(k);
			if (!pTrack->idTruth() || pTrack->idTruth() < 0 || pTrack->idTruth() > NoMuMcTracks)
				{
				cout << "Illegal idTruth " << pTrack->idTruth() << " The track is ignored" << endl;
				continue;
				}
			StMuMcTrack *mcTrack = (StMuMcTrack *) MuMcTracks->UncheckedAt(pTrack->idTruth()-1);
			if (mcTrack->Id() != pTrack->idTruth())
				{
				cout << "Mismatched idTruth " << pTrack->idTruth() << " and mcTrack Id " <<  mcTrack->Id() << " The track is ignored" << endl;
				continue;
				}

			if (! pTrack) continue;
			if (pTrack->vertexIndex() != l) continue;
//			if (pTrack->idParentVx() == 1) continue;

			const StMuProbPidTraits & PiD = pTrack->probPidTraits();
			float TrackLength = PiD.dEdxTrackLength();

			StMuProbPidTraits gTraits = pTrack->probPidTraits();

//			if (mcTrack->IdVx() != 1) continue;
//			if (mcTrack->Charge() == 0)  continue;

			if (find(cloneCheck.begin(), cloneCheck.end(), (int) mcTrack->Id()) == cloneCheck.end())
				{
				if (	pTrack				&&
					pTrack->idTruth()		&&
					pTrack->charge()		&&
					pTrack->flag() >= 100	 	&&
					pTrack->flag() % 100 != 11	&&	// bad fit or short track pointing to EEMC
					pTrack->flag() <= 1000		&&	// pile up track in TPC
					pTrack->nHitsFit() >= 5
					)
					{
					if (mcTrack->IdVx() != 1) continue;
					if (mcTrack->Charge() == 0)  continue;

					myVarMap["length"] = TrackLength;
					myVarMap["hits"] = pTrack->nHitsFit();
					myVarMap["pid"] = mcTrack->GePid();
					myVarMap["eta"] =  mcTrack->Pxyz().pseudoRapidity();
					myVarMap["rapidity"] =  mcTrack->Rapidity();
					myVarMap["pt"] =  mcTrack->Pxyz().perp();
					myVarMap["lasthitz"] =  (float) pTrack->lastPoint().z();
					myVarMap["idTruth"] =  mcTrack->Id();

//					if (fabs(mcTrack->Pxyz().pseudoRapidity()) > 1.5) cout << mcTrack->Id() << " : " << pTrack->idTruth() << endl;

					mcEtavsEta->Fill(pTrack->eta(), mcTrack->Pxyz().pseudoRapidity());

					pTrackTree->Fill();
					if (mcTrack->Pxyz().pseudoRapidity() > -1.58 && mcTrack->Pxyz().pseudoRapidity() < -1.578 && mcTrack->GePid() == 14)
						{
						cout << "RC : " << mcTrack->Id() << endl;
						rcCounter++;
						}

					cloneCheck.push_back(mcTrack->Id());
					if (mcIdVec[mcTrack->Id()] != mcTrack->Pxyz().pseudoRapidity()) cout << "DISASTER!!!!!!!!!!!!!!" << mcTrack->Id() << endl;

					hLastHitZ->Fill(pTrack->lastPoint().z());

//					dEdx vs lenth calculation =================================================
					double bgL10;
					double zPred[KPidParticles];
					double dEdx[2] = {1e6 * gTraits.dEdxTruncated(), 1e6 * gTraits.dEdxFit()};

					if (dEdx[0] <= 0 || dEdx[1] <= 0)
						{
//						cout << "There is a matched track with negative dEdx if you wanna check it out." << endl;
						continue;
						}
					double p = pTrack->pt() * TMath::CosH(pTrack->eta());

					for (int l = 0; l < KPidParticles; l++)
						{
						bgL10 = TMath::Log10(p / Masses[l]);
						zPred[l] = m_Bichsel->I70Trs(l, bgL10);
						}

//					FILLING WITH !!!ACTIVE!!! TRACK LENGTH IN STAR VOLUME
					if (fabs(pTrack->eta()) < 1) myHistoLess1A->Fill(TrackLength, TMath::Log(dEdx[0]) - zPred[kPidPion]);
					else myHistoMore1A->Fill(TrackLength, TMath::Log(dEdx[0]) - zPred[kPidPion]);
					myHistoA->Fill(TrackLength, TMath::Log(dEdx[0]) - zPred[kPidPion]);

//					FILLING WITH TRACK LENGTH IN STAR TPC VOLUME
					if (fabs(pTrack->eta()) < 1) myHistoLess1->Fill(pTrack->lengthMeasured(), TMath::Log(dEdx[0]) - zPred[kPidPion]);
					else myHistoMore1->Fill(pTrack->lengthMeasured(), TMath::Log(dEdx[0]) - zPred[kPidPion]);
					myHisto->Fill(pTrack->lengthMeasured(), TMath::Log(dEdx[0]) - zPred[kPidPion]);
//					============================================================================

					int tofHitIndex = pTrack->index2BTofHit();
					if (tofHitIndex >= 0)
						{
						const StMuBTofPidTraits& btofPidTraits = pTrack->btofPidTraits();
//						cout << pTrack->nSigmaPion() << " : " <<  btofPidTraits.beta() << endl;
						}

//					============================================================================
//					cloneCheck.push_back(mcTrack->Id());
					}
//				cloneCheck.push_back(mcTrack->Id());
				}
			}
		}

//	=================================================================================================

	if (! NoMuMcVertices || ! NoMuMcTracks)
		{
		cout << "Event has no MC information ==> skip it" << endl;
		return kStOK;
		}

	if(NoPrimaryVertices != (int)mMuDstMaker->muDst()->numberOfPrimaryVertices())
		{
		cout << "No. of Primary Vertices inconsistent! " << endl;
		return kStOk;
		}


	if(NoGlobalTracks != (int) mMuDstMaker->muDst()->numberOfGlobalTracks())
		cout << "Number of global tracks inconsistent! " << endl;


	LOG_DEBUG << "filled track object" << endm;
	trackTree->Fill();

	return kStOK;
	}
