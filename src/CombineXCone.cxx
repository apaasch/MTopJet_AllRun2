#include "UHH2/MTopJet/include/CombineXCone.h"

// Get Primary Lepton
Particle CombineXCone::GetLepton(uhh2::Event & event){

  Particle lepton;
  bool muonislep = false;
  bool elecislep = false;

  if(event.muons->size() > 0 && event.electrons->size() > 0){
    if(event.muons->at(0).pt() > event.electrons->at(0).pt()) muonislep = true;
    else elecislep = true;
  }
  else if(event.muons->size() > 0) muonislep = true;
  else if(event.electrons->size() > 0) elecislep = true;

  if(muonislep) lepton = event.muons->at(0);
  if(elecislep) lepton = event.electrons->at(0);

  return lepton;
}

GenParticle CombineXCone::GetLepton_gen(uhh2::Event & event){

  GenParticle lepton;
  std::vector<GenParticle>* genparts = event.genparticles;

  for (unsigned int i=0; i<(genparts->size()); ++i){
    GenParticle p = genparts->at(i);
    if(abs(p.pdgId()) == 13) lepton = p;
    else if(abs(p.pdgId()) == 11) lepton = p;
  }
  return lepton;
}


// Combine Subjets to jet (do not use subjet if pt < ptmin)
Jet CombineXCone::AddSubjets(vector<Jet> subjets, double ptmin){
  double px=0, py=0, pz=0, E=0;
  TLorentzVector jet_v4;
  Jet jet;
  for(unsigned int i=0; i < subjets.size(); ++i){
    if(subjets.at(i).pt() < ptmin) continue;
    px += subjets.at(i).v4().Px();
    py += subjets.at(i).v4().Py();
    pz += subjets.at(i).v4().Pz();
    E += subjets.at(i).v4().E();
  }
  jet_v4.SetPxPyPzE(px, py, pz, E);
  jet.set_pt(jet_v4.Pt());
  jet.set_eta(jet_v4.Eta());
  jet.set_phi(jet_v4.Phi());
  jet.set_energy(jet_v4.E());

  return jet;
}

Particle CombineXCone::AddSubjets_gen(vector<Particle> subjets, double ptmin){
  double px=0, py=0, pz=0, E=0;
  TLorentzVector jet_v4;
  Particle jet;
  for(unsigned int i=0; i < subjets.size(); ++i){
    if(subjets.at(i).pt() < ptmin) continue;
    px += subjets.at(i).v4().Px();
    py += subjets.at(i).v4().Py();
    pz += subjets.at(i).v4().Pz();
    E += subjets.at(i).v4().E();
  }
  jet_v4.SetPxPyPzE(px, py, pz, E);
  jet.set_pt(jet_v4.Pt());
  jet.set_eta(jet_v4.Eta());
  jet.set_phi(jet_v4.Phi());
  jet.set_energy(jet_v4.E());

  return jet;
}



// Get final Jets from 3+3 Method on Reco level
CombineXCone33::CombineXCone33(uhh2::Context & ctx, const std::string & name_had, const std::string & name_lep):
  h_xcone33hadjets(ctx.declare_event_output<std::vector<Jet>>(name_had)),
  h_xcone33lepjets(ctx.declare_event_output<std::vector<Jet>>(name_lep)),
  h_fatjets(ctx.get_handle<std::vector<TopJet>>("XConeTopJets")) {}

bool CombineXCone33::process(uhh2::Event & event){
  //---------------------------------------------------------------------------------------
  //--------------------------------- get subjets and lepton ------------------------------
  //---------------------------------------------------------------------------------------
  std::vector<TopJet> jets = event.get(h_fatjets);
  std::auto_ptr<CombineXCone> combine(new CombineXCone);
  Particle lepton = combine->GetLepton(event);

  //---------------------------------------------------------------------------------------
  //------------- define had and lep jet (deltaR) -----------------------------------------
  //---------------------------------------------------------------------------------------
  TopJet fathadjet, fatlepjet;
  float dR1 = deltaR(lepton, jets.at(0));
  float dR2 = deltaR(lepton, jets.at(1));
  if(dR1 < dR2){
    fatlepjet = jets.at(0);
    fathadjet = jets.at(1);
  }
  else{
    fatlepjet = jets.at(1);
    fathadjet = jets.at(0);
  }

  //---------------------------------------------------------------------------------------
  //-------- set Lorentz Vectors of subjets and combine them ------------------------------
  //---------------------------------------------------------------------------------------
  std::vector<Jet> subjets_lep = fatlepjet.subjets();
  std::vector<Jet> subjets_had = fathadjet.subjets();
  Jet lepjet = combine->AddSubjets(subjets_lep, 0);
  Jet hadjet = combine->AddSubjets(subjets_had, 0);
  vector<Jet> hadjets;
  vector<Jet> lepjets;
  hadjets.push_back(hadjet);
  lepjets.push_back(lepjet);

  //---------------------------------------------------------------------------------------
  //--------------------------------- Write Jets ------------------------------------------
  //---------------------------------------------------------------------------------------
  event.set(h_xcone33hadjets, hadjets);
  event.set(h_xcone33lepjets, lepjets);
  
  return true;
}

