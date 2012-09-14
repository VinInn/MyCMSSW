#include <memory>

// user include files
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDAnalyzer.h"

#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"

#include "FWCore/ParameterSet/interface/ParameterSet.h"
//
// class declaration
//

class VinPixelPlot : public edm::EDAnalyzer {
public:
  explicit  VinPixelPlot(const edm::ParameterSet&){}
  ~VinPixelPlot(){}
  
  //  static void fillDescriptions(edm::ConfigurationDescriptions& descriptions);
  
  
private:
  virtual void beginJob() {}
  virtual void analyze(const edm::Event&, const edm::EventSetup&);
  virtual void endJob() {}
};

//define this as a plug-in
DEFINE_FWK_MODULE(VinPixelPlot);

#include <iostream>
void
VinPixelPlot::analyze(const edm::Event& iEvent, const edm::EventSetup&) {

  //  edm::Handle<ExampleData> pIn;
  // iEvent.getByLabel("example",pIn);

  std::cout << "ciao" << std::endl;

}
