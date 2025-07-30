#pragma once
#include <vector>

class State
{
	private:

	std::vector<State*> children;
	int currentChild;

	public:

	int state;

	State()
	{
		children = {};
		state = -1;
		currentChild = -1;
	}
	State(std::vector<State*> _children, int startingState)
	{
		children = _children;
		state = startingState;
		currentChild = state;
	}
	~State() {}

	virtual void Start() {}
	virtual void Update() {}
	virtual void Exit() {}

	void BaseStart()
	{
		Start();
		if (currentChild != -1) children[currentChild]->BaseStart();
	}
	void BaseUpdate()
	{
		Update();
		if (state != currentChild)
		{
			if (currentChild != -1) children[currentChild]->BaseExit();
			currentChild = state;
			if (state != -1) children[currentChild]->BaseStart();
		}
		else if (currentChild != -1) children[currentChild]->BaseUpdate();
	}
	void BaseExit()
	{
		if (currentChild != -1) children[currentChild]->BaseExit();
		Exit();
	}
};

/*class ExampleState : public State
{
	public:

	BabyState babyState;
	SkidmarkState skidmarkState;

	ExampleState() : State({ &babyState, &skidmarkState }, 1)
	{
		//
	}

	void Start() override
	{
		printf("override start things\n");
	}
	void Update() override
	{
		printf("override update shenanigans\n");
	}
	void Exit() override
	{
		printf("override exit japes and jibulations\n");
	}
};*/