import FWCore.ParameterSet.Config as cms

process = cms.Process("Demo")

process.load("FWCore.MessageService.MessageLogger_cfi")

process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(-1) )

process.source = cms.Source("PoolSource",
    # replace 'myfile.root' with the source file you want to use
    fileNames = cms.untracked.vstring(
        'file:/home/users/mcitron/tsgGeneration/output.root'
    )
)
process.HLT_L1MET_DTCluster50Plots = cms.EDAnalyzer('DTPlotsAnalyzer',
    dtClusters = cms.InputTag( "hltDisplacedHLTDTClusterCollectionProducer" ),
    triggerString = cms.string("HLT_L1MET_DTCluster50")
)
process.HLT_L1MET_DTClusterNoMB1S50Plots = cms.EDAnalyzer('DTPlotsAnalyzer',
    dtClusters = cms.InputTag( "hltDisplacedHLTDTClusterNoMB1SCollectionProducer" ),
    triggerString = cms.string("HLT_L1MET_DTClusterNoMB1S50")
)
process.HLT_CaloMET60_DTCluster50Plots = cms.EDAnalyzer('DTPlotsAnalyzer',
    dtClusters = cms.InputTag( "hltDisplacedHLTDTClusterCollectionProducer" ),
    triggerString = cms.string("HLT_CaloMET60_DTCluster50")
)
process.HLT_CaloMET60_DTClusterNoMB1S50Plots = cms.EDAnalyzer('DTPlotsAnalyzer',
    dtClusters = cms.InputTag( "hltDisplacedHLTDTClusterNoMB1SCollectionProducer" ),
    triggerString = cms.string("HLT_CaloMET60_DTClusterNoMB1S50")
)
process.HLT_PFMET100_DTCluster50Plots = cms.EDAnalyzer('DTPlotsAnalyzer',
    dtClusters = cms.InputTag( "hltDisplacedHLTDTClusterCollectionProducer" ),
    triggerString = cms.string("HLT_PFMET100_DTCluster50")
)
process.HLT_PFMET100_DTClusterNoMB1S50Plots = cms.EDAnalyzer('DTPlotsAnalyzer',
    dtClusters = cms.InputTag( "hltDisplacedHLTDTClusterNoMB1SCollectionProducer" ),
    triggerString = cms.string("HLT_PFMET100_DTClusterNoMB1S50")
)
process.HLT_HT430_DTCluster50Plots = cms.EDAnalyzer('DTPlotsAnalyzer',
    dtClusters = cms.InputTag( "hltDisplacedHLTDTClusterCollectionProducer" ),
    triggerString = cms.string("HLT_HT430_DTCluster50")
)
process.HLT_HT430_DTClusterNoMB1S50Plots = cms.EDAnalyzer('DTPlotsAnalyzer',
    dtClusters = cms.InputTag( "hltDisplacedHLTDTClusterNoMB1SCollectionProducer" ),
    triggerString = cms.string("HLT_HT430_DTClusterNoMB1S50")
)
process.TFileService = cms.Service("TFileService",
                                   fileName = cms.string( "outDT.root" )
                                   )
process.options.SkipEvent = cms.untracked.vstring('ProductNotFound')
process.p = cms.Path(process.HLT_L1MET_DTCluster50Plots+process.HLT_CaloMET60_DTCluster50Plots+process.HLT_PFMET100_DTCluster50Plots+process.HLT_PFMET100_DTClusterNoMB1S50Plots+process.HLT_HT430_DTCluster50Plots+process.HLT_HT430_DTClusterNoMB1S50Plots)
