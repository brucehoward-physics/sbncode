#ifndef __sbnanalysis_core_Event__
#define __sbnanalysis_core_Event__

/**
 * \file Event.hh
 *
 * The standard minimum output tree.
 *
 * This event structure is written out by every Processor subclass.
 *
 * Author: A. Mastbaum <mastbaum@uchicago.edu>, 2018/01/25
 */

#include <map>
#include <string>
#include <vector>
#include <TTree.h>
#include <TVector3.h>
#include "Experiment.hh"
#include <canvas/Persistency/Common/Wrapper.h>

/** Identifier for known experiments. */
typedef enum {
  kExpSBND,
  kExpMicroBooNE,
  kExpICARUS,
  kExpDUNEND,
  kExpDUNEFD,
  kExpLArIAT,
  kExpOther = 1000
} Experiment;

/**
 * \class Event
 * \brief The standard event data definition.
 */
class Event {
public:
  /**
   * \class Event::Metadata
   * \brief Event-level information
   */
  class Metadata {
  public:
    /** Constructor. */
    Metadata() : run(kUnfilled), subrun(kUnfilled), eventID(kUnfilled) {}

    /** Reset members to defaults. */
    void Init() {
      run = kUnfilled;
      subrun = kUnfilled;
      eventID = kUnfilled;
    }

    int run;  //!< Run ID
    int subrun;  //!< Subrun ID
    int eventID;  //!< Event ID
  };

  /**
   * \class Event::Neutrino
   * \brief Neutrino interaction information
   */
  class Neutrino {
  public:
    /** Constructor. */
    Neutrino()
      : isnc(false), iscc(false), pdg(0), targetPDG(0), genie_intcode(0),
        bjorkenX(kUnfilled), inelasticityY(kUnfilled), Q2(kUnfilled),
        q0(kUnfilled), modq(kUnfilled), q0_lab(kUnfilled), modq_lab(kUnfilled),
        w(kUnfilled), t(kUnfilled), energy(kUnfilled),
        momentum(kUnfilled, kUnfilled, kUnfilled), parentPDG(0),
        parentDecayMode(0), parentDecayVtx(kUnfilled, kUnfilled, kUnfilled) {}

    bool isnc;                //!< same as LArSoft "ccnc" - 0=CC, 1=NC
    bool iscc;                //!< CC (true) or NC/interference (false)
    int pdg;                  //!< PDG code of probe neutrino
    int targetPDG;            //!< PDG code of struck target
    int genie_intcode;        //!< Interaction mode (as for LArSoft MCNeutrino::Mode() )
    double bjorkenX;          //!< Bjorken x
    double inelasticityY;     //!< Inelasticity y
    double Q2;                //!< Q squared
    double q0;                //!< q0, struck nucleon rest frame
    double modq;              //!< |q|, struck nucleon rest frame
    double q0_lab;            //!< q0, lab frame
    double modq_lab;          //!< |q|, lab frame
    double w;                 //!< Hadronic invariant mass W
    double t;                 //!< Kinematic t
    double eccqe;             //!< CCQE energy
    double energy;            //!< Neutrino energy (GeV)
    TVector3 momentum;        //!< Neutrino three-momentum
    TVector3 position;        //!< Neutrino interaction position
    int parentPDG;            //!< Parent hadron/muon PDG
    int parentDecayMode;      //!< Parent hadron/muon decay mode
    TVector3 parentDecayVtx;  //!< Parent hadron/muon decay vertex
  };

  /**
   * \class Event::FinalStateParticle
   * \brief Final state particle information
   */
  class FinalStateParticle {
    public:
      /** Default Constructor. */
      FinalStateParticle()
        : mc_id(kUnfilled), pdg(kUnfilled), energy(kUnfilled), trans_mom(kUnfilled),
        momentum(kUnfilled, kUnfilled, kUnfilled),
        vertex(kUnfilled, kUnfilled, kUnfilled),
        end(kUnfilled, kUnfilled, kUnfilled){}

