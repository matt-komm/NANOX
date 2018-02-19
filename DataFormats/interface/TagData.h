#ifndef XTAG_DATAFORMATS_TAGDATA_H
#define XTAG_DATAFORMATS_TAGDATA_H

#include "FWCore/Utilities/interface/Exception.h"

#include <string>
#include <vector>
#include <memory>
#include <iostream> 

#include "TObject.h"
#include <unordered_map>

namespace xtag
{

class TagData;
class ArrayInterface;

class PropertyContainer
{
    public:
        virtual ~PropertyContainer()
        {
        }
};

class Accessor
{
    public:
        virtual void fill(const PropertyContainer* property, const std::string& name, ArrayInterface& array, unsigned int index) = 0;
};

class ArrayInterface
{
    protected:
        std::unordered_map<std::string, std::shared_ptr<xtag::Accessor>> accessors_;
    public:
        virtual unsigned int size() const = 0;
        virtual void bookProperty(const std::string& name) = 0;
        virtual void fillFloat(const std::string& name, float value, unsigned int index) = 0;
        
        //for convenience
        virtual void bookProperty(const std::string& name, std::shared_ptr<Accessor> acc)
        {
            accessors_[name]=acc;
            this->bookProperty(name);
        }
        
        virtual void fill(const PropertyContainer* property, unsigned int index)
        {
            for (auto itPair: accessors_)
            {
                itPair.second->fill(property,itPair.first,*this,index);
            }
        }
        
        template<class PROPERTY, class TYPE> void bookProperty(const std::string& name,const TYPE PROPERTY::*data);
};

template<class PROPERTY, class TYPE>
class AccessorTmpl:
    public Accessor
{
    public:
        const TYPE PROPERTY::*data_;
        AccessorTmpl(const TYPE PROPERTY::*data):
            data_(data)
        {
        }
        virtual void fill(const PropertyContainer* property, const std::string& name, ArrayInterface& array, unsigned int index)
        {
            const PROPERTY* obj = dynamic_cast<const PROPERTY*>(property);
            if (not obj) throw cms::Exception("Cannot cast property object to type "+std::string(typeid(PROPERTY).name()));
            array.fillFloat(name,obj->*data_, index);
        }
};

template<class PROPERTY, class TYPE> void ArrayInterface::bookProperty(const std::string& name,const TYPE PROPERTY::*data)
{
    this->bookProperty(
        name, 
        std::shared_ptr<Accessor>(new AccessorTmpl<PROPERTY,TYPE>(data))
    );
}


class ArchiveInterface
{
    public:
        virtual ArrayInterface& initArray(
            const std::string& name,
            unsigned int size
        ) = 0;
};



class TagData
{
    protected:
    public:
        virtual void saveTagData(ArchiveInterface& archive) const
        {
        } 
        virtual ~TagData()
        {
        }
};

}

#endif
