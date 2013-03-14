// user include files
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDAnalyzer.h"

#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"

#include "FWCore/ParameterSet/interface/ParameterSet.h"

#include "DataFormats/TrackReco/interface/TrackFwd.h"
#include "DataFormats/TrackReco/interface/Track.h"

class VinDumpTrack : public edm::EDAnalyzer {
public:
  explicit  VinDumpTrack(const edm::ParameterSet&){}
  ~VinDumpTrack(){}
  
  //  static void fillDescriptions(edm::ConfigurationDescriptions& descriptions);
  
  
private:
  virtual void beginJob();
  virtual void analyze(const edm::Event&, const edm::EventSetup&);
  virtual void endJob();

  std::string trackLabel="generalTracks";

};


//define this as a plug-in
DEFINE_FWK_MODULE(VinDumpTrack);

#include<iostream>

void VinDumpTrack::beginJob() {}
void VinDumpTrack::endJob() {}


void VinDumpTrack::analyze(const edm::Event& iEvent, const edm::EventSetup&) {

  // nev++;
  edm::Handle<reco::TrackCollection> tracks;
  iEvent.getByLabel(trackLabel,tracks);
  int ntot[10];
  int hpt[10];
  for (int i=0; i!=10;++i){ ntot[i]=0; hpt[i]=0;}
  for (auto const &  track : *tracks ) {
    int algo = track.algo();
    if (algo<4 || algo>14) continue;
    algo -=4;
    ntot[algo]++;
    if(track.pt()>5) hpt[algo]++;
  }
  std::cout << "tracks: all ";
  for (int i=0; i!=10;++i) std::cout << ntot[i] << "/";
  std::cout << std::endl;
  std::cout << "tracks: hpt ";
  for (int i=0; i!=10;++i) std::cout << hpt[i] << "/";
  std::cout << std::endl;

}