// Get final Jets from 3+3 Method on Gen level
CombineXCone33_gen::CombineXCone33_gen(uhh2::Context & ctx):
  h_GENxcone33hadjets(ctx.declare_event_output<std::vector<Particle>>("GEN_XCone33_had_Combined")),
  h_GENxcone33lepjets(ctx.declare_event_output<std::vector<Particle>>("GEN_XCone33_lep_Combined")),
  h_GENfatjets(ctx.get_handle<std::vector<GenTopJet>>("genXCone33TopJets")) {}

bool CombineXCone33_gen::process(uhh2::Event & event){
  //---------------------------------------------------------------------------------------
  //--------------------------------- get subjets and lepton ------------------------------
  //---------------------------------------------------------------------------------------
  std::vector<GenTopJet> jets = event.get(h_GENfatjets);
  std::auto_ptr<CombineXCone> combine(new CombineXCone);
  GenParticle lepton = combine->GetLepton_gen(event);

  //---------------------------------------------------------------------------------------
  //------------- define had and lep jet (deltaR) -----------------------------------------
  //---------------------------------------------------------------------------------------
  GenTopJet fathadjet, fatlepjet;
  float dR1 = deltaR(lepton, jets.at(0));
  float dR2 = deltaR(lepton, jets.at(1));
  if(dR1 < dR2){
    fatlepjet = jets.at(0);
    fathadjet = jets.at(1);
  }
  else{
    fatlepjet = jets.at(1);
    fathadjet = jets.at(0);
  }

  //---------------------------------------------------------------------------------------
  //-------- set Lorentz Vectors of subjets and combine them ------------------------------
  //---------------------------------------------------------------------------------------
  std::vector<Particle> subjets_lep = fatlepjet.subjets();
  std::vector<Particle> subjets_had = fathadjet.subjets();
  Particle lepjet = combine->AddSubjets_gen(subjets_lep, 0);
  Particle hadjet = combine->AddSubjets_gen(subjets_had, 0);
  vector<Particle> hadjets;
  vector<Particle> lepjets;
  hadjets.push_back(hadjet);
  lepjets.push_back(lepjet);

  //---------------------------------------------------------------------------------------
  //--------------------------------- Write Jets ------------------------------------------
  //---------------------------------------------------------------------------------------
  event.set(h_GENxcone33hadjets, hadjets);
  event.set(h_GENxcone33lepjets, lepjets);

  // delete combine;
  return true;
}

// Get final Jets from 2+3 Method on Gen level
CombineXCone23_gen::CombineXCone23_gen(uhh2::Context & ctx):
  h_GENxcone23hadjets(ctx.declare_event_output<std::vector<Particle>>("GEN_XCone23_had_Combined")),
  h_GENxcone23lepjets(ctx.declare_event_output<std::vector<Particle>>("GEN_XCone23_lep_Combined")),
  h_GEN23fatjets(ctx.get_handle<std::vector<GenTopJet>>("genXCone23TopJets")) {}


bool CombineXCone23_gen::process(uhh2::Event & event){
  //---------------------------------------------------------------------------------------
  //--------------------------------- get subjets and lepton ------------------------------
  //---------------------------------------------------------------------------------------
  std::vector<GenTopJet> jets23 = event.get(h_GEN23fatjets);
  std::auto_ptr<CombineXCone> combine(new CombineXCone);
  GenParticle lepton = combine->GetLepton_gen(event);

  //---------------------------------------------------------------------------------------
  //------------- define had and lep jet (deltaR) -----------------------------------------
  //---------------------------------------------------------------------------------------
  GenTopJet fathadjet, fatlepjet;
  float dR1 = deltaR(lepton, jets23.at(0));
  float dR2 = deltaR(lepton, jets23.at(1));
  if(dR1 < dR2){
    fatlepjet = jets23.at(0);
    fathadjet = jets23.at(1);
  }
  else{
    fatlepjet = jets23.at(1);
    fathadjet = jets23.at(0);
  }

  //---------------------------------------------------------------------------------------
  //-------- set Lorentz Vectors of subjets and combine them ------------------------------
  //---------------------------------------------------------------------------------------
  std::vector<Particle> subjets_lep = fatlepjet.subjets();
  std::vector<Particle> subjets_had = fathadjet.subjets();
  Particle lepjet = combine->AddSubjets_gen(subjets_lep, 0);
  Particle hadjet = combine->AddSubjets_gen(subjets_had, 0);
  vector<Particle> hadjets;
  vector<Particle> lepjets;
  hadjets.push_back(hadjet);
  lepjets.push_back(lepjet);

  //---------------------------------------------------------------------------------------
  //--------------------------------- Write Jets ------------------------------------------
  //---------------------------------------------------------------------------------------
  event.set(h_GENxcone23hadjets, hadjets);
  event.set(h_GENxcone23lepjets, lepjets);

  return true;
}

CopyJets::CopyJets(uhh2::Context & ctx, const std::string & old_name, const std::string & new_name):
  h_new(ctx.declare_event_output<std::vector<TopJet>>(new_name)),
  h_old(ctx.get_handle<std::vector<TopJet>>(old_name)) {}


bool CopyJets::process(uhh2::Event & event){
  std::vector<TopJet> jets = event.get(h_old);
  event.set(h_new, jets);

return true;
}