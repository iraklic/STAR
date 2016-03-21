#define MAX_NUM 800000
struct TupTrackEntry
{
	//Event level
	Int_t mIEvt;
	Int_t OriginMult; //uncorrectedNumberOfPrimaries
	Int_t McMult;	// number of primary tracks * 10000 + something like McrefMult
	Float_t	mRcVertexX;
	Float_t	mRcVertexY;
	Float_t	mRcVertexZ;
	Float_t	mMcVertexX;
	Float_t	mMcVertexY;
	Float_t	mMcVertexZ;
	Float_t	MagField;

	//mc track quantities
	Int_t mNMcTrk;
	Int_t mMcId[MAX_NUM];
	Int_t mGeantId[MAX_NUM];
	float mMcPt[MAX_NUM];
	float mMcPx[MAX_NUM];
	float mMcPy[MAX_NUM];
	float mMcPz[MAX_NUM];
	float mMcEta[MAX_NUM];
	float mMcPhi[MAX_NUM];

	//properties of reco track
	Int_t mNRcTrk;
	Int_t mRcId[MAX_NUM];
	Int_t mRcAssoId[MAX_NUM];
	Int_t mRcAssoGId[MAX_NUM];

	int RcMult;
//***********************************************

	int nv0;
//	int ndau1;
	int ndau2;
	int ndau3;

	int parentgepid[MAX_NUM];
	int parentidvx[MAX_NUM];
	int parentidvxend[MAX_NUM];
	int parentcharge[MAX_NUM];
	float parenteta[MAX_NUM];
	float parentdecayx[MAX_NUM];
	float parentdecayy[MAX_NUM];
	float parentdecayz[MAX_NUM];
	float parentpt[MAX_NUM];
	float parentpx[MAX_NUM];
	float parentpy[MAX_NUM];
	float parentpz[MAX_NUM];

	float v0mass[MAX_NUM];
	float v0pt[MAX_NUM];
	float v0rapidity[MAX_NUM];
	float v0eta[MAX_NUM];
	float v0x[MAX_NUM];
	float v0y[MAX_NUM];
	float v0z[MAX_NUM];
	float v0px[MAX_NUM];
	float v0py[MAX_NUM];
	float v0pz[MAX_NUM];
	float v0declen[MAX_NUM];
	float v0dca[MAX_NUM];
//	float dca1to2[MAX_NUM];
//	float dca1to3[MAX_NUM];
	float dca2to3[MAX_NUM];
//	float dca1toxv0123[MAX_NUM];
//	float dca2toxv0123[MAX_NUM];
//	float dca3toxv0123[MAX_NUM];
//	float dca1toxv012[MAX_NUM];
//	float dca2toxv012[MAX_NUM];
	float dca2toxv023[MAX_NUM];
	float dca3toxv023[MAX_NUM];
//	float dca1toxv013[MAX_NUM];
//	float dca3toxv013[MAX_NUM];
//	float xv012toPV[MAX_NUM];
	float xv023toPV[MAX_NUM];
//	float xv013toPV[MAX_NUM];
//	float dcaxv012toPV[MAX_NUM];
	float dcaxv023toPV[MAX_NUM];
//	float dcaxv013toPV[MAX_NUM];
;
/*	int dau1id[MAX_NUM];
	float dau1dca[MAX_NUM];
	int dau1nhits[MAX_NUM];
	int dau1nhitsfit[MAX_NUM];
	int dau1nhitsposs[MAX_NUM];
	int dau1nhitsdedx[MAX_NUM];
	float dau1eta[MAX_NUM];
	float dau1dedx[MAX_NUM];
	float dau1Z[MAX_NUM];
	float dau1nsigma[MAX_NUM];
	float dau1pt[MAX_NUM];
	float dau1px[MAX_NUM];
	float dau1py[MAX_NUM];
	float dau1pz[MAX_NUM];
	int dau1tpc[MAX_NUM];
	float dau1mass[MAX_NUM];
	float dau1beta[MAX_NUM];
*/;
	int dau2id[MAX_NUM];
	int dau2charge[MAX_NUM];
	float dau2dca[MAX_NUM];
	int dau2nhits[MAX_NUM];
	int dau2nhitsfit[MAX_NUM];
	int dau2nhitsposs[MAX_NUM];
	int dau2nhitsdedx[MAX_NUM];
	float dau2pathlength[MAX_NUM];
	float dau2eta[MAX_NUM];
	float dau2dedx[MAX_NUM];
	float dau2nsigma[MAX_NUM];
	float dau2pt[MAX_NUM];
	float dau2px[MAX_NUM];
	float dau2py[MAX_NUM];
	float dau2pz[MAX_NUM];
//	int dau2tpc[MAX_NUM];
//	float dau2mass[MAX_NUM];
//	float dau2beta[MAX_NUM];
	int dau2GePid[MAX_NUM];
	float dau2ParentVxid[MAX_NUM];
	float dau2assopt[MAX_NUM];
	float dau2assopx[MAX_NUM];
	float dau2assopy[MAX_NUM];
	float dau2assopz[MAX_NUM];
	int dau2assocharge[MAX_NUM];
	int dau2assoidvx[MAX_NUM];
	int dau2assoidvxend[MAX_NUM];

	int dau3id[MAX_NUM];
	int dau3charge[MAX_NUM];
	float dau3dca[MAX_NUM];
	int dau3nhits[MAX_NUM];
	int dau3nhitsfit[MAX_NUM];
	int dau3nhitsposs[MAX_NUM];
	int dau3nhitsdedx[MAX_NUM];
	float dau3pathlength[MAX_NUM];
	float dau3eta[MAX_NUM];
	float dau3dedx[MAX_NUM];
	float dau3nsigma[MAX_NUM];
	float dau3pt[MAX_NUM];
	float dau3px[MAX_NUM];
	float dau3py[MAX_NUM];
	float dau3pz[MAX_NUM];
//	int dau3tpc[MAX_NUM];
//	float dau3mass[MAX_NUM];
//	float dau3beta[MAX_NUM];
	int dau3GePid[MAX_NUM];
	float dau3ParentVxid[MAX_NUM];
	float dau3assopt[MAX_NUM];
	float dau3assopx[MAX_NUM];
	float dau3assopy[MAX_NUM];
	float dau3assopz[MAX_NUM];
	int dau3assocharge[MAX_NUM];
	int dau3assoidvx[MAX_NUM];
	int dau3assoidvxend[MAX_NUM];
//***********************************************

};
	TupTrackEntry mTrackEvent;

