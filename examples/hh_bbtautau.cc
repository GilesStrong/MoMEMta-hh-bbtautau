/*
 *  MoMEMta: a modular implementation of the Matrix Element Method
 *  Copyright (C) 2016  Universite catholique de Louvain (UCL), Belgium
 *
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */


#include <momemta/ConfigurationReader.h>
#include <momemta/MoMEMta.h>
#include <momemta/Unused.h>

#include <TH1D.h>

#include <chrono>

using namespace std::chrono;
using namespace momemta;

int main(int argc, char** argv) {

    UNUSED(argc);
    UNUSED(argv);

    logging::set_level(logging::level::debug);

    ConfigurationReader configuration("../examples/hh_bbtautau.lua");

    MoMEMta weight(configuration.freeze());

   /* // bjet
    Particle bjet0 { "bjet0", LorentzVector(-92.264977, -42.952817, -67.447789, 122.648092), 5 };
    // anti-bjet
    Particle bjet1 { "bjet1", LorentzVector(-2.795940, -51.372875, 20.067071, 56.199769), -5 };
    // bjet
    Particle tau0 { "tau0", LorentzVector(30.757134, -13.697116, -54.096689, 63.743401), -15 };
    // anti-bjet
    Particle tau1 { "tau1", LorentzVector(113.214732, 119.024643, -3.917881, 164.325738), 15 };*/

    Particle bjet0 { "bjet0", LorentzVector(84.0921554565,-56.7863349915,-444.73727417,456.191253662), 5 };
    Particle bjet1 { "bjet1", LorentzVector(13.5419635773,-46.7071304321,-36.5659751892,61.0331993103), -5 };
    Particle tau0 { "tau0", LorentzVector(27.4124488831,49.1357765198,-146.326705933,156.781463623), 15 };
    Particle tau1 { "tau1", LorentzVector(-118.289733887,58.8872528076,-316.725891113,343.188873291), -15 };
/*
    Particle bjet0 { "bjet0", LorentzVector(15.68735, -189.8133, -210.1001, 288.65808), 5 };
    // anti-bjet
    Particle bjet1 { "bjet1", LorentzVector(-81.89723, -59.53946, -75.95126, 127.12102), -5 };
    // bjet
    Particle tau0 { "tau0", LorentzVector(84.728727, 127.45076, -243.5151, 289.04342), -15 };
    // anti-bjet
    Particle tau1 { "tau1", LorentzVector(-18.90508, 86.760458, -76.85704, 117.55769), 15 };*/

    auto start_time = system_clock::now();
    std::vector<std::pair<double, double>> weights = weight.computeWeights({bjet0, bjet1, tau0, tau1});
    auto end_time = system_clock::now();

    LOG(debug) << "Result:";
    for (const auto& r: weights) {
        LOG(debug) << r.first << " +- " << r.second;
    }

    LOG(info) << "Weight computed in " << std::chrono::duration_cast<milliseconds>(end_time - start_time).count() << "ms";


    return 0;
}
