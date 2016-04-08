#ifndef L1Trigger_L1TGlobal_CorrCondition_h
#define L1Trigger_L1TGlobal_CorrCondition_h

/**
 * \class CorrCondition
 * 
 * 
 * Description: evaluation of a correlation condition.
 * 
 * Implementation:
 *    <TODO: enter implementation details>
 *   
 * \author: Vasile Mihai Ghete   - HEPHY Vienna 
 * 
 *
 */

// system include files
#include <iosfwd>
#include <string>

// user include files
//   base classes
#include "L1Trigger/L1TGlobal/interface/ConditionEvaluation.h"
#include "L1Trigger/L1TGlobal/interface/L1TGlobalScales.h"

// forward declarations
class GtCondition;
class CorrelationTemplate;

namespace l1t {

class L1Candidate;

class GtBoard;

// class declaration
class CorrCondition : public ConditionEvaluation
{

public:

    /// constructors
    ///     default
    CorrCondition();

    ///     from base template condition (from event setup usually)
    CorrCondition(const GtCondition*,
                  const GtCondition*,
		  const GtCondition*, 
                  const GtBoard*

            );

    // copy constructor
    CorrCondition(const CorrCondition&);

    // destructor
    virtual ~CorrCondition();

    // assign operator
    CorrCondition& operator=(const CorrCondition&);

public:

    /// the core function to check if the condition matches
    const bool evaluateCondition(const int bxEval) const;

    /// print condition
     void print(std::ostream& myCout) const;

public:

    ///   get / set the pointer to a Condition
    inline const CorrelationTemplate* gtCorrelationTemplate() const {
        return m_gtCorrelationTemplate;
    }

    void setGtCorrelationTemplate(const CorrelationTemplate*);

    ///   get / set the pointer to uGt GtBoard
    inline const GtBoard* getuGtB() const {
        return m_uGtB;
    }

    void setuGtB(const GtBoard*);
    
    void setScales(const L1TGlobalScales*);  

/*   //BLW Comment out for now
    ///   get / set the number of bits for eta of calorimeter objects
    inline const int gtIfCaloEtaNumberBits() const {
        return m_ifCaloEtaNumberBits;
    }


    void setGtIfCaloEtaNumberBits(const int&);

    ///   get / set maximum number of bins for the delta phi scales
    inline const int gtCorrParDeltaPhiNrBins() const {
        return m_corrParDeltaPhiNrBins;
    }

    void setGtCorrParDeltaPhiNrBins(const int&);
*/
private:

    ///  copy function for copy constructor and operator=
    void copy(const CorrCondition& cp);

    /// load  candidates
    const l1t::L1Candidate* getCandidate(const int bx, const int indexCand) const;

    /// function to check a single object if it matches a condition
    const bool
    checkObjectParameter(const int iCondition, const l1t::L1Candidate& cand) const;

private:

    /// pointer to a CorrelationTemplate
    const CorrelationTemplate* m_gtCorrelationTemplate;


    // pointer to subconditions
    const GtCondition* m_gtCond0;
    const GtCondition* m_gtCond1;

    /// pointer to uGt GtBoard, to be able to get the trigger objects
    const GtBoard* m_uGtB;
    
    const L1TGlobalScales* m_gtScales;


/*   //BLW comment out for now
    /// number of bits for eta of calorimeter objects
    int m_ifCaloEtaNumberBits;

    // maximum number of bins for the delta phi scales
    unsigned int m_corrParDeltaPhiNrBins;
*/


};

}
#endif
