/**
 * \class L1TGtObjectMapRecord
 * 
 * 
 * Description: see header file.  
 *
 * Implementation:
 *    <TODO: enter implementation details>
 *   
 * \author: Vasile Mihai Ghete - HEPHY Vienna
 * 
 *
 */

// this class header
#include "DataFormats/L1TGlobal/interface/L1TGtObjectMapRecord.h"

// system include files

#include <algorithm>

// user include files
#include "FWCore/MessageLogger/interface/MessageLogger.h"

// forward declarations



// methods

/// return the object map for the algorithm algoNameVal
const L1TGtObjectMap* L1TGtObjectMapRecord::getObjectMap(
    const std::string& algoNameVal) const {

    for (std::vector<L1TGtObjectMap>::const_iterator 
        itObj = m_gtObjectMap.begin(); itObj != m_gtObjectMap.end(); ++itObj) {

        if (itObj->algoName() == algoNameVal) {

            return &((*itObj));
        }

    }

    // no algoName found, return zero pointer!
    edm::LogError("L1TGtObjectMapRecord")
        << "\n\n  ERROR: The requested algorithm name = " << algoNameVal
        << "\n  does not exists in the trigger menu."
        << "\n  Returning zero pointer for getObjectMap\n\n" << std::endl;

    return 0;

}
    
/// return the object map for the algorithm with bit number const int algoBitNumberVal
const L1TGtObjectMap* L1TGtObjectMapRecord::getObjectMap(
    const int algoBitNumberVal) const {
 
    for (std::vector<L1TGtObjectMap>::const_iterator 
        itObj = m_gtObjectMap.begin(); itObj != m_gtObjectMap.end(); ++itObj) {

        if (itObj->algoBitNumber() == algoBitNumberVal) {

            return &((*itObj));
        }

    }

    // no algoBitNumberVal found, return zero pointer!
    edm::LogError("L1TGtObjectMapRecord")
        << "\n\n  ERROR: The requested algorithm with bit number = " << algoBitNumberVal
        << "\n  does not exists in the trigger menu."
        << "\n  Returning zero pointer for getObjectMap\n\n" << std::endl;

    return 0;
    
}

// return all the combinations passing the requirements imposed in condition condNameVal
// from algorithm algoNameVal
const CombinationsInCond* L1TGtObjectMapRecord::getCombinationsInCond(
    const std::string& algoNameVal, const std::string& condNameVal) const
{

    for (std::vector<L1TGtObjectMap>::const_iterator itObj = m_gtObjectMap.begin();
            itObj != m_gtObjectMap.end(); ++itObj) {

        if ( itObj->algoName() == algoNameVal ) {

            return itObj->getCombinationsInCond(condNameVal);
            
        }
    }

    // no (algoName, condName) found, return zero pointer!
    edm::LogError("L1TGtObjectMapRecord")
    << "\n\n  ERROR: The requested \n    (algorithm name, condition name) = ("
    << algoNameVal << ", " << condNameVal
    << ") \n  does not exists in the trigger menu."
    << "\n  Returning zero pointer for getCombinationsInCond\n\n"
    << std::endl;

    return 0;

}

// return all the combinations passing the requirements imposed in condition condNameVal
// from algorithm with bit number algoBitNumberVal
const CombinationsInCond* L1TGtObjectMapRecord::getCombinationsInCond(
    const int algoBitNumberVal, const std::string& condNameVal) const
{

    for (std::vector<L1TGtObjectMap>::const_iterator itObj = m_gtObjectMap.begin();
            itObj != m_gtObjectMap.end(); ++itObj) {

        if ( itObj->algoBitNumber() == algoBitNumberVal ) {
            return itObj->getCombinationsInCond(condNameVal);
        }
    }

    // no (algoBitNumber, condName) found, return zero pointer!
    edm::LogError("L1TGtObjectMapRecord")
    << "\n\n  ERROR: The requested \n    (algorithm bit number, condition name) = ("
    << algoBitNumberVal << ", " << condNameVal
    << ") \n  does not exists in the trigger menu."
    << "\n  Returning zero pointer for getCombinationsInCond\n\n"
    << std::endl;

    return 0;

}

// return the result for the condition condNameVal
// from algorithm with name algoNameVal
bool L1TGtObjectMapRecord::getConditionResult(
    const std::string& algoNameVal, const std::string& condNameVal) const
{

    for (std::vector<L1TGtObjectMap>::const_iterator itObj = m_gtObjectMap.begin();
            itObj != m_gtObjectMap.end(); ++itObj) {

        if ( itObj->algoName() == algoNameVal ) {
            return itObj->getConditionResult(condNameVal);
        }
    }

    // no (algoName, condName) found, return false!
    edm::LogError("L1TGtObjectMapRecord")
    << "\n\n  ERROR: The requested \n    (algorithm name, condition name) = ("
    << algoNameVal << ", " << condNameVal
    << ") \n  does not exists in the trigger menu."
    << "\n  Returning false for condition result! Unknown result, in fact!\n\n"
    << std::endl;

    return false;

}

// return the result for the condition condNameVal
// from algorithm with bit number algoBitNumberVal
bool L1TGtObjectMapRecord::getConditionResult(
    const int algoBitNumberVal, const std::string& condNameVal) const
{

    for (std::vector<L1TGtObjectMap>::const_iterator itObj = m_gtObjectMap.begin();
            itObj != m_gtObjectMap.end(); ++itObj) {

        if ( itObj->algoBitNumber() == algoBitNumberVal ) {
            return itObj->getConditionResult(condNameVal);
        }
    }

    // no (algoBitNumber, condName) found, return false!
    edm::LogError("L1TGtObjectMapRecord")
    << "\n\n  ERROR: The requested \n    (algorithm bit number, condition name) = ("
    << algoBitNumberVal << ", " << condNameVal
    << ") \n  does not exists in the trigger menu."
    << "\n  Returning false for condition result! Unknown result, in fact!\n\n"
    << std::endl;

    return false;

}
