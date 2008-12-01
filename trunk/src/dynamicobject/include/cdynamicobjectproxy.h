/***************************************************************************
 *   Copyright (C) 2007 by Paf                                             *
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


/*---------------------------CDynamicObjectProxy---------------------------*\
|   This is a DynamicObject proxy                                           |
|                                                                           |
|   Changelog :                                                             |
|               12/14/2007 - Bruno Mahe - Initial release                   |
|               02/15/2008 - Bruno Mahe - Make destructor virtual           |
|               02/15/2008 - Bruno Mahe - Add a new method                  |
|                                          getAttributeOrDefault()          |
|                                                                           |
\*-------------------------------------------------------------------------*/


#ifndef __CDYNAMICOBJECTPROXY_H__
#define __CDYNAMICOBJECTPROXY_H__


#include <boost/shared_ptr.hpp>
#include <libxml++/libxml++.h>
#include <glibmm/ustring.h>


#include "iattribute.h"
#include "iattributehandler.h"
#include "dynamicobject.h"

#include "string.h"
#include "list.h"


using namespace std;
using namespace boost;

namespace Gnoll
{
	namespace DynamicObject
	{


		/**
		 *	This is a proxy for Dynamic Object.
		 *	This object can be used as a base for objects using DynamicObject capabilities </br>
		 * @see IAttribute </br>
		 * @see DynamicObject
		 */
		class CDynamicObjectProxy
		{


			protected:
				shared_ptr<DynamicObject> m_self;



			public:

				/**
				 * This is a constructor
				 * @param _instanceName Instance name of the DynamicObject
				 */
				CDynamicObjectProxy (string _instanceName);


				/**
				 * This is a destructor
				 */
				virtual ~CDynamicObjectProxy ();

				/**
				 * Save the instance
				 */
				void save();

				/**
				 * This sets an attribute
				 * @param _name The name of the attribute
				 * @param _value The attribute itself
				 */
				void setAttribute( const Glib::ustring _name, shared_ptr<IAttribute> _value );


				/**
				 * This return an attribute
				 * @param _name The name of the attribute we want
				 * @return The attribute
				 */
				template< class T > shared_ptr<T> getAttribute( const Glib::ustring _name  ) const
				{
					return m_self->getAttribute<T>(_name);
				}


				/**
				 * This return an attribute. If the attribute doesn't exists, it will<br/>
				 * return a default value
				 * @param _name The name of the attribute we want
				 * @return The attribute if it exists, or a default value
				 */
				template< class T >
						shared_ptr<T> getAttributeOrDefault( const Glib::ustring _name, shared_ptr<T> _defaultValue = shared_ptr<T>()  )
				{
					return m_self->getAttributeOrDefault<T>(_name, _defaultValue);
				}


				/**
				 * Check if an attribute exists
				 * @param _name Name of the attribute
				 * @return It returns true if an attribute with such a name exists, or false if it doesn't
				 */
				bool hasAttribute ( const Glib::ustring & _name ) const;


				/**
				 * Delete an attribute
				 * @param _name Name of the attribute to delete
				 */
				void deleteAttribute ( Glib::ustring _name );


				/**
				 * Delete all the attributes
				 */
				void deleteAllAttributes ( void ) ;

				/**
				 * Get a list of attributes names
				 * @return A list of all attribute's names
				 */
				List getAttributesNames(void);

				/**
				 * Get a const_iterator on the first element of the map of the attributes
				 * @return const_iterator at the beginning of the map of attributes
				 */
				DynamicObject::mapAttributes::const_iterator begin() const;

				/**
				 * Get a const_iterator on the end of the map of the attributes
				 * @return const_iterator at the end of the map of attributes
				 */
				DynamicObject::mapAttributes::const_iterator end() const;

				/**
				 * Get the instance name of the DynamicObject
				 * @return The instance name
				 */
				string getInstance() const;

		};
	}
}

#endif // __CDYNAMICOBJECTPROXY_H__
