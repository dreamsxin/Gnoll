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


/*-------------------------AttributeHandlerRegistry------------------------*\
|   This is the Attributes Handler Registry. Each attribute handler is      |
|     required to register. So when we want to handle an attribute          |
|     we can retrive its handler quite easily                               |
|                                                                           |
|   Changelog :                                                             |
|               08/10/2007 - Paf - Initial release                          |
|               08/11/2007 - Paf - Add some comments, removed unused        |
|                                    libxml++ include                       |
|               09/25/2007 - Paf - Add float.h                              |
|                                - Replace namespace Viracocha by Gnoll     |
|               09/26/2007 - Paf - Add string.h                             |
|                                - Add double.h                             |
|               09/28/2007 - Paf - Add list.h                               |
|                                                                           |
\*-------------------------------------------------------------------------*/


#ifndef __ATTRIBUTEHANDLERREGISTRY_H__
#define __ATTRIBUTEHANDLERREGISTRY_H__


#include <boost/shared_ptr.hpp>
#include <string>

#include "iattributehandler.h"
#include "../../core/include/singleton.h"




using namespace boost;
using namespace std;
using namespace Gnoll::Core;


namespace Gnoll
{

	namespace DynamicObject
	{

		/**
		*	This is where an attribute has its handler registered.</br>
		*	This is a Singleton.
		*/ 
		class AttributeHandlerRegistry: public Singleton<AttributeHandlerRegistry>
		{

			private:

				/**
				 * This the map that associates an attribute handler to its attribute
				 */
				map<string, shared_ptr<IAttributeHandler> > m_handlersMap;

			public:

				/**
				* This is a constructor.
				*/
				AttributeHandlerRegistry();


				/**
				* This is a destructor.
				*/
				~AttributeHandlerRegistry();


				/**
				 * This method provides a way to register an attribute handler
				 * @param _attributeType The attribute to associate the attribute handler with
				 * @param _handler The attributehHandler
				 */	
				void registerHandler(string _attributeType, shared_ptr<IAttributeHandler> _handler);


				/**
				 *	This method unregisters an attribute handler
				 *	@param _attributeType The attribute that is going to be unregistered
				 */
				void unregisterHandler(string _attributeType);

				/**
				 * This method returns the attribute handler associated to an attribute type
				 * @param _attributeType The attribute type
				 */
				shared_ptr<IAttributeHandler> getHandler(string _attributeType);

		};

	}
}

#endif // __IATTRIBUTEHANDLERREGISTRY_H__
