#include "Includes/obfuscate.h"
#include "Il2cppEnc.h"
#include "base64.h"
#include <iostream>
#include <sstream>
#include <iomanip>
#include <cstring>
#include <cstdlib>
#include <string>

// Chuyển đổi chuỗi nhị phân thành chuỗi thập lục phân
std::string bin2hex3(const std::string& input) {
    std::ostringstream hexStream;
    for (unsigned char c : input) {
        hexStream << std::hex << std::setw(2) << std::setfill('0') << static_cast<int>(c);
    }
    return hexStream.str();
}

// Chuyển đổi chuỗi thập lục phân thành chuỗi nhị phân
std::string hex2bin3(const std::string& input) {
    std::string result;
    result.reserve(input.length() / 2);
    for (size_t i = 0; i < input.length(); i += 2) {
        std::istringstream hexChars(input.substr(i, 2));
        int value;
        hexChars >> std::hex >> value;
        result.push_back(static_cast<char>(value));
    }
    return result;
}

// Hàm mã hóa/giải mã XOR
std::string xorEncryptReal3(const std::string& input, const std::string& key1, const std::string& key2) {
    std::string result;
    size_t len = input.length();
    size_t key1Len = key1.length();
    size_t key2Len = key2.length();
    for (size_t i = 0; i < len; ++i) {
        result += input[i] ^ key1[i % key1Len] ^ key2[i % key2Len];
    }
    return result;
}

std::string xorEncrypt3(const std::string& input, const std::string& key1, const std::string& key2) {
    return base64_encode(bin2hex3(xorEncryptReal3(input, key1, key2)));
}

std::string xorDecrypt3(const std::string& input, const std::string& key1, const std::string& key2) {
    return xorEncryptReal3(hex2bin3(base64_decode(input)), key1, key2);
}

// Hàm hỗ trợ để ghi đè dữ liệu nhạy cảm
void SecureZeroMemory(void* ptr, size_t len) {
    volatile unsigned char* p = (volatile unsigned char*)ptr;
    while (len--) {
        *p++ = 0;
    }
}

#define targetLib OBFUSCATE("libil2cpp.so")
#define g_LogTag OBFUSCATE("IL2CPP-LOGGER")

typedef unsigned short UTF16;
typedef wchar_t UTF32;
typedef char UTF8;

namespace {
	CheatString *(*il2cpp_string_new)(const char *str);
    CheatString *(*il2cpp_string_new_utf16)(const wchar_t *str, int32_t length);
	void **(*il2cpp_domain_get_assemblies)(const void *domain, size_t *size);
	void *(*il2cpp_domain_get)();
	const void *(*il2cpp_assembly_get_image)(const void *assembly);
	const char *(*il2cpp_image_get_name)(void *image);
    void *(*il2cpp_class_from_name)(const void *image, const char *namespaze, const char *name);
    void *(*il2cpp_class_get_field_from_name)(void *klass, const char *name);
	void (*il2cpp_field_static_get_value)(void *field, void *value);
    void (*il2cpp_field_static_set_value)(void *field, void *value);
	void *(*il2cpp_class_get_method_from_name)(void *klass, const char *name, int argsCount);
	size_t (*il2cpp_field_get_offset)(void *field);
}

const std::string key1 = OBFUSCATE("SEdNT0Q=");
const std::string key2 = OBFUSCATE("VklFVE5BTQ==");

void *Cheat::CheatGetImageByName(const char *image) {
    std::string decodedImageStr = xorDecrypt3(image, key1, key2);
    const char* decodedImage = decodedImageStr.c_str();
	
	size_t size;
	
	void **assemblies = il2cpp_domain_get_assemblies(il2cpp_domain_get(), &size);
	
	for(int i = 0; i < size; ++i) {
		void *img = (void *)il2cpp_assembly_get_image(assemblies[i]);
		const char *img_name = il2cpp_image_get_name(img);
		
		if (strcmp(img_name, decodedImage) == 0) {
			SecureZeroMemory(&decodedImageStr[0], decodedImageStr.size());
			return img;
		}
	}
	SecureZeroMemory(&decodedImageStr[0], decodedImageStr.size());
	return nullptr;
}

