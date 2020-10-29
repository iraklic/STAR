void extractPromptHits() {
	TCanvas * c = new TCanvas
	FitP->Draw("mu-3.31351:x>>dOT0(24,0.5,24.5)","i&&j&&mu>0&&j>40","prof")
	FitP->Draw("mu-3.31351:x>>dIT0(24,0.5,24.5)","i&&j&&mu>0&&j<=40","profSAME")
}
