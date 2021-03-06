#include "GameObject.h"
#include "components/Components.h"
#include <Artemis/ImmutableBag.h>
#include <sstream>
#include <Artemis/Component.h>
#include "../game/Game.h"

GameObject::GameObject(artemis::Entity & objectEntity):
entity(objectEntity){
    // Every object in the game will have at least a position
    this->entity.addComponent(new components::Position());
    this->entity.refresh();
    
    this->parent = NULL;
}

GameObject::~GameObject() {
}

/**
 * Makes this object child of another object.
 * 
 * From now on, the position of the object will be relative to the parent's position.
 * @param parent        Object which will be the parent.
 */
void GameObject::setParent(GameObject * parent) {
    this->parent = parent;

    // Add the parent to the PositionComponent
    components::Position * component = (components::Position *)this->entity.getComponent<components::Position>();
    components::Position * parentComponent = (components::Position *) parent->entity.getComponent<components::Position>();
    
    if (parentComponent != NULL) {
        component->setParentPosition(parentComponent);
    } else {
        std::cout << "Error! - GameObject: Trying to add parent with no Position Component." << std::endl;
    }   
}

/**
 * The object will be no more child of its actual parent and its coordinates will be global again.
 */
void GameObject::removeParent() {
    if (this->parent != NULL) {
        // Unset the parent
        this->parent = NULL;
        
        components::Position * component = (components::Position *)this->entity.getComponent<components::Position>();
        component->removeParentPosition();
        
    } else {
        std::cout << "Error! - GameObject: Trying to remove parent from object with no parent." << std::endl;
    }
}

/**
 * Adds a component to the Game Object.
 * @param component     Component derived from artemis::Component.
 */
artemis::Component * GameObject::addComponent(artemis::Component * component) {
    if (component != NULL) {
        this->entity.addComponent(component);
        this->entity.refresh();
    } else {
        std::cout << "Error! - You tried to attach unnitialized component to GameObject " << this->entity.getId() << std::endl;
    }
    return component;
}

void GameObject::removeComponent(artemis::Component* component) {
    // TODO
}

/**
 * 
 * @return 
 */
Vector2D GameObject::getRelativePosition() {
    components::Position * component = (components::Position *)this->entity.getComponent<components::Position>();
    
    if (component != NULL) {
        return component->getLocalPosition();
    } else {
        // Something terrible happened!
        std::cout << "Error! - GameObject " << this->entity.getId() << " has no position component." << std::endl;
    }
    return Vector2D();
}

Vector2D GameObject::getPositionPerspective() {
    Vector2D position;
    Vector2D cameraCorrection;
    Vector2D cameraPosition;
    Vector2D parallaxCorrection;
    float parallaxIndex;

    position = this->getPosition();
    cameraPosition = Game::getMainCameraObject()->getPosition();
    cameraCorrection = Game::getMainCamera()->getCorrectionVector();

    components::SpriteRenderer* sprite = (components::SpriteRenderer*)getComponent<components::SpriteRenderer>();
    components::TextRenderer* text = (components::TextRenderer*)getComponent<components::TextRenderer>();

    if (sprite != NULL) {
        parallaxIndex = sprite->getParallaxIndex();
        parallaxCorrection = sprite->getParallaxCompensation();
    } else if (text != NULL) {
        parallaxIndex = text->getParallaxIndex();
        parallaxCorrection = text->getParallaxCompensation();
    } else {
        parallaxIndex = 1.0f;
        // parallaxCorrection = (0, 0)
    }

    position = position - ((cameraPosition - cameraCorrection) * parallaxIndex) + parallaxCorrection;

    return position;
}

/**
 * 
 * @param position
 */
void GameObject::setPosition(Vector2D& position) {
    components::Position * component = (components::Position *)this->entity.getComponent<components::Position>();
    
    if (component != NULL) {
        component->setPosition(position);
    } else {
        // Something terrible happened!
        std::cout << "Error! - GameObject " << this->entity.getId() << " has no position component." << std::endl;
    }
}

void GameObject::setPosition(const int x, const int y) {
    Vector2D pos(x,y);
    setPosition(pos);
}

/**
 * Calculates the real position of the object, taking in account the parent's (if present) position.
 * @return      Real position in the global coordinates.
 */
Vector2D GameObject::getPosition() {
    Vector2D realPosition;
    
    if (this->parent != NULL) {
        realPosition = realPosition + this->parent->getPosition();
    }
    
    realPosition = realPosition + this->getRelativePosition();
    
    return realPosition;
}

/**
 * 
 * @return 
 */
bool GameObject::hasParent() {
    if (this->parent != NULL) {
        return true;
    }
    
    return false;
}

/**
 * 
 * @param force
 */
void GameObject::addForce(Vector2D & force) {
    components::Velocity * component = (components::Velocity *) this->entity.getComponent<components::Velocity>();
    
    if (component != NULL) {
        component->addVelocity(force);
    } else {
        SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR,
                "Missing Component",
                "Error! - GameObject: Trying to add force to an object with no VelocityComponent",
                NULL);
    }
}

void GameObject::addForce(const float x, const float y) {
    Vector2D force(x, y);
    addForce(force);
}

void GameObject::resetForce() {
    components::Velocity * component = (components::Velocity *) this->entity.getComponent<components::Velocity>();
    
    if (component != NULL) {
        component->resetVelocity();
    } else {
        SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR,
                "Missing Component",
                "Error! - GameObject: Trying to reset force to an object with no VelocityComponent",
                NULL);
    }
}

void GameObject::changeAnimation(const char *animation) {
    components::SpriteRenderer* sprite = (components::SpriteRenderer*)entity.getComponent<components::SpriteRenderer>();

    if (sprite != NULL) {
        sprite->changeAnimation(animation);
    }
}

/**
 * 
 */
void GameObject::showInfo() {
   std::stringstream str;
   
   int numberOfComponents = 0;
   int id = 0;
   float x = 0;
   float y = 0;
   
   components::Position * component = (components::Position *)this->entity.getComponent<components::Position>();
   
   x = component->getPosition().x();
   y = component->getPosition().y();
   
   id = this->entity.getId();
   artemis::ImmutableBag<artemis::Component * > & components = this->entity.getComponents();
   numberOfComponents = components.getCount();
   
   str << "ID = " << id << std::endl;
   str << "Number of Components = " << numberOfComponents << std::endl;
   str << "X = " << x << " Y = " << y << std::endl;
   
   SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_INFORMATION,
           "GameObject",
           str.str().c_str(),
           NULL);
}
