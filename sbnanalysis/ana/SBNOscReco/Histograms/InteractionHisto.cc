#include "InteractionHisto.h"
#include "Derived.h"
#include "../TriggerEmulator/PMTTrigger.h"

#include "TH1D.h"
#include "TH2D.h"
#include "TProfile.h"

namespace ana {
  namespace SBNOsc {

static const std::vector<std::string> FINALSTATE {"p0 #pi0", "p1 #pi0", "p2 #pi0", "p3+ #pi0", "pn #pi1", "pn #pi2+"};

const char *FinalStateStr(unsigned n_proton, unsigned n_pion) {
  switch (n_pion) {
    case 0:
      switch (n_proton) { 
        case 0: return FINALSTATE[0].c_str();
        case 1: return FINALSTATE[1].c_str();
        case 2: return FINALSTATE[2].c_str();
        default: return FINALSTATE[3].c_str();
      }
    case 1: return FINALSTATE[4].c_str();
    default: return FINALSTATE[5].c_str();
  }
  return NULL; // unreachable
}

void InteractionHistos::Initialize(const std::string &postfix, const geo::BoxBoundedGeo &detector_volume, const std::vector<double> &tagger_volume) {
#define INT_HISTO(name, n_bins, lo, hi)    name = new TH1D((#name + postfix).c_str(), #name, n_bins, lo, hi); StoreHisto(name)
#define INT_HISTO2D(name, n_binsx, xlo, xhi, n_binsy, ylo, yhi) name = new TH2D((#name + postfix).c_str(), #name, n_binsx, xlo, xhi, n_binsy, ylo, yhi); StoreHisto(name)
#define INT_HISTO2D_BINSY(name, n_binsx, xlo, xhi, n_binsy, binsy) name = new TH2D((#name + postfix).c_str(), #name, n_binsx, xlo, xhi, n_binsy, binsy); StoreHisto(name)
#define INT_PROFILE(name, n_bins, lo, hi) name = new TProfile((#name + postfix).c_str(), #name, n_bins, lo, hi); StoreHisto(name)

  INT_HISTO(track_length, 100, 0., 600.);
  INT_HISTO(track_p, 50, 0., 5.);
  INT_HISTO(nuE, 50, 0., 5.);
  INT_HISTO(beam_center_distance, 60, 0., 300.);
  INT_HISTO(Q2, 50, 0., 10.);
  INT_HISTO(true_contained_length, 100, 0., 600.);
  INT_HISTO(true_track_multiplicity, 10, 0., 10);
  INT_HISTO(crosses_tpc, 2, -0.5, 1.5);
  INT_HISTO(dist_to_match, 101, -1., 100.);
  INT_HISTO(primary_track_completion, 100, 0., 1.);
  INT_HISTO(n_reco_vertices, 10, -0.5, 9.5);
  INT_HISTO(maxpe_crt_intime_hit, 1000, 0., 1000.);
  INT_HISTO(crt_pes, 1000, 0., 1000.);

  INT_HISTO(crt_hit_times, 300, -20., 10.);
  INT_HISTO(closest_crt_hit_time, 300, -20., 10.);

  INT_HISTO(fmatch_score, 200, 0., 200.);
  INT_HISTO2D(fmatch_score_true_time, 100, 0., 100., 1400, -4000., 3000.);
  INT_HISTO2D(fmatch_score_true_time_zoom, 100, 0., 100., 100, -5., 5.);
  double intout_times[4] = {-3000., 0., 1.6, 3000.};
  INT_HISTO(fmatch_score_intime, 100, 0., 100.);
  INT_HISTO(fmatch_score_outtime, 100, 0., 100.);
  INT_HISTO2D(fmatch_time_true_time_zoom, 100, -5., 5., 100, -5., 5.);
  INT_HISTO(fmatch_time_real_time, 4000, -2, 2);
  INT_HISTO(fmatch_time, 700, -5., 2.);
  INT_HISTO(nu_score, 50, 0., 1.);
  INT_PROFILE(nu_score_nu_energy, 50, 0., 5.);
  INT_PROFILE(nu_score_muon_length, 100, 0., 600.);

  // INT_HISTO2D(light_trigger, 20, 0.5, 20.5, 200, 6000, 8000);

  INT_HISTO2D(intime_crt_hits_xy, 100, tagger_volume[0], tagger_volume[3], 100, tagger_volume[1], tagger_volume[4]);
  INT_HISTO2D(intime_crt_hits_xz, 100, tagger_volume[0], tagger_volume[3], 100, tagger_volume[2], tagger_volume[5]);
  INT_HISTO2D(intime_crt_hits_yz, 100, tagger_volume[1], tagger_volume[4], 100, tagger_volume[2], tagger_volume[5]);

  INT_HISTO2D(vertex_xy, 100, detector_volume.MinX(), detector_volume.MaxX(), 100, detector_volume.MinY(), detector_volume.MaxY());
  INT_HISTO2D(vertex_yz, 100, detector_volume.MinY(), detector_volume.MaxY(), 100, detector_volume.MinZ(), detector_volume.MaxZ());
  INT_HISTO2D(vertex_xz, 100, detector_volume.MinX(), detector_volume.MaxX(), 100, detector_volume.MinZ(), detector_volume.MaxZ());

  INT_HISTO(vertex_x, 100, detector_volume.MinX(), detector_volume.MaxX());
  INT_HISTO(vertex_y, 100, detector_volume.MinY(), detector_volume.MaxY());
  INT_HISTO(vertex_z, 100, detector_volume.MinZ(), detector_volume.MaxZ());

  INT_HISTO2D(finalstate_tr, FINALSTATE.size(), 0, FINALSTATE.size(), FINALSTATE.size(), 0, FINALSTATE.size());
  for (unsigned i = 0; i < FINALSTATE.size(); i++) {
    finalstate_tr->GetXaxis()->SetBinLabel(i+1, FINALSTATE[i].c_str());
    finalstate_tr->GetYaxis()->SetBinLabel(i+1, FINALSTATE[i].c_str());
  }

#undef INT_HISTO
}

void InteractionHistos::Fill(const event::Interaction &interaction, unsigned mctruth_id, const numu::RecoEvent &event) {
  double dist = numu::dist2Match(interaction, event.reco);
  dist_to_match->Fill(dist);
  if (interaction.neutrino.iscc) {
    primary_track_completion->Fill(numu::trackMatchCompletion(interaction.lepton.G4ID, event));
  }

  vertex_xy->Fill(interaction.neutrino.position.X(), interaction.neutrino.position.Y());
  vertex_yz->Fill(interaction.neutrino.position.Y(), interaction.neutrino.position.Z());
  vertex_xz->Fill(interaction.neutrino.position.X(), interaction.neutrino.position.Z());

  vertex_x->Fill(interaction.neutrino.position.X());
  vertex_y->Fill(interaction.neutrino.position.Y());
  vertex_z->Fill(interaction.neutrino.position.Z());


  // TODO: how to fix these histograms?
  // crosses_tpc

  // only fill for muon tracks (CC interactions)
  if (abs(interaction.lepton.pdg) == 13) {
    track_p->Fill(interaction.lepton.momentum.Mag());
    true_contained_length->Fill(interaction.lepton.contained_length);
  } 

  true_track_multiplicity->Fill(interaction.nfinalstate);
  nuE->Fill(interaction.neutrino.energy);
  Q2->Fill(interaction.neutrino.Q2);

  FillEvent(event);
}

void InteractionHistos::Fill(
  const numu::RecoInteraction &vertex,
  const numu::RecoEvent &event,
  const std::vector<event::Interaction> &truth) {

  const numu::RecoTrack &primary_track = event.tracks.at(vertex.primary_track_index);

  if (vertex.slice.truth.interaction_id >= 0) {
    dist_to_match->Fill((vertex.position - truth[vertex.slice.truth.interaction_id].neutrino.position).Mag());
  }
  else {
    dist_to_match->Fill(-1);
  }

  vertex_xy->Fill(vertex.position.X(), vertex.position.Y());
  vertex_yz->Fill(vertex.position.Y(), vertex.position.Z());
  vertex_xz->Fill(vertex.position.X(), vertex.position.Z());

  vertex_x->Fill(vertex.position.X());
  vertex_y->Fill(vertex.position.Y());
  vertex_z->Fill(vertex.position.Z());

  nu_score->Fill(vertex.slice.particles.at(vertex.slice.primary_index).p_nu_score);

  if (vertex.slice.truth.interaction_id >= 0) {
    nu_score_nu_energy->Fill(truth[vertex.slice.truth.interaction_id].neutrino.energy, vertex.slice.particles.at(vertex.slice.primary_index).p_nu_score);
  }

  if (vertex.slice.truth.interaction_id >= 0) {
    const event::Interaction &interaction = truth[vertex.slice.truth.interaction_id];
    float threshold = 0.1; // 100 MeV
    unsigned n_proton = 0;
    unsigned n_pion = 0;
    for (unsigned i = 0; i < interaction.finalstate.size(); i++) {
      if (abs(interaction.finalstate[i].pdg) == 2212 && 
          interaction.finalstate[i].kinetic_energy > threshold &&
          interaction.finalstate[i].status_code == event::FinalStateParticle::kIStStableFinalState) {
        n_proton ++;
      }
      if (abs(interaction.finalstate[i].pdg) == 211 && 
          interaction.finalstate[i].kinetic_energy > threshold &&
          interaction.finalstate[i].status_code == event::FinalStateParticle::kIStStableFinalState) {
        n_pion ++;
      }
    }
    finalstate_tr->Fill(FinalStateStr(n_proton, n_pion), FinalStateStr(vertex.nproton, vertex.npion), 1.);
  }


  if (vertex.slice.flash_match.present) {
    fmatch_score->Fill(vertex.slice.flash_match.score);
    fmatch_time->Fill(vertex.slice.flash_match.time);

    assert(vertex.primary_track_index >= 0);
    int mcparticle_id = event.tracks.at(vertex.primary_track_index).truth.GetPrimaryMatchID();
    if (event.particles.count(mcparticle_id)) {
      double true_time = event.particles.at(mcparticle_id).start_time;
      fmatch_score_true_time->Fill(vertex.slice.flash_match.score, true_time);
      fmatch_score_true_time_zoom->Fill(vertex.slice.flash_match.score, true_time);
      fmatch_time_true_time_zoom->Fill(vertex.slice.flash_match.time, true_time);
      if (true_time < 0. || true_time > 1.6) {
        fmatch_score_outtime->Fill(vertex.slice.flash_match.score);
      }
      else {
        fmatch_score_intime->Fill(vertex.slice.flash_match.score);
      }

      fmatch_time_real_time->Fill(vertex.slice.flash_match.time - true_time);
    }
  }

  track_length->Fill(primary_track.length);
  nu_score_muon_length->Fill(primary_track.length, vertex.slice.particles.at(vertex.slice.primary_index).p_nu_score); 
  int mcparticle_id = primary_track.truth.GetPrimaryMatchID();
  if (event.particles.count(mcparticle_id)) {
    primary_track_completion->Fill(primary_track.truth.matches[0].energy / event.particles.at(mcparticle_id).deposited_energy); 
  
    int crosses_tpc_val = event.particles.at(mcparticle_id).crosses_tpc;
    crosses_tpc->Fill(crosses_tpc_val);
  }
   
  if (vertex.slice.truth.interaction_id >= 0) {
    int interaction_id = vertex.slice.truth.interaction_id; 

    track_p->Fill(truth[interaction_id].lepton.momentum.Mag());
    true_contained_length->Fill(truth[interaction_id].lepton.contained_length);
    true_track_multiplicity->Fill(truth[interaction_id].nfinalstate);
    nuE->Fill(truth[interaction_id].neutrino.energy);
    Q2->Fill(truth[interaction_id].neutrino.Q2);
    // get the distance from the beam center
    /*
    float beam_center_distance = sqrt( (truth[mctruth_id].neutrino.position.X() - _config.beamCenterX) * 
      (truth[mctruth_id].neutrino.position.X() - _config.beamCenterX) +
      (truth[mctruth_id].neutrino.position.Y() - _config.beamCenterY) *
      (truth[mctruth_id].neutrino.position.Y() - _config.beamCenterY));

    beam_center_distance->Fill(beam_center_distance);
    */
  }

  // fill the event-info based histograms
  FillEvent(event);
}

void InteractionHistos::FillEvent(const numu::RecoEvent &event) {
  n_reco_vertices->Fill(event.reco.size());

  double maxpe = 0.;
  double closest_time_dist = -1;
  double closest_time = 0.;
  for (const numu::CRTHit &hit: event.in_time_crt_hits) {
    intime_crt_hits_xy->Fill(hit.location.X(), hit.location.Y());
    intime_crt_hits_xz->Fill(hit.location.X(), hit.location.Z());
    intime_crt_hits_yz->Fill(hit.location.Y(), hit.location.Z());

    crt_pes->Fill(hit.pes);
    if (hit.pes > maxpe) maxpe = hit.pes;

    //if (hit.pes < 100.) continue;

    crt_hit_times->Fill(hit.time);

    if (closest_time_dist < 0. || closest_time_dist > 1e-3) {
       double this_time_dist = -1;
       if (hit.time > -0.2 && hit.time < 1.8) this_time_dist = 0.;
       else if (hit.time < 0.) this_time_dist = -hit.time -0.2;
       if (this_time_dist >= 0. && (this_time_dist < closest_time_dist || closest_time_dist < 0.)) {
         closest_time = hit.time;
         closest_time_dist = this_time_dist;
       }
    }
  }
  if (closest_time_dist >= 0.) {
    closest_crt_hit_time->Fill(closest_time);
  }
  else {
    closest_crt_hit_time->Fill(30.);
  }

  if (event.in_time_crt_hits.size() == 0) {
    maxpe_crt_intime_hit->Fill(-1);
  }
  else {
    maxpe_crt_intime_hit->Fill(maxpe);
  }
}

  } // namespace SBNOsc
} // namespace ana

