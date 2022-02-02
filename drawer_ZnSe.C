// ROOT Macro to draw results

#include <iostream>
#include <vector>
#include "TMath.h"
#include "TCanvas.h"
#include "TFile.h"
#include "TTree.h"
#include "TH1F.h"
#include "TGraph.h"
#include "drawer_ZnSe.h"

void drawer_ZnSe(){
    auto rootffs = new TH1F();
    rootffs->Print();
    delete rootffs;
    drawenergy("build/output.root");
}

void drawenergy(TString fname){

    TCanvas *c4 = new TCanvas("Energy spectrum", "Energy spectrum",1920,1080); 
    Int_t nbins = 100;
    Double_t etemp = 0;
    TFile *inpf = new TFile(fname);
    if (!inpf->IsOpen()) {std::cout << "Fatal: Cannot open file " << fname << std::endl; return; }
    TTree *inpTree = (TTree*) inpf->Get("Hits");
    inpTree->SetBranchAddress("Wavelength", &etemp);
    const Int_t nEntries = inpTree->GetEntries();
    TH1F *temphist = new TH1F ("", "", nbins, 550, 750);// temporary storage

    for (Int_t i1 = 0; i1 < nEntries; i1++) {// cycle for entries
        inpTree->GetEntry(i1);
        temphist->Fill(etemp); // filling temporary hist
    }

    Double_t e1[nbins], val1[nbins];
    for (Int_t i1 = 0; i1<nbins; i1++){
        e1[i1]= temphist->GetBinCenter(i1);
        val1[i1]= temphist->GetBinContent(i1);
        // val1[i1]*=10;
    }

    auto gr1 = new TGraph(nbins,e1,val1);
    // auto gr1 = new TGraph(nbins,e2,val2);
    //gr1->SetTitle();
    gr1->SetLineColor(2);
    gr1->SetLineWidth(2);
    inpf->Close();
    gr1->Draw("ACP");
    return;
}