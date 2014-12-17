#include "Between.h"
#include "StringUtil.h"

USING_NS_CC;

const int Between::UNDEFINED_POSITION = -2048;
const int Between::MAX_OPACITY = 255;

cocos2d::Spawn* Between::to(Node* node, std::string to, float duration, Easing easing)
{
    Vector<FiniteTimeAction*> spawn;
 
	std::vector<std::string> items = StringUtil::split(to, ',');
    
    float toX = UNDEFINED_POSITION;
    float byX = UNDEFINED_POSITION;
    
    for(auto item : items) {
        std::vector<std::string> i = StringUtil::split(item, ':');
        
        std::string key = StringUtil::trim(i[0]);
        float value = Value(StringUtil::trim(i[1])).asFloat();
        ActionInterval* action;
        
        if(key == "x") {
            toX = value;
            continue;
        } else if(key == "y") {
            if (toX == UNDEFINED_POSITION) {
                action = MoveTo::create(duration, Point(node->getPositionX(), value));
            } else {
                action = MoveTo::create(duration, Point(toX, value));
                toX = UNDEFINED_POSITION;
            }
        } else if (key == "+x") {
            byX = value;
            continue;
        } else if (key == "+y") {
            if(byX == UNDEFINED_POSITION){
                action = MoveBy::create(duration, Point(0, value));
            } else {
                action = MoveBy::create(duration, Point(byX, value));
                byX = UNDEFINED_POSITION;
            }
        } else if (key == "alpha") {
            action = FadeTo::create(duration, value * 255);
        } else if (key == "scale") {
            action = ScaleTo::create(duration, value);
        } else if (key == "rot") {
            action = RotateTo::create(duration, value);
        } else {
            CCLOG("[Between::to] invalid action");
            continue;
        }
        spawn.pushBack(TargetedAction::create(node, Between::getEasing(action, duration, easing)));
    }
    if (toX != UNDEFINED_POSITION) {
        auto action = MoveTo::create(duration, Point(toX, node->getPositionY()));
        spawn.pushBack(TargetedAction::create(node, Between::getEasing(action, duration, easing)));
    }
    if (byX != UNDEFINED_POSITION) {
        auto action = MoveTo::create(duration, Point(byX, 0));
        spawn.pushBack(TargetedAction::create(node, Between::getEasing(action, duration, easing)));
    }
    return Spawn::create(spawn);
}

Spawn* Between::from(Node* node, std::string from, float duration, Easing easing)
{
    Vector<FiniteTimeAction*> spawn;
    
    std::vector<std::string> items = StringUtil::split(from, ',');
    
    for(auto item : items) {
        std::vector<std::string> i = StringUtil::split(item, ':');
        
        std::string key = StringUtil::trim(i[0]);
        float value = Value(StringUtil::trim(i[1])).asFloat();
        ActionInterval* action;
        
        if(key == "x") {
            action = MoveTo::create(duration, Point(node->getPositionX(), 0));
            node->setPositionX(value);
        } else if(key == "y") {
            action = MoveTo::create(duration, Point(0, node->getPositionY()));
            node->setPositionY(value);
        } else if(key == "alpha") {
            action = FadeTo::create(duration, node->getOpacity());
            node->setOpacity(value * MAX_OPACITY);
        } else if(key == "scale") {
            action = ScaleTo::create(duration, node->getScaleX());
            node->setScale(value);
        } else if(key == "rot") {
            action = RotateTo::create(duration, node->getRotation());
            node->setScale(value);
        } else {
            CCLOG("[Between::from] invalid action");
            continue;
        }
        spawn.pushBack(TargetedAction::create(node, Between::getEasing(action, duration, easing)));
    }
    return Spawn::create(spawn);
}

Spawn* Between::tween(cocos2d::Node* node, std::string to, std::string from, float duration, Easing easing)
{
    Vector<FiniteTimeAction*> spawn;
    
    std::vector<std::string> items = StringUtil::split(from, ',');
    for(auto item : items) {
        std::vector<std::string> i = StringUtil::split(item, ':');
        
        std::string key = StringUtil::trim(i[0]);
        float value = Value(StringUtil::trim(i[1])).asFloat();
        if(key == "x") {
            node->setPositionX(value);
        } else if(key == "y") {
            node->setPositionY(value);
        } else if(key == "alpha") {
            node->setOpacity(value * MAX_OPACITY);
        } else if(key == "scale") {
            node->setScale(value);
        } else if(key == "rot") {
            node->setRotation(value);
        } else {
            CCLOG("[Between::tween] invalid action");
        }
    }
    return Between::to(node, to, duration, easing);
}

ActionInterval* Between::getEasing(ActionInterval *action, float duration, Easing easing)
{
    switch (easing) {
        case Easing::QuadIn:
            return EaseQuadraticActionIn::create(action);
        case Easing::QuadOut:
            return EaseQuadraticActionOut::create(action);
        case Easing::QuadInOut:
            return EaseQuadraticActionInOut::create(action);
        
        case Easing::ElasticIn:
            return EaseElasticIn::create(action, duration);
        case Easing::ElasticOut:
            return EaseElasticOut::create(action, duration);
        case Easing::ElasticInOut:
            return EaseElasticInOut::create(action, duration);
        
        case Easing::ExpoIn:
            return EaseExponentialIn::create(action);
        case Easing::ExpoOut:
            return EaseExponentialOut::create(action);
        case Easing::ExpoInOut:
            return EaseExponentialInOut::create(action);
        
        case Easing::BackIn:
            return EaseBackIn::create(action);
        case Easing::BackOut:
            return EaseBackOut::create(action);
        case Easing::BackInOut:
            return EaseBackInOut::create(action);

        case Easing::BounceIn:
            return EaseBounceIn::create(action);
        case Easing::BounceOut:
            return EaseBounceOut::create(action);
        case Easing::BounceInOut:
            return EaseBounceInOut::create(action);
            
        default:
            return action;
    }
}
