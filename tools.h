#ifndef TOOLS_H
#define TOOLS_H

#include <cstring>

template<typename T>
T convert(const void* data)
{
    return *reinterpret_cast<const T*>(data);
}

template<typename T>
void write(void* data, T value)
{
    memcpy(data, &value, sizeof(T));
}

#endif // TOOLS_H
