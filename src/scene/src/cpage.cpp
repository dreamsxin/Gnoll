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


/*--------------------------------CPage------------------------------------*\
|   This is a page. A page is a chunk of the scene.                         |
|                                                                           |
|                                                                           |
\*-------------------------------------------------------------------------*/

#include "../../graphic/include/cgraphicmodule.h"
#include "../../log/include/logmodule.h"
#include "../../dynamicobject/include/dynamicobjectmanager.h"
#include "../../dynamicobject/include/float.h"
#include "../include/cpage.h"
#include "../include/ipagerenderer.h"
#include "../include/cstaticgobject.h"
#include "../include/gobject.h"
#include <OgreCamera.h>


using namespace std;
using namespace boost;
using namespace Gnoll::Graphic;

namespace Gnoll
{
	namespace Scene
	{


		CPage::CPage(string _instanceName): DynamicObjectProxy(_instanceName)
		{
		}

		void CPage::init()
		{
			Ogre::SceneManager* sm = CGraphicModule::getInstancePtr()->getSceneManager();
			SceneNode* root = sm->getRootSceneNode();

			// Creating root page node
			root->createChildSceneNode( this->getInstance() );


			/**
			 * Load the page renderer
			 */
			if (this->hasAttribute( CPage::ATTRIBUTE_PAGE_RENDERER() ))
			{
				Gnoll::Log::LogModule::getInstancePtr()->logMessage( "From " + this->getInstance() + " // Initializing PageRenderer" );
				shared_ptr<IPageRenderer> pRenderer = this->getAttribute<IPageRenderer>( CPage::ATTRIBUTE_PAGE_RENDERER() );
				pRenderer->init(this);
			}


			/**
			 * Load game objects
			 */
			if (this->hasAttribute( CPage::ATTRIBUTE_GAME_OBJECTS() ))
			{
				shared_ptr< Gnoll::DynamicObject::List > listGameObjects = this->getAttribute < Gnoll::DynamicObject::List > ( CPage::ATTRIBUTE_GAME_OBJECTS() );

				typedef list< shared_ptr<Gnoll::DynamicObject::AbstractAttribute> >::iterator ListIterator;

				for( ListIterator it = listGameObjects->begin(); it != listGameObjects->end(); it++)
				{
					if (shared_ptr<Gnoll::Scene::GObject> gObject = dynamic_pointer_cast<Gnoll::Scene::GObject>(*it))
					{
						gObject->init(this);
					}
				}
			}
		}

		void CPage::unInit()
		{
			Gnoll::Log::LogModule::getInstancePtr()->logMessage( "!!!!!!!!!!!!!!!! DE-INIT OF PAGE " + this->getInstance() );

			if (this->isInitialized())
			{
				Ogre::SceneManager* sm = CGraphicModule::getInstancePtr()->getSceneManager();

				/**
				 * Unload static objects
				 */
				if (this->hasAttribute( CPage::ATTRIBUTE_GAME_OBJECTS() ))
				{
					shared_ptr< Gnoll::DynamicObject::List > listGameObjects = this->getAttribute < Gnoll::DynamicObject::List > ( CPage::ATTRIBUTE_GAME_OBJECTS() );

					typedef list< shared_ptr<Gnoll::DynamicObject::AbstractAttribute> >::iterator ListIterator;

					for( ListIterator it = listGameObjects->begin(); it != listGameObjects->end(); it++)
					{
						if (shared_ptr<Gnoll::Scene::GObject> gObject = dynamic_pointer_cast<Gnoll::Scene::GObject>(*it))
						{
							gObject->exit();
						}
					}
				}


				if (this->hasAttribute( CPage::ATTRIBUTE_PAGE_RENDERER() ))
				{
					shared_ptr<IPageRenderer> pRenderer = this->getAttribute<IPageRenderer>( CPage::ATTRIBUTE_PAGE_RENDERER() );
					pRenderer->exit();
				}

				SceneNode* node = sm->getSceneNode( this->getInstance() );
				node->removeAndDestroyAllChildren();
			}
		}


		CPage::~CPage()
		{
		}


		Ogre::SceneNode * CPage::getPageRootNode() const
		{
			Ogre::SceneManager* sm = CGraphicModule::getInstancePtr()->getSceneManager();
			SceneNode* node = NULL;

			if ( sm->hasSceneNode(this->getInstance()) )
			{
				node = sm->getSceneNode( this->getInstance() );
			}

			return node;
		}

