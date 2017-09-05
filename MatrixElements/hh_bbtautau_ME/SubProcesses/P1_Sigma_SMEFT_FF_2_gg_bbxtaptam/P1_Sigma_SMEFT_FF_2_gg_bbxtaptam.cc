// 
// *  This file was automatically generated by MoMEMta-MaGMEE,
// *  A MadGraph Matrix Element Exporter plugin for MoMEMta.
// *
// *  It is subject to MoMEMta-MaGMEE's license and copyright:
// *
// *  Copyright (C) 2016  Universite catholique de Louvain (UCL), Belgium
// *
// *  This program is free software: you can redistribute it and/or modify
// *  it under the terms of the GNU General Public License as published by
// *  the Free Software Foundation, either version 3 of the License, or
// *  (at your option) any later version.
// *
// *  This program is distributed in the hope that it will be useful,
// *  but WITHOUT ANY WARRANTY; without even the implied warranty of
// *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// *  GNU General Public License for more details.
// *
// *  You should have received a copy of the GNU General Public License
// *  along with this program.  If not, see <http://www.gnu.org/licenses/>.
// 

#include <string> 
#include <utility> 
#include <vector> 
#include <map> 

#include <P1_Sigma_SMEFT_FF_2_gg_bbxtaptam.h> 
#include <HelAmps_SMEFT_FF_2.h> 

#include <momemta/ParameterSet.h> 
#include <momemta/SLHAReader.h> 

