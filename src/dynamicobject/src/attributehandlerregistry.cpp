/***************************************************************************
 *   Copyright (C) 2006 by Puzzle Team                                     *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
 ***************************************************************************/


/*---------------------------AttributeHandlerRegistry----------------------*\
|   This is the interface of all the attributes. Each Attribute has to be   |
|     (de)serializable                                                      |
|                                                                           |
|   Changelog :                                                             |
|               08/02/2007 - Paf - Initial release                          |
|               09/25/2007 - Paf - Replace namespace Viracocha by Gnoll     |
|                                - Add float attribute handler              |
|               09/26/2007 - Paf - Add string attribute handler             |
|                                - Add double attribute handler             |
|               09/28/2007 - Paf - Add list attribute handler               |
|               10/17/2007 - Paf - Add inherits attribute handler           |
|               12/17/2007 - Paf - Add attribute handler for                |
|                                    CMeshPageRenderer                      |
|                                                                           |
\*-------------------------------------------------------------------------*/


#include "../include/attributehandlerregistry.h"

// Default Handlers
#include "../include/genericattributehandler.h"
#include "../include/inheritsattributehandler.h"

#include "../include/integer.h"
#include "../include/float.h"
#include "../include/string.h"
#include "../include/double.h"
#include "../include/vector3.h"
#include "../include/list.h"
#include "../include/set.h"
#include "../include/inherits.h"
#include "../../scene/include/cmeshpagerenderer.h"
#include "../../scene/include/cstaticgobject.h"



namespace Gnoll
{

	namespace DynamicObject
	{

		AttributeHandlerRegistry::AttributeHandlerRegistry()
		{
			// Default handlers are added here
			this->registerHandler(string("Vector3"), shared_ptr<IAttributeHandler>(new GenericAttributeHandler<Vector3>()));
			this->registerHandler(string("integer"), shared_ptr<IAttributeHandler>(new GenericAttributeHandler<Integer>()));
			this->registerHandler(string("float"), shared_ptr<IAttributeHandler>(new GenericAttributeHandler<Float>()));
			this->registerHandler(string("string"), shared_ptr<IAttributeHandler>(new GenericAttributeHandler<String>()));
			this->registerHandler(string("double"), shared_ptr<IAttributeHandler>(new GenericAttributeHandler<Double>()));
			this->registerHandler(string("list"), shared_ptr<IAttributeHandler>(new GenericAttributeHandler<List>()));
			this->registerHandler(string("set"), shared_ptr<IAttributeHandler>(new GenericAttributeHandler<Set>()));
			this->registerHandler(string("inherits"), shared_ptr<IAttributeHandler>(new InheritsAttributeHandler()));
			this->registerHandler(string("CMeshPageRenderer"), shared_ptr<IAttributeHandler>(new GenericAttributeHandler<Gnoll::Scene::CMeshPageRenderer>()));
			this->registerHandler(string("cstaticgobject"), shared_ptr<IAttributeHandler>(new GenericAttributeHandler<Gnoll::Scene::CStaticGObject>()));

		}


		AttributeHandlerRegistry::~AttributeHandlerRegistry()
		{

		}



		void AttributeHandlerRegistry::registerHandler(string _attributeType, shared_ptr<IAttributeHandler> _handler)
		{
			m_handlersMap[_attributeType] = _handler;
		}


		void AttributeHandlerRegistry::unregisterHandler(string _attributeType)
		{
			m_handlersMap.erase(_attributeType);
		}


		shared_ptr<IAttributeHandler> AttributeHandlerRegistry::getHandler(string _attributeType)
		{
			return m_handlersMap[_attributeType];
		}

	}
}