		bool CPage::isInitialized()
		{
			bool result = false;

			/**
			 * The way to check if a page has been initialized, is to check if the Page Root Node has
			 * been created.
			 */
			if (this->getPageRootNode())
			{
				result = true;
			}

			return result;
		}


		bool CPage::isVisibleFromCamera(shared_ptr< Gnoll::DynamicObject::String > _cameraName) const
		{
			// XXX TODO
			// Get page root node coordinates
			// get page length
			// Compute AABB box
			// Check with the current camera if the AABB box is inside the view frustrum

			Gnoll::Log::LogModule::getInstancePtr()->logMessage( "       Checking visibility from camera : " + string(*_cameraName) + " for page " + this->getInstance());
			Ogre::Camera* ogreCamera = CGraphicModule::getInstancePtr()->getSceneManager()->getCamera(*_cameraName);
			Ogre::SceneNode* rootNode = this->getPageRootNode();

			bool result = false;

			/**
			 * Two cases :
			 * 1/ This page is initialized and we can directly use the Ogre::SceneNode
			 * 2/ This page is not initialized => we need to find an initialized neighbor
			 */
			if (rootNode)
			{
				std::ostringstream tmpString;
				tmpString << "        root node available (" <<  this->getInstance() << ")" << endl;
				tmpString << "         Number of children : " << rootNode->numChildren() << endl;
				Gnoll::Log::LogModule::getInstancePtr()->logMessage( tmpString.str() );
				tmpString.clear();

				//const Ogre::AxisAlignedBox aabb = rootNode->_getWorldAABB();
				Ogre::AxisAlignedBox aabb;


				//////////////////////////////////////////////////

				/**
				 * The OctreeSceneManager doesn't take children nodes in account
				 * when computing AABB box even though this is taken in account by
				 * Ogre::SceneNode.
				 * So this is computed by hand like does Ogre::SceneNode.
				 */

				// Update bounds from own attached objects
				SceneNode::ObjectIterator oi = rootNode->getAttachedObjectIterator();
				while (oi.hasMoreElements())
				{
					MovableObject *m = oi.getNext();

					// Merge world bounds of each object
					aabb.merge(m->getWorldBoundingBox());
				}

				// Merge with children
				Node::ChildNodeIterator oit = rootNode->getChildIterator();
				while (oit.hasMoreElements())
				{
					SceneNode* sceneChild = static_cast<SceneNode*>(oit.getNext());
					aabb.merge(sceneChild->_getWorldAABB());
				}

				////////////////////////////////////////

				Ogre::Vector3 min = aabb.getMinimum();
				Ogre::Vector3 max = aabb.getMaximum();


				tmpString << "         AABB Min : " <<  min  << endl;
				tmpString << "         AABB Max : " <<  max  << endl;

				tmpString << "         AABB : " << aabb << endl;

				if (aabb.isNull())
				{
					tmpString << "         AABB IS NULL !!!" << endl;
				}
				tmpString << "         Camera : " << *ogreCamera << endl;

				Gnoll::Log::LogModule::getInstancePtr()->logMessage( tmpString.str() );

				result = ogreCamera->isVisible(aabb);


			} else
			{
				/**
				 * We need to check if any direct neighbor is initialized
				 */

				Gnoll::Log::LogModule::getInstancePtr()->logMessage( "        root node unavailable (" + this->getInstance() + ")" );;
				Gnoll::Log::LogModule::getInstancePtr()->logMessage( "        looking at neighbors" );

				// Find an initialized neighbor (return false if no neighbor is found)
				// Get its world position
				// Add offset to get current page world position
				// Construct an Ogre::AxisAlignedBox
				// Check if visible from ogrecamera
				const char* neighbors[] = {
											CPage::ATTRIBUTE_NORTH_LINK(),
											CPage::ATTRIBUTE_SOUTH_LINK(),
											CPage::ATTRIBUTE_EAST_LINK(),
											CPage::ATTRIBUTE_WEST_LINK()
										};

				DynamicObjectManager *pom = DynamicObjectManager::getInstancePtr();


				shared_ptr< Gnoll::DynamicObject::String > loadedNeighbor;

				/**
				 * Loop through direct neighbors
				 */
				std::ostringstream tmpString;
				for (unsigned int i = 0; (i < 4) && (loadedNeighbor.get() == NULL); i++)
				{

					Gnoll::Log::LogModule::getInstancePtr()->logMessage( "                Looking at neighbor from " + string(neighbors[i]) );

					if (this->hasAttribute( neighbors[i] ))
					{

						shared_ptr< Gnoll::DynamicObject::String > neighbor = this->getAttribute< Gnoll::DynamicObject::String >( neighbors[i] );

						tmpString << "                Looking at neighbor " << *neighbor << " from " << neighbors[i] << endl;
						Gnoll::Log::LogModule::getInstancePtr()->logMessage( tmpString.str() );
						tmpString.clear();

						if (pom->isInstanceInCache (*neighbor) )
						{
							CPage neighborPage(*neighbor);
							if (neighborPage.isInitialized())
							{
								loadedNeighbor = neighbor;
								Gnoll::Log::LogModule::getInstancePtr()->logMessage( "Youpi !" );
							}
						}
					}
				}


				/**
				 * We have found an initialized neighbor
				 */
				if (loadedNeighbor)
				{
					CPage neighborPage(*loadedNeighbor);
					Gnoll::Log::LogModule::getInstancePtr()->logMessage( "        neighbor found : " +  neighborPage.getInstance() );
					const char* neighborStr = string(*loadedNeighbor).c_str();

					Ogre::SceneNode* rootNode = neighborPage.getPageRootNode();

					//const Ogre::AxisAlignedBox aabbOrig = rootNode->_getWorldAABB();
					Ogre::AxisAlignedBox aabbOrig;

					//////////////////////////////////////////////////

					/**
					 * The OctreeSceneManager doesn't take children nodes in account
					 * when computing AABB box even though this is taken in account by
					 * Ogre::SceneNode.
					 * So this is computed by hand like does Ogre::SceneNode.
					 */


					// Update bounds from own attached objects
					SceneNode::ObjectIterator oi = rootNode->getAttachedObjectIterator();
					while (oi.hasMoreElements())
					{
						MovableObject *m = oi.getNext();

						// Merge world bounds of each object
						aabbOrig.merge(m->getWorldBoundingBox(true));
					}

					// Merge with children
					Node::ChildNodeIterator oit = rootNode->getChildIterator();
					while (oit.hasMoreElements())
					{
						SceneNode* sceneChild = static_cast<SceneNode*>(oit.getNext());
						aabbOrig.merge(sceneChild->_getWorldAABB());
					}

					////////////////////////////////////////


					Ogre::Vector3 min = aabbOrig.getMinimum();
					Ogre::Vector3 max = aabbOrig.getMaximum();

					std::ostringstream tmpString;
					tmpString << "         AABB Min : " << min << endl;
					tmpString << "         AABB Max : " << max;
					Gnoll::Log::LogModule::getInstancePtr()->logMessage( tmpString.str() );

					shared_ptr<Gnoll::DynamicObject::Float> pageSize = neighborPage.getAttribute<Float>(CPage::ATTRIBUTE_SIZE());

					Ogre::Vector3 neighborOffset;

					if (strcmp( neighborStr, string(*(this->getAttribute< Gnoll::DynamicObject::String >( CPage::ATTRIBUTE_NORTH_LINK()))).c_str()) == 0)
					{
						neighborOffset = Ogre::Vector3(0.0, 0.0, -*pageSize);

					} else if (strcmp( neighborStr, string(*(this->getAttribute< Gnoll::DynamicObject::String >( CPage::ATTRIBUTE_SOUTH_LINK()))).c_str()) == 0)
					{
						neighborOffset = Ogre::Vector3(0.0, 0.0, *pageSize);

					} else if (strcmp( neighborStr, string(*(this->getAttribute< Gnoll::DynamicObject::String >( CPage::ATTRIBUTE_EAST_LINK()))).c_str()) == 0)
					{
						neighborOffset = Ogre::Vector3( *pageSize, 0.0, 0.0 );

					} else if (strcmp( neighborStr, string(*(this->getAttribute< Gnoll::DynamicObject::String >( CPage::ATTRIBUTE_WEST_LINK()))).c_str()) == 0)
					{
						neighborOffset = Ogre::Vector3( -(*pageSize), 0.0, 0.0 );
					} else {
						neighborOffset = Ogre::Vector3( 0.0, 0.0, 0.0 );
					}

					Ogre::AxisAlignedBox aabb(
														min.x + neighborOffset.x,
														min.y + neighborOffset.y,
														min.z + neighborOffset.z,
														max.x + neighborOffset.x,
														max.y + neighborOffset.y,
														max.z + neighborOffset.z
														);

					result = ogreCamera->isVisible(aabb);

				}
			}

			std::ostringstream tmpString;
			tmpString << "             Is " << this->getInstance() << " visible : " << result;
			Gnoll::Log::LogModule::getInstancePtr()->logMessage( tmpString.str() );
			return result;
		}


