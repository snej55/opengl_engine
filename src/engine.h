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

    EngineObject* getParent() const
    {
        return m_parent;
    }
    
    void clearParent() {m_parent = nullptr;}
    void setParent(EngineObject* parent) {m_parent = parent;}

    std::vector<EngineObject*> getChildren() const
    {
        return m_children;
    }

protected:
    EngineObject* m_parent;
    std::vector<EngineObject*> m_children;
};

#endif