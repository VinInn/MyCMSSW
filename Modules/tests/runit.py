import FWCore.ParameterSet.Config as cms

process = cms.Process("Demo")

process.load("FWCore.MessageService.MessageLogger_cfi")

process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(-1) )

process.source = cms.Source("PoolSource",
    # replace 'myfile.root' with the source file you want to use
    fileNames = cms.untracked.vstring(
        'file:reco2012.root'
    )
)

process.demo = cms.EDAnalyzer('VinPixelPlot')
process.mypath = cms.Path(process.demo)
process.schedule = cms.Schedule(process.mypath)


