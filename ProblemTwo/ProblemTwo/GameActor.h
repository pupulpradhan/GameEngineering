#pragma once
#include "math/Vector2.h"
#include "math/Vector2.h"
using namespace Engine::Math;

// This is our base class for a GameActor, something in our world that moves and has health
class GameActor
{
private:
	Engine::Math::Vector2	m_Position;
	int						m_Health;
public:
	GameActor(const Engine::Math::Vector2& i_InitialPosition, int i_InitialHealth) :
		m_Position(i_InitialPosition),
		m_Health(i_InitialHealth)
	{ }
	GameActor(const GameActor& ga) : m_Position(ga.m_Position) {
		m_Position = ga.m_Position;
		m_Health = ga.m_Health;
	}
	virtual ~GameActor() {}

	// move it
	void UpdatePosition(const Engine::Math::Vector2& i_Movement) {
		m_Position += i_Movement;
	} 

	// modify it's health
	void UpdateHealth(int i_Adjustment) {
		m_Health += i_Adjustment;
	}

	// how much health does it have left?
	int GetHealth() const { return m_Health; }

	// each variant has it's own implementation
	virtual const char* getTypeName() const = 0;


};
;;