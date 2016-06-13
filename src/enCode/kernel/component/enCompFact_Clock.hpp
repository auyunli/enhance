#ifndef EN_COMPONENT_FACTORY_H
#define EN_COMPONENT_FACTORY_H

#include "enComponentMeta.h"

class enComponentFactory : public enComponentMeta {
public:  
    enComponentFactory : enComponentMeta( enComponentType::FACTORY ){}
    template< class Abstract, class Concrete >
    Abstract * gen_component( Concrete * component ){
	return (Abstract *) component;
    }
};

#endif
