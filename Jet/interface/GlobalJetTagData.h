#ifndef XTAG_DATAFORMATS_GLOBALJETTAGDATA_H
#define XTAG_DATAFORMATS_GLOBALJETTAGDATA_H

#include "XTag/DataFormats/interface/TagData.h"
#include <iostream>

namespace xtag
{

class GlobalJetTagData:
    public TagData
{
    public:
        std::vector<float> jetPt;
        std::vector<float> jetEta;
        
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
            archive.saveVectorFloat(jetPt,"jetPt");
            archive.saveVectorFloat(jetEta,"jetEta");
        }
        
        virtual ~GlobalJetTagData()
        {
        }
};

}

#endif
