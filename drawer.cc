#include <iostream>
#include <vector>
#include "drawer.hh"
#include "TMath.h"
#include <fstream>

void drawer()
{
    //init
    
    TString fname = "run1/out_merged.root";

    //general style options
    gStyle->SetPalette(kRainBow); //list of palettes https://root.cern.ch/doc/master/classTColor.html "Predefined palettes"
    // TColor::InvertPalette();

    //reading
    read_data_single(fname);
    process_hits();
    // draw_counts();
    calc_and_draw_amplitude(nEvent_vec, "amp_root");
    calc_and_draw_amplitude(nEvent_vec_corr, "amp_root_corrected");


}

void read_data_single(TString fname) {
    TFile *inpf = new TFile(fname);
    if (!inpf->IsOpen()) {std::cout << "Error: Cannot open file " << fname << std::endl; return;}
    TTree *inpTree_ph = (TTree*) inpf->Get("Photons");

    Double_t cur_ang, cur_x, cur_y, cur_z, cur_ene, cur_nEvent;

    // inpTree_ph->SetBranchAddress("angle", &cur_ang);
    // inpTree_ph->SetBranchAddress("X", &cur_x);
    // inpTree_ph->SetBranchAddress("Y", &cur_y);
    // inpTree_ph->SetBranchAddress("Z", &cur_z);
    inpTree_ph->SetBranchAddress("Energy", &cur_ene);
    inpTree_ph->SetBranchAddress("nEvent", &cur_nEvent);

    Int_t nEntries = inpTree_ph->GetEntries();
    // nEntries = 100000000;
    cout << "Reading " << nEntries << " events" << endl;

    for (Int_t i = 0; i < nEntries; i++) {
        inpTree_ph->GetEntry(i);
        // angles.push_back(cur_ang);
        // x.push_back(cur_x);
        // y.push_back(cur_y);
        // z.push_back(cur_z);
        energies.push_back(cur_ene);
        nEvent_vec.push_back(cur_nEvent);
        if (i%100000==0) cout << "Event # " << i << endl;
        if (cur_nEvent>max_nEvents) max_nEvents = cur_nEvent;
    }

    // TTree *inpTree_amp = (TTree*) inpf->Get("Amplitude");

    // Double_t cur_counts, cur_couts_ene;
    // inpTree_amp->SetBranchAddress("Counts", &cur_counts);
    // inpTree_amp->SetBranchAddress("Countsenergy", &cur_couts_ene);

    // nEntries = inpTree_amp->GetEntries();

    // for (Int_t i = 0; i < nEntries; i++) {
    //     inpTree_amp->GetEntry(i);
    //     // cout << cur_counts << " " << cur_couts_ene << endl;
    //     counts.push_back(cur_counts);
    //     counts_ene.push_back(cur_couts_ene);
    // }
    inpf->Close();
    return;

}

void process_hits(){
    
    //read and apply SiPM efficiency
    //read
    vector<Double_t> eff_ph_ene, eff;
    ifstream eff_file;
    eff_file.open("Sipm_props.txt");
    if (!eff_file.is_open()) {
        cout << "[FATAL]: cannot open file with SiPM props" << endl;
        return;
    }

    Double_t cur_ph_ene, cur_eff;
    for (Int_t i = 0; eff_file>>cur_ph_ene>>cur_eff; ++i) {
        eff_ph_ene.push_back(1239.8 /cur_ph_ene);
        eff.push_back(cur_eff);
    }

    reverse(eff_ph_ene.begin(), eff_ph_ene.end());
    reverse(eff.begin(), eff.end());


    // TCanvas *c1 = new TCanvas("SiPM_eff", "SiPM_eff", 1920, 1080);
    // c1->cd();
    // auto gr = new TGraph(eff_ph_ene.size(), &eff_ph_ene[0], &eff[0]);
    // gr->SetTitle("SiPM efficiency; Energy; eff");
    // gr->SetLineColor(2);
    // gr->SetLineWidth(2);
    // gr->Draw("ACP");
    // c1->Modified();
    // c1->SaveAs("Sipm_eff.png");

    //apply
    cout << "applying efficiency" << endl;

    for (Int_t i = 0; i<energies.size(); i++) {
        Int_t idx = search_closest(eff_ph_ene, energies[i]);
        Double_t found_eff = eff[idx];
        // cout << "energies[i] = " << energies[i] << " idx = " << idx << " found_eff = " << found_eff << endl;
        Double_t cur_rand_num = gRandom->Uniform()*100;
        if (cur_rand_num<found_eff) nEvent_vec_corr.push_back(nEvent_vec[i]);

    }
    // cout << nEvent_vec.size() << endl;
    // cout << nEvent_vec_corr.size()<< endl;

}

