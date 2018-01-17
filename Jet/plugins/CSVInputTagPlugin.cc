#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/EventSetup.h"
#include "FWCore/Framework/interface/ConsumesCollector.h"

#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/Utilities/interface/InputTag.h"

#include "DataFormats/Common/interface/Handle.h"

#include "FWCore/Framework/interface/ProducerBase.h"

#include "DataFormats/BTauReco/interface/ShallowTagInfo.h"
#include "DataFormats/PatCandidates/interface/Jet.h"

#include "XTag/XTagProducer/interface/XTagPlugin.h"
#include "XTag/XTagProducer/interface/XTagPluginFactory.h"
#include "XTag/Jet/interface/CSVInputTagData.h"

#include <iostream>

namespace xtag
{

class CSVInputTagDataPlugin:
    public XTagPlugin
{
    private:
        edm::InputTag inputTag_;
        edm::EDGetTokenT<edm::View<pat::Jet>> token_;
        std::string tagName_;
    public:
        CSVInputTagDataPlugin(
            const std::string& name, 
            const edm::ParameterSet& pset, 
            edm::ConsumesCollector& collector,
            edm::ProducerBase& prod
        ):
            XTagPlugin(name,pset,collector,prod),
            inputTag_(pset.getParameter<edm::InputTag>("src")),
            token_(collector.consumes<edm::View<pat::Jet>>(inputTag_)),
            tagName_(pset.getParameter<std::string>("tagName"))
        {
            prod.produces<std::vector<xtag::CSVInputTagData>>(name);
        }
        
        virtual void produce(edm::Event& event, const edm::EventSetup&) const
        {
            edm::Handle<edm::View<pat::Jet>> jetCollection;
            event.getByToken(token_, jetCollection);
            

            std::unique_ptr<std::vector<xtag::CSVInputTagData>> output(
                new std::vector<xtag::CSVInputTagData>(1)
            );

            for (unsigned int ijet = 0; ijet < jetCollection->size(); ++ijet)
            {
                const pat::Jet& jet = jetCollection->at(ijet);
                const reco::ShallowTagInfo* tagInfo = dynamic_cast<const reco::ShallowTagInfo*>(
                    jet.tagInfo(tagName_)
                );
                if (not tagInfo)
                {
                    std::string availableTagNames = "[";
                    for (auto name: jet.tagInfoLabels())
                    {
                        availableTagNames+=name+",";
                    }
                    availableTagNames+="]";
                    throw cms::Exception("Not shallow tag information '"+tagName_+"' found in jet. Only these tag labels are available: "+availableTagNames);
                }
                reco::TaggingVariableList vars = tagInfo->taggingVariables();
                //http://cmslxr.fnal.gov/source/DataFormats/BTauReco/interface/TaggingVariable.h?v=CMSSW_9_4_0_pre1#0033
                output->at(0).trackSumJetEtRatio.push_back(vars.get(reco::btau::trackSumJetEtRatio, -1));
                output->at(0).trackSumJetDeltaR.push_back(vars.get(reco::btau::trackSumJetDeltaR, -1));
                output->at(0).vertexCategory.push_back(vars.get(reco::btau::vertexCategory, -1));
                output->at(0).jetNSelectedTracks.push_back(vars.get(reco::btau::jetNSelectedTracks, -1));
                output->at(0).jetNTracksEtaRel.push_back(vars.get(reco::btau::jetNTracksEtaRel, -1));
                
                float trackSip2dValAboveCharm = vars.get(reco::btau::trackSip2dValAboveCharm, -10);
                float trackSip2dSigAboveCharm = vars.get(reco::btau::trackSip2dSigAboveCharm, -10);
                //the significances take often the values -1 
                //this maybe a bug and happen if trackSip2dValAboveCharm = -1 thus the reset value is ignored
                //cmslxr.fnal.gov/source/RecoBTag/SecondaryVertex/plugins/TemplatedSecondaryVertexProducer.cc?v=CMSSW_9_4_0
                if (trackSip2dValAboveCharm<0)
                {
                    trackSip2dSigAboveCharm = -10;
                }
                output->at(0).trackSip2dValAboveCharm.push_back(trackSip2dValAboveCharm);
                output->at(0).trackSip2dSigAboveCharm.push_back(trackSip2dSigAboveCharm);
                
                float trackSip3dValAboveCharm = vars.get(reco::btau::trackSip2dValAboveCharm, -10);
                float trackSip3dSigAboveCharm = vars.get(reco::btau::trackSip3dSigAboveCharm, -10);
                if (trackSip3dValAboveCharm<0)
                {
                    trackSip3dSigAboveCharm = -10;
                }
                output->at(0).trackSip3dValAboveCharm.push_back(trackSip3dValAboveCharm);
                output->at(0).trackSip3dSigAboveCharm.push_back(trackSip3dSigAboveCharm);
            }
            
            event.put(std::move(output),this->name());
        }
};

}

DEFINE_EDM_PLUGIN(xtag::XTagPluginFactory, xtag::CSVInputTagDataPlugin, "CSVInputTagData");

