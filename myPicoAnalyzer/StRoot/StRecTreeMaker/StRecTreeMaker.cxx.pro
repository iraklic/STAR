//Modified from StMiniMcMaker.cxx
///////////////////////////////////////////////////////////
#include "StRecTreeMaker.h"
#include "StEventTypes.h"
#include "StEvent.h"
#include "TNtuple.h"
#include "TFile.h"
#include "TTree.h"
#include "TBranch.h"
#include "TMath.h"

#include "StMcEventTypes.hh"
#include "StMcEvent.hh"
#include "StMcEvent/StMcTrack.hh"
#include "StMcEvent/StMcTpcHitCollection.hh"
#include "StMcEvent/StMcTpcHit.hh"
#include "StEvent/StTpcHitCollection.h"
#include "StEvent/StTpcHit.h"
#include "StEventUtilities/StuRefMult.hh"
#include "StBFChain.h"
#include "StMessMgr.h"

#include "RecTree.h"

StGlobalTrack*  partnerTrack(mcTrackMapType* map, StMcTrack* mT) {
  mcTrackMapIter i = map->find(mT);
  StGlobalTrack* rT = 0; 
  //  if (i != map->end()) rT = const_cast<StGlobalTrack*>((*i).second->partnerTrack());
  if (i != map->end()) rT = (*i).second->partnerTrack();
  return rT; 
} 
 
StMcTrack*  partnerMcTrack(rcTrackMapType* map, StGlobalTrack* rT) {
  rcTrackMapIter i = map->find(rT);
  StMcTrack* mT = 0;
  //  if (i != map->end()) mT = const_cast<StMcTrack*>((*i).second->partnerMcTrack());
  if (i != map->end()) mT = (*i).second->partnerMcTrack();
  return mT;
} 

ClassImp(StRecTreeMaker)       // this a macro, no ";" here

StRecTreeMaker::StRecTreeMaker(const Char_t *outname) : 
  StMaker("name","title"),
  mTupFile(0),
  trackTree(0),
  mRcEvent(0),
  mMcEvent(0)
{
  outName=outname;
}



StRecTreeMaker::~StRecTreeMaker() {}

