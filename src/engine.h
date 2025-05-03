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
                e = nullptr;
            }
        }
    }

    template <typename T>
    T* get_parent() const
    {
        return static_cast<T*>(m_parent);
    }

    EngineObject* get_parent() const
    {
        return m_parent;
    }
    
    void clear_parent() {m_parent = nullptr;}
    void set_parent(EngineObject* parent) {m_parent = parent;}

    std::vector<EngineObject*> get_children() const
    {
        return m_children;
    }

protected:
    EngineObject* m_parent;
    std::vector<EngineObject*> m_children;
};

#endif