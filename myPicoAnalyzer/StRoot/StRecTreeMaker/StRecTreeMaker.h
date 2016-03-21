//Modified from StMiniMcMaker.h
///////////////////////////////////////////////////////////////
#ifndef StRecTreeMaker_hh     
#define StRecTreeMaker_hh

#include "StMaker.h"
#include "TString.h"
#include <vector>
#include <utility>
#include <map>
#include "TTree.h"
#include "StThreeVectorF.hh"
#include "TH1.h"
#include "TH2.h"
#include "TH3.h"

#define MAX_NUM 800000

class StMuDstMaker;
class StMuDst;
class StMuTrack;

class StRun;
class StEventInfo;
class StTrack;
class StPrimaryTrack;
class StChain;

class St_particle;
class St_g2t_event;
class St_g2t_pythia;

class TFile;
class TNtuple;
class TString;
class TTree;
class TMinuit;

class TH1F;
class TH1D;
class TH2F;
class TH2D;
class TH3F;
class TH3D;

class StRecTreeMaker : public StMaker
	{
	public:

	StRecTreeMaker(StMuDstMaker *, const Char_t *);
	~StRecTreeMaker();

	void Clear(Option_t *option="");    // called after every event to cleanup 
	Int_t  Init();                      // called once at the beginning
	Int_t  Make();                      // invoked for every event
	Int_t  Finish();                    // called once at the end

	TTree *mEvent;

	virtual const char *GetCVS() const
		{
//		static const char cvs[]="Tag $Name:  $ $Id: StRecTreeMaker.h,v 1.1 2012/03/06  perev Exp $ built "__DATE__" "__TIME__ ; 
		static const char cvs[]="Tag $Name:  $ $Id: StRecTreeMaker.h,v 1.1 2012/03/06  perev Exp $ built " __DATE__ " " __TIME__ ; 
		return cvs;
		}
  
	protected:

	TString outName;
	TFile*  mTupFile;
	TTree*  trackTree;
	TTree*  pTrackTree;
	TTree*  mcTrackTree;

	map<int, double> mcIdVec;
	vector<int> cloneCheck;

	int mcCounter = 0;
	int rcCounter = 0;

	map<int, int> trackMultMap; // IRAKLI
	map<string, float> myVarMap;
	map<string, float> myMCVarMap;

//	StMuDst*     	  mu;
	StMuDstMaker*     mMuDstMaker;

//	Added by xyf
	FILE * myHitsFile;
	map<int, int> mcEtaMap, gTrackEtaMap, gTrackEtaMapNoGhost, gTrackEtaMapNoClone;
	map<int, int> gTrackEtaMapClone, gTrackEtaMapCloneNoGhost, gTrackEtaMapNoCloneNoGhost;

	void InitHisto();

	TH1F * hLastHitZ;

	TH2F * myHisto;
	TH2F * myHistoLess1;
	TH2F * myHistoMore1;

	TH2F * mcEtavsEta;

	TH2F * myHistoA;
	TH2F * myHistoLess1A;
	TH2F * myHistoMore1A;

	TH2F * I_MCHitsvsEta;
	TH2F * I_LengthvsEtaP;
	TH2F * I_LengthvsEtaF;
	TH2F * I_HitsvsEtaP;
	TH2F * I_HitsvsEtaF;

	TH2F * I_HitsvsLength;

	TH1F * I_pTrackLength;

	TH1F * I_NgTracksvsEta15;
	TH1F * I_NgTracksvsEta30;
	TH1F * I_NMCTracksvsEta;
	TH1F * I_eff15;
	TH1F * I_eff30;

	TH2F * I_MCTrackPtvsEta;
	TH2F * I_MCTrackPtvsEta15;
	TH2F * I_gTrackPtvsEta;
	TH2F * I_gTrackPtvsEta15;
	TH2F * I_gTrackPtvsEta30;

	TH1D * I_eff_pt;
	TH1D * I_eff_pt15;
	TH1D * I_eff_pt30;
	TH1D * I_eff_eta;
	TH1D * I_eff_eta15;
	TH1D * I_eff_eta30;

	TH2D* h_dedx_p;
	TH2D* h_proton_nsigma_p;
	TH2D* h_pion_nsigma_p;
	TH2D* h_select_proton_nsigma_p;
	TH2D* h_select_pion_nsigma_p;

	TH1F * h_hitsTimesLength;
	TH1F * h_hitsTimesLengthClone;
	TH1F * h_hitsTimesLengthNoClone;
	TH1F * h_mcEta;
	TH2D * h_mcHits_mcTrackPt;
	TH1D * h_mcHits;

	TH1D * h_gTrackHits;
	TH1D * h_gTrackHitsClone;
	TH1D * h_gTrackHitsNoGhost;
	TH1D * h_gTrackHitsCloneNoGhost;
	TH1D * h_gTrackHitsNoClone;
	TH1D * h_gTrackHitsNoCloneNoGhost;

	TH1D * h_gTrackEta;
	TH1D * h_gTrackEtaNoGhost;
	TH1D * h_gTrackEtaNoClone;
	TH1D * h_Eff;
	TH1D * h_EffNoGhost;
	TH1D * h_EffNoClone;

	vector<StMuTrack *> mDauVec1;
	vector<StMuTrack *> mDauVec2;
	vector<StMuTrack *> mDauVec3;

	vector<unsigned int> mDau1GePid;
	vector<unsigned int> mDau2GePid;
	vector<unsigned int> mDau3GePid;

	vector<double> mDauDcaVec1;
	vector<double> mDauDcaVec2;
	vector<double> mDauDcaVec3;

	vector<double> mDauMass1;
	vector<double> mDauMass2;
	vector<double> mDauMass3;

	vector<double> mDauBeta1;
	vector<double> mDauBeta2;
	vector<double> mDauBeta3;

//	Added by xyf
	int tb_mIEvt;
	int tb_OriginMult;
	int tb_McMult;
	double tb_mRcVertexX;
	double tb_mRcVertexY;
	double tb_mRcVertexZ;
	double tb_mMcVertexX;
	double tb_mMcVertexY;
	double tb_mMcVertexZ;
	double tb_MagField;

	int tb_mNMcTrk;
	int tb_mNRcTrk;

	int tb_nv0;
//	int tb_ndau1;
	int tb_ndau2;
	int tb_ndau3;

	double tb_v0mass;
	double tb_v0pt;
	double tb_v0rapidity;
	double tb_v0eta;
	double tb_v0x;
	double tb_v0y;
	double tb_v0z;
	double tb_v0px;
	double tb_v0py;
	double tb_v0pz;
	double tb_v0declen;
	double tb_v0dca;
//	double tb_dca1to2;
//	double tb_dca1to3;
	double tb_dca2to3;
//	double tb_dca1toxv0123;
//	double tb_dca2toxv0123;
//	double tb_dca3toxv0123;

//	double tb_dca1toxv012;
//	double tb_dca2toxv012;
	double tb_dca2toxv023;
	double tb_dca3toxv023;
//	double tb_dca1toxv013;
//	double tb_dca3toxv013;
//	double tb_xv012toPV;
	double tb_xv023toPV;
//	double tb_xv013toPV;
//	double tb_dcaxv012toPV;
	double tb_dcaxv023toPV;
//	double tb_dcaxv013toPV;

/*	int tb_dau1id;
	double tb_dau1dca;
	int tb_dau1nhits;
	int tb_dau1nhitsfit;
	int tb_dau1nhitsposs;
	int tb_dau1nhitsdedx;
	double tb_dau1eta;
	double tb_dau1dedx;
	double tb_dau1Z;
	double tb_dau1nsigma;
	double tb_dau1pt;
	double tb_dau1px;
	double tb_dau1py;
	double tb_dau1pz;
	int tb_dau1tpc;
	double tb_dau1mass;
	double tb_dau1beta;
*/
	int tb_dau2id;
	double tb_dau2dca;
	int tb_dau2nhits;
	int tb_dau2nhitsfit;
	int tb_dau2nhitsposs;
	int tb_dau2nhitsdedx;
	double tb_dau2eta;
	double tb_dau2dedx;
	double tb_dau2nsigma;
	double tb_dau2pt;
	double tb_dau2px;
	double tb_dau2py;
	double tb_dau2pz;
//	int tb_dau2tpc;
//	int tb_dau2mass;
//	int tb_dau2beta;
	int tb_dau2GePid;
	int tb_dau2ParentVxid;

	int tb_dau3id;
	double tb_dau3dca;
	int tb_dau3nhits;
	int tb_dau3nhitsfit;
	int tb_dau3nhitsposs;
	int tb_dau3nhitsdedx;
	double tb_dau3eta;
	double tb_dau3dedx;
	double tb_dau3nsigma;
	double tb_dau3pt;
	double tb_dau3px;
	double tb_dau3py;
	double tb_dau3pz;
//	int tb_dau3tpc;
//	double tb_dau3mass;
//	double tb_dau3beta;
	int tb_dau3GePid;
	int tb_dau3ParentVxid;

//	This is needed to make your maker known to root4star.
//	It must be always the last statement in the class.
//	Note that this is a macro, that's why the ';' is missing.

	ClassDef(StRecTreeMaker,0)
	};
#endif
