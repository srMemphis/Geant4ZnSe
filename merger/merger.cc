#include <iostream>
#include <vector>
#include <fstream>

// #include "merger.hh"

using namespace std;

TFile *out_file;
TTree *out_ph_tree;

Double_t ang;
Double_t x ,y ,z, ene, event = 100;
Int_t sum_events;


void process_file(TString in_fname, Double_t weight);


void merger()
{
    TString dir = "../run1/";
    vector<TString> fnames = {"out_alpha.root", "out_gamma.root", "out_neutrons.root", };
    vector<Double_t> num_exitations = {1.9e13, 1.3e13, 2.34e12};
    TString out_fname = "out_merged.root";
    //create output tree with branches
    out_file = new TFile(dir + out_fname, "RECREATE");
    out_file->cd();

    out_ph_tree = new TTree("Photons","Merged photons tree");

    out_ph_tree->Branch("angle",&ang,"angle/D");
    out_ph_tree->Branch("X",&x,"X/D");
    out_ph_tree->Branch("Y",&y,"Y/D");
    out_ph_tree->Branch("Z",&z,"Z/D");
    out_ph_tree->Branch("Energy",&ene,"Energy/D");
    out_ph_tree->Branch("nEvent",&event,"nEvent/D");

    //calculate weights from number of exitations
    vector<Double_t> weights = {1}; //take alpha output as 1
    for (Int_t i = 1; i<num_exitations.size(); i++) {
        weights.push_back(num_exitations[i]/num_exitations[0]);
    }

    for (Int_t i = 0; i<fnames.size(); i++) {
        process_file(dir + fnames[i], weights[i]);
        // cout << "cur event number " << event << ", sum_events " << sum_events << endl;
    }

    // //open out file and write tree
    out_file->cd();
    out_ph_tree->Write();
    out_file->Close();

}

void process_file(TString in_fname, Double_t weight) {
    TFile *cur_infile = new TFile(in_fname);
    if (!cur_infile->IsOpen()) {std::cout << "Error: Cannot open file " << in_fname << std::endl; return;}
    cout << "File opened: " << in_fname << endl;

    TTree *inpTree_ph = (TTree*) cur_infile->Get("Photons");

    Double_t cur_in_ang, cur_in_x, cur_in_y, cur_in_z, cur_in_ene, cur_in_nEvent;
    Double_t cur_max_event = 0;

    inpTree_ph->SetBranchAddress("angle", &cur_in_ang);
    inpTree_ph->SetBranchAddress("X", &cur_in_x);
    inpTree_ph->SetBranchAddress("Y", &cur_in_y);
    inpTree_ph->SetBranchAddress("Z", &cur_in_z);
    inpTree_ph->SetBranchAddress("Energy", &cur_in_ene);
    inpTree_ph->SetBranchAddress("nEvent", &cur_in_nEvent);

    Int_t nEntries = (Int_t) inpTree_ph->GetEntries()*weight;
    // nEntries = 10;
    cout << "Reading " << nEntries << " events" << endl;

    for (Int_t i = 0; i < nEntries; i++) {

        if (i%10000==0) cout << "Event # " << i << endl;

        inpTree_ph->GetEntry(i);
        if (cur_in_nEvent>cur_max_event) cur_max_event = cur_in_nEvent;

        ang = cur_in_ang;
        x = cur_in_x; y = cur_in_y; z = cur_in_z;
        ene = cur_in_ene;
        event = cur_in_nEvent + sum_events; // numeration of events in this file starts from the number of last event in last file + 1
        //cout << "cur event number " << event << ", sum_events " << sum_events << endl;
        // cout << event << " " << &event << endl;

        out_ph_tree->Fill();

    }

    sum_events += cur_max_event + 1;
    cur_infile->Close();

}