Int_t StRecTreeMaker::Init()
{

  outName.ReplaceAll("root","rectree.root");
  mTupFile =  new TFile(outName, "RECREATE");
  mTupFile->SetCompressionLevel(9);
  trackTree=new TTree("mTrackEvent","trackTree");
  trackTree->SetAutoSave(1000000);

  cout << "Initialize the recotree ... " << endl;

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
  trackTree->Branch("mParentGeantId",mTrackEvent.mParentGeantId,"mParentGeantId[mNMcTrk]/I");
  trackTree->Branch("mParentMcId",mTrackEvent.mParentMcId,"mParentMcId[mNMcTrk]/I");
  trackTree->Branch("mMcPt",mTrackEvent.mMcPt,"mMcPt[mNMcTrk]/F");
  trackTree->Branch("mMcPz",mTrackEvent.mMcPz,"mMcPz[mNMcTrk]/F");
  trackTree->Branch("mMcEta",mTrackEvent.mMcEta,"mMcEta[mNMcTrk]/F");
  trackTree->Branch("mMcPhi",mTrackEvent.mMcPhi,"mMcPhi[mNMcTrk]/F");
  trackTree->Branch("mMcMass",mTrackEvent.mMcMass,"mMcMass[mNMcTrk]/F");
  trackTree->Branch("mMcStartX",mTrackEvent.mMcStartX,"mMcStartX[mNMcTrk]/F");   
  trackTree->Branch("mMcStartY",mTrackEvent.mMcStartY,"mMcStartY[mNMcTrk]/F");
  trackTree->Branch("mMcStartZ",mTrackEvent.mMcStartZ,"mMcStartZ[mNMcTrk]/F");
  trackTree->Branch("mMcNhits",mTrackEvent.mMcNhits,"mMcNhits[mNMcTrk]/I");
  trackTree->Branch("mMcTpcHitX",mTrackEvent.mMcTpcHitX,"mMcTpcHitX[mNMcTrk][45]/F");
  trackTree->Branch("mMcTpcHitY",mTrackEvent.mMcTpcHitY,"mMcTpcHitY[mNMcTrk][45]/F");
  trackTree->Branch("mMcTpcHitZ",mTrackEvent.mMcTpcHitZ,"mMcTpcHitZ[mNMcTrk][45]/F");
  trackTree->Branch("mMcTpcHitId",mTrackEvent.mMcTpcHitId,"mMcTpcHitId[mNMcTrk][45]/I");

  trackTree->Branch("mNRcTrk",&mTrackEvent.mNRcTrk,"mNRcTrk/I");
  trackTree->Branch("mRcId",mTrackEvent.mRcId,"mRcId[mNRcTrk]/I");
  trackTree->Branch("mRcAssoId",mTrackEvent.mRcAssoId,"mRcAssoId[mNRcTrk]/I");
  trackTree->Branch("mRcAssoOffset",mTrackEvent.mRcAssoOffset,"mRcAssoOffset[mNRcTrk]/I");
  trackTree->Branch("mRcPt",mTrackEvent.mRcPt,"mRcPt[mNRcTrk]/F");
  trackTree->Branch("mRcPz",mTrackEvent.mRcPz,"mRcPz[mNRcTrk]/F");
  trackTree->Branch("mRcEta",mTrackEvent.mRcEta,"mRcEta[mNRcTrk]/F");
  trackTree->Branch("mRcPhi",mTrackEvent.mRcPhi,"mRcPhi[mNRcTrk]/F");
  trackTree->Branch("mRcNhits",mTrackEvent.mRcNhits,"mRcNhits[mNRcTrk]/I");
  trackTree->Branch("mRcNhitsPts",mTrackEvent.mRcNhitsPts,"mRcNhitsPts[mNRcTrk]/I");
  trackTree->Branch("mRcTpcHitX",mTrackEvent.mRcTpcHitX,"mRcTpcHitX[mNRcTrk][45]/F");
  trackTree->Branch("mRcTpcHitY",mTrackEvent.mRcTpcHitY,"mRcTpcHitY[mNRcTrk][45]/F");
  trackTree->Branch("mRcTpcHitZ",mTrackEvent.mRcTpcHitZ,"mRcTpcHitZ[mNRcTrk][45]/F");
  trackTree->Branch("mRcTpcHitErrX",mTrackEvent.mRcTpcHitErrX,"mRcTpcHitErrX[mNRcTrk][45]/F");
  trackTree->Branch("mRcTpcHitErrY",mTrackEvent.mRcTpcHitErrY,"mRcTpcHitErrY[mNRcTrk][45]/F");
  trackTree->Branch("mRcTpcHitErrZ",mTrackEvent.mRcTpcHitErrZ,"mRcTpcHitErrZ[mNRcTrk][45]/F");

  trackTree->Branch("mHelixCurv",mTrackEvent.mHelixCurv,"mHelixCurv[mNRcTrk]/F");
  trackTree->Branch("mHelixDip",mTrackEvent.mHelixDip,"mHelixDip[mNRcTrk]/F");
  trackTree->Branch("mHelixPhase",mTrackEvent.mHelixPhase,"mHelixPhase[mNRcTrk]/F");
  trackTree->Branch("mHelixX",mTrackEvent.mHelixX,"mHelixX[mNRcTrk]/F");
  trackTree->Branch("mHelixY",mTrackEvent.mHelixY,"mHelixY[mNRcTrk]/F");
  trackTree->Branch("mHelixZ",mTrackEvent.mHelixZ,"mHelixZ[mNRcTrk]/F");
  trackTree->Branch("mCharge",mTrackEvent.mCharge,"mCharge[mNRcTrk]/F");

  trackTree->Branch("mOuterHelixCurv",mTrackEvent.mOuterHelixCurv,"mOuterHelixCurv[mNRcTrk]/F");
  trackTree->Branch("mOuterHelixDip",mTrackEvent.mOuterHelixDip,"mOuterHelixDip[mNRcTrk]/F");
  trackTree->Branch("mOuterHelixPhase",mTrackEvent.mOuterHelixPhase,"mOuterHelixPhase[mNRcTrk]/F");
  trackTree->Branch("mOuterHelixX",mTrackEvent.mOuterHelixX,"mOuterHelixX[mNRcTrk]/F");
  trackTree->Branch("mOuterHelixY",mTrackEvent.mOuterHelixY,"mOuterHelixY[mNRcTrk]/F");
  trackTree->Branch("mOuterHelixZ",mTrackEvent.mOuterHelixZ,"mOuterHelixZ[mNRcTrk]/F");
  trackTree->Branch("mOuterCharge",mTrackEvent.mOuterCharge,"mOuterCharge[mNRcTrk]/F");

  trackTree->Branch("sharedTpcHits",mTrackEvent.sharedTpcHits,"sharedTpcHits[mNRcTrk]/I");
  trackTree->Branch("percentSharedTpcHits",mTrackEvent.percentSharedTpcHits,"percentSharedTpcHits[mNRcTrk]/F");
    
  return StMaker::Init();
}

