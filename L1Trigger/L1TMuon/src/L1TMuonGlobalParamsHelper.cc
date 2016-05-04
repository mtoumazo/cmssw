#include <iomanip>
#include <strstream>

#include "L1Trigger/L1TMuon/interface/L1TMuonGlobalParamsHelper.h"

L1TMuonGlobalParamsHelper::L1TMuonGlobalParamsHelper(const L1TMuonGlobalParams p) : L1TMuonGlobalParams(p)
{
  if (pnodes_.size() != NUM_GMTPARAMNODES) {
    pnodes_.resize(NUM_GMTPARAMNODES);
  }
}


std::bitset<72> L1TMuonGlobalParamsHelper::inputFlags(const int &nodeIdx) const
{
  std::bitset<72> inputFlags;
  if (pnodes_[nodeIdx].uparams_.size() != 4) {
    return inputFlags;
  }

  for (size_t i = 0; i < 28; ++i) {
    inputFlags[CALOLINK1 + i] = ((pnodes_[nodeIdx].uparams_[CALOINPUTS] >> i) & 0x1);
    if (i < CALOLINK1) {
      // disable unused inputs
      inputFlags[i] = 0x1;
    }
    if (i < 12) {
      inputFlags[BMTFLINK1 + i] = ((pnodes_[nodeIdx].uparams_[BMTFINPUTS] >> i) & 0x1);
      if (i < 6) {
        inputFlags[EMTFPLINK1 + i] = ((pnodes_[nodeIdx].uparams_[EMTFINPUTS] >> i) & 0x1);
        inputFlags[OMTFPLINK1 + i] = ((pnodes_[nodeIdx].uparams_[OMTFINPUTS] >> i) & 0x1);
        inputFlags[OMTFNLINK1 + i] = ((pnodes_[nodeIdx].uparams_[OMTFINPUTS] >> (i + 6)) & 0x1);
        inputFlags[EMTFNLINK1 + i] = ((pnodes_[nodeIdx].uparams_[EMTFINPUTS] >> (i + 6)) & 0x1);
      }
    }
  }
  return inputFlags;
}


std::bitset<28> L1TMuonGlobalParamsHelper::caloInputFlags(const int &nodeIdx) const
{
  if (pnodes_[nodeIdx].uparams_.size() == 4) {
    return std::bitset<28>(pnodes_[nodeIdx].uparams_[CALOINPUTS]);
  } else {
    return std::bitset<28>();
  }
}


std::bitset<12> L1TMuonGlobalParamsHelper::tfInputFlags(const int &nodeIdx, const int &tfIdx) const
{
  if (pnodes_[nodeIdx].uparams_.size() == 4) {
    return std::bitset<12>(pnodes_[nodeIdx].uparams_[tfIdx]);
  } else {
    return std::bitset<12>();
  }
}


std::bitset<6> L1TMuonGlobalParamsHelper::eomtfInputFlags(const int &nodeIdx, const size_t &startIdx, const int &tfIdx) const
{
  std::bitset<6> inputFlags;
  if (pnodes_[nodeIdx].uparams_.size() == 4) {
    for (size_t i = 0; i < 6; ++i) {
      inputFlags[i] = ((pnodes_[nodeIdx].uparams_[tfIdx] >> (i + startIdx)) & 0x1);
    }
  }
  return inputFlags;
}


void L1TMuonGlobalParamsHelper::setFwVersion(unsigned fwVersion)
{
  pnodes_[FWVERSION].uparams_.resize(1);
  pnodes_[FWVERSION].uparams_[FWVERSION_IDX] = fwVersion;
}


