#include <momemta/ConfigurationReader.h>
#include <momemta/MoMEMta.h>
#include <momemta/Unused.h>

#include <string>
#include <iostream>
#include <fstream>

#include <TH1D.h>
#include "TFile.h"
#include "TH1F.h"
#include "TBranch.h"
#include "TTree.h"

#include <chrono>

using namespace std::chrono;
using namespace momemta;

bool debug = false;

void normalizeInput(LorentzVector& p4) {
     if (p4.M() > 0)
         return;
 
     // Increase the energy until M is positive
     while (p4.M2() < 0) {
         double delta = p4.E() * 1e-12;
         p4.SetE(p4.E() + delta);
     };
 }

 void fixMass(LorentzVector& p4, double mass) {
 
     double new_P = std::sqrt(p4.E() * p4.E() - mass * mass);
     double ratio = new_P / p4.P();
 
     p4.SetPxPyPzE(p4.Px() * ratio, p4.Py() * ratio, p4.Pz() * ratio, p4.E());
 
     normalizeInput(p4);
 }

 void showHelp() {
    /*Show help for input arguments*/
    std::cout << "-i : input mask\n";
    std::cout << "-o : output name\n";
    std::cout << "-s : Event number to start on, default 0\n";
    std::cout << "-d : run in debug mode [0/1], default 0\n";
    std::cout << "-n : Number of events to process, default all\n";
}

 std::map<std::string, std::string> getOptions(int argc, char* argv[]) {
    /*Interpret input arguments*/
    std::map<std::string, std::string> options;
    options.insert(std::make_pair("-i", "/home/giles/cernbox/CMS_HH_bbtautau_MVA/Data/mu_tau_b_b_MCData.root")); //Input mask
    options.insert(std::make_pair("-o", "/home/giles/cernbox/CMS_HH_bbtautau_MVA/Data/mu_tau_b_b_MCData_Bkg.root")); //Output name
    options.insert(std::make_pair("-d", "0")); //Debug mode
    options.insert(std::make_pair("-s", "0")); //Start number
    options.insert(std::make_pair("-n", "-1")); //Number ot process
    if (argc >= 2) {
        std::string option(argv[1]);
        if (option == "-h" || option == "--help") {
            showHelp();
            options.clear();
            return options;
        }
    }
    for (int i = 1; i < argc; i = i+2) {
        std::string option(argv[i]);
        std::string argument(argv[i+1]);
        if (option == "-h" || option == "--help" || argument == "-h" || argument == "--help") {
            showHelp();
            options.clear();
            return options;
        }
        options[option] = argument;
    }
    if (options["-i"] == "" || options["-o"] == "") {
        showHelp();
        options.clear();
        return options;
    }
    if (options["-d"] == "1") {
        debug = true;
        logging::set_level(logging::level::debug);
        std::cout << "Running in debug mode\n";
    }
    return options;
}

double stringToDouble(const std::string& s) {
    std::istringstream i(s);
    double x;
    if (!(i >> x))
        return 0;
    return x;
}

