#ifndef ENGINE_TYPES
#define ENGINE_TYPES

#include <string>
#include <iostream>

class EngineObject
{
public:
    EngineObject(const char* name, EngineObject* parent = nullptr)
     : m_name{name}, m_parent{parent}
    {
    }

    virtual ~EngineObject()
    {
        std::cout << "Freed {" << m_name << "}, child of {" << (
            m_parent == nullptr ? "NONE" : m_parent->getName()
        ) << std::endl;
    };

    const char* getName() const { return m_name.c_str(); }
    EngineObject* getParent() const { return m_parent; }

    // return index in arena pool
    unsigned int getID() const { return m_ID; }
    void setID(const unsigned int idx) { m_ID = idx; }

private:
    std::string m_name;
    EngineObject* m_parent;
    unsigned int m_ID;
};

#endif