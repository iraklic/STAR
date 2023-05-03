void pullAliases() {
	StiPulls->SetAlias("sector","((mHitsG.mHardwarePosition-(mHitsG.mHardwarePosition%16))/16)%32");
	StiPulls->SetAlias("padrow","((mHitsG.mHardwarePosition-(mHitsG.mHardwarePosition%512))/512)%128");
	StiPulls->SetAlias("localresid","((mHitsG.lYHit-mHitsG.lYFit)*sign(mHitsG.lZHit))");
	StiPulls->SetAlias("resid","(mHitsG.lYFit-mHitsG.lYHit)");
	StiPulls->SetAlias("residZ","(mHitsG.lZFit-mHitsG.lZHit)");
	StiPulls->SetAlias("residT","((mHitsG.lZFit-mHitsG.lZHit)*sign(mHitsG.lZHit)/5.5)");

	StiPulls->SetAlias("psector","((mHitsP.mHardwarePosition-(mHitsP.mHardwarePosition%16))/16)%32");
	StiPulls->SetAlias("ppadrow","((mHitsP.mHardwarePosition-(mHitsP.mHardwarePosition%512))/512)%128");
	StiPulls->SetAlias("plocalresid","((mHitsP.lYHit-mHitsP.lYFit)*sign(mHitsG.lZHit))");
	StiPulls->SetAlias("presid","(mHitsP.lYFit-mHitsG.lYHit)");
	StiPulls->SetAlias("presidZ","(mHitsP.lZFit-mHitsP.lZHit)");
	StiPulls->SetAlias("presidT","((mHitsP.lZFit-mHitsP.lZHit)*sign(mHitsP.lZHit)/5.5)");

	StiPulls->SetAlias("evNum", "mEv");
}