      int mc_id;         //|< MC ID, for MC particles it corresponds to itself, for reco particles it corresponds to the truth-matched MC particle 
      int pdg;           //!< PDG Code
      double energy;     //!< Energy
      double trans_mom;  //!< Transverse momentum
      TVector3 momentum; //!< Three-momentum
      TVector3 vertex;   //!< Vertex position of the particle
      TVector3 end;      //!< End position of the particle

  };

  /** 
   * \class  Event::FinalStateReconstructedParticle
   * \brief  Reconstructed final state particle, inherits from FinalStateParticle as it will share most variables
   */
  class FinalStateReconstructedParticle : public FinalStateParticle {
    public:
      /** Default Constructor **/
      FinalStateReconstructedParticle()
        : hits(kUnfilled), mc_id_hits(kUnfilled), mc_id_energy(kUnfilled), mc_id_charge(kUnfilled), 
        mcs_momentum_muon(kUnfilled), 
        range_momentum_muon(kUnfilled), range_momentum_proton(kUnfilled),
        range_momentum_pion(kUnfilled),
        chi2_muon(kUnfilled), chi2_proton(kUnfilled), chi2_pion(kUnfilled),
        pida(kUnfilled) {}

      int hits;                     //!< Number of hits associated with the particle 
      int mc_id_hits;               //!< Associated MCParticle ID from hits
      int mc_id_energy;             //!< Associated MCParticle ID from energy
      int mc_id_charge;             //!< Associated MCParticle ID from charge
      double mcs_momentum_muon;     //!< multiple coulomb scattering momentum is the particle  is an escaping muon
      double range_momentum_muon;   //!< range momentum if the particle is a contained muon 
      double range_momentum_proton; //!< range momentum if the particle is a contained proton
      double range_momentum_pion;   //!< range momentum if the particle is a contained pion
      double chi2_proton;           //!< chi2 under the proton hypothesis          
      double chi2_muon;             //!< chi2 under the muon hypothesis            
      double chi2_pion;             //!< chi2 under the pion hypothesis            
      double pida;                  //!< PIDA value of the particle
  };

  /**
   * \class Event::Interaction
   * \brief All truth information associated with one neutrino interaction
   */
  class Interaction {
    public:
      Neutrino neutrino;  //!< The neutrino
      FinalStateParticle lepton;  //!< The primary final state lepton

      /** The other final state particles. */
      std::vector<FinalStateParticle> finalstate; //!< Final state particles

      /**
       * Event weights.
       *
       * This is a map from the weight calculator name to the list of weights
       * for all the sampled universes.
       */
      std::map<std::string, std::vector<double> > weights;
  };

  /**
   * \class RecoInteraction
   * \brief Contains truth level information and additional fields for
   * selection-defined reconstruction information
   */
  class RecoInteraction {
    public:
      /** Default Constructor */
      RecoInteraction(): 
        truth_index(-1), 
        reco_energy(kUnfilled),
        weight(1.) {}

      /** Fill in truth information -- other fields set as in default */
      explicit RecoInteraction(const Interaction &t, int index): 
        truth(t), 
        truth_index(index),
        reco_energy(kUnfilled),
        weight(1.) {}

      Interaction truth; //!< Contains truth level information about interaction

      /** The reconstructed final state particle **/
      std::vector<FinalStateReconstructedParticle> recofinalstate; //!< Reconstructed final state particles

      /**
       * Index into the vector of truth interaction objects in the Event
       * (same as the index into MCTruth objects). Equal to -1 if there is
       * no corresponding truth interaction.
       */
      int truth_index;

      /**
       * Selection defined reconstructed energy of neutrino. Units in GeV to keep
       * consistent w/ Interaction class. */
      double reco_energy;

      /**
       * Selection defined weight of reconstructed interaction to be used by downstream
       * analyis. */
      double weight;  
  };

  Metadata metadata;  //!< Event metadata
  std::vector<Interaction> truth; //!< All truth interactions
  std::vector<RecoInteraction> reco; //!< Reconstructed interactions

  Experiment experiment;  //!< Experiment identifier
  static const int kUnfilled = -99999;  //!< Value for unfilled variables
};

#endif  // __sbnanalysis_core_Event__

