# Configuration file to run L1CaloTkTausProducer.cc
# using: 
# Revision: 1.19 
# Source: /local/reps/CMSSW/CMSSW/Configuration/Applications/python/ConfigBuilder.py,v 
import FWCore.ParameterSet.Config as cms

from Configuration.ProcessModifiers.convertHGCalDigisSim_cff import convertHGCalDigisSim
from Configuration.StandardSequences.Eras import eras

#process = cms.Process('RERUNL1',eras.Phase2_trigger)
process = cms.Process('REPR',eras.Phase2_trigger,convertHGCalDigisSim)
#process = cms.Process('REPR',eras.Phase2C4_timing_layer_bar)

# import of standard configurations
process.load('Configuration.StandardSequences.Services_cff')
process.load('SimGeneral.HepPDTESSource.pythiapdt_cfi')
process.load('FWCore.MessageService.MessageLogger_cfi')
process.load('Configuration.EventContent.EventContent_cff')
process.load('SimGeneral.MixingModule.mixNoPU_cfi')
process.load('Configuration.Geometry.GeometryExtended2023D17Reco_cff')
process.load('Configuration.Geometry.GeometryExtended2023D17_cff')
process.load('Configuration.StandardSequences.MagneticField_cff')
process.load('Configuration.StandardSequences.SimL1Emulator_cff')
process.load('Configuration.StandardSequences.EndOfProcess_cff')
process.load('Configuration.StandardSequences.FrontierConditions_GlobalTag_cff')


process.maxEvents = cms.untracked.PSet(
    input = cms.untracked.int32(20)
)

# Input source
process.source = cms.Source("PoolSource",
                            fileNames = cms.untracked.vstring(
        "/store/mc/PhaseIIFall17D/GluGluHToTauTau_M125_14TeV_powheg_pythia8/GEN-SIM-DIGI-RAW/L1TPU200_93X_upgrade2023_realistic_v5-v1/90000/00D9F890-9739-E811-A931-E0071B73B6B0.root",
        "/store/mc/PhaseIIFall17D/GluGluHToTauTau_M125_14TeV_powheg_pythia8/GEN-SIM-DIGI-RAW/L1TPU200_93X_upgrade2023_realistic_v5-v1/90000/08467E34-8139-E811-8059-008CFA1C6564.root",
        "/store/mc/PhaseIIFall17D/GluGluHToTauTau_M125_14TeV_powheg_pythia8/GEN-SIM-DIGI-RAW/L1TPU200_93X_upgrade2023_realistic_v5-v1/90000/08732B85-7E39-E811-A730-008CFA197BBC.root",
        "/store/mc/PhaseIIFall17D/GluGluHToTauTau_M125_14TeV_powheg_pythia8/GEN-SIM-DIGI-RAW/L1TPU200_93X_upgrade2023_realistic_v5-v1/90000/0A0DD1BB-7E39-E811-AC07-B496910A85DC.root",
        ),
                            secondaryFileNames = cms.untracked.vstring()
)

process.options = cms.untracked.PSet(
    
)

# Production Info
process.configurationMetadata = cms.untracked.PSet(
    annotation = cms.untracked.string('repr nevts:2'),
    name = cms.untracked.string('Applications'),
    version = cms.untracked.string('$Revision: 1.19 $')
)

# Other statements
from Configuration.AlCa.GlobalTag import GlobalTag
process.GlobalTag = GlobalTag(process.GlobalTag, '103X_upgrade2023_realistic_v2', '') 

process.load('SimCalorimetry.HcalTrigPrimProducers.hcaltpdigi_cff')

# Path and EndPath definitions
process.L1simulation_step = cms.Path(process.SimL1Emulator)

process.load('L1Trigger.TrackFindingTracklet.L1TrackletTracks_cff')
process.L1TrackTrigger_step = cms.Path(process.L1TrackletTracks)


# ----                                                                              

# Now we produce the L1CaloTkTauParticles
 
process.load("L1Trigger.Phase2L1Taus.L1CaloTkTauParticleProducer_cfi")
process.pL1CaloTkTaus = cms.Path( process.L1CaloTkTaus )

# ---------------------------------------------------------------------------

process.Out = cms.OutputModule( "PoolOutputModule",
    fileName = cms.untracked.string( "l1calotktausTest.root" ),
    fastCloning = cms.untracked.bool( False ),
    outputCommands = cms.untracked.vstring(
        "keep *_L1CaloTkTaus_*_*"
        )
)

# ---------------------------------------------------------------------------

process.end = cms.EndPath( process.Out )

# ---------------------------------------------------------------------------

# Automatic addition of the customisation function from L1Trigger.Configuration.customiseUtils
from L1Trigger.Configuration.customiseUtils import DropDepricatedProducts,L1TrackTriggerTracklet,DropOutputProducts 

#call to customisation function DropDepricatedProducts imported from L1Trigger.Configuration.customiseUtils
process = DropDepricatedProducts(process)

# ---------------------------------------------------------------------------

#dump_file = open("dump_file.py", "w")
#dump_file.write(process.dumpPython())