void StRecTreeMaker::Clear(Option_t *opt)
{
    StMaker::Clear();
}

Int_t StRecTreeMaker::Finish()
{

  mTupFile->Write();
  mTupFile->Close();
    
  return kStOK;
}

Int_t StRecTreeMaker::Make()
{

  mRcEvent=(StEvent *) GetInputDS("StEvent");
  if (!mRcEvent){
    gMessMgr->Warning() << "StRecTreeMaker::Make : No StEvent" << endl;
    return kStOK;        // if no event, we're done
  }
  mMcEvent = (StMcEvent*) GetDataSet("StMcEvent");
  if(!mMcEvent) return kStErr;

  bool validRcVertex = true;
  if(!mRcEvent->primaryVertex(0)) {
        cout << "\tno primary vertex from stevent" << endl;
	//	return kFALSE;  // dongx
	validRcVertex = false;  // dongx
  }
  if(!mMcEvent->primaryVertex()){
        cout << "\tno primary vertex from stmcevent" << endl;
        return kFALSE;
  }

  if(validRcVertex) {
    mRcVertexPos = &mRcEvent->primaryVertex(0)->position();
  } else {
    mRcVertexPos = 0;
  }
  mMcVertexPos = &mMcEvent->primaryVertex()->position();

  StAssociationMaker* assoc = 0;
  assoc = (StAssociationMaker*) GetMaker("StAssociationMaker");
  
  // multimaps
  if (assoc) {
    mRcTrackMap = assoc->rcTrackMap();
    mMcTrackMap = assoc->mcTrackMap();
  }

  StSPtrVecTrackNode trknode = mRcEvent->trackNodes();
  cout << "#track nodes in stevent: " << trknode.size() << endl;
  mTrackEvent.OriginMult=trknode.size();
  /*
  if(trknode.size()>0) {
    for (int nn=0; nn<trknode.size(); nn++) {
      StTrackNode *node = trknode[nn];
      cout << "find " << node->entries(global) << " global tracks !" << endl;

      StGlobalTrack *gtrk = dynamic_cast<StGlobalTrack*>(node->track(global));
      if(!gtrk) continue;
      if(gtrk->geometry()->momentum().perp()>10) {
	mTrackEvent.McMult = gtrk->detectorInfo()->hits(kTpcId).size();
	
      }
    }
  }
  */  



  cout<<"Fill TrackEvent object"<<endl;

  LOG_DEBUG<<"about to fill track object"<<endm;

  //event level
  mTrackEvent.mIEvt        = (Int_t) mRcEvent->id();

  
  Int_t NUncorrPr    = (Int_t) uncorrectedNumberOfPrimaries(*mRcEvent);
  
  if(validRcVertex) {
    //    mTrackEvent.OriginMult   = NUncorrPr;
    mTrackEvent.mRcVertexX   = mRcVertexPos->x();
    mTrackEvent.mRcVertexY   = mRcVertexPos->y();
    mTrackEvent.mRcVertexZ   = mRcVertexPos->z();
  } else {
    //    mTrackEvent.OriginMult = 0;
    mTrackEvent.mRcVertexX   = -999.;
    mTrackEvent.mRcVertexY   = -999.;
    mTrackEvent.mRcVertexZ   = -999.;
  }
  
  mTrackEvent.mMcVertexX   = mMcVertexPos->x();
  mTrackEvent.mMcVertexY   = mMcVertexPos->y();
  mTrackEvent.mMcVertexZ   = mMcVertexPos->z();
  mTrackEvent.MagField     = mRcEvent->runInfo()->magneticField();


  cout << "Event variables filled" << endl;

  //track level
  StSPtrVecMcTrack mctracks=mMcEvent->tracks();
  //  cout<<"got track vector"<<endl;
  cout<<"number of MC tracks: "<<mctracks.size()<<endl;

  mTrackEvent.mNMcTrk      = 0;
  mTrackEvent.mNRcTrk      = 0;

  Int_t irc = 0; // reco track id
  Int_t imc = 0;

  Int_t nMcRefMult = 0;
  for(int xyz=0;xyz<(int) mctracks.size();xyz++){

    if(imc>=50000 || irc>=50000) { 
      cout << " out of array size limit, exit" << endl;
      continue;
    }

    StMcTrack* tr = dynamic_cast<StMcTrack *>(mctracks[xyz]);
    if(!tr) continue;

    if(tr->key()==0 && tr->geantId()==0) continue;  // not geant tracks

    const StThreeVectorF& mcMom       = tr->momentum();

    if(tr->startVertex()==mMcEvent->primaryVertex() && fabs(mcMom.pseudoRapidity())<0.5) nMcRefMult++;
    
    mTrackEvent.mMcId[imc]            = tr->key();
    mTrackEvent.mGeantId[imc]         = tr->geantId(); 

    if(!tr->parent()) {
      mTrackEvent.mParentGeantId[imc]    = -999; 
      mTrackEvent.mParentMcId[imc]       = -999;
    } else { 
      mTrackEvent.mParentGeantId[imc]    = tr->parent()->geantId();
      mTrackEvent.mParentMcId[imc]       = tr->parent()->key();
    }

    mTrackEvent.mMcPt[imc]            = mcMom.perp();
    mTrackEvent.mMcPz[imc]            = mcMom.z();
    mTrackEvent.mMcEta[imc]           = mcMom.pseudoRapidity();
    mTrackEvent.mMcPhi[imc]           = mcMom.phi();
    mTrackEvent.mMcMass[imc]          = tr->fourMomentum().m();
    
    if(!tr->startVertex()) {
      //      cout << "Can not find start vertex!" << endl;
      mTrackEvent.mMcStartX[imc]      = -999.; 
      mTrackEvent.mMcStartY[imc]      = -999.; 
      mTrackEvent.mMcStartZ[imc]      = -999.; 
    } else { 
      mTrackEvent.mMcStartX[imc]      = tr->startVertex()->position().x();
      mTrackEvent.mMcStartY[imc]      = tr->startVertex()->position().y();
      mTrackEvent.mMcStartZ[imc]      = tr->startVertex()->position().z();
    } 

    
    StPtrVecMcTpcHit& myTpcHit        = tr->tpcHits();
    int numTpcHits                    = myTpcHit.size();
    mTrackEvent.mMcNhits[imc]         = numTpcHits;


    if(numTpcHits>45) cout << "Warning: Mc Tpc Hits exceed 45! " << endl;
    int nnmchits = ((numTpcHits<45)?numTpcHits:45);

    for(int itpchit=0; itpchit<45; itpchit++) {
      mTrackEvent.mMcTpcHitX[imc][itpchit]  = -999.;
      mTrackEvent.mMcTpcHitY[imc][itpchit]  = -999.;
      mTrackEvent.mMcTpcHitZ[imc][itpchit]  = -999.;
      mTrackEvent.mMcTpcHitId[imc][itpchit] = -999;
    }

    if(nnmchits>0) {
      //    if(numTpcHits > 0 && numTpcHits < 45) {
      for(int itpchit=0; itpchit<nnmchits; itpchit++) {
	StMcTpcHit* tpchit = myTpcHit[itpchit];
    	mTrackEvent.mMcTpcHitX[imc][itpchit]  = tpchit->position().x();
	mTrackEvent.mMcTpcHitY[imc][itpchit]  = tpchit->position().y();
	mTrackEvent.mMcTpcHitZ[imc][itpchit]  = tpchit->position().z();
	mTrackEvent.mMcTpcHitId[imc][itpchit] = tpchit->key();
      }
    }



    /*
    StPtrVecMcSsdHit&   mySsdHit      = tr->ssdHits();
    StPtrVecMcIstHit&   myIstHit      = tr->istHits();
    StPtrVecMcPixelHit& myHftHit      = tr->pixelHits();
    Int_t numSsdHits                  = mySsdHit.size();
    Int_t numIstHits                  = myIstHit.size();
    Int_t numHftHits                  = myHftHit.size();

    
    // observation 070623 (yymmdd) : for pions, almost 1% with at least 2 hft hits have more than 5 of them!!
    if(numSsdHits > 5) numSsdHits     = 5;
    if(numIstHits > 5) numIstHits     = 5;
    if(numHftHits > 5) numHftHits     = 5;
    mTrackEvent.mMcNhitsSsd[imc]      = numSsdHits;
    mTrackEvent.mMcNhitsIst[imc]      = numIstHits;
    mTrackEvent.mMcNhitsHft[imc]      = numHftHits;

    for(int itpchit=0; itpchit<20; itpchit++) {
      mTrackEvent.mMcTpcHitX[imc][itpchit]  = -999.;
      mTrackEvent.mMcTpcHitY[imc][itpchit]  = -999.;
      mTrackEvent.mMcTpcHitZ[imc][itpchit]  = -999.;
      mTrackEvent.mMcTpcHitId[imc][itpchit] = -999;
      mTrackEvent.mMcTpcHitSide[imc][itpchit] = -999;
    }

    if(numSsdHits > 0) {
      for(int issdhit=0; issdhit<numSsdHits; issdhit++) {
	StMcSsdHit* ssdhit = mySsdHit[issdhit];
	mTrackEvent.mMcTpcHitX[imc][issdhit]  = ssdhit->position().x();
	mTrackEvent.mMcTpcHitY[imc][issdhit]  = ssdhit->position().y();
	mTrackEvent.mMcTpcHitZ[imc][issdhit]  = ssdhit->position().z();
	mTrackEvent.mMcTpcHitId[imc][issdhit] = ssdhit->key();
      }
    }

    if(numIstHits > 0) {
      for(int iisthit=0; iisthit<numIstHits; iisthit++) {
	StMcIstHit* isthit = myIstHit[iisthit];
	mTrackEvent.mMcTpcHitX[imc][iisthit+5]  = isthit->position().x();
	mTrackEvent.mMcTpcHitY[imc][iisthit+5]  = isthit->position().y();
	mTrackEvent.mMcTpcHitZ[imc][iisthit+5]  = isthit->position().z();
	mTrackEvent.mMcTpcHitId[imc][iisthit+5] = isthit->key();
	//	mTrackEvent.mMcTpcHitSide[imc][iisthit+5] = isthit->side();
	mTrackEvent.mMcTpcHitSide[imc][iisthit+5] = 0;
      }
    }

    if(numHftHits > 0) {
      for(int ihfthit=0; ihfthit<numHftHits; ihfthit++) {
	StMcPixelHit* hfthit = myHftHit[ihfthit];
	mTrackEvent.mMcTpcHitX[imc][ihfthit+15]  = hfthit->position().x();
	mTrackEvent.mMcTpcHitY[imc][ihfthit+15]  = hfthit->position().y();
	mTrackEvent.mMcTpcHitZ[imc][ihfthit+15]  = hfthit->position().z();
	mTrackEvent.mMcTpcHitId[imc][ihfthit+15] = hfthit->key();
      }
    }
    */


    // find best matched rcTrack
    std::pair<mcTrackMapType::iterator,mcTrackMapType::iterator> itpair3=mMcTrackMap->equal_range(tr);

    float mpsht=0.;
    int hnsht;
    StGlobalTrack* tMatched=0;
    StGlobalTrack* tTemp=0;

    for(mcTrackMapType::iterator mit3=itpair3.first;mit3!=itpair3.second;++mit3){
      StTrackPairInfo* tpinfo=(*mit3).second;
      tTemp=tpinfo->partnerTrack();
      //      tTemp=const_cast<StGlobalTrack*>(tpinfo->partnerTrack());

      if(tpinfo->percentOfPairedTpcHits()>mpsht){
	tMatched=tTemp;
	mpsht=tpinfo->percentOfPairedTpcHits();
	hnsht=tpinfo->commonTpcHits();
      }
    }

    
    // matched tracks, indexed by "irc"

    if(tMatched){

      mTrackEvent.mRcId[irc]          = tMatched->key();
      mTrackEvent.mRcAssoId[irc]      = tr->key();
      mTrackEvent.mRcAssoOffset[irc]  = imc;

      mTrackEvent.mRcPt[irc]          = tMatched->geometry()->momentum().perp();
      mTrackEvent.mRcPz[irc]          = tMatched->geometry()->momentum().z();
      mTrackEvent.mRcEta[irc]         = tMatched->geometry()->momentum().pseudoRapidity();
      mTrackEvent.mRcPhi[irc]         = tMatched->geometry()->momentum().phi();

      mTrackEvent.mRcNhits[irc]       = tMatched->detectorInfo()->hits(kTpcId).size();
      mTrackEvent.mRcNhitsPts[irc]    = tMatched->fitTraits().numberOfFitPoints(kTpcId);



      StPtrVecHit PartnerTpcHits      = tMatched->detectorInfo()->hits(kTpcId);
      int nPartnerTpcHits             = (int) PartnerTpcHits.size();

      if(nPartnerTpcHits>45) cout << "Warning: Rc Tpc Hits exceed 45!" << endl;
      int nnrchits = ((nPartnerTpcHits<45)?nPartnerTpcHits:45);

      for(int itpchit=0; itpchit<45; itpchit++) {
        mTrackEvent.mRcTpcHitX[irc][itpchit]  = -999.;
        mTrackEvent.mRcTpcHitY[irc][itpchit]  = -999.;
        mTrackEvent.mRcTpcHitZ[irc][itpchit]  = -999.;
      }

      if(nnrchits > 0) {
	//      if(nPartnerTpcHits > 0 && nPartnerTpcHits < 45) {
	for(int itpchit=0; itpchit<nnrchits; itpchit++) {
	  mTrackEvent.mRcTpcHitX[irc][itpchit]  = PartnerTpcHits[itpchit]->position().x();
	  mTrackEvent.mRcTpcHitY[irc][itpchit]  = PartnerTpcHits[itpchit]->position().y();
	  mTrackEvent.mRcTpcHitZ[irc][itpchit]  = PartnerTpcHits[itpchit]->position().z();
	  mTrackEvent.mRcTpcHitErrX[irc][itpchit]  = PartnerTpcHits[itpchit]->positionError().x();
	  mTrackEvent.mRcTpcHitErrY[irc][itpchit]  = PartnerTpcHits[itpchit]->positionError().y();
	  mTrackEvent.mRcTpcHitErrZ[irc][itpchit]  = PartnerTpcHits[itpchit]->positionError().z();
	}
      }


      /*
      StPtrVecHit PartnerSsdHits      = tMatched->detectorInfo()->hits(kSsdId);
      StPtrVecHit PartnerIstHits      = tMatched->detectorInfo()->hits(kIstId);
      //      StPtrVecHit PartnerHftHits      = tMatched->detectorInfo()->hits(kHftId);
      StPtrVecHit PartnerHftHits      = 0;

      int nPartnerSsdHits             = (int) PartnerSsdHits.size();
      int nPartnerIstHits             = (int) PartnerIstHits.size();
      int nPartnerHftHits             = (int) PartnerHftHits.size();

      if(nPartnerSsdHits > 5) nPartnerSsdHits = 5;
      if(nPartnerIstHits > 5) nPartnerIstHits = 5;
      if(nPartnerHftHits > 5) nPartnerHftHits = 5;

      mTrackEvent.mRcNhitsSsd[irc]    = nPartnerSsdHits;
      mTrackEvent.mRcNhitsIst[irc]    = nPartnerIstHits;
      mTrackEvent.mRcNhitsHft[irc]    = nPartnerHftHits;


      for(int itpchit=0; itpchit<20; itpchit++) {
        mTrackEvent.mRcTpcHitX[irc][itpchit]  = -999.;
        mTrackEvent.mRcTpcHitY[irc][itpchit]  = -999.;
        mTrackEvent.mRcTpcHitZ[irc][itpchit]  = -999.;
        mTrackEvent.mRcTpcHitId[irc][itpchit] = -999;
      }

      if(nPartnerSsdHits > 0) {
	for(int issdhit=0; issdhit<nPartnerSsdHits; issdhit++) {
	  mTrackEvent.mRcTpcHitX[irc][issdhit]  = PartnerSsdHits[issdhit]->position().x();
	  mTrackEvent.mRcTpcHitY[irc][issdhit]  = PartnerSsdHits[issdhit]->position().y();
	  mTrackEvent.mRcTpcHitZ[irc][issdhit]  = PartnerSsdHits[issdhit]->position().z();
	  // ssd uses slow simulator, so no StTpcHit available
	}
      }
      
      if(nPartnerIstHits > 0) {
	for(int iisthit=0; iisthit<nPartnerIstHits; iisthit++) {
	  mTrackEvent.mRcTpcHitX[irc][iisthit+5]  = PartnerIstHits[iisthit]->position().x();
	  mTrackEvent.mRcTpcHitY[irc][iisthit+5]  = PartnerIstHits[iisthit]->position().y();
	  mTrackEvent.mRcTpcHitZ[irc][iisthit+5]  = PartnerIstHits[iisthit]->position().z();

	  StHit *h= PartnerIstHits[iisthit];
	  if (h) {
	    StTpcHit *r=dynamic_cast<StTpcHit*>(h);
	    Int_t key = r->key();
	    mTrackEvent.mRcTpcHitId[irc][iisthit+5] = key;
	  }
	}
      }

      if(nPartnerHftHits > 0) {
	for(int ihfthit=0; ihfthit<nPartnerHftHits; ihfthit++) {
	  mTrackEvent.mRcTpcHitX[irc][ihfthit+15]  = PartnerHftHits[ihfthit]->position().x();
	  mTrackEvent.mRcTpcHitY[irc][ihfthit+15]  = PartnerHftHits[ihfthit]->position().y();
	  mTrackEvent.mRcTpcHitZ[irc][ihfthit+15]  = PartnerHftHits[ihfthit]->position().z();
	  
	  StHit *h= PartnerHftHits[ihfthit];
	  if (h) {
	    StTpcHit *r=dynamic_cast<StTpcHit*>(h);
	    Int_t key = r->key();
	    mTrackEvent.mRcTpcHitId[irc][ihfthit+15] = key;
	  }
	}
      }
      */

      
      StPhysicalHelixD PartnerHelix = tMatched->geometry()->helix();

      mTrackEvent.mHelixCurv[irc]   = PartnerHelix.curvature();
      mTrackEvent.mHelixDip[irc]    = PartnerHelix.dipAngle();
      mTrackEvent.mHelixPhase[irc]  = PartnerHelix.phase();  
      mTrackEvent.mHelixX[irc]      = PartnerHelix.origin().x();
      mTrackEvent.mHelixY[irc]      = PartnerHelix.origin().y();
      mTrackEvent.mHelixZ[irc]      = PartnerHelix.origin().z();    
      mTrackEvent.mCharge[irc]      = tMatched->geometry()->charge();    

      StPhysicalHelixD PartnerOuterHelix = tMatched->outerGeometry()->helix();

      mTrackEvent.mOuterHelixCurv[irc]   = PartnerOuterHelix.curvature();
      mTrackEvent.mOuterHelixDip[irc]    = PartnerOuterHelix.dipAngle();
      mTrackEvent.mOuterHelixPhase[irc]  = PartnerOuterHelix.phase();  
      mTrackEvent.mOuterHelixX[irc]      = PartnerOuterHelix.origin().x();
      mTrackEvent.mOuterHelixY[irc]      = PartnerOuterHelix.origin().y();
      mTrackEvent.mOuterHelixZ[irc]      = PartnerOuterHelix.origin().z();    
      mTrackEvent.mOuterCharge[irc]      = tMatched->outerGeometry()->charge();    
       
      mTrackEvent.percentSharedTpcHits[irc] = mpsht;
      mTrackEvent.sharedTpcHits[irc]        = hnsht;

      irc++;
    } 

    imc++;
  }
    mTrackEvent.McMult       = (Int_t) mMcEvent->numberOfPrimaryTracks()*10000+nMcRefMult;
  mTrackEvent.mNRcTrk = irc;
  mTrackEvent.mNMcTrk = imc;

  LOG_DEBUG<<"filled track object"<<endm;
  trackTree->Fill();

  //  istHitMap.clear();
  //  hftHitMap.clear();

  return kStOK;
}
