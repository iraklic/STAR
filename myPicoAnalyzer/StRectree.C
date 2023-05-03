#include "TH1.h"
#include "TH2.h"
#include "TChain.h"
#include "TSystem.h"
#include <iostream>

class StMuDstMaker;
class StRecTreeMaker;

StMuDstMaker* maker;
StRecTreeMaker *myAnalysis;

void load()
	{
	gROOT->LoadMacro("$STAR/StRoot/StMuDSTMaker/COMMON/macros/loadSharedLibraries.C");
	loadSharedLibraries();
//	gROOT->LoadMacro("/afs/rhic.bnl.gov/star/packages/.DEV2/StRoot/StMuDSTMaker/COMMON/macros/lMuDst.C");
//	lMuDst();

//	gSystem->Load("StMagF");
//	gSystem->Load("StDbUtilities");

	gSystem->Load("StRecTreeMaker");
	}

//void StRectree(Int_t nevents=1, const Char_t *fileList, const Char_t *qaflag, const Char_t *outroot="test.root");
void StRectree(const Char_t *fileList, const Char_t *qaflag = "")
	{
	char outroot[200];
	sprintf (outroot, "%s.root", fileList);
	load();
//	StChain *chain = new StChain; // xuyifei yuri
	chain->SetDebug(0);
	StMuDebug::setLevel(0);  // switch of some debug output
	StMuTimer timer;
	timer.start();
//	StMuDbReader* dbReader = StMuDbReader::instance();
//	dbReader->addDb("P02gc.db");

	char theFilter[80];
	sprintf(theFilter,".MuDst.root:MuDst.root");

	maker = new StMuDstMaker(0, 0, "", fileList, theFilter, 1000);   // set up maker in read mode

//	StMuDbReader...
//	StMuDbReader* db = StMuDbReader::instance();

//	StMuDst2StEventMaker *muDst2StEvent = new StMuDst2StEventMaker();

	myAnalysis = new StRecTreeMaker(maker, outroot);
	chain->Init();
	maker->SetActive();
//	maker->Init();// xuyifei yuri
	maker->SetStatus("*",0);
	maker->SetStatus("MuEventAll",1);
//	maker->SetStatus("EmcAll",1);
//	maker->SetStatus("Emc*",1);
	maker->SetStatus("Event",1);
	maker->SetStatus("MuEvent",1);
//	maker->SetStatus("EmcTow",1);
//	maker->SetStatus("EmcPrs",1);
//	maker->SetStatus("EmcSmde",1);
//	maker->SetStatus("EmcSmdp",1);
	maker->SetStatus("PrimaryTracks",1);
	maker->SetStatus("GlobalTracks",1);
//	maker->SetStatus("V0",1);
	maker->SetStatus("PrimaryVertices",1);
	maker->SetStatus("StStMuMcVertex",1);
	maker->SetStatus("StStMuMcTrack",1);

	int nevents = maker->chain()->GetEntries();
	cout << "total entries: " << nevents << endl;
	for (Int_t iev = 0; iev < nevents; iev++)
		{
		if(iev % 100 == 0) cout << "Working on eventNumber " << iev << endl;
		chain->Clear();
		int iret = chain->Make(iev); // This should call the Make() method in ALL makers    
		if (iret)
			{
			cout << "Bad return code!" << endl;
			break;
			}
		} // Event Loop

//	if(nevents>1) chain->Finish();    
//	delete chain;
	}
