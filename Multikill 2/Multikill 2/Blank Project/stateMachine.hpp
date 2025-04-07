#pragma once
#include <vector>
using namespace std;

class State
{
	public:
	vector<State*> states = {};
	State* currentState = nullptr;

	virtual void Enter() = 0;
	void BaseUpdate()
	{
		Update();
		currentState->Update();
	};
	virtual void Update() = 0;
	virtual void Exit() = 0;
	void ChangeState(State* nextState)
	{
		if (nextState != currentState)
		{
			currentState->Exit();
			currentState = nextState;
			currentState->Enter();
		}
	}
};

class PlayerParentState : State
{
	public:
	
	PlayerParentState()
	{
		states = {};
		currentState = nullptr;
	}

	void Enter() override
	{

	}
	void Update() override
	{

	}
	void Exit() override
	{

	}
};