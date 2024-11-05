#pragma once
#include "Graphics.h"
#include "Physics.h"
#include "Observer.h"
#include <map>

class EntityState : public Subject
{
protected:
	IGraphicsComponentPtr _graphicsPtr;
	IPhysicsComponentPtr _physicsPtr;

	std::map<Event, std::shared_ptr<EntityState>> _adjecentStates;


public:
	EntityState(IGraphicsComponentPtr graphics, IPhysicsComponentPtr physics);

	virtual void update();

	void addState(Event const& e, std::shared_ptr<EntityState> toWhere);
	virtual std::shared_ptr<EntityState> tryModifyState(Event const& e) const;

	IPhysicsComponentPtr const& getPhysics() const;
	virtual void reset(cv::Point const& TL);

	void draw(cv::Mat& canvas);
};
typedef std::shared_ptr<EntityState> EntityStatePtr;


class Entity : public IObserver
{
protected:
	EntityStatePtr _state;

public:
	Entity(EntityStatePtr state);

	// Inherited via IObserver
	virtual void onNotify(Event const& e) override;
	virtual void reset(cv::Point const& TL);

	void draw(cv::Mat& canvas);
};
typedef std::shared_ptr< Entity> EntityPtr;