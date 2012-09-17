#include <memory>

// user include files
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDAnalyzer.h"

#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"

#include "FWCore/ParameterSet/interface/ParameterSet.h"

#include "DataFormats/SiPixelCluster/interface/SiPixelCluster.h"

#include "FWCore/ServiceRegistry/interface/Service.h"
#include "CommonTools/UtilAlgos/interface/TFileService.h"
#include "TH1F.h"

class VinPixelPlot : public edm::EDAnalyzer {
public:
  explicit  VinPixelPlot(const edm::ParameterSet&){}
  ~VinPixelPlot(){}
  
  //  static void fillDescriptions(edm::ConfigurationDescriptions& descriptions);
  
  
private:
  virtual void beginJob();
  virtual void analyze(const edm::Event&, const edm::EventSetup&);
  virtual void endJob();

  std::string pixelclusterLabel="siPixelClusters";

  int nev=0;
  int nclus=0;
  double maxRatio=0.;
  double minRatio=2.;
  double totRatio=0.;
  int maxSizeY=0;
  int maxSizeX=0;

  TH1F * hClusSize;
  TH1F * hClusSizeX;
  TH1F * hClusSizeY;
  TH1F * hClusSizeRatio;
  TH1F * hClusSizeInvRatio;

};

//define this as a plug-in
DEFINE_FWK_MODULE(VinPixelPlot);

void VinPixelPlot::beginJob() {

  edm::Service<TFileService> fs;
  
  hClusSize=fs->make<TH1F>("clusSize", "cluster size",100,0.,100.) ;
  hClusSizeX=fs->make<TH1F>("clusSizeX", "cluster X size",100,0.,100.) ;
  hClusSizeY=fs->make<TH1F>("clusSizeY", "cluster Y size",100,0.,100.) ;
  hClusSizeRatio=fs->make<TH1F>("clusRatio", "size/(x*y)",100,0.01,1.01);
  hClusSizeInvRatio=fs->make<TH1F>("clusInvRatio", "(x*y)/size",100,0.,10.);

}

#include <iostream>
void
VinPixelPlot::analyze(const edm::Event& iEvent, const edm::EventSetup&) {

  nev++;
  edm::Handle<edmNew::DetSetVector<SiPixelCluster> > pixelclusters;
  iEvent.getByLabel(pixelclusterLabel,pixelclusters  );    
  for (auto const &  DSV : *pixelclusters ) {
    // uint32_t detid = DSV.id();
    nclus += DSV.size();
    for(auto const & clus : DSV) {  
      int n = clus.size();
      int nx= clus.sizeX();
      int ny= clus.sizeY();
      maxSizeX = std::max(maxSizeX,nx);
      maxSizeY = std::max(maxSizeY,ny);
      double ratio = double(n)/double(nx*ny);
      maxRatio = std::max(maxRatio,ratio);
      minRatio = std::max(minRatio,1./ratio);
      totRatio+=ratio;
      hClusSize->Fill(n);
      hClusSizeX->Fill(nx);
      hClusSizeY->Fill(ny);
      hClusSizeRatio->Fill(ratio);
      hClusSizeInvRatio->Fill(1./ratio);

    }
  }
  // std::cout << "ciao" << std::endl;

}

#include<cstdio>
void VinPixelPlot::endJob() {

  printf("\nVinPixelPlot: nev/nclus/maxSizeX/maxSizeY/minRatio/maxRatio/aveRatio %d/%d/%f/%f/%f\n",
	 nev,nclus,maxSizeX,maxSizeY,minRatio,maxRatio,totRatio/double(nclus));
  
}
