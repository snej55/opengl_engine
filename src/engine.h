#ifndef ENGINE_H
#define ENGINE_H

#include <vector>

// base class for engine objects
class EngineObject
{
public:
    EngineObject(void* parent, const std::vector<void*> children = {})
     : m_parent{parent}, m_children{children}
    {
    }

    virtual void free()
    {
        
    }

    template <typename T>
    T* get_parent() const
    {
        return static_cast<T*>(m_parent);
    }

    void* get_parent() const
    {
        return m_parent;
    }
    
    void clear_parent() {m_parent = nullptr;}
    void set_parent(void* parent) {m_parent = parent;}

    std::vector<void*> get_children() const
    {
        return m_children;
    }

protected:
    void* m_parent;
    std::vector<void*> m_children;
};

#endif