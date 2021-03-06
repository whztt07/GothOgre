#include "Precompiled.h"
#include "EntityCreator.h"
#include "SceneManagerExImpl.h"


namespace GothOgre
{
	//----------------------------------------------------------------------
	static const String PREFIX_FOR_AUTOGENERATED_MATERIALS = "Auto";
	NameGenerator EntityCreator::ms_NameGenerator(PREFIX_FOR_AUTOGENERATED_MATERIALS);
	//----------------------------------------------------------------------
	EntityCreator::EntityCreator( SceneManagerExImpl* _sceneManager )
	{
		mSceneManager = _sceneManager;
	}
	//----------------------------------------------------------------------
	EntityCreator::~EntityCreator()
	{
	}
	//----------------------------------------------------------------------
	Entity* EntityCreator::createEntity( const MeshExPtr& _mesh, uint32 _queryMask )
	{
		_mesh->load();

		setupMaterialNames( _mesh );

		String entityName = ms_NameGenerator.generate();
		Entity* entity = ((SceneManager*)mSceneManager)->createEntity( entityName, _mesh->getName(), _mesh->getGroup() );
		
		entity->setQueryFlags(_queryMask);
		entity->setVisibilityFlags(_queryMask);
		
		return entity;
	}
	//----------------------------------------------------------------------
	void EntityCreator::setupMaterialNames( const MeshExPtr& _mesh )
	{
		MeshEx::SubMeshIterator it = _mesh->getSubMeshIterator();
		while( it.hasMoreElements() )
		{
			SubMeshEx* subMesh = it.getNext();
			MaterialPtr material = mSceneManager->generateMaterial( subMesh->getMtl(), subMesh->getMatGenParams() );
			subMesh->setMaterialName( material->getName(), material->getGroup() );
		}

		ushort numLodLevels = _mesh->getNumLodLevels();
		for(ushort i = 0; i != numLodLevels; ++i)
		{
			const MeshLodUsage& mlu = _mesh->getLodLevel(i);
			MeshPtr lodMesh = mlu.manualMesh;

			if( lodMesh.isNull() && !mlu.manualName.empty() )
				lodMesh = MeshManagerEx::getSingleton().findByName( mlu.manualName, mlu.manualGroup );

			if( !lodMesh.isNull() )
				setupMaterialNames( lodMesh );
		}
	}

} // namespace GothOgre