		void CPage::initPosition()
		{
			shared_ptr<Gnoll::DynamicObject::Float> pageSize = this->getAttribute<Float>(CPage::ATTRIBUTE_SIZE());
			SceneNode* sceneNode                             = this->getPageRootNode();
			shared_ptr< Gnoll::DynamicObject::String > neighbor;
			CPage neighborPage("");

			//
			// Translate the page about the first neighbor page that is
			// initialized
			//
			if (this->hasAttribute( CPage::ATTRIBUTE_NORTH_LINK() ))
			{
				neighbor = this->getAttribute< Gnoll::DynamicObject::String >( CPage::ATTRIBUTE_NORTH_LINK() );
				neighborPage = CPage(*neighbor);
				if(neighborPage.isInitialized())
				{
					shared_ptr<Gnoll::DynamicObject::Float> neighborSize = neighborPage.getAttribute<Float>(CPage::ATTRIBUTE_SIZE());
					float amountTranslation = - ((*pageSize) / 2.0f + (*neighborSize) / 2.0f);
					Ogre::Vector3 translatingVector = Ogre::Vector3(0.0, 0.0, amountTranslation);
					sceneNode->setPosition(neighborPage.getPageRootNode()->getPosition());
					sceneNode->translate(translatingVector);
					return;
				}
			}

			if (this->hasAttribute( CPage::ATTRIBUTE_SOUTH_LINK() ))
			{
				neighbor = this->getAttribute< Gnoll::DynamicObject::String >( CPage::ATTRIBUTE_SOUTH_LINK() );
				neighborPage = CPage(*neighbor);
				if(neighborPage.isInitialized())
				{
					shared_ptr<Gnoll::DynamicObject::Float> neighborSize = neighborPage.getAttribute<Float>(CPage::ATTRIBUTE_SIZE());
					float amountTranslation = ((*pageSize) / 2.0f + (*neighborSize) / 2.0f);
					Ogre::Vector3 translatingVector = Ogre::Vector3(0.0, 0.0, amountTranslation);
					sceneNode->setPosition(neighborPage.getPageRootNode()->getPosition());
					sceneNode->translate(translatingVector);
					return;
				}
			}

			if (this->hasAttribute( CPage::ATTRIBUTE_EAST_LINK() ))
			{
				neighbor = this->getAttribute< Gnoll::DynamicObject::String >( CPage::ATTRIBUTE_EAST_LINK() );
				neighborPage = CPage(*neighbor);
				if(neighborPage.isInitialized())
				{
					shared_ptr<Gnoll::DynamicObject::Float> neighborSize = neighborPage.getAttribute<Float>(CPage::ATTRIBUTE_SIZE());
					float amountTranslation = - ((*pageSize) / 2.0f + (*neighborSize) / 2.0f);
					Ogre::Vector3 translatingVector = Ogre::Vector3(amountTranslation, 0.0, 0.0);
					sceneNode->setPosition(neighborPage.getPageRootNode()->getPosition());
					sceneNode->translate(translatingVector);
					return;
				}
			}

			if (this->hasAttribute( CPage::ATTRIBUTE_WEST_LINK() ))
			{
				neighbor = this->getAttribute< Gnoll::DynamicObject::String >( CPage::ATTRIBUTE_WEST_LINK() );
				neighborPage = CPage(*neighbor);
				if(neighborPage.isInitialized())
				{
					shared_ptr<Gnoll::DynamicObject::Float> neighborSize = neighborPage.getAttribute<Float>(CPage::ATTRIBUTE_SIZE());
					float amountTranslation = ((*pageSize) / 2.0f + (*neighborSize) / 2.0f);
					Ogre::Vector3 translatingVector = Ogre::Vector3(amountTranslation, 0.0, 0.0);
					sceneNode->setPosition(neighborPage.getPageRootNode()->getPosition());
					sceneNode->translate(translatingVector);
					return;
				}
			}

			//unInit();
		}
	}
}


