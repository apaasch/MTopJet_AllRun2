#include <iostream>
#include <memory>

#include <UHH2/core/include/AnalysisModule.h>
#include <UHH2/core/include/Event.h>
#include <UHH2/core/include/Selection.h>

#include <UHH2/common/include/CleaningModules.h>
#include <UHH2/common/include/NSelections.h>
#include <UHH2/common/include/LumiSelection.h>
#include <UHH2/common/include/TriggerSelection.h>
#include <UHH2/common/include/JetCorrections.h>
#include <UHH2/common/include/ObjectIdUtils.h>
#include <UHH2/common/include/MuonIds.h>
#include <UHH2/common/include/ElectronIds.h>
#include <UHH2/common/include/JetIds.h>
#include <UHH2/common/include/TopJetIds.h>
#include <UHH2/common/include/TTbarGen.h>
#include <UHH2/common/include/Utils.h>
#include <UHH2/common/include/AdditionalSelections.h>
#include <UHH2/common/include/TTbarGenHists.h>
#include <UHH2/common/include/ElectronHists.h>
#include <UHH2/common/include/MuonHists.h>

#include <UHH2/MTopJet/include/ModuleBASE.h>
#include <UHH2/MTopJet/include/MTopJetSelections.h>
#include <UHH2/MTopJet/include/MTopJetGenSelections.h>
#include <UHH2/MTopJet/include/MTopJetGenHists.h>
#include <UHH2/MTopJet/include/MTopJetRecoHists.h>
#include <UHH2/MTopJet/include/MTopJetRecoGenHists.h>
#include <UHH2/MTopJet/include/GenHists_topjet.h>
#include <UHH2/MTopJet/include/RecoHists_topjet.h>
#include <UHH2/MTopJet/include/RecoGenHists_topjet.h>
#include <UHH2/MTopJet/include/MTopJetUtils.h>
#include <UHH2/MTopJet/include/JetCluster.h>
class MTopJetGenPostSelectionModule : public ModuleBASE {

 public:
  explicit MTopJetGenPostSelectionModule(uhh2::Context&);
  virtual bool process(uhh2::Event&) override;

 protected:

  // cleaners
  std::unique_ptr<MuonCleaner>     muoSR_cleaner;
  std::unique_ptr<ElectronCleaner> eleSR_cleaner;
  std::unique_ptr<TopJetCorrector> topjet_corrector;
  std::unique_ptr<SubJetCorrector> topjet_subjet_corrector;
  std::unique_ptr<GenericJetResolutionSmearer> topjetER_smearer;
  std::unique_ptr<TopJetCleaner> topjet_cleaner;

  
  // selections
  std::unique_ptr<uhh2::AnalysisModule> ttgenprod;
  std::unique_ptr<uhh2::AnalysisModule> cleaner_gen;
  std::unique_ptr<uhh2::AnalysisModule> cleaner_topgen;
  std::unique_ptr<uhh2::AnalysisModule> cleaner_rec;
  std::unique_ptr<uhh2::AnalysisModule> cleaner_toprec;

  std::unique_ptr<uhh2::AnalysisModule> jetprod; // to produce Jets
  std::unique_ptr<uhh2::Selection> matching;
  std::unique_ptr<uhh2::Selection> topjetpt;
  std::unique_ptr<uhh2::Selection> masscut;
  std::unique_ptr<uhh2::Selection> n_genjets;
  std::unique_ptr<uhh2::Selection> deltaR;
  std::unique_ptr<uhh2::Selection> matching_top;
  std::unique_ptr<uhh2::Selection> topjetpt_top;
  std::unique_ptr<uhh2::Selection> masscut_top;
  std::unique_ptr<uhh2::Selection> n_genjets_top;
  std::unique_ptr<uhh2::Selection> deltaR_top;
  std::unique_ptr<uhh2::Selection> n_recjets;
  std::unique_ptr<uhh2::Selection> topjetpt_rec;
  std::unique_ptr<uhh2::Selection> met_sel;
  std::unique_ptr<uhh2::Selection> htlep_sel;
  std::unique_ptr<uhh2::Selection> twodcut_sel;
  std::unique_ptr<uhh2::Selection> deltaR_rec;
  std::unique_ptr<uhh2::Selection> masscut_rec;
  std::unique_ptr<uhh2::Selection> massbin1;
  std::unique_ptr<uhh2::Selection> massbin2;
  std::unique_ptr<uhh2::Selection> massbin3;
  std::unique_ptr<uhh2::Selection> massbin4;
  std::unique_ptr<uhh2::Selection> massbin5;
  std::unique_ptr<uhh2::Selection> massbin6;
  std::unique_ptr<uhh2::Selection> massbin7;
  std::unique_ptr<uhh2::Selection> massbin1_top;
  std::unique_ptr<uhh2::Selection> massbin2_top;
  std::unique_ptr<uhh2::Selection> massbin3_top;
  std::unique_ptr<uhh2::Selection> massbin4_top;
  std::unique_ptr<uhh2::Selection> massbin5_top;
  std::unique_ptr<uhh2::Selection> massbin6_top;
  std::unique_ptr<uhh2::Selection> massbin7_top;

