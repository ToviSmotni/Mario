#pragma once
#include <memory>
#include <vector>
#include <list>

class Event
{
public:
	int sender;
	int type;
	int code;
	bool operator < (Event const& other) const;
};

class IObserver
{
public:
	virtual void onNotify(Event const& e) = 0;
};

typedef std::shared_ptr<IObserver> IObserverPtr;

class Subject
{
private:
	std::list<IObserverPtr> _observers;

public:
	void Register(IObserverPtr o);
	void Notify(Event const& e);
};