import FWCore.ParameterSet.Config as cms
import os, sys

from Configuration.StandardSequences.Eras import eras

process = cms.Process('RERUNL1',eras.Phase2_trigger)

process.load("FWCore.MessageService.MessageLogger_cfi")

# import of standard configurations
process.load('Configuration.StandardSequences.Services_cff')
process.load('SimGeneral.HepPDTESSource.pythiapdt_cfi')
process.load('FWCore.MessageService.MessageLogger_cfi')
process.load('Configuration.EventContent.EventContent_cff')
process.load('SimGeneral.MixingModule.mix_POISSON_average_cfi')
process.load('Configuration.Geometry.GeometryExtended2023D17Reco_cff')
process.load('Configuration.StandardSequences.MagneticField_cff')
process.load('Configuration.StandardSequences.SimL1Emulator_cff')
process.load('Configuration.StandardSequences.L1TrackTrigger_cff')
process.load('SimCalorimetry.HcalTrigPrimProducers.hcalTTPDigis_cff')
process.load('Configuration.StandardSequences.EndOfProcess_cff')
process.load('Configuration.StandardSequences.FrontierConditions_GlobalTag_cff')

process.maxEvents = cms.untracked.PSet(
    input = cms.untracked.int32(200)
)


# Input source
process.source = cms.Source("PoolSource",
                            fileNames = cms.untracked.vstring(
        #'/store/mc/PhaseIIFall17D/SingleNeutrino/GEN-SIM-DIGI-RAW/L1TPU200_93X_upgrade2023_realistic_v5-v1/80000/C66CC055-8F5B-E811-A655-0025905C53F0.root'),
        "/store/mc/PhaseIIFall17D/GluGluHToTauTau_M125_14TeV_powheg_pythia8/GEN-SIM-DIGI-RAW/L1TPU200_93X_upgrade2023_realistic_v5-v1/90000/00D9F890-9739-E811-A931-E0071B73B6B0.root",
        "/store/mc/PhaseIIFall17D/GluGluHToTauTau_M125_14TeV_powheg_pythia8/GEN-SIM-DIGI-RAW/L1TPU200_93X_upgrade2023_realistic_v5-v1/90000/08467E34-8139-E811-8059-008CFA1C6564.root",
        "/store/mc/PhaseIIFall17D/GluGluHToTauTau_M125_14TeV_powheg_pythia8/GEN-SIM-DIGI-RAW/L1TPU200_93X_upgrade2023_realistic_v5-v1/90000/08732B85-7E39-E811-A730-008CFA197BBC.root",
        "/store/mc/PhaseIIFall17D/GluGluHToTauTau_M125_14TeV_powheg_pythia8/GEN-SIM-DIGI-RAW/L1TPU200_93X_upgrade2023_realistic_v5-v1/90000/0A0DD1BB-7E39-E811-AC07-B496910A85DC.root",
    ),
    secondaryFileNames = cms.untracked.vstring(),
    inputCommands = cms.untracked.vstring(
                    "keep *",
                    "keep *_hgcalBackEndLayer2Producer_*_*",
                    "drop l1tEMTFHitExtras_simEmtfDigis_CSC_HLT",
                    "drop l1tEMTFHitExtras_simEmtfDigis_RPC_HLT",
                    "drop l1tEMTFTrackExtras_simEmtfDigis__HLT",
                    "drop l1tEMTFHit2016Extras_simEmtfDigis_CSC_HLT",
                    "drop l1tEMTFHit2016Extras_simEmtfDigis_RPC_HLT",
                    "drop l1tEMTFHit2016s_simEmtfDigis__HLT",
                    "drop l1tEMTFTrack2016Extras_simEmtfDigis__HLT",
                    "drop l1tEMTFTrack2016s_simEmtfDigis__HLT"
                    )

)
# Production Info
process.configurationMetadata = cms.untracked.PSet(
    annotation = cms.untracked.string('step2 nevts:1'),
    name = cms.untracked.string('Applications'),
    version = cms.untracked.string('$Revision: 1.19 $')
)

# Additional output definition
# ---- Global Tag :
process.load('Configuration.StandardSequences.FrontierConditions_GlobalTag_cff')
from Configuration.AlCa.GlobalTag import GlobalTag
process.GlobalTag = GlobalTag(process.GlobalTag, '100X_upgrade2023_realistic_v1', '')
#process.Timing = cms.Service("Timing")


process.HcalHardcodeGeometryEP = cms.ESProducer("HcalHardcodeGeometryEP",
    UseOldLoader = cms.bool(False)
)


process.HcalTrigTowerGeometryESProducer = cms.ESProducer("HcalTrigTowerGeometryESProducer")

