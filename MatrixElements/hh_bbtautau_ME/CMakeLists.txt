project(hh_bbtautau_ME C CXX Fortran)

set(CMAKE_Fortran_FLAGS "${CMAKE_Fortran_FLAGS} -fno-underscoring -fPIC -ffree-line-length-none")

set(SOURCES
    "src/VVSS3_0.f"
    "src/VVSS2_0.f"
    "src/VVS2_3.f"
    "src/HelAmps_SMEFT_FF_2.cc"
    "src/Parameters_SMEFT_FF_2.cc"
    "SubProcesses/P1_Sigma_SMEFT_FF_2_gg_bbxtaptam/P1_Sigma_SMEFT_FF_2_gg_bbxtaptam.cc"
    )

add_library(me_hh_bbtautau_ME OBJECT ${SOURCES}) 

target_include_directories(me_hh_bbtautau_ME PRIVATE "include")
target_include_directories(me_hh_bbtautau_ME PRIVATE "SubProcesses/P1_Sigma_SMEFT_FF_2_gg_bbxtaptam/")
