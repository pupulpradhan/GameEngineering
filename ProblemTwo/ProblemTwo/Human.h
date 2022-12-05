#pragma once

#include "GameActor.h"
#include "math/Vector2.h"

#include <string.h>
#include <stdlib.h>

// I am a GameActor that is a human being
class Human : public GameActor
{
private:
	const char* m_Name;
public:
	Human(const char* i_pName, const Engine::Math::Vector2& i_InitialPosition, int i_InitialHealth) :
		GameActor(i_InitialPosition, i_InitialHealth),
		m_Name(_strdup(i_pName ? i_pName : "Unknown"))
	{ }
	// Copy constructor
	Human(const Human& h) : GameActor(h)
	{
		m_Name = _strdup(h.m_Name ? h.m_Name : "Unknown") ;
	}
	~Human()
	{
		if (m_Name)
			free(const_cast<char*>(m_Name));
	}

	// what's my name
	const char* getName() const { return m_Name; }

	// return my specific type
	virtual const char* getTypeName() const { return "Human"; }

};

