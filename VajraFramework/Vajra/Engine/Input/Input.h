#ifndef INPUT_H
#define INPUT_H

#include "Vajra/Common/Objects/Object.h"
#include "Libraries/glm/glm.hpp"

#include <iostream>
#include <vector>
#include <map>

enum TouchPhase
{
	Began,
	Ended,
	Moved,
	Stationary,
	Cancelled
};

// TODO [Implement] Change this to use pools of Touch objects
struct Touch
{
private:
	int uId;
public:
	int fingerId;
	glm::vec2 pos;
	glm::vec2 prevPos;
	TouchPhase phase;

friend class Input;
};

class Input : public Object {

static const int MAX_TOUCHES = 5;

public:
	~Input();

	Touch GetTouch(int index);
	void AddTouch(int uId, float startX, float startY, TouchPhase phase = TouchPhase::Began);
	void UpdateTouch(int uId, float curX, float curY, TouchPhase phase);
	int GetTouchCount() { return this->frameTouches.size(); }
private:
	Input();
	void init();
	void destroy();

	void updateInput();
	void logTouches();

    std::vector<Touch> frameTouches;
    std::vector<Touch> asyncTouches;

	friend class Engine;
};

#endif // INPUT_H