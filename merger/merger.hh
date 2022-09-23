#include <vector>

using namespace std;

TFile *out_file;
TTree *out_ph_tree;

Double_t ang;
Double_t x ,y ,z, ene, event = 100;
Int_t sum_events;


void process_file(TString in_fname);