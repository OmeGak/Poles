#include "GameObject.h"
#include "components/Components.h"

GameObject::GameObject(artemis::Entity & objectEntity):
entity(objectEntity){
    // Every object in the game will have at least a position
    this->entity.addComponent(new PositionComponent());
    this->entity.refresh();
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
    PositionComponent * component = (PositionComponent *)this->entity.getComponent<PositionComponent>();
    PositionComponent * parentComponent = (PositionComponent *) parent->entity.getComponent<PositionComponent>();
    
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
        
        PositionComponent * component = (PositionComponent *)this->entity.getComponent<PositionComponent>();
        component->removeParentPosition();
        
    } else {
        std::cout << "Error! - GameObject: Trying to remove parent from object with no parent." << std::endl;
    }
}

/**
 * Adds a component to the Game Object.
 * @param component     Component derived from artemis::Component.
 */
void GameObject::addComponent(artemis::Component * component) {
    if (component != NULL) {
        this->entity.addComponent(component);
        this->entity.refresh();
    } else {
        std::cout << "Error! - You tried to attach unnitialized component to GameObject " << this->entity.getId() << std::endl;
    }
}

void GameObject::removeComponent(artemis::Component* component) {
    
}

/**
 * 
 * @return 
 */
Vector2D GameObject::getPosition() {
    PositionComponent * component = (PositionComponent *)this->entity.getComponent<PositionComponent>();
    
    if (component != NULL) {
        return component->getLocalPosition();
    } else {
        // Something terrible happened!
        std::cout << "Error! - GameObject " << this->entity.getId() << " has no position component." << std::endl;
    }
}

/**
 * 
 * @param position
 */
void GameObject::setPosition(Vector2D& position) {
    PositionComponent * component = (PositionComponent *)this->entity.getComponent<PositionComponent>();
    
    if (component != NULL) {
        component->setPosition(position);
    } else {
        // Something terrible happened!
        std::cout << "Error! - GameObject " << this->entity.getId() << " has no position component." << std::endl;
    }
}

/**
 * Calculates the real position of the object, taking in account the parent's (if present) position.
 * @return      Real position in the global coordinates.
 */
Vector2D GameObject::getRealPosition() {
    Vector2D realPosition;
    
    if (this->parent != NULL) {
        realPosition = realPosition + this->parent->getRealPosition();
    }
    
    realPosition = realPosition + this->getPosition();
    
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
