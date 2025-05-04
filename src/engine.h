#ifndef ENGINE_H
#define ENGINE_H

#include <vector>

// base class for engine objects
class EngineObject
{
public:
    EngineObject(EngineObject* parent, const std::vector<EngineObject*> children = {})
     : m_parent{parent}, m_children{children}
    {
        if (parent != nullptr)
        {
            // add self as node
            parent->addChild(this);
        }
    }

    virtual void free()
    {
        for (EngineObject*& e: m_children)
        {
            if (e != nullptr)
            {
                e->free();
                delete e;
                e = nullptr;
            }
        }
    }

    template <typename T>
    T* getParent() const
    {
        return static_cast<T*>(m_parent);
    }

    virtual EngineObject* getParent() const
    {
        return m_parent;
    }
    
    virtual void clearParent() {m_parent = nullptr;}
    virtual void setParent(EngineObject* parent) {m_parent = parent;}

    virtual std::vector<EngineObject*> getChildren() const
    {
        return m_children;
    }

    virtual void addChild(EngineObject* child)
    {
        m_children.push_back(child);
    }

protected:
    EngineObject* m_parent;
    std::vector<EngineObject*> m_children;
};

#endif