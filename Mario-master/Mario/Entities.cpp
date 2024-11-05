#include "Entities.h"
#include "Graphics.h"
#include "Physics.h"
#include "Animation.h"
#include "Config.h"

#include <memory>
using namespace std;
using namespace cv;

EntityState::EntityState(IGraphicsComponentPtr graphics, IPhysicsComponentPtr physics)
	:_graphicsPtr(graphics), _physicsPtr(physics)
{
}

void EntityState::update()
{
	bool graphicsFinished = _graphicsPtr->update();
	bool physicsFinished = _physicsPtr->update(_graphicsPtr->getCollisionMask());

	if (physicsFinished)
		Notify(Event{ EventSenders::SENDER_ENTITY_STATE, EventTypes::EVENT_PHYSICS, EventCodes::ENTITY_PHYSICS_FINISHED });
	if (graphicsFinished) {
		Notify(Event{ EventSenders::SENDER_ENTITY_STATE, EventTypes::EVENT_GRAPHICS, EventCodes::ENTITY_FINISHED_ANIMATION });
		// @4 TODO.
	}
}

void EntityState::addState(Event const& e, std::shared_ptr<EntityState> toWhere)
{
	_adjecentStates[e] = toWhere;
}

std::shared_ptr<EntityState> EntityState::tryModifyState(Event const& e) const
{
	bool isEventMapped = _adjecentStates.count(e);
	if (isEventMapped)
		return _adjecentStates.at(e);

	return nullptr;
}

IPhysicsComponentPtr const& EntityState::getPhysics() const
{
	return _physicsPtr;
}

void EntityState::reset(cv::Point const& TL)
{
	_graphicsPtr->reset();
	_physicsPtr->reset(TL);
}

void EntityState::draw(cv::Mat& canvas)
{
	_graphicsPtr->draw(canvas, _physicsPtr->getTL());
}


////////////////////////////////////////////////////////

Entity::Entity(EntityStatePtr state)
	:_state(state)
{
}

void Entity::onNotify(Event const& e)
{
	if (e.sender == EventSenders::SENDER_TIMER
		&&
		e.type == EventTypes::EVENT_TIMER
		&&
		e.code == EventCodes::TIME_TICK)
	{
		_state->update();
	}

	auto newStateCandidate = _state->tryModifyState(e);

	if (newStateCandidate)
	{
		newStateCandidate->reset(_state->getPhysics()->getTL());
		_state = newStateCandidate;
	}
}

void Entity::reset(cv::Point const& TL)
{
	_state->reset(TL);
}

void Entity::draw(cv::Mat& canvas)
{
	_state->draw(canvas);
}