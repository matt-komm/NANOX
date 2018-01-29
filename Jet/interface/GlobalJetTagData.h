#ifndef XTAG_DATAFORMATS_GLOBALJETTAGDATA_H
#define XTAG_DATAFORMATS_GLOBALJETTAGDATA_H

#include "XTag/DataFormats/interface/TagData.h"
#include <iostream>

namespace xtag
{


class GlobalJetTagData:
    public TagData
{
    class JetData:
        public Property
    {
        public:
            float pt;
            float eta;
            JetData(float pt, float eta):
                pt(pt),
                eta(eta)
            {
            }
            
        
    };
    
    public:
        std::vector<JetData> jetData;
        
        /*
        'jet_pt',
        'jet_eta',
        'nCpfcand',
        'nNpfcand',
        'nsv',
        'npv',
        'TagVarCSV_trackSumJetEtRatio', 
        'TagVarCSV_trackSumJetDeltaR', 
        'TagVarCSV_vertexCategory', 
        'TagVarCSV_trackSip2dValAboveCharm', 
        'TagVarCSV_trackSip2dSigAboveCharm', 
        'TagVarCSV_trackSip3dValAboveCharm', 
        'TagVarCSV_trackSip3dSigAboveCharm', 
        'TagVarCSV_jetNSelectedTracks', 
        'TagVarCSV_jetNTracksEtaRel'
        */
        
        virtual void saveTagData(ArchiveInterface& archive) const override
        {
            ArrayInterface& array = archive.bookArray("global",jetData.size());
            array.bookFloat("pt", new xtag::AccessorTmpl<JetData,float>(&JetData::pt));
            for (auto data: jetData)
            {
                array.fill(&data);
            }
        }
        
        virtual ~GlobalJetTagData()
        {
        }
};

}

#endif
