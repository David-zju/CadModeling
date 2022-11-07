#pragma once
#include <cassert>

template<typename T> class Entity
{
protected:
	static T* ms_Entity;

protected:
	Entity( void ){
		assert( !ms_Entity );
		ms_Entity = static_cast<T*>(this);
	}
public:
	~Entity( void ){  assert( ms_Entity );  ms_Entity = 0;  }

    static T& GetEntity( void ){return *(GetEntityPtr());}

	static T* GetEntityPtr( void ){ 
		if(!ms_Entity){
			ms_Entity = new T();
		}
		return ms_Entity;  
	}
};

template<class T> T* Entity<T>::ms_Entity = 0;