void L1TMuonGlobalParamsHelper::setInputFlags(const int &nodeIdx, const std::bitset<72> &inputFlags)
{
  pnodes_[nodeIdx].uparams_.resize(4);
  for (size_t i = 0; i < 28; ++i) {
    pnodes_[nodeIdx].uparams_[CALOINPUTS] += (inputFlags.test(CALOLINK1 + i) << i);
    if (i < 12) {
      pnodes_[nodeIdx].uparams_[BMTFINPUTS] += (inputFlags.test(BMTFLINK1 + i) << i);
      if (i < 6) {
        pnodes_[nodeIdx].uparams_[OMTFINPUTS] += (inputFlags.test(OMTFPLINK1 + i) << i);
        pnodes_[nodeIdx].uparams_[OMTFINPUTS] += (inputFlags.test(OMTFNLINK1 + i) << (i + 6));
        pnodes_[nodeIdx].uparams_[EMTFINPUTS] += (inputFlags.test(EMTFPLINK1 + i) << i);
        pnodes_[nodeIdx].uparams_[EMTFINPUTS] += (inputFlags.test(EMTFNLINK1 + i) << (i + 6));
      }
    }
  }
}


void L1TMuonGlobalParamsHelper::setCaloInputFlags(const int &nodeIdx, const std::bitset<28> &inputFlags)
{
  pnodes_[nodeIdx].uparams_.resize(4);
  for (size_t i = 0; i < 28; ++i) {
    pnodes_[nodeIdx].uparams_[CALOINPUTS] += (inputFlags.test(i) << i);
  }
}


void L1TMuonGlobalParamsHelper::setTfInputFlags(const int &nodeIdx, const int &tfIdx, const std::bitset<12> &inputFlags)
{
  pnodes_[nodeIdx].uparams_.resize(4);
  for (size_t i = 0; i < 12; ++i) {
    pnodes_[nodeIdx].uparams_[tfIdx] += (inputFlags.test(i) << i);
  }
}


void L1TMuonGlobalParamsHelper::setEOmtfInputFlags(const int &nodeIdx, const size_t &startIdx, const int &tfIdx, const std::bitset<6> &inputFlags)
{
  pnodes_[nodeIdx].uparams_.resize(4);
  for (size_t i = 0; i < 6; ++i) {
    pnodes_[nodeIdx].uparams_[tfIdx] += (inputFlags.test(i) << (i + startIdx));
  }
}


