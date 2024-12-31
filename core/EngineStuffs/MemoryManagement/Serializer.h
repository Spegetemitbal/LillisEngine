//
// Created by Somed on 12/17/2024.
//

#ifndef SERIALIZER_H
#define SERIALIZER_H

#include <string>

class Serializer {
  public:
  template <typename T>
  size_t InsertToBuffer(T* object)
  {
    char* object_as_bytes = (char*)object;
    for (int i = 0; i < sizeof(T); i++) {
      buffer.push_back(object_as_bytes[i]);
    }
    return sizeof(T);
  }

  template <typename T>
  size_t GetFromBuffer(T* object)
  {
    if (sizeof(T) > buffer.length()) return 0;
    char* object_as_bytes = (char*)object;
    for (int i = 0; i < sizeof(T); i++) {
      object_as_bytes[i] = buffer[i];
    }
    buffer.erase(0, sizeof(T));
    return sizeof(T);
  }

  size_t InsertStringToBuffer(std::string* object)
  {
    const char* object_as_bytes = object->c_str();
    size_t stringSize = 0;
    for (int i = 0; object_as_bytes[i] != '\0'; i++)
    {
      buffer.push_back(object_as_bytes[i]);
      stringSize++;
    }
    return stringSize;
  }

  size_t GetStringFromBuffer(std::string* object)
  {
    if (buffer.length() == 0) return 0;

    size_t stringSize = 0;
    for (int i = 0; buffer[i] != '\0'; i++)
    {
      object->push_back(buffer[i]);
      buffer.erase(buffer.front());
      stringSize++;
      if (i + 1 == buffer.length())
      {
        return 0;
      }
    }
    return stringSize;
  }

  private:
  std::string buffer;
};



#endif //SERIALIZER_H