void *Cheat::CheatGetClassType(const char *image, const char *namespaze, const char *clazz) {
    std::string decodedNamespazeStr = xorDecrypt3(namespaze, key1, key2);
    std::string decodedClazzStr = xorDecrypt3(clazz, key1, key2);

    const char* decodedNamespaze = decodedNamespazeStr.c_str();
    const char* decodedClazz = decodedClazzStr.c_str();
	
	std::map<std::string, void *> cache;
	
	std::string s = image;
	s += decodedNamespaze;
	s += decodedClazz;
	if (cache.count(s) > 0) {
		SecureZeroMemory(&decodedNamespazeStr[0], decodedNamespazeStr.size());
        SecureZeroMemory(&decodedClazzStr[0], decodedClazzStr.size());
		return cache[s];
	}
		
	void *img = Cheat::CheatGetImageByName(image);
	if (!img) {
		//__android_log_print(ANDROID_LOG_ERROR, g_LogTag, OBFUSCATE("Can't find image %s!"), image);
		SecureZeroMemory(&decodedNamespazeStr[0], decodedNamespazeStr.size());
        SecureZeroMemory(&decodedClazzStr[0], decodedClazzStr.size());
		return nullptr;
	}
	
	void *klass = il2cpp_class_from_name(img, decodedNamespaze, decodedClazz);
	if (!klass) {
		//__android_log_print(ANDROID_LOG_ERROR, g_LogTag, OBFUSCATE("Can't find class %s!"), clazz);
		SecureZeroMemory(&decodedNamespazeStr[0], decodedNamespazeStr.size());
        SecureZeroMemory(&decodedClazzStr[0], decodedClazzStr.size());
		return nullptr;
	}
	
	cache[s] = klass;
	SecureZeroMemory(&decodedNamespazeStr[0], decodedNamespazeStr.size());
    SecureZeroMemory(&decodedClazzStr[0], decodedClazzStr.size());
	return klass;
}

void Cheat::CheatGetStaticFieldValue(const char *image, const char *namespaze, const char *clazz, const char *name, void *output) {
	void *img = Cheat::CheatGetImageByName(image);
	if (!img) {
		//__android_log_print(ANDROID_LOG_ERROR, g_LogTag, OBFUSCATE("Can't find image %s!"), image);
		return;
	}
	void *klass = Cheat::CheatGetClassType(image, namespaze, clazz);
	if (!klass) {
		//__android_log_print(ANDROID_LOG_ERROR, g_LogTag, OBFUSCATE("Can't find field %s!"), clazz, name);
		return;
	}
    void *field = il2cpp_class_get_field_from_name(klass, name);
	if (!field) {
		//__android_log_print(ANDROID_LOG_ERROR, g_LogTag, OBFUSCATE("Can't find field %s in class %s!"), name, clazz);
		return;
	}
	il2cpp_field_static_get_value(field, output);
}

void Cheat::CheatSetStaticFieldValue(const char *image, const char *namespaze, const char *clazz, const char *name, void* value) {
	void *img = Cheat::CheatGetImageByName(image);
	if (!img) {
		//__android_log_print(ANDROID_LOG_ERROR, g_LogTag, OBFUSCATE("Can't find image %s!"), image);
		return;
	}
	void *klass = Cheat::CheatGetClassType(image, namespaze, clazz);
	if (!klass) {
		//__android_log_print(ANDROID_LOG_ERROR, g_LogTag, OBFUSCATE("Can't find field %s!"), clazz, name);
		return;
	}
	void *field = il2cpp_class_get_field_from_name(klass, name);
	if (!field) {
		//__android_log_print(ANDROID_LOG_ERROR, g_LogTag, OBFUSCATE("Can't find field %s in class %s!"), name, clazz);
		return;
	}
	il2cpp_field_static_set_value(field, value);
}

