#include <memory>

// user include files
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDAnalyzer.h"

#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"

#include "FWCore/ParameterSet/interface/ParameterSet.h"

#include <DataFormats/SiPixelCluster/interface/SiPixelCluster.h>

class VinPixelPlot : public edm::EDAnalyzer {
public:
  explicit  VinPixelPlot(const edm::ParameterSet&){}
  ~VinPixelPlot(){}
  
  //  static void fillDescriptions(edm::ConfigurationDescriptions& descriptions);
  
  
private:
  virtual void beginJob() {}
  virtual void analyze(const edm::Event&, const edm::EventSetup&);
  virtual void endJob();

  std::string pixelclusterLabel_;

  long long nev=0;
  long long nclus=0;
  float maxRatio=0.f;
  double totRatio=0.f;

};

//define this as a plug-in
DEFINE_FWK_MODULE(VinPixelPlot);

#include <iostream>
void
VinPixelPlot::analyze(const edm::Event& iEvent, const edm::EventSetup&) {

  nev++;
  edm::Handle<edmNew::DetSetVector<SiPixelCluster> > pixelclusters;
  iEvent.getByLabel(pixelclusterLabel_,pixelclusters  );    
  for (auto const &  DSV : *pixelclusters ) {
    uint32_t detid = DSV.id();
    nclus += DSV.size();
    for(auto const & clus : DSV) {  
      double n = clus.size();
      double nx= clus.sizeX();
      double ny= clus.sizeY();
      double ratio = n/(nx*ny);
      maxRatio = std::max(maxRatio,ratio);
      totRatio+=ratio;
    }
  }
  std::cout << "ciao" << std::endl;

}

#include<cstdio>
void VinPixelPlot::endJob() {

  printf("\nVinPixelPlot: nev/nclus/maxRatio,aveRatio %d/%d/%f/%f\n",nev,nclus,maxRatio,totRatio/double(nclus));
  
}