process.CaloGeometryBuilder = cms.ESProducer("CaloGeometryBuilder",
    SelectedCalos = cms.vstring('HCAL', 
        'ZDC', 
        'EcalBarrel', 
        'TOWER', 
        'HGCalEESensitive', 
        'HGCalHESiliconSensitive')
)




process.CaloTopologyBuilder = cms.ESProducer("CaloTopologyBuilder")


process.CaloTowerHardcodeGeometryEP = cms.ESProducer("CaloTowerHardcodeGeometryEP")


process.CaloTowerTopologyEP = cms.ESProducer("CaloTowerTopologyEP")

#from SimTracker.TrackTriggerAssociation.TTTrackAssociation_cfi import *
#TrackTriggerAssociatorTracks = cms.Sequence(TTTrackAssociatorFromPixelDigis)

process.load('L1Trigger.L1THGCal.hgcalTriggerPrimitives_cff')

process.hgcl1tpg_step = cms.Path(process.hgcalTriggerPrimitives)


# load ntuplizer
#process.load('L1Trigger.L1THGCal.hgcalTriggerNtuples_cff')
#process.ntuple_step = cms.Path(process.hgcalTriggerNtuples)

process.load('SimCalorimetry.EcalEBTrigPrimProducers.ecalEBTriggerPrimitiveDigis_cff')
process.EcalEBtp_step = cms.Path(process.simEcalEBTriggerPrimitiveDigis)

# Path and EndPath definitions
#process.HcalTPsimulation_step = cms.Path(process.hcalTTPSequence)
#process.L1simulation_step = cms.Path(process.SimL1Emulator)

process.load('L1Trigger.TrackFindingTracklet.L1TrackletTracks_cff')
process.L1TrackTrigger_step = cms.Path(process.L1TrackletTracks)

process.load('L1Trigger.L1CaloTrigger.L1EGammaCrystalsEmulatorProducer_cfi')
process.pL1EG = cms.Path( process.L1EGammaClusterEmuProducer )

process.load('L1Trigger.L1CaloTrigger.l1EGammaEEProducer_cfi')
process.pL1EGHGCal = cms.Path( process.l1EGammaEEProducer )

# ----                                                                                                                                                         
process.load("L1Trigger.Phase2L1Taus.L1TkEGTauParticleProducer_cfi")
process.pL1TkEGTausProd = cms.Path( process.L1TkEGTaus )

process.load("L1Trigger.Phase2L1Taus.L1TkEGTausAnalyzer_cff")
process.pL1TkEGTausAna = cms.Path(process.TkEGRate + process.TkEGEff)
#process.pTrkObjAna = cms.Path(process.MuonRate +  process.PhotonRate + process.ElectronRate + process.IsoElectronRate)

# ---------------------------------------------------------------------------

# Now we produce L1TkEmParticles and L1TkElectrons
'''
process.load("L1Trigger.L1TTrackMatch.L1TkObjectProducers_cff")
process.pTrkObjProd = cms.Path(process.L1TkElectrons
                             + process.L1TkIsoElectrons 
                             + process.L1TkPhotons 
                             + process.L1TkMuons)


process.load("L1Trigger.L1TTrackMatch.L1TkObjectAnalyzer_cff")
process.pTrkObjAna = cms.Path(process.MuonRate +  process.PhotonRate + process.ElectronRate + process.IsoElectronRate)

'''


# root file with histograms produced by the analyzer
filename = "L1TkEGTausPerformance.root"
process.TFileService = cms.Service("TFileService", fileName = cms.string(filename), closeFileFast = cms.untracked.bool(True))

# ---------------------------------------------------------------------------

process.dumpED = cms.EDAnalyzer("EventContentAnalyzer")
process.pDumpED = cms.Path(process.dumpED)


#process.schedule = cms.Schedule(process.L1simulation_step, process.pL1TkEGTausProd,process.pL1TkEGTausAna
#process.schedule = cms.Schedule(process.L1TrackTrigger_step, process.pL1EG, process.pL1TkEGTausProd, process.pL1TkEGTausAna)
process.schedule = cms.Schedule(process.L1TrackTrigger_step, process.pL1EG, process.pL1EGHGCal, process.pL1TkEGTausProd, process.pL1TkEGTausAna)
#process.schedule = cms.Schedule(process.L1TrackTrigger_step,process.hgcl1tpg_step, process.pL1EG, process.pL1EGHGCal, process.pL1TkEGTausProd, process.pL1TkEGTausAna)

#dump_file = open("dump_file.py", "w")
#dump_file.write(process.dumpPython())