void *Cheat::CheatGetMethodOffset(const char *image, const char *namespaze, const char *clazz, const char *name, int argsCount) {
	/*
	std::string decodedImageStr = xorEncrypt3(image, key1, key2);
    std::string decodedNamespazeStr = xorEncrypt3(namespaze, key1, key2);
    std::string decodedClazzStr = xorEncrypt3(clazz, key1, key2);
    std::string decodedNameStr = xorEncrypt3(name, key1, key2);
	
	std::string result = std::string("#define Method_") + clazz + "_" + name + std::string(" Cheat::CheatGetMethodOffset(OBFUSCATE(\"") + decodedImageStr
	+ std::string("\"), OBFUSCATE(\"") + decodedNamespazeStr + std::string("\"), OBFUSCATE(\"")
	+ decodedClazzStr + std::string("\"), OBFUSCATE(\"") + decodedNameStr + std::string("\"), ") + std::to_string(argsCount) + std::string(")")
	;
	
	__android_log_print(ANDROID_LOG_ERROR, g_LogTag, "%s", 
						result.c_str());
	return nullptr;
	*/
	
	
	std::string decodedNameStr = xorDecrypt3(name, key1, key2);

    const char* decodedName = decodedNameStr.c_str();
	void *img = Cheat::CheatGetImageByName(image);
	if (!img) {
		//__android_log_print(ANDROID_LOG_ERROR, g_LogTag, OBFUSCATE("Can't find image %s!"), image);
		SecureZeroMemory(&decodedNameStr[0], decodedNameStr.size());
		return nullptr;
	}
	void *klass = Cheat::CheatGetClassType(image, namespaze, clazz);
	if (!klass) {
		//__android_log_print(ANDROID_LOG_ERROR, g_LogTag, OBFUSCATE("Can't find method %s!"), clazz, name);
		SecureZeroMemory(&decodedNameStr[0], decodedNameStr.size());
		return nullptr;
	}
	void **method = (void**)il2cpp_class_get_method_from_name(klass, decodedName, argsCount);
	if (!method) {
		//__android_log_print(ANDROID_LOG_ERROR, g_LogTag, OBFUSCATE("Can't find method %s in class %s!"), name, clazz);
		SecureZeroMemory(&decodedNameStr[0], decodedNameStr.size());
		return nullptr;
	}
	//__android_log_print(ANDROID_LOG_DEBUG, g_LogTag, OBFUSCATE("%s - [%s] %s::%s: %p"), image, namespaze, clazz, name, *method);
	SecureZeroMemory(&decodedNameStr[0], decodedNameStr.size());
	return *method;
	
}


void *CheatGetMethodOffset(const char *image, const char *namespaze, const char *clazz, const char *name, int argsCount) {
    
    // Giải mã các chuỗi
    std::string decodedImageStr = xorDecrypt3(image, key1, key2);
    std::string decodedNamespazeStr = xorDecrypt3(namespaze, key1, key2);
    std::string decodedClazzStr = xorDecrypt3(clazz, key1, key2);
    std::string decodedNameStr = xorDecrypt3(name, key1, key2);

    // Chuyển các chuỗi đã giải mã sang kiểu const char*
    const char* decodedImage = decodedImageStr.c_str();
    const char* decodedNamespaze = decodedNamespazeStr.c_str();
    const char* decodedClazz = decodedClazzStr.c_str();
    const char* decodedName = decodedNameStr.c_str();

    // Sử dụng các chuỗi đã giải mã để làm việc
    void *img = Cheat::CheatGetImageByName(decodedImage);
    if (!img) {
        // Log lỗi nếu không tìm thấy hình ảnh
        SecureZeroMemory(&decodedImageStr[0], decodedImageStr.size());
        SecureZeroMemory(&decodedNamespazeStr[0], decodedNamespazeStr.size());
        SecureZeroMemory(&decodedClazzStr[0], decodedClazzStr.size());
        SecureZeroMemory(&decodedNameStr[0], decodedNameStr.size());
        return nullptr;
    }

    void *klass = Cheat::CheatGetClassType(decodedImage, decodedNamespaze, decodedClazz);
    if (!klass) {
        // Log lỗi nếu không tìm thấy lớp
        SecureZeroMemory(&decodedImageStr[0], decodedImageStr.size());
        SecureZeroMemory(&decodedNamespazeStr[0], decodedNamespazeStr.size());
        SecureZeroMemory(&decodedClazzStr[0], decodedClazzStr.size());
        SecureZeroMemory(&decodedNameStr[0], decodedNameStr.size());
        return nullptr;
    }

    void **method = (void**)il2cpp_class_get_method_from_name(klass, decodedName, argsCount);
    if (!method) {
        // Log lỗi nếu không tìm thấy phương thức
        SecureZeroMemory(&decodedImageStr[0], decodedImageStr.size());
        SecureZeroMemory(&decodedNamespazeStr[0], decodedNamespazeStr.size());
        SecureZeroMemory(&decodedClazzStr[0], decodedClazzStr.size());
        SecureZeroMemory(&decodedNameStr[0], decodedNameStr.size());
        return nullptr;
    }

    // Giải phóng bộ nhớ được cấp phát cho các chuỗi sau khi sử dụng
    SecureZeroMemory(&decodedImageStr[0], decodedImageStr.size());
    SecureZeroMemory(&decodedNamespazeStr[0], decodedNamespazeStr.size());
    SecureZeroMemory(&decodedClazzStr[0], decodedClazzStr.size());
    SecureZeroMemory(&decodedNameStr[0], decodedNameStr.size());

    // Trả về offset của phương thức
    return *method;
}

