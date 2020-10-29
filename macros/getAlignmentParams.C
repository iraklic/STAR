void getAlignmentParams() {
	TFile * f = new TFile("stPlots.Cut.Errors.root");
	gSystem->Load(" ~/STAR/macros/TpcAlignerDraw_C.so");
	TpcAlignerDraw(2);
}