void L1TMuonGlobalParamsHelper::loadFromOnline(l1t::trigSystem& trgSys, const std::string& processorId)
{
  std::string procId = processorId;
  // if the procId is an empty string use the one from the trigSystem (the uGMT only has one processor)
  if (procId == "" ) {
    const std::map<std::string, std::string>& procRoleMap = trgSys.getProcRole();
    if (procRoleMap.size() != 1) {
      if (procRoleMap.size() == 0) {
        edm::LogError("uGMT config from online") << "No processor id found for uGMT HW configuration.";
      } else {
        edm::LogError("uGMT config from online") << "More than one processor id found for uGMT HW configuration.";
      }
    } else {
      procId = procRoleMap.cbegin()->first;
    }
  }

  // FIXME the fwVersion needs to be set from somewhere else
  // Perhaps directly in the O2O ESProducer
  setFwVersion(1);

  // get the settings and masks for the processor id
  std::map<std::string, l1t::setting> settings = trgSys.getSettings(procId);
  std::map<std::string, l1t::mask> masks = trgSys.getMasks(procId);
  //for (auto& it: settings) {
  //   std::cout << "Key: " << it.first << ", procRole: " << it.second.getProcRole() << ", type: " << it.second.getType() << ", id: " << it.second.getId() << ", value as string: [" << it.second.getValueAsStr() << "]" << std::endl;
  //}
  //for (auto& it: masks) {
  //   std::cout << "Key: " << it.first << ", procRole: " << it.second.getProcRole() << ", id: " << it.second.getId() << std::endl;
  //}

  std::stringstream ss;
  // uGMT disabled inputs
  bool disableCaloInputs = settings["caloInputsDisable"].getValue<bool>();
  std::string bmtfInputsToDisableStr = settings["bmtfInputsToDisable"].getValueAsStr();
  std::string omtfInputsToDisableStr = settings["omtfInputsToDisable"].getValueAsStr();
  std::string emtfInputsToDisableStr = settings["emtfInputsToDisable"].getValueAsStr();
  std::vector<unsigned> bmtfInputsToDisable(12, 0);
  std::vector<unsigned> omtfInputsToDisable(12, 0);
  std::vector<unsigned> emtfInputsToDisable(12, 0);
  // translate the bool and the strings to the vectors
  for (unsigned i = 0; i < 12; ++i) {
     ss.str("");
     ss << "BMTF" << i+1;
     if (bmtfInputsToDisableStr.find(ss.str()) != std::string::npos) {
        bmtfInputsToDisable[i] = 1;
     }
     ss.str("");
     ss << "OMTF";
     if (i < 6) {
        ss << "p" << i+1;
     } else {
        ss << "n" << i-5;
     }
     if (omtfInputsToDisableStr.find(ss.str()) != std::string::npos) {
        omtfInputsToDisable[i] = 1;
     }
     ss.str("");
     ss << "EMTF";
     if (i < 6) {
        ss << "p" << i+1;
     } else {
        ss << "n" << i-5;
     }
     if (emtfInputsToDisableStr.find(ss.str()) != std::string::npos) {
        emtfInputsToDisable[i] = 1;
     }
  }

  // set the condFormats parameters for uGMT disabled inputs
  if (disableCaloInputs) {
     setCaloInputsToDisable(std::bitset<28>(0xFFFFFFF));
  } else {
     setCaloInputsToDisable(std::bitset<28>());
  }

  std::bitset<12> bmtfDisables;
  for (size_t i = 0; i < bmtfInputsToDisable.size(); ++i) {
    bmtfDisables.set(i, bmtfInputsToDisable[i] > 0);
  }
  setBmtfInputsToDisable(bmtfDisables);

  std::bitset<6> omtfpDisables;
  std::bitset<6> omtfnDisables;
  for (size_t i = 0; i < omtfInputsToDisable.size(); ++i) {
    if (i < 6) {
      omtfpDisables.set(i, omtfInputsToDisable[i] > 0);
    } else {
      omtfnDisables.set(i-6, omtfInputsToDisable[i] > 0);
    }
  }
  setOmtfpInputsToDisable(omtfpDisables);
  setOmtfnInputsToDisable(omtfnDisables);

  std::bitset<6> emtfpDisables;
  std::bitset<6> emtfnDisables;
  for (size_t i = 0; i < emtfInputsToDisable.size(); ++i) {
    if (i < 6) {
      emtfpDisables.set(i, emtfInputsToDisable[i] > 0);
    } else {
      emtfnDisables.set(i-6, emtfInputsToDisable[i] > 0);
    }
  }
  setEmtfpInputsToDisable(emtfpDisables);
  setEmtfnInputsToDisable(emtfnDisables);

  // uGMT masked inputs
  bool caloInputsMasked = true;
  std::vector<unsigned> maskedBmtfInputs(12, 0);
  std::vector<unsigned> maskedOmtfInputs(12, 0);
  std::vector<unsigned> maskedEmtfInputs(12, 0);
  ss << std::setfill('0');
  // translate the bool and the strings to the vectors
  for (unsigned i = 0; i < 28; ++i) {
     ss.str("");
     ss << "inputPorts.CaloL2_" << std::setw(2) << i+1;
     // for now set as unmasked if one input is not masked
     if (!trgSys.isMasked(procId, ss.str())) {
        caloInputsMasked = false;
     }
     if (i < 12) {
        ss.str("");
        ss << "inputPorts.BMTF_" << std::setw(2) << i+1;
        if (trgSys.isMasked(procId, ss.str())) {
           maskedBmtfInputs[i] = 1;
        }
        ss.str("");
        ss << "inputPorts.OMTF";
        if (i < 6) {
           ss << "+_" << std::setw(2) << i+1;
        } else {
           ss << "-_" << std::setw(2) << i-5;
        }
        if (trgSys.isMasked(procId, ss.str())) {
           maskedOmtfInputs[i] = 1;
        }
        ss.str("");
        ss << "inputPorts.EMTF";
        if (i < 6) {
           ss << "+_" << std::setw(2) << i+1;
        } else {
           ss << "-_" << std::setw(2) << i-5;
        }
        if (trgSys.isMasked(procId, ss.str())) {
           maskedEmtfInputs[i] = 1;
        }
     }
  }
  ss << std::setfill(' ');

  // set the condFormats parameters for uGMT masked inputs
  if (caloInputsMasked) {
     setMaskedCaloInputs(std::bitset<28>(0xFFFFFFF));
  } else {
     setMaskedCaloInputs(std::bitset<28>());
  }

  std::bitset<12> bmtfMasked;
  for (size_t i = 0; i < maskedBmtfInputs.size(); ++i) {
    bmtfMasked.set(i, maskedBmtfInputs[i] > 0);
  }
  setMaskedBmtfInputs(bmtfMasked);

  std::bitset<6> omtfpMasked;
  std::bitset<6> omtfnMasked;
  for (size_t i = 0; i < maskedOmtfInputs.size(); ++i) {
    if (i < 6) {
      omtfpMasked.set(i, maskedOmtfInputs[i] > 0);
    } else {
      omtfnMasked.set(i-6, maskedOmtfInputs[i] > 0);
    }
  }
  setMaskedOmtfpInputs(omtfpMasked);
  setMaskedOmtfnInputs(omtfnMasked);

  std::bitset<6> emtfpMasked;
  std::bitset<6> emtfnMasked;
  for (size_t i = 0; i < maskedEmtfInputs.size(); ++i) {
    if (i < 6) {
      emtfpMasked.set(i, maskedEmtfInputs[i] > 0);
    } else {
      emtfnMasked.set(i-6, maskedEmtfInputs[i] > 0);
    }
  }
  setMaskedEmtfpInputs(emtfpMasked);
  setMaskedEmtfnInputs(emtfnMasked);

  // LUTs from settings with address width and output width
  setAbsIsoCheckMemLUT(settings["AbsIsoCheckMem"].getLUT(5, 1));
  setRelIsoCheckMemLUT(settings["RelIsoCheckMem"].getLUT(14, 1));
  setIdxSelMemPhiLUT(settings["IdxSelMemPhi"].getLUT(10, 6));
  setIdxSelMemEtaLUT(settings["IdxSelMemEta"].getLUT(9, 5));
  setFwdPosSingleMatchQualLUT(settings["EmtfPosSingleMatchQual"].getLUT(7, 1));
  setFwdNegSingleMatchQualLUT(settings["EmtfNegSingleMatchQual"].getLUT(7, 1));
  setOvlPosSingleMatchQualLUT(settings["OmtfPosSingleMatchQual"].getLUT(7, 1));
  setOvlNegSingleMatchQualLUT(settings["OmtfNegSingleMatchQual"].getLUT(7, 1));
  setBOPosMatchQualLUT(settings["BOPosMatchQual"].getLUT(7, 1));
  setBONegMatchQualLUT(settings["BONegMatchQual"].getLUT(7, 1));
  setFOPosMatchQualLUT(settings["EOPosMatchQual"].getLUT(7, 1));
  setFONegMatchQualLUT(settings["EONegMatchQual"].getLUT(7, 1));
  setBPhiExtrapolationLUT(settings["BPhiExtrapolation"].getLUT(12, 3));
  setOPhiExtrapolationLUT(settings["OPhiExtrapolation"].getLUT(12, 3));
  setFPhiExtrapolationLUT(settings["EPhiExtrapolation"].getLUT(12, 3));
  setBEtaExtrapolationLUT(settings["BEtaExtrapolation"].getLUT(12, 4));
  setOEtaExtrapolationLUT(settings["OEtaExtrapolation"].getLUT(12, 4));
  setFEtaExtrapolationLUT(settings["EEtaExtrapolation"].getLUT(12, 4));
  setSortRankLUT(settings["SortRank"].getLUT(13, 10));
}