size_t Cheat::CheatGetFieldOffset(const char *image, const char *namespaze, const char *clazz, const char *name) {
	/*
	std::string decodedImageStr = xorEncrypt3(image, key1, key2);
    std::string decodedNamespazeStr = xorEncrypt3(namespaze, key1, key2);
    std::string decodedClazzStr = xorEncrypt3(clazz, key1, key2);
    std::string decodedNameStr = xorEncrypt3(name, key1, key2);
	
	std::string result = std::string("#define Field_") + clazz + "_" + name + std::string(" Cheat::CheatGetFieldOffset(OBFUSCATE(\"") + decodedImageStr
	+ std::string("\"), OBFUSCATE(\"") + decodedNamespazeStr + std::string("\"), OBFUSCATE(\"")
	+ decodedClazzStr + std::string("\"), OBFUSCATE(\"") + decodedNameStr + std::string("\"))")
	;
	
	__android_log_print(ANDROID_LOG_ERROR, g_LogTag, "%s", 
						result.c_str());
	return 0;
	*/
	
	std::string decodedNameStr = xorDecrypt3(name, key1, key2);

    const char* decodedName = decodedNameStr.c_str();
	void *img = Cheat::CheatGetImageByName(image);
	if (!img) {
		//__android_log_print(ANDROID_LOG_ERROR, g_LogTag, OBFUSCATE("Can't find image %s!"), image);
		SecureZeroMemory(&decodedNameStr[0], decodedNameStr.size());
		return -1;
	}
	void *klass = Cheat::CheatGetClassType(image, namespaze, clazz);
	if (!klass) {
		//__android_log_print(ANDROID_LOG_ERROR, g_LogTag, OBFUSCATE("Can't find field %s!"), clazz, name);
		SecureZeroMemory(&decodedNameStr[0], decodedNameStr.size());
		return -1;
	}
	void *field = il2cpp_class_get_field_from_name(klass, decodedName);
	if (!field) {
		//__android_log_print(ANDROID_LOG_ERROR, g_LogTag, OBFUSCATE("Can't find field %s in class %s!"), clazz, name);
		SecureZeroMemory(&decodedNameStr[0], decodedNameStr.size());
		return -1;
	}
	auto result = il2cpp_field_get_offset(field);
	//__android_log_print(ANDROID_LOG_DEBUG, g_LogTag, OBFUSCATE("%s - [%s] %s::%s: %p"), image, namespaze, clazz, name, (void *) result);
	SecureZeroMemory(&decodedNameStr[0], decodedNameStr.size());

	return result;
	
}

int is_surrogatect(UTF16 uc) {
    return (uc - 0xd800u) < 2048u;
}

