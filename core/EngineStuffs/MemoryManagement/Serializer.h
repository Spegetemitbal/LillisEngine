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
  private:
  std::string buffer;
};



#endif //SERIALIZER_H