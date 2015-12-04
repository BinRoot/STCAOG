//
// Created by binroot on 11/20/15.
//

#ifndef DRAWMESH_PROPERTY_H
#define DRAWMESH_PROPERTY_H

#include <memory>
#include <vector>
#include <unordered_map>
#include <string>
#include <ostream>

class PropertyValue
{
public:
    virtual ~PropertyValue() {}

    virtual void print(std::ostream& os) const = 0;
};


typedef std::unordered_map<std::string, std::shared_ptr<PropertyValue>> PropertyDict;
typedef std::vector<std::shared_ptr<PropertyValue>> PropertyList;


// TypedPropertyValue
// A template class allowing values in property list or dict to be
// of heterogeneous types.
template <class T>
class TypedPropertyValue : public PropertyValue
{
public:
    TypedPropertyValue(const T& data)
    {
        data_ = data;
    }
    virtual ~TypedPropertyValue() {}

    virtual void operator >> (T& var) const
    {
        var = data_;
    }

    virtual void print(std::ostream& os) const
    {
        os << data_;
    };

    T get_value() const {return data_;}
private:
    T data_;
};

//
// Helper functions
//

// Create a shared pointer using a single value.
// E.g.
//      dict["name"] = make_value<std::string>("Hello");
//      dict["height"] = make_value<int>(1);
template <class T>
std::shared_ptr<PropertyValue> make_value(const T& value)
{
    return std::make_shared<TypedPropertyValue<T>>(value);
}


// Get the value of by key and type.
// E.g.
//      std::cout << get_value<std::string>(dict["name"]) << std::endl;
//      std::cout << get_value<int>(dict["height"]) << std::endl;
template <class T>
T get_value(std::shared_ptr<PropertyValue> ptr)
{
    auto value = std::static_pointer_cast<TypedPropertyValue<T>>(ptr);
    return value->get_value();
}

// Get the value by key and store into a variable.
// E.g.
//      double d;
//      dict["weight"] >> d;
template <class T>
inline void operator >> (std::shared_ptr<PropertyValue> ptr, T& var)
{
    auto value = std::static_pointer_cast<TypedPropertyValue<T>>(ptr);
    var = value->get_value();
}


// Output Property
inline std::ostream& operator<< (std::ostream& os,
                                 const std::shared_ptr<PropertyValue>& property)
{
    property->print(os);
    return os;
}

#endif //DRAWMESH_PROPERTY_H
