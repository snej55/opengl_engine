#ifndef ENGINE_TYPES
#define ENGINE_TYPES

#include <string>

class EngineObject
{
public:
    EngineObject(const char* name, EngineObject* parent = nullptr)
     : m_name{name}, m_parent{parent}
    {
    }

    const char* getName() const {return m_name.c_str();}
    EngineObject* getParent() const {return m_parent;}

private:
    std::string m_name;
    EngineObject* m_parent;
};

#endif