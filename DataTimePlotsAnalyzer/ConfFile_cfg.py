import FWCore.ParameterSet.Config as cms

process = cms.Process("Demo")

process.load("FWCore.MessageService.MessageLogger_cfi")

process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(-1) )

process.source = cms.Source("PoolSource",
    # replace 'myfile.root' with the source file you want to use
    fileNames = cms.untracked.vstring(
        'file:/home/users/mcitron/tsgGeneration/CMSSW_12_0_0_pre2/src/output.root'
    )
)

process.demo = cms.EDAnalyzer('DataTimePlotsAnalyzer'
    jets = cms.InputTag( "hltDisplacedHLTCaloJetCollectionProducerMidPt" ),
    jetTimes = cms.InputTag( "hltDisplacedHLTCaloJetCollectionProducerMidPtTiming" ),
)

process.options.SkipEvent = cms.untracked.vstring('ProductNotFound')
process.demo = cms.EDAnalyzer('TriggerAnalyzerRAWMiniAOD')
process.TFileService = cms.Service("TFileService",
        fileName = cms.string( "out.root" )
        )

process.p = cms.Path(process.demo)
