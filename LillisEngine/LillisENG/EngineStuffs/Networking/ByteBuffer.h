#pragma once

//Snagged from in-class code

template <typename T>
size_t copy_to_buffer(char* buffer, T* object, size_t buffer_size)
{
	if (sizeof(T) > buffer_size) return 0;
	char* object_as_bytes = (char*)object;
	for (int i = 0; i < sizeof(T); i++) {
		buffer[i] = object_as_bytes[i];
	}
	return sizeof(T);
}

template <typename T>
size_t copy_from_buffer(const char* buffer, T* object, size_t buffer_size)
{
	if (sizeof(T) > buffer_size) return 0;
	char* object_as_bytes = (char*)object;
	for (int i = 0; i < sizeof(T); i++) {
		object_as_bytes[i] = buffer[i];
	}
	return sizeof(T);
}