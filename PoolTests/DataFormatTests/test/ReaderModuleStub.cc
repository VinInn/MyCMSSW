#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDAnalyzer.h"

#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/Framework/interface/Event.h"
#include "DataFormats/Common/interface/Handle.h"
#include "SimDataFormats/Track/interface/SimTrackContainer.h"


//
// class declaration
//
namespace {
  template<typename Product>
  class ReaderStub : public edm::EDAnalyzer {
  public:
    explicit ReaderStub(const edm::ParameterSet& pset) :
      label(pset.getParameter<std::string>("label")),
      m_size(0)
    {}
    ~ReaderStub(){}
    
    
  private:
    virtual void beginJob(const edm::EventSetup&){}
    virtual void analyze(const edm::Event& ev, const edm::EventSetup&) {
      edm::Handle<Product > h;
      ev.getByLabel(label,h);
      m_size= h->size();
    }
    virtual void endJob(){}
    
  private:
    
    std::string const label;
    int m_size;
    
  };


}

typedef ReaderStub<std::vector<SimTrack> > SimTrackReaderVI;

DEFINE_FWK_MODULE(SimTrackReaderVI);
