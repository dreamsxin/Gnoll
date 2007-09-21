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


/*-------------------------------cmessage----------------------------------*\
|   This is the interface of all the attributes. Each Attribute has to be   |
|     (de)serializable                                                      |
|                                                                           |
|   Changelog :                                                             |
|               08/02/2007 - Paf - Initial release                          |
|                                                                           |
\*-------------------------------------------------------------------------*/


#include "../include/attributehandlerregistry.h"



namespace Viracocha 
{

	namespace Core
	{

		AttributeHandlerRegistry::AttributeHandlerRegistry()
		{
			// Default handlers are added here

			this->registerHandler(string("integer"), shared_ptr<IAttributeHandler>(new GenericAttributeHandler<Integer>()));

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