int is_high_surrogatect(UTF16 uc) {
    return (uc & 0xfffffc00) == 0xd800;
}

int is_low_surrogatect(UTF16 uc) {
    return (uc & 0xfffffc00) == 0xdc00;
}

UTF32 surrogate_to_utf32ct(UTF16 high, UTF16 low) {
    return (high << 10) + low - 0x35fdc00;
}

const char* utf16_to_utf8ct(const UTF16* source, size_t len) {
    std::u16string s(source, source + len);
    std::wstring_convert<std::codecvt_utf8_utf16<char16_t>, char16_t> convert;
    return convert.to_bytes(s).c_str();
}

const wchar_t* utf16_to_utf32ct(const UTF16* source, size_t len) {
	auto output = new UTF32[len + 1];

    for (int i = 0; i < len; i++) {
        const UTF16 uc = source[i];
        if (!is_surrogatect(uc)) {
            output[i] = uc;
        }
        else {
            if (is_high_surrogatect(uc) && is_low_surrogatect(source[i]))
                output[i] = surrogate_to_utf32ct(uc, source[i]);
            else
                output[i] = L'?';
        }
    }

    output[len] = L'\0';
    return output;
}

const char* CheatString::CString() {
    return utf16_to_utf8ct(&this->start_char, this->length);
}

const wchar_t* CheatString::WCString() {
    return utf16_to_utf32ct(&this->start_char, this->length);
}

CheatString *CheatString::Create(const char *s) {
    return il2cpp_string_new(s);
}

CheatString *CheatString::Create(const wchar_t *s, int len) {
    return il2cpp_string_new_utf16(s, len);
}

void Cheat::CheatAttach() {
	void *IL2Cpp_Handle = nullptr;
	
	while (!IL2Cpp_Handle) {
		IL2Cpp_Handle = dlopen(targetLib, 4);
		sleep(1);
	}
	il2cpp_domain_get_assemblies = (void **(*)(const void *, size_t *)) dlsym(IL2Cpp_Handle, OBFUSCATE("il2cpp_domain_get_assemblies"));
	il2cpp_string_new = (CheatString *(*)(const char *)) dlsym(IL2Cpp_Handle, OBFUSCATE("il2cpp_string_new"));
    il2cpp_string_new_utf16 = (CheatString *(*)(const wchar_t *, int32_t)) dlsym(IL2Cpp_Handle, OBFUSCATE("il2cpp_string_new_utf16"));
	il2cpp_domain_get = (void *(*)()) dlsym(IL2Cpp_Handle, OBFUSCATE("il2cpp_domain_get"));
    il2cpp_assembly_get_image = (const void *(*)(const void *)) dlsym(IL2Cpp_Handle, OBFUSCATE("il2cpp_assembly_get_image"));
	il2cpp_image_get_name = (const char *(*)(void *)) dlsym(IL2Cpp_Handle, OBFUSCATE("il2cpp_image_get_name"));
    il2cpp_class_from_name = (void* (*)(const void*, const char*, const char *)) dlsym(IL2Cpp_Handle, OBFUSCATE("il2cpp_class_from_name"));
    il2cpp_class_get_field_from_name = (void* (*)(void*, const char *)) dlsym(IL2Cpp_Handle, OBFUSCATE("il2cpp_class_get_field_from_name"));
    il2cpp_field_static_get_value = (void (*)(void*, void *)) dlsym(IL2Cpp_Handle, OBFUSCATE("il2cpp_field_static_get_value"));
	il2cpp_field_static_set_value = (void (*)(void*, void *)) dlsym(IL2Cpp_Handle, OBFUSCATE("il2cpp_field_static_set_value"));
    il2cpp_class_get_method_from_name = (void* (*)(void *, const char*, int)) dlsym(IL2Cpp_Handle, OBFUSCATE("il2cpp_class_get_method_from_name"));
    il2cpp_field_get_offset = (size_t (*)(void *)) dlsym(IL2Cpp_Handle, OBFUSCATE("il2cpp_field_get_offset"));
	dlclose(IL2Cpp_Handle);
}
