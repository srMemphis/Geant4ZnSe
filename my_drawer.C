#include <iostream>
#include <vector>
#include "TMath.h"
#include "TCanvas.h"
#include "TFile.h"
#include "TTree.h"
#include "TH1F.h"
#include "TGraph.h"
#include "TMultiGraph.h"

void wavelength(TString fileName);
void counts_vs_thic();
void multy_wavelength();

void my_drawer()
{
    wavelength("build/0.25.root");
    counts_vs_thic();
    multy_wavelength();
}

void wavelength(TString fileName)
{
    TCanvas *canvas = new TCanvas("Energy spectrum", "Energy spectrum",960, 540);

    // reading file
    TFile *inpFile  = new TFile(fileName);
    if(!inpFile->IsOpen())
    {
        std::cout << "Can not open file: " << fileName << std::endl;
        return;
    }

    // reading tree
    TTree *tree = (TTree*) inpFile->Get("Hits");
    const int nEntries = tree->GetEntries();

    double wavelengthT;
    tree->SetBranchAddress("Wavelength", &wavelengthT);

    //creating hist
    int nBins = 50;
    TH1F *temphist = new TH1F ("", "", nBins, 550, 750);

    //filling hist
    for(int i = 0; i < nEntries; i++)
    {
        tree->GetEntry(i);
        temphist->Fill(wavelengthT);
    }


    Double_t factor = 1.;
    temphist->Scale(factor/temphist->Integral(), "width");

    //reading hist
    double wavelength[nBins], count[nBins];

    for(int i = 0; i < nBins; i++)
    {
        wavelength[i] = temphist->GetBinCenter(i);
        count[i] = temphist->GetBinContent(i);
    }

    // creating graph
    TGraph *graph = new TGraph(nBins, wavelength, count);

    graph->SetLineColor(2);
    graph->SetLineWidth(2);
    graph->SetTitle("A Nigger");
    graph->Draw("ACP");

    // GG
    inpFile->Close();
    return;

}


void counts_vs_thic()
{

    TCanvas *canvas2 = new TCanvas("canvas2", "canvas2",960, 540);
    canvas2->cd();

    std::vector<TString> names{ "build/0.25.root","build/0.50.root","build/0.75.root","build/1.00.root",
                                "build/1.25.root","build/1.50.root","build/1.75.root","build/2.00.root",
                                "build/2.25.root","build/2.50.root","build/2.75.root","build/3.00.root",
                                "build/3.25.root","build/3.50.root","build/3.75.root","build/4.00.root"};
    double counts[names.size()];
    for(int i = 0; i < names.size(); i++)
    {
        TFile *file = new TFile(names[i]);
        if(!file->IsOpen())
        {
            std::cout << "Can not open file: " << names[i] << std::endl;
            return;
        }

        TTree *tree = (TTree*) file->Get("Hits");
        counts[i] = tree->GetEntries()/8;
        file->Close();
    }

    double thic[16]{0.25, 0.50, 0.75, 1.00,
                    1.25, 1.50, 1.75, 2.00,
                    2.25, 2.50, 2.75, 3.00,
                    3.25, 3.50, 3.75, 4.00};

    TGraph *graph = new TGraph(names.size(), thic, counts);
    graph->SetLineColor(2);
    graph->SetLineWidth(2);
    graph->SetTitle("Another Nigger");
    graph->Draw("ACP*");

    return;
}



void multy_wavelength()
{
    TCanvas *canvas3 = new TCanvas("canvas3", "canvas3",960, 540);
    canvas3->cd();

    TMultiGraph *mg = new TMultiGraph();

    // std::vector<TString> names{ "build/0.25.root","build/1.00.root",
    //                             "build/1.25.root","build/2.00.root",
    //                             "build/2.25.root","build/3.00.root",
    //                             "build/3.25.root","build/4.00.root"};

        std::vector<TString> names{ "build/1.75.root", "build/2.00.root", "build/2.25.root"};

    int colorPool[8]{2,3,4,5,6,7,8,9};

    for(int i = 0; i < names.size(); i++)
    {
        // reading file
        TFile *inpFile  = new TFile(names[i]);
        if(!inpFile->IsOpen())
        {
            std::cout << "Can not open file: " << names[i] << std::endl;
            return;
        }

        // reading tree
        TTree *tree = (TTree*) inpFile->Get("Hits");
        const int nEntries = tree->GetEntries();

        double wavelengthT;
        tree->SetBranchAddress("Wavelength", &wavelengthT);

        //creating hist
        int nBins = 50;
        TH1F *temphist = new TH1F ("", "", nBins, 550, 750);

        //filling hist
        for(int i = 0; i < nEntries; i++)
        {
            tree->GetEntry(i);
            temphist->Fill(wavelengthT);
        }


        // Double_t factor = 1.;
        // temphist->Scale(factor/temphist->Integral(), "width");

        //reading hist+
        double wavelength[nBins], count[nBins];

        for(int i = 0; i < nBins; i++)
        {
            wavelength[i] = temphist->GetBinCenter(i);
            count[i] = temphist->GetBinContent(i)/8;
        }

        // creating graph
        TGraph *graph = new TGraph(nBins, wavelength, count);

        graph->SetLineColor(colorPool[i]);
        graph->SetLineWidth(2);
        graph->SetTitle((TString)names[i](6,4));

        mg->Add(graph);

    }

    mg->Draw("ACP");
    canvas3->BuildLegend();
    canvas3->Update();
}

