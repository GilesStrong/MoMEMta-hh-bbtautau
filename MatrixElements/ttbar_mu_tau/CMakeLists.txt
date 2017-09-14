project(ttbar_mu_tau CXX)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-unused-variable -std=c++11 -O3 -fcx-fortran-rules -fcx-limited-range")

set(SOURCES
    "src/HelAmps_sm.cc"
    "src/Parameters_sm.cc"
    "SubProcesses/P1_Sigma_sm_gg_tapvtbmumvmxbx/P1_Sigma_sm_gg_tapvtbmumvmxbx.cc"
    )

add_library(me_ttbar_mu_tau OBJECT ${SOURCES}) 

target_include_directories(me_ttbar_mu_tau PRIVATE "include")
target_include_directories(me_ttbar_mu_tau PRIVATE "SubProcesses/P1_Sigma_sm_gg_tapvtbmumvmxbx/")