void L1TMuonGlobalParamsHelper::print(std::ostream& out) const {

  out << "L1 MicroGMT Parameters" << std::endl;

  out << "Firmware version: " << this->fwVersion() << std::endl;

  out << "InputsToDisable: " << this->inputsToDisable() << std::endl;
  out << "                 EMTF-|OMTF-|   BMTF    |OMTF+|EMTF+|            CALO           |  res  0" << std::endl;

  out << "Masked Inputs:   " << this->maskedInputs() << std::endl;
  out << "                 EMTF-|OMTF-|   BMTF    |OMTF+|EMTF+|            CALO           |  res  0" << std::endl;

  out << "LUT paths (LUTs are generated analytically if path is empty)" << std::endl;
  out << " Abs isolation checkMem LUT path: "        << this->absIsoCheckMemLUTPath() << std::endl;
  out << " Rel isolation checkMem LUT path: "        << this->relIsoCheckMemLUTPath() << std::endl;
  out << " Index selMem phi LUT path: "              << this->idxSelMemPhiLUTPath() << std::endl;
  out << " Index selMem eta LUT path: "              << this->idxSelMemEtaLUTPath() << std::endl;
  out << " Forward pos MatchQual LUT path: "         << this->fwdPosSingleMatchQualLUTPath() << ", max dR (Used when LUT path empty): " << this->fwdPosSingleMatchQualLUTMaxDR() << std::endl;
  out << " Forward neg MatchQual LUT path: "         << this->fwdNegSingleMatchQualLUTPath() << ", max dR (Used when LUT path empty): " << this->fwdNegSingleMatchQualLUTMaxDR() << std::endl;
  out << " Overlap pos MatchQual LUT path: "         << this->ovlPosSingleMatchQualLUTPath() << ", max dR (Used when LUT path empty): " << this->ovlPosSingleMatchQualLUTMaxDR() << std::endl;
  out << " Overlap neg MatchQual LUT path: "         << this->ovlNegSingleMatchQualLUTPath() << ", max dR (Used when LUT path empty): " << this->ovlNegSingleMatchQualLUTMaxDR() << std::endl;
  out << " Barrel-Overlap pos MatchQual LUT path: "  << this->bOPosMatchQualLUTPath() << ", max dR (Used when LUT path empty): " << this->bOPosMatchQualLUTMaxDR() << ", max dR when eta-fine bit set: " << this->bOPosMatchQualLUTMaxDREtaFine() << std::endl;
  out << " Barrel-Overlap neg MatchQual LUT path: "  << this->bONegMatchQualLUTPath() << ", max dR (Used when LUT path empty): " << this->bONegMatchQualLUTMaxDR() << ", max dR when eta-fine bit set: " << this->bONegMatchQualLUTMaxDREtaFine() << std::endl;
  out << " Forward-Overlap pos MatchQual LUT path: " << this->fOPosMatchQualLUTPath() << ", max dR (Used when LUT path empty): " << this->fOPosMatchQualLUTMaxDR() << std::endl;
  out << " Forward-Overlap neg MatchQual LUT path: " << this->fONegMatchQualLUTPath() << ", max dR (Used when LUT path empty): " << this->fONegMatchQualLUTMaxDR() << std::endl;
  out << " Barrel phi extrapolation LUT path: "      << this->bPhiExtrapolationLUTPath() << std::endl;
  out << " Overlap phi extrapolation LUT path: "     << this->oPhiExtrapolationLUTPath() << std::endl;
  out << " Forward phi extrapolation LUT path: "     << this->fPhiExtrapolationLUTPath() << std::endl;
  out << " Barrel eta extrapolation LUT path: "      << this->bEtaExtrapolationLUTPath() << std::endl;
  out << " Overlap eta extrapolation LUT path: "     << this->oEtaExtrapolationLUTPath() << std::endl;
  out << " Forward eta extrapolation LUT path: "     << this->fEtaExtrapolationLUTPath() << std::endl;
  out << " Sort rank LUT path: "                     << this->sortRankLUTPath() << ", pT and quality factors (Used when LUT path empty): pT factor: " << this->sortRankLUTPtFactor() << ", quality factor: " << this->sortRankLUTQualFactor() << std::endl;
}
