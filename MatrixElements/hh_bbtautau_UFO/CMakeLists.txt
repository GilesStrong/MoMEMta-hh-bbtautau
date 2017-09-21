project(hh_bbtautau_UFO C CXX Fortran)

set(CMAKE_Fortran_FLAGS "${CMAKE_Fortran_FLAGS} -fno-underscoring -fPIC -ffixed-line-length-none")

set(SOURCES
    "src/HelAmps_SMEFT_UFO.cc"
    "src/Parameters_SMEFT_UFO.cc"
    "SubProcesses/P1_Sigma_SMEFT_UFO_gg_bbxtaptam/P1_Sigma_SMEFT_UFO_gg_bbxtaptam.cc"
    )

add_library(me_hh_bbtautau_UFO OBJECT ${SOURCES}) 

target_include_directories(me_hh_bbtautau_UFO PRIVATE "include")
target_include_directories(me_hh_bbtautau_UFO PRIVATE "SubProcesses/P1_Sigma_SMEFT_UFO_gg_bbxtaptam/")