  // store Hist collection as member variables
  std::unique_ptr<Hists> h_GenHists0a, h_GenHists0b, h_GenHists1, h_GenHists2, h_GenHists3, h_GenHists4, h_GenHists1_m, h_GenHists2_m, h_GenHists3_m, h_GenHists4_m, h_GenHists1_u, h_GenHists2_u, h_GenHists3_u, h_GenHists4_u;
  std::unique_ptr<Hists> h_GenHists0a_top, h_GenHists0b_top, h_GenHists1_top, h_GenHists2_top, h_GenHists3_top, h_GenHists4_top, h_GenHists1_m_top, h_GenHists2_m_top, h_GenHists3_m_top, h_GenHists4_m_top, h_GenHists1_u_top, h_GenHists2_u_top, h_GenHists3_u_top, h_GenHists4_u_top;
  std::unique_ptr<Hists> h_RecHists0a, h_RecHists0b, h_RecHists0c, h_RecHists0d, h_RecHists1, h_RecHists2, h_RecHists3, h_RecHists4;
  std::unique_ptr<Hists> h_RecGenHists0, h_RecGenHists1, h_RecGenHists2, h_RecGenHists3, h_RecGenHists4, h_RecGenHists5, h_RecGenHists6, h_RecGenHists7;
  std::unique_ptr<Hists> h_RecGenHists0_top, h_RecGenHists1_top, h_RecGenHists2_top, h_RecGenHists3_top, h_RecGenHists4_top, h_RecGenHists5_top, h_RecGenHists6_top, h_RecGenHists7_top;
  std::unique_ptr<Hists> h_Elec, h_Muon, h_Elec2, h_Muon2;
};