void calc_and_draw_amplitude(const vector<Double_t>& in_vector, TString hist_name){
    // Int_t nEvents = counts.size();
    Int_t nEvents = max_nEvents;
    vector<Int_t> amp(nEvents, 0);
    for (Int_t i = 0; i<in_vector.size(); i++) {
        amp[in_vector[i]]++;
    }

    auto hist = new TH1F(hist_name, hist_name, 150, 0, 150); 
    hist->SetLineWidth(2);

    for (Int_t i = 0; i<amp.size(); i++) {
        if (amp[i]>0) hist->Fill(amp[i]);
    }
    TCanvas *c1 = new TCanvas(hist_name, hist_name, 1920, 1080);
    c1->cd();
    hist->Draw("PLC");
    // c1->SetLogy();
    c1->Modified();

    // TCanvas *c2 = new TCanvas(hist_name + "_1", hist_name + "_1", 1920, 1080);
    // c2->cd();
    // vector<Int_t> v; 
    // for (Int_t i = 0; i<amp.size(); i++) {
    //     v.push_back(i);
    // }
    // auto gr = new TGraph(amp.size(), &v[0], &amp[0]);
    // gr->SetTitle("amp for events; nEvent; amp");
    // gr->SetLineWidth(2);
    // gr->Draw("ACP");
    // c2->Modified();

}

void draw_counts(){

    auto hist = new TH1F("Counts", "Counts", 1000, 0, 1000); 
    hist->SetLineWidth(2);
    auto hist1 = new TH1F("Counts*energy", "Counts*energy", 1000, 0, 1000); 
    hist1->SetLineWidth(2);
    
    for (Int_t i = 0; i<counts.size(); i++) {
        if (counts[i]>0) hist->Fill(counts[i]);
        if (counts_ene[i]>0) hist1->Fill(counts_ene[i]);
        // if (i<100) {
        //     cout << counts[i] << " " << counts_ene[i] << endl;
        // }
    }


    TCanvas *c1 = new TCanvas("Counts", "Counts", 1920, 1080);
    c1->cd();
    hist->Draw("PLC");
    // c1->SetLogy();
    c1->BuildLegend();
    c1->Modified();
    // c1->SaveAs("pics/spec "+descr.ReplaceAll('.', ",")+".png");

    TCanvas *c2 = new TCanvas("Counts*energy", "Counts*energy", 1920, 1080);
    c2->cd();
    hist1->Draw("PLC");
    // c2->SetLogy();
    c2->BuildLegend();
    c2->Modified();

    return;
}

Int_t search_closest(const vector<Double_t>& sorted_array, Double_t x) {

    auto iter_geq = lower_bound(
        sorted_array.begin(), 
        sorted_array.end(), 
        x
    );

    if (iter_geq == sorted_array.begin()) {
        return 0;
    }

    double a = *(iter_geq - 1);
    double b = *(iter_geq);

    if (fabs(x - a) < fabs(x - b)) {
        return iter_geq - sorted_array.begin() - 1;
    }

    return iter_geq - sorted_array.begin();
}
