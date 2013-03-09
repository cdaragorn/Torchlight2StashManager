#ifndef TOOLS_H
#define TOOLS_H

template<typename T>
T convert(void* data)
{
    return *reinterpret_cast<T*>(data);
}

#endif // TOOLS_H
