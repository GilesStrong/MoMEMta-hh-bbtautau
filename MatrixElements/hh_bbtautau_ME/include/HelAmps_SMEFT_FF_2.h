//==========================================================================
// This file has been automatically generated for C++ Standalone
// MadGraph5_aMC@NLO v. 2.6.0, 2017-08-16
// By the MadGraph5_aMC@NLO Development Team
// Visit launchpad.net/madgraph5 and amcatnlo.web.cern.ch
//==========================================================================

#ifndef HelAmps_SMEFT_FF_2_H
#define HelAmps_SMEFT_FF_2_H

#include <cmath> 
#include <complex> 

using namespace std;

extern "C"
{
    void vvss2_3_0(complex<double> v1[], complex<double> v2[], complex<double> s3[], complex<double> s4[], complex<double> *coup1, complex<double> *coup2, complex<double>* vertex);
      void vvs2_3(complex<double> v1[], complex<double> v2[], complex<double> *coup, double *m3, double *w3, complex<double> s3[]);
  } 

namespace hh_bbtautau_ME_SMEFT_FF_2 
{
void sxxxxx(double p[4], int nss, std::complex<double> sc[3]); 

void vxxxxx(double p[4], double vmass, int nhel, int nsv, std::complex<double>
    v[6]);

void ixxxxx(double p[4], double fmass, int nhel, int nsf, std::complex<double>
    fi[6]);

void oxxxxx(double p[4], double fmass, int nhel, int nsf, std::complex<double>
    fo[6]);

void txxxxx(double p[4], double tmass, int nhel, int nst, std::complex<double>
    fi[18]);

double Sgn(double e, double f); 

void SSS1_0(std::complex<double> S1[], std::complex<double> S2[],
    std::complex<double> S3[], std::complex<double> COUP, std::complex<double>
    & vertex);

void FFS1_3(std::complex<double> F1[], std::complex<double> F2[],
    std::complex<double> COUP, double M3, double W3, std::complex<double> S3[]);

}  // end namespace hh_bbtautau_ME_SMEFT_FF_2

#endif  // HelAmps_SMEFT_FF_2_H
