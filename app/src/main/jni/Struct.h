//
// Created by Admin on 11/10/2023.
//

#pragma once

bool IsNativeObjectAlive(void *unity_obj){
    return (unity_obj != nullptr && (*(uintptr_t*)((uintptr_t)unity_obj + 0x8))); //class Object -> private IntPtr m_CachedPtr; // 0x8
}

#define OBFUSCATE_METHOD(image, namespaze, clazz, name, args) \
IL2Cpp::Il2CppGetMethodOffset(OBFUSCATE(image), OBFUSCATE(namespaze), OBFUSCATE(clazz), OBFUSCATE(name), args)

#define OBFUSCATE_FIELD(image, namespaze, clazz, name) \
IL2Cpp::Il2CppGetFieldOffset(OBFUSCATE(image), OBFUSCATE(namespaze), OBFUSCATE(clazz), OBFUSCATE(name))


class LOrganWrapper {
public:

    bool get_isTower() {
        static const auto _ = (bool(*)(void*)) OBFUSCATE_METHOD("Project.Plugins_d.dll", "NucleusDrive.Logic", "LOrganWrapper", "get_isTower", 0);
        return this && _ && _(this);
    }
};

class ValuePropertyComponent {
public:

    int get_actorHp() {
        static const auto _ = (int(*)(void*)) OBFUSCATE_METHOD("Project.Plugins_d.dll", "NucleusDrive.Logic", "ValuePropertyComponent", "get_actorHp", 0);
        return (this && _) ? _(this) : -1;
    }

    void set_actorHp(int value) {
        static const auto _ = (void(*)(void*, int)) OBFUSCATE_METHOD("Project.Plugins_d.dll", "NucleusDrive.Logic", "ValuePropertyComponent", "set_actorHp", 1);
        if (this && _) _(this, value);
    }
};

class HudComponent3D {
public:

    void SetPlayerName(const char* playerName, const char* prefixName = "", bool isGuideLevel = false) {
        static const auto _ = (void(*)(void*, Il2CppString*, Il2CppString*, bool)) OBFUSCATE_METHOD("Project_d.dll", "Assets.Scripts.GameLogic", "HudComponent3D", "SetPlayerName", 3);
        if (this && _) _(this, Il2CppString::Create(playerName), Il2CppString::Create(prefixName), isGuideLevel);
    }
};

class ActorLinker {
public:

    HudComponent3D *HudControl() {
        static uintptr_t _ = OBFUSCATE_FIELD("Project_d.dll", "Kyrios.Actor", "ActorLinker", "HudControl");
        return (this && _) ? *(HudComponent3D**) ((uintptr_t) this + _) : nullptr;
    }

    bool IsHostPlayer() {
        static const auto _ = (bool(*)(void*)) OBFUSCATE_METHOD("Project_d.dll", "Kyrios.Actor", "ActorLinker", "IsHostPlayer", 0);
        return this && _ && _(this);
    }

    int get_objCamp() {
        static const auto _ = (int(*)(void*)) OBFUSCATE_METHOD("Project_d.dll", "Kyrios.Actor", "ActorLinker", "get_objCamp", 0);
        return (this && _) ? _(this) : -1;
    }

    void *AsHero() {
        static const auto _ = (void*(*)(void*)) OBFUSCATE_METHOD("Project_d.dll", "Kyrios.Actor", "ActorLinker", "AsHero", 0);
        return (this && _) ? _(this) : nullptr;
    }
};

class LActorRoot {
public:

    ValuePropertyComponent *ValueComponent() {
        static uintptr_t _ = OBFUSCATE_FIELD("Project.Plugins_d.dll", "NucleusDrive.Logic", "LActorRoot", "ValueComponent");
        return (this && _) ? *(ValuePropertyComponent**) ((uintptr_t) this + _) : nullptr;
    }

    LOrganWrapper *AsOrgan() {
        static const auto _ = (LOrganWrapper *(*)(void*)) OBFUSCATE_METHOD("Project.Plugins_d.dll", "NucleusDrive.Logic", "LActorRoot", "AsOrgan", 0);
        return (this && _) ? _(this) : nullptr;
    }

    int GiveMyEnemyCamp() {
        static const auto _ = (int (*)(void*)) OBFUSCATE_METHOD("Project.Plugins_d.dll", "NucleusDrive.Logic", "LActorRoot", "GiveMyEnemyCamp", 0);
        return (this && _) ? _(this) : -1;
    }
};