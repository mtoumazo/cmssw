#include "FWCore/Framework/interface/MakerMacros.h"

#include "EventFilter/L1TRawToDigi/interface/Unpacker.h"

#include "CaloCollections.h"

namespace l1t {
   class EtSumUnpacker : public Unpacker {
      public:
         virtual bool unpack(const unsigned block_id, const unsigned size, const unsigned char *data, UnpackerCollections *coll) override;
   };
}

// Implementation

namespace l1t {
   bool
   EtSumUnpacker::unpack(const unsigned block_id, const unsigned size, const unsigned char *data, UnpackerCollections *coll)
   {

     LogDebug("L1T") << "Block ID  = " << block_id << " size = " << size;

     int nBX = int(ceil(size / 4)); // Since there are 4 EtSum objects reported per event (see CMS IN-2013/005)

     // Find the central, first and last BXs
     int firstBX = -(ceil((double)nBX/2.)-1);
     int lastBX;
     if (nBX % 2 == 0) {
       lastBX = ceil((double)nBX/2.);
     } else {
       lastBX = ceil((double)nBX/2.)-1;
     }

     auto res_ = static_cast<CaloCollections*>(coll)->getEtSums();
     res_->setBXRange(firstBX, lastBX);

     LogDebug("L1T") << "nBX = " << nBX << " first BX = " << firstBX << " lastBX = " << lastBX;

     // Initialise index
     int unsigned i = 0;

     // Loop over multiple BX and fill EtSums collection
     for (int bx=firstBX; bx<lastBX; bx++){

       // ET

       uint32_t raw_data = pop(data,i); // pop advances the index i internally

       l1t::EtSum et = l1t::EtSum();
    
       et.setHwPt(raw_data & 0xFFF);
       et.setType(l1t::EtSum::kTotalEt);       

       LogDebug("L1T") << "ET: pT " << et.hwPt();

       res_->push_back(bx,et);

       // HT

       raw_data = pop(data,i); // pop advances the index i internally

       l1t::EtSum ht = l1t::EtSum();
    
       ht.setHwPt(raw_data & 0xFFF);
       ht.setType(l1t::EtSum::kTotalHt);       

       LogDebug("L1T") << "HT: pT " << ht.hwPt();

       res_->push_back(bx,ht);

       //  MET

       raw_data = pop(data,i); // pop advances the index i internally

       l1t::EtSum met = l1t::EtSum();
    
       met.setHwPt(raw_data & 0xFFF);
       met.setHwPhi((raw_data >> 12) & 0xFF);
       met.setType(l1t::EtSum::kMissingEt);       

       LogDebug("L1T") << "MET: phi " << met.hwPhi() << " pT " << met.hwPt();

       res_->push_back(bx,met);

       // MHT

       raw_data = pop(data,i); // pop advances the index i internally

       l1t::EtSum mht = l1t::EtSum();
    
       mht.setHwPt(raw_data & 0xFFF);
       mht.setHwPhi((raw_data >> 12) & 0xFF);
       mht.setType(l1t::EtSum::kMissingHt);       

       LogDebug("L1T") << "MHT: phi " << mht.hwPhi() << " pT " << mht.hwPt();

       res_->push_back(bx,mht);       
     }

     return true;
   }
}

DEFINE_L1T_UNPACKER(l1t::EtSumUnpacker);
