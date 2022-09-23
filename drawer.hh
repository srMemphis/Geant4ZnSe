using namespace std;

vector<Double_t> energies;
vector<Double_t> x, y, z;
vector<Double_t> angles;
vector<Double_t> counts, counts_ene;
vector<Double_t> nEvent_vec, nEvent_vec_corr;
Int_t max_nEvents = 0;

Double_t yield_scale_modifier = 10; // times yield value was scaled down

void read_data_single(TString fname);
void draw_counts();
void process_hits();
void calc_and_draw_amplitude(const vector<Double_t>& in_vector, TString hist_name);
Int_t search_closest(const vector<Double_t>& sorted_array, Double_t x);