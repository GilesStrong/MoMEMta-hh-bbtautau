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

int main(int argc, char** argv) {

    UNUSED(argc);
    UNUSED(argv);

    //logging::set_level(logging::level::debug);

    ConfigurationReader configuration("../examples/hh_bbtautau.lua");

    MoMEMta weight(configuration.freeze());

    TFile *f = new TFile("/home/giles/cernbox/CMS_HH_bbtautau_MVA/Data/mu_tau_b_b_MCData.root");
    //TFile *f = new TFile("/home/giles/cernbox/sample_analysis/processedData.root","update");
    TTree *T = (TTree*)f->Get("tree");
    Double_t regB_b_0_px, regB_b_0_py, regB_b_0_pz, regB_b_0_E;
    Double_t regB_b_1_px, regB_b_1_py, regB_b_1_pz, regB_b_1_E;
    Double_t regTau_t_0_px, regTau_t_0_py, regTau_t_0_pz, regTau_t_0_E;
    Double_t regTau_t_1_px, regTau_t_1_py, regTau_t_1_pz, regTau_t_1_E;

    T->SetBranchAddress("regB_b_0_px",&regB_b_0_px);
    T->SetBranchAddress("regB_b_0_py",&regB_b_0_py);
    T->SetBranchAddress("regB_b_0_pz",&regB_b_0_pz);
    T->SetBranchAddress("regB_b_0_E",&regB_b_0_E);
    T->SetBranchAddress("regB_b_1_px",&regB_b_1_px);
    T->SetBranchAddress("regB_b_1_py",&regB_b_1_py);
    T->SetBranchAddress("regB_b_1_pz",&regB_b_1_pz);
    T->SetBranchAddress("regB_b_1_E",&regB_b_1_E);
    T->SetBranchAddress("regTau_t_0_px",&regTau_t_0_px);
    T->SetBranchAddress("regTau_t_0_py",&regTau_t_0_py);
    T->SetBranchAddress("regTau_t_0_pz",&regTau_t_0_pz);
    T->SetBranchAddress("regTau_t_0_E",&regTau_t_0_E);
    T->SetBranchAddress("regTau_t_1_px",&regTau_t_1_px);
    T->SetBranchAddress("regTau_t_1_py",&regTau_t_1_py);
    T->SetBranchAddress("regTau_t_1_pz",&regTau_t_1_pz);
    T->SetBranchAddress("regTau_t_1_E",&regTau_t_1_E);

    auto start_time = system_clock::now();
    Long64_t nentries = T->GetEntries();
    LorentzVector v_bjet0, v_bjet1, v_tau0, v_tau1;
    std::vector<std::vector<double> > outputs;
    std::vector<double> buffer(3);
    for (Long64_t i=0;i<nentries;i++) {
        if (i%100 == 0) {
            LOG(info) <<"event: "<<i<<" of "<<nentries<<" ("<<(Double_t)(nentries-i)/(Double_t)nentries*100.0<<"% to go)";
            LOG(info) << "Latest weight: " << buffer[1];
        }
        T->GetEntry(i);
        v_bjet0 = LorentzVector(regB_b_0_px, regB_b_0_py, regB_b_0_pz, regB_b_0_E);
        v_bjet1 = LorentzVector(regB_b_1_px, regB_b_1_py, regB_b_1_pz, regB_b_1_E);
        v_tau0 = LorentzVector(regTau_t_0_px, regTau_t_0_py, regTau_t_0_pz, regTau_t_0_E);
        v_tau1 = LorentzVector(regTau_t_1_px, regTau_t_1_py, regTau_t_1_pz, regTau_t_1_E);
        buffer[0] = i;
        buffer[1] = -1;
        buffer[2] = -1;
        fixMass(v_bjet0, 0);
        fixMass(v_bjet1, 0);
        fixMass(v_tau0, 0);
        fixMass(v_tau1, 0);
        if ((v_bjet0.M() >= 0) & (v_bjet1.M() >= 0) & (v_tau0.M() >= 0) & (v_tau1.M() >= 0)) {
            if  ((v_bjet0.P() >= 0) & (v_bjet1.P() >= 0) & (v_tau0.P() >= 0) & (v_tau1.P() >= 0)) {
                Particle bjet0{"bjet0", v_bjet0, 5};
                Particle bjet1{"bjet1", v_bjet1, -5};
                Particle tau0{"tau0", v_tau0, -15};
                Particle tau1{"tau1", v_tau1, 15};
                std::vector<std::pair<long double, long double>> weights = weight.computeWeights({bjet0, bjet1, tau0, tau1});
                buffer[1] = weights[0].first;
                buffer[2] = weights[1].first;
                outputs.push_back(buffer);
                if (weights.size() > 1) std::cout << "Number of solutions: " << weights.size() << "\n";
/*                if (memWeight != memWeight) {
                    LOG(info) << "NaN found: " << memWeight;
                    break;
                }*/
            } else {
                std::cout << "Unphysical event due to momentum: " << v_bjet0.P() << " " << v_bjet1.P() << " " << v_tau0.P() << " " << v_tau1.P() << "\n";
                break;
            }
        } else {
                std::cout << "Unphysical event due to mass: " << v_bjet0.M() << " " << v_bjet1.M() << " " << v_tau0.M() << " " << v_tau1.M() << "\n";
                break;
        }
    }
    auto end_time = system_clock::now();
    LOG(info) << "Weights computed in " << std::chrono::duration_cast<milliseconds>(end_time - start_time).count() << "ms";
    std::ofstream outFile;
    outFile.open ("/home/giles/cernbox/CMS_HH_bbtautau_MVA/Data/mu_tau_b_b_MCData_Sig.csv");
    outFile << ",memSigWeight, memSigWeight_Error\n";
    for (int i = 0; i < outputs.size(); ++i) {
        outFile << outputs[i][0] << "," << outputs[i][1] << "," << outputs[i][2] << "\n";
    }
    outFile.close();
    delete f;
    return 0;
}