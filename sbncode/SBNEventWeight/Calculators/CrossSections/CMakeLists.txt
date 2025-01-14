# Headers needed for the GenieWeightCalc class
include_directories ( $ENV{GENIE_INC} )
include_directories ( $ENV{GENIE_INC}/GENIE )
include_directories ( $ENV{LOG4CPP_INC} )

# Builds of MicroBooNE-specific larsim releases should uncomment the
# "add_definitions" line below. It enables extra reweighting functionality
# which is present only in "MicroBooNE patch" releases of GENIE v3.0.4. These
# require building against a special ups product.

# Define a macro that indicates whether a GENIE version before 3.0.0 is in use
 parse_ups_version( ${GENIE_VERSION} )
 if( ${VMAJ} LESS 3 )
   add_definitions(-DGENIE_PRE_R3)
   endif()

# not applying the following, YET
#add_definitions(-DGENIE_UB_PATCH)

art_make(
  LIBRARY_NAME sbncode_SBNEventWeight_Calculators_CrossSection
  LIB_LIBRARIES
    sbncode_SBNEventWeight_Base
    nugen::EventGeneratorBase_GENIE
    nurandom::RandomUtils_NuRandomService_service
    art::Framework_Principal
    art::Persistency_Provenance
    art::Framework_Services_Registry
    fhiclcpp::fhiclcpp

   #GENIE_LIB_LIST is explicitly listed below:
    log4cpp::log4cpp
    GENIE::GFwMsg
    GENIE::GFwReg
    GENIE::GFwAlg
    GENIE::GFwInt
    GENIE::GFwGHEP
    GENIE::GFwNum
    GENIE::GFwUtl
    GENIE::GFwParDat
    GENIE::GFwEG
    GENIE::GFwNtp
    GENIE::GPhXSIg
    GENIE::GPhPDF
    GENIE::GPhNuclSt
    GENIE::GPhCmn
    GENIE::GPhDcy
    GENIE::GPhHadTransp
    GENIE::GPhHadnz
    GENIE::GPhDeEx
    GENIE::GPhAMNGXS
    GENIE::GPhAMNGEG
    GENIE::GPhChmXS
    GENIE::GPhCohXS
    GENIE::GPhCohEG
    GENIE::GPhDISXS
    GENIE::GPhDISEG
    GENIE::GPhDfrcXS
    GENIE::GPhDfrcEG
    GENIE::GPhGlwResXS
    GENIE::GPhGlwResEG
    GENIE::GPhIBDXS
    GENIE::GPhIBDEG
    GENIE::GPhMNucXS
    GENIE::GPhMNucEG
    GENIE::GPhMEL
    GENIE::GPhNuElXS
    GENIE::GPhNuElEG
    GENIE::GPhQELXS
    GENIE::GPhQELEG
    GENIE::GPhResXS
    GENIE::GPhResEG
    GENIE::GPhStrXS
    GENIE::GPhStrEG
    GENIE::GPhNDcy
    GENIE::GTlGeo
    GENIE::GTlFlx
    GENIE::GRwFwk
    GENIE::GRwIO
    GENIE::GRwClc
	GSL
    #GENIE libraries are above
    messagefacility::MF_MessageLogger
    cetlib_except::cetlib_except
)

install_headers()
install_fhicl()
install_source()

