#include "Physics.h"
#include <algorithm>

using namespace cv;
using namespace std;
/// //////////////////////////////////////////

// @2: Read this function and try to understand what it does. Use Image Watch !
bool checkPixelLevelCollision(IPhysicsComponent const* first, IPhysicsComponentPtr const& other)
{
	cv::Mat const& mask_First = first->getCollisionMask();
	cv::Mat const& mask_Other = other->getCollisionMask();
	if (mask_Other.empty() || mask_First.empty())
		return false;

	Point TL_first = first->getTL();
	Point TL_other = other->getTL();
	
	// ROI = Reagion of Interest
	Rect firstROI(TL_first, TL_first + Point(mask_First.size()));
	Rect othersROI(TL_other, TL_other + Point(mask_Other.size()));
	Rect intersection = firstROI & othersROI;
	if (intersection.empty())
		return false;

	Rect intersection_RelativeToMe(
		intersection.tl() - TL_first, intersection.br() - TL_first);

	Rect intersection_RelativeToOther(
		intersection.tl() - TL_other, intersection.br() - TL_other);

	Mat myMaskROI = mask_First(intersection_RelativeToMe);
	Mat othersMaskROI = mask_Other(intersection_RelativeToOther);
	Mat whereMeAndOtherIntersect = myMaskROI.mul(othersMaskROI);

	return countNonZero(whereMeAndOtherIntersect);
}

/// //////////////////////////////////////////

FixedWidgetPhysics::FixedWidgetPhysics()
	:_topLeft(0,0), _mask()
{
}

void FixedWidgetPhysics::reset(cv::Point const& tl)
{
	_topLeft = tl;
}

bool FixedWidgetPhysics::update(cv::Mat const& collisionMask)
{
	_mask = collisionMask;
	return false;
}

bool FixedWidgetPhysics::checkCollision(IPhysicsComponentPtr const& other) const
{
	return checkPixelLevelCollision(this, other);
}

cv::Point const& FixedWidgetPhysics::getTL() const
{
	return _topLeft;
}

cv::Mat const& FixedWidgetPhysics::getCollisionMask() const
{
	return _mask;
}

//--------------ConstVelocityPhysics
ConstVelocityPhysics::ConstVelocityPhysics(cv::Point const& velocity):_velocity(velocity),
_currTL(0,0),_mask()
{

}

void ConstVelocityPhysics::reset(cv::Point const& tl)
{
	_currTL = tl;
}

bool ConstVelocityPhysics::update(cv::Mat const& collisionMask)
{
	_currTL = _currTL + _velocity;
	_mask = collisionMask;
	return false;
}

cv::Mat const& ConstVelocityPhysics::getCollisionMask() const
{
	return _mask;
}

bool ConstVelocityPhysics::checkCollision(IPhysicsComponentPtr const& other) const
{
	return checkPixelLevelCollision(this,other);
}

cv::Point const& ConstVelocityPhysics::getTL() const
{
	return _currTL;
}
///-----------------JumpPhysics-----------------------///
JumpPhysics::JumpPhysics(int horizontalVelocity, int initialVerticalVelocity, int gravity)
	: _initialJumpVelocity(horizontalVelocity,initialVerticalVelocity), _gravity(gravity)
{
	if (initialVerticalVelocity < 0 || gravity < 0)
	{
		throw("error");
	}
}

void JumpPhysics::reset(cv::Point const& tl)
{
	_currTL = tl;
	_jumpStartTL = tl;
	_currVelocity = _initialJumpVelocity;
}

bool JumpPhysics::update(cv::Mat const& collisionMask)
{
	_mask = collisionMask;
	_currTL.x += _currVelocity.x;
	_currTL.y -= _currVelocity.y;

	if (_currTL.y >= _jumpStartTL.y)
	{
		_currTL.y = _jumpStartTL.y;
		_currVelocity = Point(0, 0);
		return true;
	}
	_currVelocity.y -= _gravity;
	return false;
}
cv::Mat const& JumpPhysics::getCollisionMask() const
{
	return _mask;
}

bool JumpPhysics::checkCollision(IPhysicsComponentPtr const& other) const
{
	return checkPixelLevelCollision(this, other);
}

cv::Point const& JumpPhysics::getTL() const
{
	return _currTL;
}
////////////----------------NonCollidingPhysicsDecorator-------------------------------//////////////////
NonCollidingPhysicsDecorator::NonCollidingPhysicsDecorator(IPhysicsComponentPtr base)
	:_base(base)
{
}

void NonCollidingPhysicsDecorator::reset(cv::Point const& tl)
{
	
	_base->reset(tl);
}

bool NonCollidingPhysicsDecorator::update(cv::Mat const& collisionMask)
{
	return _base->update(collisionMask);
}

cv::Mat const& NonCollidingPhysicsDecorator::getCollisionMask() const
{
	return Mat();
}

bool NonCollidingPhysicsDecorator::checkCollision(std::shared_ptr<IPhysicsComponent> const& other) const
{
	return false;
}

cv::Point const& NonCollidingPhysicsDecorator::getTL() const
{
	return _base->getTL();
}

///--------------------------------------------------///
BoundedPhysicsDecorator::BoundedPhysicsDecorator(IPhysicsComponentPtr base):_base(base)

{
	
	_bounds.x = 0;
	_bounds.y = 0;
	Mat background = imread(R"(../Animations/background.png)", IMREAD_UNCHANGED);
	_bounds.width = background.size().width - getCollisionMask().size().width;
	_bounds.height = background.size().height - getCollisionMask().size().height;
}

void BoundedPhysicsDecorator::test()
{
	//***


	if (_base->getTL().x > _bounds.x + _bounds.width - _base->getCollisionMask().size().width|| _base->getTL().x <=0)
	
	{
	    reset(Point(_bounds.width - _base->getCollisionMask().size().width, _base->getTL().y));
	
	/*if (_base->getTL().x <= 0)
	{
		reset(Point( _base->getCollisionMask().size().width, _base->getTL().y));
	}*/
	
	}
}

void BoundedPhysicsDecorator::reset(cv::Point const& tl)
{
	_base->reset(tl);
}

bool BoundedPhysicsDecorator::update(cv::Mat const& collisionMask)
{
	_mask = collisionMask;
	test();
	return _base->update(collisionMask);
}

cv::Mat const& BoundedPhysicsDecorator::getCollisionMask() const
{
	return _base->getCollisionMask();

}

bool BoundedPhysicsDecorator::checkCollision(IPhysicsComponentPtr const& other) const
{
	return false;
}

cv::Point const& BoundedPhysicsDecorator::getTL() const
{
   return _base->getTL();
}