namespace hh_bbtautau_ME_SMEFT_FF_2 
{

//==========================================================================
// Class member functions for calculating the matrix elements for
// Process: g g > h h WEIGHTED<=6 @1
// *   Decay: h > b b~ WEIGHTED<=2
// *   Decay: h > ta+ ta- WEIGHTED<=2

//--------------------------------------------------------------------------

// Initialize process.

P1_Sigma_SMEFT_FF_2_gg_bbxtaptam::P1_Sigma_SMEFT_FF_2_gg_bbxtaptam(const
    ParameterSet& configuration)
{

  std::string param_card = configuration.get < std::string > ("card"); 
  params.reset(new Parameters_SMEFT_FF_2(SLHA::Reader(param_card))); 

  // Set external particle masses for this matrix element
  mME.push_back(std::ref(params->ZERO)); 
  mME.push_back(std::ref(params->ZERO)); 
  mME.push_back(std::ref(params->mdl_MB)); 
  mME.push_back(std::ref(params->mdl_MB)); 
  mME.push_back(std::ref(params->mdl_MTA)); 
  mME.push_back(std::ref(params->mdl_MTA)); 

  mapFinalStates[{5, -5, -15, 15}] = 
  {
    {
      &P1_Sigma_SMEFT_FF_2_gg_bbxtaptam::matrix_1_gg_hh_h_bbx_h_taptam, 
      false, 
      {
        std::make_pair(21, 21)
      }, 
      64, 
      256
    }
  }; 

}

void P1_Sigma_SMEFT_FF_2_gg_bbxtaptam::resetHelicities() 
{
  for (auto& finalState: mapFinalStates)
  {
    for (auto& subProcess: finalState.second)
    {
      subProcess.resetHelicities(); 
    }
  }
}


//--------------------------------------------------------------------------
// Evaluate |M|^2, return a map of final states

std::map < std::pair < int, int > , double >
    P1_Sigma_SMEFT_FF_2_gg_bbxtaptam::compute(const std::pair <
    std::vector<double> , std::vector<double> > &initialMomenta, const
    std::vector < std::pair < int, std::vector<double> > > &finalState)
{

  // Set initial particle momenta
  momenta[0] = (double * ) (&initialMomenta.first[0]); 
  momenta[1] = (double * ) (&initialMomenta.second[0]); 
  std::cout << "initial momenta:" << *momenta[0] << " " << *momenta[1] << "\n";
  // Suppose final particles are passed in the "correct" order
  std::vector<int> selectedFinalState(6 - 2); 
  for (size_t index = 0; index < (6 - 2); index++ )
  {
    selectedFinalState[index] = finalState[index].first; 
    momenta[index + 2] = (double * ) (&finalState[index].second[0]); 
  }

  // Set the event specific parameters
  params->updateParameters(); 
  params->updateCouplings(); 

  // Initialise result object
  std::map < std::pair < int, int > , double > result; 

  // Define permutation
  int perm[6]; 
  for(int i = 0; i < 6; i++ )
  {
    perm[i] = i; 
  }

  for(auto &me: mapFinalStates[selectedFinalState])
  {

    double me_sum = 0; 
    double me_mirror_sum = 0; 

    for(int ihel = 0; ihel < 64; ihel++ )
    {

      if(me.goodHel[ihel])
      {

        double sum = 0.; 
        calculate_wavefunctions(perm, helicities[ihel]); 
        double meTemp = me.callback( * this); 
        sum += meTemp; 
        me_sum += meTemp/me.denominator; 

        if(me.hasMirrorProcess)
        {
          perm[0] = 1; 
          perm[1] = 0; 
          // Calculate wavefunctions
          calculate_wavefunctions(perm, helicities[ihel]); 
          // Mirror back
          perm[0] = 0; 
          perm[1] = 1; 
          meTemp = me.callback( * this); 
          sum += meTemp; 
          me_mirror_sum += meTemp/me.denominator; 
        }

        if( !sum)
          me.goodHel[ihel] = false; 
      }
    }

    for (auto const &initialState: me.initialStates)
    {
      result[initialState] = me_sum; 
      if (me.hasMirrorProcess)
        result[std::make_pair(initialState.second, initialState.first)] =
            me_mirror_sum;
    }
  }


  return result; 
}

//==========================================================================
// Private class member functions

//--------------------------------------------------------------------------
// Evaluate |M|^2 for each subprocess

void P1_Sigma_SMEFT_FF_2_gg_bbxtaptam::calculate_wavefunctions(const int
    perm[], const int hel[])
{
  std::complex<double> test[2];
  std::cout << "Test: " << test[0] << " " << test[1] << "\n";
  // Calculate wavefunctions for all processes
  static std::complex<double> w[9][18]; 
  std::cout << "\nBeginning wave function calculation\n";
  std::cout << "pre-vxxxxx:" << momenta[perm[0]][0] << " " << mME[0] << " " << hel[0] << " " << *w[0] << "\n";
  // Calculate all wavefunctions
  vxxxxx(&momenta[perm[0]][0], mME[0], hel[0], -1, w[0]); 
  std::cout << "post-vxxxxx:" << momenta[perm[0]][0] << " " << mME[0] << " " << hel[0] << " " << *w[0] << "\n";
  std::cout << "pre-vxxxxx:" << momenta[perm[1]][0] << " " << mME[1] << " " << hel[1] << " " << *w[1] << "\n";
  vxxxxx(&momenta[perm[1]][0], mME[1], hel[1], -1, w[1]); 
  std::cout << "post-vxxxxx:" << momenta[perm[1]][0] << " " << mME[1] << " " << hel[1] << " " << *w[1] << "\n";
  std::cout << "pre-oxxxxx:" << momenta[perm[2]][0] << " " << mME[2] << " " << hel[2] << " " << *w[2] << "\n";
  oxxxxx(&momenta[perm[2]][0], mME[2], hel[2], +1, w[2]); 
  std::cout << "post-oxxxxx:" << momenta[perm[2]][0] << " " << mME[2] << " " << hel[2] << " " << *w[2] << "\n";
  std::cout << "pre-ixxxxx:" << momenta[perm[3]][0] << " " << mME[3] << " " << hel[3] << " " << *w[3] << "\n";
  ixxxxx(&momenta[perm[3]][0], mME[3], hel[3], -1, w[3]); 
  std::cout << "post-vxxxxx:" << momenta[perm[3]][0] << " " << mME[3] << " " << hel[3] << " " << *w[3] << "\n";
  std::cout << "pre-FFS1_3:" << *w[3] << " " << *w[2] << " " << params->GC_31 << " " << params->mdl_MH << " " << params->mdl_WH << " " << *w[4] << "\n";
  FFS1_3(w[3], w[2], params->GC_31, params->mdl_MH, params->mdl_WH, w[4]); 
  std::cout << "post-FFS1_3:" << *w[3] << " " << *w[2] << " " << params->GC_31 << " " << params->mdl_MH << " " << params->mdl_WH << " " << *w[4] << "\n";
  std::cout << "pre-ixxxxx:" << momenta[perm[4]][0] << " " << mME[4] << " " << hel[4] << " " << *w[5] << "\n";
  ixxxxx(&momenta[perm[4]][0], mME[4], hel[4], -1, w[5]); 
  std::cout << "post-ixxxxx:" << momenta[perm[4]][0] << " " << mME[4] << " " << hel[4] << " " << *w[5] << "\n";
  std::cout << "pre-oxxxxx:" << momenta[perm[5]][0] << " " << mME[5] << " " << hel[5] << " " << *w[6] << "\n";
  oxxxxx(&momenta[perm[5]][0], mME[5], hel[5], +1, w[6]);
  std::cout << "post-oxxxxx:" << momenta[perm[5]][0] << " " << mME[5] << " " << hel[5] << " " << *w[6] << "\n";
  std::cout << "pre-FFS1_3:" << *w[5] << " " << *w[6] << " " << params->GC_33 << " " << params->mdl_MH << " " << params->mdl_WH << " " << *w[7] << "\n";
  FFS1_3(w[5], w[6], params->GC_33, params->mdl_MH, params->mdl_WH, w[7]); 
  std::cout << "post-FFS1_3:" << *w[5] << " " << *w[6] << " " << params->GC_33 << " " << params->mdl_MH << " " << params->mdl_WH << " " << *w[7] << "\n";
  std::cout << "pre-vvs2_3:" << *w[0] << " " << *w[1] << " " << params->GC_25 << " " << params->mdl_MH << " " << params->mdl_WH << " " << *w[8] << "\n";
  vvs2_3(w[0], w[1], &(params->GC_25), &(params->mdl_MH), &(params->mdl_WH), w[8]); 
  std::cout << "post-vvs2_3:" << *w[0] << " " << *w[1] << " " << params->GC_25 << " " << params->mdl_MH << " " << params->mdl_WH << " " << *w[8] << "\n";

  // Calculate all amplitudes
  // Amplitude(s) for diagram number 0
  std::cout << "pre-vvss2_3_0:" << *w[0] << " " << *w[1] << " " << *w[4] << " " << *w[7] << " " << params->GC_22 << " " << params->GC_34 << " " << amp[0] << "\n";
  vvss2_3_0(w[0], w[1], w[4], w[7], &(params->GC_22), &(params->GC_34), &(amp[0])); 
  std::cout << "post-vvss2_3_0:" << *w[0] << " " << *w[1] << " " << *w[4] << " " << *w[7] << " " << params->GC_22 << " " << params->GC_34 << " " << amp[0] << "\n";
  std::cout << "pre-SSS1_0:" << *w[4] << " " << *w[7] << " " << *w[8] << " " << params->GC_28 << " " << amp[1] << "\n";
  SSS1_0(w[4], w[7], w[8], params->GC_28, amp[1]); 
  std::cout << "post-SSS1_0:" << *w[4] << " " << *w[7] << " " << *w[8] << " " << params->GC_28 << " " << amp[1] << "\n";
  std::cout << "\nEnding wave function calculation\n";
}
double P1_Sigma_SMEFT_FF_2_gg_bbxtaptam::matrix_1_gg_hh_h_bbx_h_taptam() 
{

  static std::complex<double> ztemp; 
  static std::complex<double> jamp[1]; 
  // The color matrix
  static const double denom[1] = {1}; 
  static const double cf[1][1] = {{6}}; 

  // Calculate color flows
  jamp[0] = +2. * (-amp[0] - amp[1]); 

  // Sum and square the color flows to get the matrix element
  double matrix = 0; 
  for(int i = 0; i < 1; i++ )
  {
    ztemp = 0.; 
    for(int j = 0; j < 1; j++ )
      ztemp = ztemp + cf[i][j] * jamp[j]; 
    matrix = matrix + real(ztemp * conj(jamp[i]))/denom[i]; 
  }

  return matrix; 
}



}

// Register matrix element with MoMEMta
#include <momemta/MatrixElementFactory.h> 
REGISTER_MATRIX_ELEMENT("hh_bbtautau_ME_SMEFT_FF_2_P1_Sigma_SMEFT_FF_2_gg_bbxtaptam", hh_bbtautau_ME_SMEFT_FF_2::P1_Sigma_SMEFT_FF_2_gg_bbxtaptam); 