MTopJetGenPostSelectionModule::MTopJetGenPostSelectionModule(uhh2::Context& ctx){

  //// CONFIGURATION
  const bool isMC = (ctx.get("dataset_type") == "MC");
  
  // set up which jet to use/ possible are: ak06_gen, ak08_gen, ak10_gen, ak12_gen, ak08_rec, ak12_rec, HOTVR, HOTVR_rec, slimmedJetsAK8_SoftDrop, slimmedGenJetsAK8
  const std::string jet_label_gen("ak08_gen");
  const std::string jet_label_rec("ak08_rec");
  const std::string jet_label_hotvr("HOTVR_rec");
  const std::string jet_label_xcone("xcone");
  float jet_radius = 0.8;

  ////

  //// COMMON MODULES

  const std::string ttbar_gen_label("ttbargen");
  ttgenprod.reset(new TTbarGenProducer(ctx, ttbar_gen_label, false));

  // To produce Jets:
  jetprod.reset(new RecoJetProducer(ctx, jet_label_rec, 150, jet_radius)); // set to akt algorithm
  // jetprod.reset(new RecoHOTVRJetProducer(ctx, jet_label_hotvr));
  // jetprod.reset(new RecoXCONEJetProducer(ctx, jet_label_xcone, 2, jet_radius, 2.0, 150));  //(context, jet_label, N, R, beta, ptmin)


  

  //// OBJ CLEANING
  const     MuonId muoSR(AndId<Muon>    (PtEtaCut  (45., 2.1), MuonIDMedium()));
  const ElectronId eleSR(AndId<Electron>(PtEtaSCCut(45., 2.5), ElectronID_Spring15_25ns_tight_noIso));
  muoSR_cleaner.reset(new     MuonCleaner(muoSR));
  eleSR_cleaner.reset(new ElectronCleaner(eleSR));
  cleaner_gen.reset(new GenJetLeptonCleaner(ctx, jet_label_gen, jet_radius));
  cleaner_topgen.reset(new GenTopJetLeptonCleaner(ctx, 0.8));
  cleaner_rec.reset(new RecoJetLeptonCleaner(ctx, jet_label_rec, jet_radius));
  cleaner_toprec.reset(new RecoTopJetLeptonCleaner(ctx, 0.8));

  //// JEC
  std::vector<std::string> JEC_AK4, JEC_AK8;
  if(isMC){
    JEC_AK4 = JERFiles::Fall15_25ns_L123_AK4PFchs_MC;
    JEC_AK8 = JERFiles::Fall15_25ns_L123_AK8PFchs_MC;
  }
  else {
    JEC_AK4 = JERFiles::Fall15_25ns_L123_AK4PFchs_DATA;
    JEC_AK8 = JERFiles::Fall15_25ns_L123_AK8PFchs_DATA;
  }

  topjet_corrector.reset(new TopJetCorrector(ctx, JEC_AK8));
  topjet_subjet_corrector.reset(new SubJetCorrector(ctx, JEC_AK4));
  if(isMC){
    ctx.declare_event_input<std::vector<Particle> >(ctx.get("GenTopJetCollection"), "gentopjets");
    topjetER_smearer.reset(new GenericJetResolutionSmearer(ctx, "topjets", "gentopjets", false));
  }
  topjet_cleaner.reset(new TopJetCleaner(ctx, TopJetId(PtEtaCut(200., 2.4))));


  //// set up Hists classes:
  // GEN
  h_GenHists0a.reset(new MTopJetGenHists(ctx, "00a_GenHists_before_Cleaner", jet_label_gen));
  h_GenHists0b.reset(new MTopJetGenHists(ctx, "00b_GenHists_after_Cleaner", jet_label_gen));
  h_GenHists1.reset(new MTopJetGenHists(ctx, "01_GenHists_JetN", jet_label_gen));
  h_GenHists1_m.reset(new MTopJetGenHists(ctx, "01_GenHists_JetN_matched", jet_label_gen));
  h_GenHists1_u.reset(new MTopJetGenHists(ctx, "01_GenHists_JetN_unmatched", jet_label_gen));
  h_GenHists2.reset(new MTopJetGenHists(ctx, "02_GenHists_pT_Cut", jet_label_gen));
  h_GenHists2_m.reset(new MTopJetGenHists(ctx, "02_GenHists_pT_Cut_matched", jet_label_gen));
  h_GenHists2_u.reset(new MTopJetGenHists(ctx, "02_GenHists_pT_Cut_unmatched", jet_label_gen));
  h_GenHists3.reset(new MTopJetGenHists(ctx, "03_GenHists_deltaR", jet_label_gen));
  h_GenHists3_m.reset(new MTopJetGenHists(ctx, "03_GenHists_deltaR_matched", jet_label_gen));
  h_GenHists3_u.reset(new MTopJetGenHists(ctx, "03_GenHists_deltaR_unmatched", jet_label_gen));
  h_GenHists4.reset(new MTopJetGenHists(ctx, "04_GenHists_masscut", jet_label_gen));
  h_GenHists4_m.reset(new MTopJetGenHists(ctx, "04_GenHists_masscut_matched", jet_label_gen));
  h_GenHists4_u.reset(new MTopJetGenHists(ctx, "04_GenHists_masscut_unmatched", jet_label_gen));

  h_GenHists0a_top.reset(new GenHists_topjet(ctx, "00a_GenHists_top_before_Cleaner"));
  h_GenHists0b_top.reset(new GenHists_topjet(ctx, "00b_GenHists_top_after_Cleaner"));
  h_GenHists1_top.reset(new GenHists_topjet(ctx, "01_GenHists_top_JetN"));
  h_GenHists1_m_top.reset(new GenHists_topjet(ctx, "01_GenHists_top_JetN_matched"));
  h_GenHists1_u_top.reset(new GenHists_topjet(ctx, "01_GenHists_top_JetN_unmatched"));
  h_GenHists2_top.reset(new GenHists_topjet(ctx, "02_GenHists_top_pT_Cut"));
  h_GenHists2_m_top.reset(new GenHists_topjet(ctx, "02_GenHists_top_pT_Cut_matched"));
  h_GenHists2_u_top.reset(new GenHists_topjet(ctx, "02_GenHists_top_pT_Cut_unmatched"));
  h_GenHists3_top.reset(new GenHists_topjet(ctx, "03_GenHists_top_deltaR"));
  h_GenHists3_m_top.reset(new GenHists_topjet(ctx, "03_GenHists_top_deltaR_matched"));
  h_GenHists3_u_top.reset(new GenHists_topjet(ctx, "03_GenHists_top_deltaR_unmatched"));
  h_GenHists4_top.reset(new GenHists_topjet(ctx, "04_GenHists_top_masscut"));
  h_GenHists4_m_top.reset(new GenHists_topjet(ctx, "04_GenHists_top_masscut_matched"));
  h_GenHists4_u_top.reset(new GenHists_topjet(ctx, "04_GenHists_top_masscut_unmatched"));

  //RECO
  h_RecHists0a.reset(new MTopJetRecoHists(ctx, "00a_RecHists_before_Cleaner", jet_label_rec));
  h_RecHists0b.reset(new MTopJetRecoHists(ctx, "00b_RecHists_after_Cleaner", jet_label_rec));
  h_RecHists0c.reset(new MTopJetRecoHists(ctx, "00c_RecHists_LepSel", jet_label_rec));
  h_RecHists0d.reset(new MTopJetRecoHists(ctx, "00d_RecHists_background_reduction", jet_label_rec));

  h_RecHists1.reset(new MTopJetRecoHists(ctx, "01_RecHists_JetN", jet_label_rec));
  h_RecHists2.reset(new MTopJetRecoHists(ctx, "02_RecHists_pT_Cut", jet_label_rec));
  h_RecHists3.reset(new MTopJetRecoHists(ctx, "03_RecHists_deltaR", jet_label_rec));
  h_RecHists4.reset(new MTopJetRecoHists(ctx, "04_RecHists_masscut", jet_label_rec));

  h_Elec.reset(new ElectronHists(ctx, "ElecHist"));
  h_Muon.reset(new MuonHists(ctx, "MuonHist"));
  h_Elec2.reset(new ElectronHists(ctx, "ElecHist_after"));
  h_Muon2.reset(new MuonHists(ctx, "MuonHist_after"));

  //GEN+RECO (resolution)
  h_RecGenHists0.reset(new MTopJetRecoGenHists(ctx, "RecGenHists_all", jet_label_rec, jet_label_gen));
  h_RecGenHists1.reset(new MTopJetRecoGenHists(ctx, "RecGenHists_000to050", jet_label_rec, jet_label_gen));
  h_RecGenHists2.reset(new MTopJetRecoGenHists(ctx, "RecGenHists_050to100", jet_label_rec, jet_label_gen));
  h_RecGenHists3.reset(new MTopJetRecoGenHists(ctx, "RecGenHists_100to150", jet_label_rec, jet_label_gen));
  h_RecGenHists4.reset(new MTopJetRecoGenHists(ctx, "RecGenHists_150to200", jet_label_rec, jet_label_gen));
  h_RecGenHists5.reset(new MTopJetRecoGenHists(ctx, "RecGenHists_200to250", jet_label_rec, jet_label_gen));
  h_RecGenHists6.reset(new MTopJetRecoGenHists(ctx, "RecGenHists_250to300", jet_label_rec, jet_label_gen));
  h_RecGenHists7.reset(new MTopJetRecoGenHists(ctx, "RecGenHists_300to500", jet_label_rec, jet_label_gen));
  h_RecGenHists0_top.reset(new RecoGenHists_topjet(ctx, "RecGenHists_top_all"));
  h_RecGenHists1_top.reset(new RecoGenHists_topjet(ctx, "RecGenHists_top_000to050"));
  h_RecGenHists2_top.reset(new RecoGenHists_topjet(ctx, "RecGenHists_top_050to100"));
  h_RecGenHists3_top.reset(new RecoGenHists_topjet(ctx, "RecGenHists_top_100to150"));
  h_RecGenHists4_top.reset(new RecoGenHists_topjet(ctx, "RecGenHists_top_150to200"));
  h_RecGenHists5_top.reset(new RecoGenHists_topjet(ctx, "RecGenHists_top_200to250"));
  h_RecGenHists6_top.reset(new RecoGenHists_topjet(ctx, "RecGenHists_top_250to300"));
  h_RecGenHists7_top.reset(new RecoGenHists_topjet(ctx, "RecGenHists_top_300to500"));

  //// EVENT SELECTION
  // GEN
  n_genjets.reset(new NGenJets(ctx, jet_label_gen, 150, 2, 2));  // ==2 jets with pt > 150
  topjetpt.reset(new LeadingJetPT(ctx, jet_label_gen, 400)); // leading jet pt > 400
  deltaR.reset(new DeltaRCut(ctx, jet_label_gen, jet_radius)); 
  masscut.reset(new MassCut(ctx, jet_label_gen));
  matching.reset(new Matching(ctx, jet_label_gen, jet_radius));
  
  // GEN Top
  n_genjets_top.reset(new NGenTopJets(ctx, 200, 2, 2));  // ==2 jets with pt > 200 (topjets are produced >~ 180)
  topjetpt_top.reset(new LeadingTopJetPT(ctx, 400)); // leading jet pt > 400
  deltaR_top.reset(new DeltaRCut_top(ctx, 0.8)); 
  masscut_top.reset(new MassCut_top(ctx));
  matching_top.reset(new Matching_top(ctx, 0.8));  ////

  // RECO
  n_recjets.reset(new NRecoJets(ctx, jet_label_rec, 150, 2, 2));  // ==2 jets with pt > 150
  topjetpt_rec.reset(new LeadingRecoJetPT(ctx, jet_label_rec, 400)); // leading jet pt > 400
  met_sel.reset(new METCut  (20, uhh2::infinity));
  htlep_sel.reset(new HTlepCut(100, uhh2::infinity));
  twodcut_sel.reset(new TwoDCut1(.4, 40.));
  deltaR_rec.reset(new DeltaRCutReco(ctx, jet_label_rec, jet_radius)); 
  masscut_rec.reset(new MassCutReco(ctx, jet_label_rec));
  ////

  // Selection for Mass binning
  massbin1.reset(new MassCutGen1(ctx, jet_label_gen,   0,  50));
  massbin2.reset(new MassCutGen1(ctx, jet_label_gen,  50, 100));
  massbin3.reset(new MassCutGen1(ctx, jet_label_gen, 100, 150));
  massbin4.reset(new MassCutGen1(ctx, jet_label_gen, 150, 200));
  massbin5.reset(new MassCutGen1(ctx, jet_label_gen, 200, 250));
  massbin6.reset(new MassCutGen1(ctx, jet_label_gen, 250, 300));
  massbin7.reset(new MassCutGen1(ctx, jet_label_gen, 300, 500));
  massbin1_top.reset(new MassCutGen1_top(ctx,   0,  50));
  massbin2_top.reset(new MassCutGen1_top(ctx,  50, 100));
  massbin3_top.reset(new MassCutGen1_top(ctx, 100, 150));
  massbin4_top.reset(new MassCutGen1_top(ctx, 150, 200));
  massbin5_top.reset(new MassCutGen1_top(ctx, 200, 250));
  massbin6_top.reset(new MassCutGen1_top(ctx, 250, 300));
  massbin7_top.reset(new MassCutGen1_top(ctx, 300, 500));



////

}

