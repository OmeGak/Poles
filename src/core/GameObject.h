#ifndef GAMEOBJECT_H
#define	GAMEOBJECT_H

#include <Artemis/Artemis.h>
#include "Vector2D.h"

class Game;

class GameObject {
    // Game have permission to create Game Objects
    friend class Game;
public:
    void                        setParent(GameObject * parent);
    void                        removeParent();
    
    artemis::Component *        addComponent(artemis::Component * component);
    void                        removeComponent(artemis::Component * component);  
    
    template<typename c>
    inline artemis::Component *        getComponent() { return this->entity.getComponent<c>(); }
    
    Vector2D                    getPosition();
    Vector2D                    getRelativePosition();

    Vector2D                    getPositionPerspective();
    
    void                        setPosition(Vector2D & position);
    void                        setPosition(const int x, const int y);
    
    bool                        hasParent();
    
    void                        addForce(Vector2D & force);
    void                        addForce(const float x, const float y);
    void                        resetForce();

    void                        changeAnimation(const char* animation);
    
    void                        showInfo();
    
private:
    // Private constructor to not let the user create GameObjects without using Game::CreateGameObject()
    GameObject(artemis::Entity & objectEntity);
    // Private constructor. User shouldn't be able to delete GameObjects without using Game::deleteGameObject()
    ~GameObject();
    
private:
    GameObject *                parent;
    artemis::Entity &           entity;

};

#endif	/* GAMEOBJECT_H */

