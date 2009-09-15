
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

#ifndef __CMESSAGELISTENERCAMERA_H__
#define __CMESSAGELISTENERCAMERA_H__

#include "../../core/messages/include/listener.h"
#include "camera.h"
#include <boost/weak_ptr.hpp>

namespace Gnoll
{
	namespace Scene
	{
		class Camera;

        /// This is a message's listener for camera
		class CMessageListenerCamera : public Gnoll::Core::Messages::Listener
		{
		protected:
			weak_ptr<Camera> m_pCamera;

		public:
			/**
			 * This is a constructor
			 */
			CMessageListenerCamera();

			/**
			 * This is a destructor
			 */
			~CMessageListenerCamera();

			/**
			 * This method set the camera of the listener
			 * @param pCamera This is a pointer to the camera that can be affect by
			 * 				  message
			 */
			void setCamera(shared_ptr<Camera> pCamera);

			/**
			 * This method is called in order to process a message
			 * @param message The message this method will have to process
			 */
			void handle ( shared_ptr<Gnoll::Core::CMessage> message );
		};
	};
};
#endif // __CMESSAGELISTENERCAMERA_H__