bool MTopJetGenPostSelectionModule::process(uhh2::Event& event){

  //  COMMON MODULES
  // set to true / false to run analysis
  bool produce_jet = false;
  bool do_gensel = false;
  bool do_gensel_top = true;
  bool do_recsel = false;

  ttgenprod->process(event);
 

  if(produce_jet){
    jetprod->process(event); // to produce jets
    return true;
  }

  h_GenHists0a->fill(event);
  h_GenHists0a_top->fill(event);
  h_RecHists0a->fill(event);
  // ---------------------------------------------------------------------------------------
  // ---------------------------- apply Gen Selection --------------------------------------
  // ---------------------------------------------------------------------------------------

  // Cleaner
  if(!(produce_jet)){
    cleaner_gen->process(event);
    cleaner_topgen->process(event);
 
    // Cleaner for reco objects
    muoSR_cleaner->process(event);
    sort_by_pt<Muon>(*event.muons);

    eleSR_cleaner->process(event);
    sort_by_pt<Electron>(*event.electrons);

    cleaner_rec->process(event);
   
    topjet_corrector->process(event);
    topjet_subjet_corrector->process(event);
    if(topjetER_smearer.get()) topjetER_smearer->process(event);
    topjet_cleaner->process(event);
    cleaner_toprec->process(event);
    topjet_cleaner->process(event);
    sort_by_pt<TopJet>(*event.topjets);
}

  h_GenHists0b->fill(event);
  h_GenHists0b_top->fill(event);
  h_RecHists0b->fill(event);

  // ==2 Jets with pT > 150/200
  if(!(produce_jet) && do_gensel && !(n_genjets->passes(event))) return false;
  if(!(produce_jet) && do_gensel_top && !(n_genjets_top->passes(event))) return false;
  h_GenHists1->fill(event);
  h_GenHists1_top->fill(event);

  if(!(produce_jet) && do_gensel_top  && matching_top->passes(event)){
    h_GenHists1_m->fill(event);
    h_GenHists1_m_top->fill(event);
  }
  if(!(produce_jet) && do_gensel_top  && !(matching_top->passes(event))){
    h_GenHists1_u->fill(event);
    h_GenHists1_u_top->fill(event);
  }

  // pT cut on leading Jet
  if(!(produce_jet) && do_gensel  && !(topjetpt->passes(event))) return false;
  if(!(produce_jet) && do_gensel_top  && !(topjetpt_top->passes(event))) return false;
  h_GenHists2->fill(event);
  h_GenHists2_top->fill(event);

  if(!(produce_jet) && do_gensel_top  && matching_top->passes(event)){
    h_GenHists2_m->fill(event);
    h_GenHists2_m_top->fill(event);
  }
  if(!(produce_jet) && do_gensel_top  && !(matching_top->passes(event))){
    h_GenHists2_u->fill(event);
    h_GenHists2_u_top->fill(event);
  }

    
  // deltaR(lepton, 2nd jet) < jet radius
  if(!(produce_jet) && do_gensel  && !(deltaR->passes(event))) return false;
  if(!(produce_jet) && do_gensel_top  && !(deltaR_top->passes(event))) return false;
  h_GenHists3->fill(event);
  h_GenHists3_top->fill(event);

  if(!(produce_jet) && do_gensel_top  && matching_top->passes(event)){
    h_GenHists3_m->fill(event);
    h_GenHists3_m_top->fill(event);
  }
  if(!(produce_jet) && do_gensel_top  && !(matching_top->passes(event))){
    h_GenHists3_u->fill(event);
    h_GenHists3_u_top->fill(event);
  }

  // m(1st jet) > m(2nd jet + lepton)
  if(!(produce_jet) && do_gensel  && !(masscut->passes(event))) return false;
  if(!(produce_jet) && do_gensel_top  && !(masscut_top->passes(event))) return false;
  h_GenHists4->fill(event);
  h_GenHists4_top->fill(event);

  if(!(produce_jet) && do_gensel_top  && matching_top->passes(event)){
    h_GenHists4_m->fill(event);
    h_GenHists4_m_top->fill(event);
  }
  if(!(produce_jet) && do_gensel_top  && !(matching_top->passes(event))){
    h_GenHists4_u->fill(event);
    h_GenHists4_u_top->fill(event);
  }


  // ---------------------------------------------------------------------------------------
  // ---------------------------------------------------------------------------------------
  if(!(produce_jet) && do_gensel){
    h_RecGenHists0->fill(event);
    if(massbin1->passes(event)) h_RecGenHists1->fill(event);
    if(massbin2->passes(event)) h_RecGenHists2->fill(event);
    if(massbin3->passes(event)) h_RecGenHists3->fill(event);
    if(massbin4->passes(event)) h_RecGenHists4->fill(event);
    if(massbin5->passes(event)) h_RecGenHists5->fill(event);
    if(massbin6->passes(event)) h_RecGenHists6->fill(event);
    if(massbin7->passes(event)) h_RecGenHists7->fill(event);
  }
  if(!(produce_jet) && do_gensel_top){
    h_RecGenHists0_top->fill(event);
    if(massbin1_top->passes(event)) h_RecGenHists1_top->fill(event);
    if(massbin2_top->passes(event)) h_RecGenHists2_top->fill(event);
    if(massbin3_top->passes(event)) h_RecGenHists3_top->fill(event);
    if(massbin4_top->passes(event)) h_RecGenHists4_top->fill(event);
    if(massbin5_top->passes(event)) h_RecGenHists5_top->fill(event);
    if(massbin6_top->passes(event)) h_RecGenHists6_top->fill(event);
    if(massbin7_top->passes(event)) h_RecGenHists7_top->fill(event);
  }
  // ---------------------------------------------------------------------------------------
  // ---------------------------- apply Reco Selection -------------------------------------
  // ---------------------------------------------------------------------------------------

  //select one elec/muon
  if(!(produce_jet) && do_recsel){
    const bool pass_lep1 = (((event.muons->size() == 1)&&(event.electrons->size() == 0)) ||((event.muons->size() == 0)&&(event.electrons->size() == 1)));
    if(!pass_lep1) return false;
  }
  h_RecHists0c->fill(event);
  // reduction of background (MET, 2D, Ht_lep)
  if(!(produce_jet) && do_recsel && !(met_sel->passes(event))) return false;
  if(!(produce_jet) && do_recsel && !(htlep_sel->passes(event))) return false;

  
  h_Elec->fill(event);
  h_Muon->fill(event);

  /* lepton-2Dcut variables */
  if(!(produce_jet) && do_recsel){
    const bool pass_twodcut = twodcut_sel->passes(event);  {

      for(auto& muo : *event.muons){

	float    dRmin, pTrel;
	std::tie(dRmin, pTrel) = drmin_pTrel(muo, *event.jets);

	muo.set_tag(Muon::twodcut_dRmin, dRmin);
	muo.set_tag(Muon::twodcut_pTrel, pTrel);
      }

      for(auto& ele : *event.electrons){
	
	float    dRmin, pTrel;
	std::tie(dRmin, pTrel) = drmin_pTrel(ele, *event.jets);
	
	ele.set_tag(Electron::twodcut_dRmin, dRmin);
	ele.set_tag(Electron::twodcut_pTrel, pTrel);
      }
    }
    
  h_Elec->fill(event);
  h_Muon->fill(event);
  if(!(produce_jet) && do_recsel && !(pass_twodcut)) return false;
  h_Elec2->fill(event);
  h_Muon2->fill(event);
  }
  
  h_RecHists0d->fill(event);


  // ==2 Jets with pT > 150
  if(!(produce_jet) && do_recsel && !(n_recjets->passes(event))) return false;
  h_RecHists1->fill(event);
  h_RecGenHists1->fill(event);

  // pT cut on leading Jet
  if(!(produce_jet) && do_recsel && !(topjetpt_rec->passes(event))) return false;
  h_RecHists2->fill(event);

  // deltaR(lepton, 2nd jet) < jet radius
  if(!(produce_jet) && do_recsel && !(deltaR_rec->passes(event))) return false;
  h_RecHists3->fill(event);

  // m(1st jet) > m(2nd jet + lepton)
  if(!(produce_jet) && do_recsel && !(masscut_rec->passes(event))) return false;
  h_RecHists4->fill(event);
  // ---------------------------------------------------------------------------------------
  // ---------------------------------------------------------------------------------------


  return true; //false to delete all collections and keep hists
}

UHH2_REGISTER_ANALYSIS_MODULE(MTopJetGenPostSelectionModule)