int main(int argc, char** argv) {
    std::map<std::string, std::string> options = getOptions(argc, argv);

    //logging::set_level(logging::level::debug);

    ConfigurationReader configuration("../examples/ttbar_mu_tau.lua");

    MoMEMta weight(configuration.freeze());

    TFile *f = new TFile(options["-i"].c_str(),"update");
    TTree *T = (TTree*)f->Get("tree");
    Double_t b_0_px, b_0_py, b_0_pz, b_0_E;
    Double_t b_1_px, b_1_py, b_1_pz, b_1_E;
    Double_t t_0_px, t_0_py, t_0_pz, t_0_E;
    Double_t t_1_px, t_1_py, t_1_pz, t_1_E;

    Double_t memWeight;
    TBranch *memBkgWeights = T->Branch("memBkgWeights",&memWeight,"memWeight/D");
    T->SetBranchAddress("b_0_px",&b_0_px);
    T->SetBranchAddress("b_0_py",&b_0_py);
    T->SetBranchAddress("b_0_pz",&b_0_pz);
    T->SetBranchAddress("b_0_E",&b_0_E);
    T->SetBranchAddress("b_1_px",&b_1_px);
    T->SetBranchAddress("b_1_py",&b_1_py);
    T->SetBranchAddress("b_1_pz",&b_1_pz);
    T->SetBranchAddress("b_1_E",&b_1_E);
    T->SetBranchAddress("t_0_px",&t_0_px);
    T->SetBranchAddress("t_0_py",&t_0_py);
    T->SetBranchAddress("t_0_pz",&t_0_pz);
    T->SetBranchAddress("t_0_E",&t_0_E);
    T->SetBranchAddress("t_1_px",&t_1_px);
    T->SetBranchAddress("t_1_py",&t_1_py);
    T->SetBranchAddress("t_1_pz",&t_1_pz);
    T->SetBranchAddress("t_1_E",&t_1_E);

    auto start_time = system_clock::now();
    Long64_t nentries = T->GetEntries();
    LorentzVector v_bjet0, v_bjet1, v_tau0, v_tau1;
    if (options["-n"] != "-1") {
        nentries = stringToDouble(options["-s"])+stringToDouble(options["-n"]);
    }
    for (Long64_t i=stringToDouble(options["-s"]); i < nentries; i++) {
        if (i%100 == 0) {
            LOG(info) << "event: " << i << " of " << nentries << " (" << (Double_t)(nentries-i)/(Double_t)nentries*100.0 << "% to go)";
            LOG(info) << "Latest weight: " << memWeight;
        }
        T->GetEntry(i);
        v_bjet0 = LorentzVector(b_0_px, b_0_py, b_0_pz, b_0_E);
        v_bjet1 = LorentzVector(b_1_px, b_1_py, b_1_pz, b_1_E);
        v_tau0 = LorentzVector(t_0_px, t_0_py, t_0_pz, t_0_E);
        v_tau1 = LorentzVector(t_1_px, t_1_py, t_1_pz, t_1_E);
        memWeight = -1;
        fixMass(v_bjet0, 0);
        fixMass(v_bjet1, 0);
        fixMass(v_tau0, 0);
        fixMass(v_tau1, 0);
        if ((v_bjet0.M() >= 0) & (v_bjet1.M() >= 0) & (v_tau0.M() >= 0) & (v_tau1.M() >= 0)) {
            if  ((v_bjet0.P() >= 0) & (v_bjet1.P() >= 0) & (v_tau0.P() >= 0) & (v_tau1.P() >= 0)) {
                Particle bjet0{"bjet0", v_bjet0, 5};
                Particle bjet1{"bjet1", v_bjet1, -5};
                Particle tau0{"tau0", v_tau0, -15};
                Particle tau1{"tau1", v_tau1, 13};
                std::vector<std::pair<long double, long double>> weights = weight.computeWeights({bjet0, bjet1, tau0, tau1});
                memWeight = weights[0].first;
                if (weights.size() > 1) std::cout << "Number of solutions: " << weights.size() << "\n";
            } else {
                std::cout << "Unphysical event due to momentum: " << v_bjet0.P() << " " << v_bjet1.P() << " " << v_tau0.P() << " " << v_tau1.P() << "\n";
                break;
            }
        } else {
                std::cout << "Unphysical event due to mass: " << v_bjet0.M() << " " << v_bjet1.M() << " " << v_tau0.M() << " " << v_tau1.M() << "\n";
                break;
        }
        memBkgWeights->Fill();
    }
    auto end_time = system_clock::now();
    LOG(info) << "Weights computed in " << std::chrono::duration_cast<milliseconds>(end_time - start_time).count() << "ms";
    T->Write();
    delete f;
    return 0;
}