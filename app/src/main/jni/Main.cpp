#include <list>
#include <vector>
#include <string.h>
#include <pthread.h>
#include <thread>
#include <cstring>
#include <jni.h>
#include <unistd.h>
#include <fstream>
#include <iostream>
#include <dlfcn.h>
#include "Includes/Logger.h"
#include "Includes/obfuscate.h"
#include "Includes/Utils.h"
#include "KittyMemory/MemoryPatch.h"
#include "Menu/Setup.h"
#include "duymmo/Call_Me.h"
#include "Unity/MonoString.h"
#include "Includes/Macros.h"
#include "autoupdate.h"
#include "Unity/unity.h"
#include "Unity/Vector3.hpp"
#include "Unity/VInt3.hpp"
#include "duymmo/Tools/Dobby/dobby.h"
#include <cmath>
#include <chrono>
#include <jni.h>
#include <unistd.h>
#include <dlfcn.h>
#include <sys/types.h>
#include <stddef.h>

struct My_Patches {
    // let's assume we have patches for these functions for whatever game
    // like show in miniMap boolean function
    MemoryPatch srank, antiskin, lsd, fps1, fps2, fps3, fps4, fps5, giamhoichieu, fullhuanhuy1, fullhuanhuy2, fullhuanhuy3, fakerank1, fakerank2, fakerank3, fakerank4, fakerank5;

} hexPatches;
bool antiskin;
bool fps;
bool giamhoichieu;

//bool fire;



#define GetMethodOffset(a, b, c, d, e) IL2Cpp::Il2CppGetMethodOffset(OBFUSCATE(a), OBFUSCATE(b), OBFUSCATE(c), OBFUSCATE(d), e)
#define GetFieldOffset(a, b, c, d) IL2Cpp::Il2CppGetFieldOffset(OBFUSCATE(a), OBFUSCATE(b), OBFUSCATE(c), OBFUSCATE(d))
#define GetAdrMethod(a, b, c) Adr_Method(il2cppMap, OBFUSCATE(a), b, c)
#define GetAdrMethodV2(a, b, c) Adr_get_adr_Method(OBFUSCATE(a), b, c)

#define TOOL_HOOK(address, ptr, orig)  Tools::Hook((void *) (uintptr_t) address, (void *)ptr,(void **)&orig);

#define Adr_LActorRoot_UpdateLogicCn GetMethodOffset("Project.Plugins_d.dll", "NucleusDrive.Logic", "LActorRoot", "UpdateLogic", 1)
#define Adr_ActorLinker_get_ObjID GetMethodOffset("Project_d.dll", "Kyrios.Actor", "ActorLinker", "get_ObjID", 0)
#define Adr_LActorRoot_get_ObjID GetMethodOffset("Project.Plugins_d.dll", "NucleusDrive.Logic", "LActorRoot", "get_ObjID", 0)
#define Adr_ActorLinker_IsHostPlayer GetMethodOffset("Project_d.dll", "Kyrios.Actor", "ActorLinker", "IsHostPlayer", 0)
#define Adr_ActorLinker_UpdateCn GetMethodOffset("Project_d.dll", "Kyrios.Actor", "ActorLinker", "Update", 0)
#define Adr_ValuePropertyComponent_get_actorHp GetMethodOffset("Project.Plugins_d.dll", "NucleusDrive.Logic", "ValuePropertyComponent", "get_actorHp", 0)
#define Adr_ValuePropertyComponent_get_actorHpTotal GetMethodOffset("Project.Plugins_d.dll", "NucleusDrive.Logic", "ValuePropertyComponent", "get_actorHpTotal", 0)
#define Adr_LActorRoot_ValueComponent GetFieldOffset("Project.Plugins_d.dll", "NucleusDrive.Logic", "LActorRoot", "ValueComponent")
#define Adr_LActorRoot_get_forward GetMethodOffset("Project.Plugins_d.dll", "NucleusDrive.Logic", "LActorRoot", "get_forward", 0)
#define Adr_LActorRoot_get_location GetMethodOffset("Project.Plugins_d.dll", "NucleusDrive.Logic", "LActorRoot", "get_location", 0)
#define Adr_LActorRoot_AsHero GetMethodOffset("Project.Plugins_d.dll", "NucleusDrive.Logic", "LActorRoot", "AsHero", 0)
#define Adr_LActorRoot_AsOrgan GetMethodOffset("Project.Plugins_d.dll", "NucleusDrive.Logic", "LActorRoot", "AsOrgan", 0)
#define Adr_LActorRoot_AsMonster GetMethodOffset("Project.Plugins_d.dll", "NucleusDrive.Logic", "LActorRoot", "AsMonster", 0)
#define Adr_LActorRoot_GiveMyEnemyCamp GetMethodOffset("Project.Plugins_d.dll", "NucleusDrive.Logic", "LActorRoot", "GiveMyEnemyCamp", 0)
#define Adr_ActorLinker_HudControl GetFieldOffset("Project_d.dll", "Kyrios.Actor", "ActorLinker", "HudControl")
#define Adr_ActorLinker_get_objCamp GetMethodOffset("Project_d.dll", "Kyrios.Actor", "ActorLinker", "get_objCamp", 0)
#define Adr_ValuePropertyComponent_set_actorHp GetMethodOffset("Project.Plugins_d.dll", "NucleusDrive.Logic", "ValuePropertyComponent", "set_actorHp", 1)
#define Adr_ActorLinker_get_objType GetMethodOffset("Project_d.dll", "Kyrios.Actor", "ActorLinker", "get_objType", 0)
#define Adr_LObjWrapper_get_IsDeadState GetMethodOffset("Project.Plugins_d.dll", "NucleusDrive.Logic", "LObjWrapper", "get_IsDeadState", 0)
#define Adr_LActorRoot_ActorControl GetFieldOffset("Project.Plugins_d.dll", "NucleusDrive.Logic", "LActorRoot", "ActorControl")
#define Adr_InBattleMsgNetCore_SendInBattleMsg_InputChat GetMethodOffset("Project_d.dll", "Assets.Scripts.GameSystem", "InBattleMsgNetCore", "SendInBattleMsg_InputChat", 2)



using zygisk::Api;
using zygisk::AppSpecializeArgs;
using zygisk::ServerSpecializeArgs;

void hack();

JavaVM *jvm;
JNIEnv *genv;


class MyModule : public zygisk::ModuleBase {
 public:
  void onLoad(Api *api, JNIEnv *env) override {
    this->api_ = api;
    this->env_ = env;
    genv = env;
   // gEnv = env;

  }

  void preAppSpecialize(AppSpecializeArgs *args) override {
    static constexpr const char *packages[] = {
        "com.garena.game.kgvn"
    };
    const char *process = env_->GetStringUTFChars(args->nice_name, nullptr);
    for (const auto *package: packages) {
      is_game_ = (strcmp(process, package) == 0);
      if (is_game_) {
        break;
      }
    }
    env_->ReleaseStringUTFChars(args->nice_name, process);
  }

  void postAppSpecialize(const AppSpecializeArgs *args) override {
    if (is_game_) {
       genv->GetJavaVM(&jvm);
     //  genv->GetJavaVM(&gJvm);
      std::thread{hack}.detach();
    }
  }

 private:
  Api *api_ = nullptr;
  JNIEnv *env_ = nullptr;
  bool is_game_ = false;
};







struct {
    float GetFieldOfView = 0;
    float SetFieldOfView = 0;
    bool Active = false;
} WideView;
bool HackMap,Cd,Ult,Lsd,Ten,Avatar,Fps;
//hack nap
enum COM_PLAYERCAMP { //void hack map
 ComPlayercampMid = 0,
 ComPlayercamp1 = 1,
 ComPlayercamp2 = 2, };
void (*_SetVisible)(...); //void hack map
void SetVisible(void *instance, COM_PLAYERCAMP camp, bool bVisible, const bool forceSync = false) {
    if (instance != NULL && HackMap) {
    if (camp == ComPlayercamp1 || camp == ComPlayercamp2) {
      bVisible = true;
    }
  }
   return _SetVisible(instance, camp, bVisible, forceSync);
}

//cam xa
float (*old_GetCameraHeightRateValue)(void *instance, int *type);
float GetCameraHeightRateValue(void *instance, int *type) {
 if (instance != NULL) {
  WideView.GetFieldOfView = old_GetCameraHeightRateValue(instance, type);
  if (WideView.SetFieldOfView != 0) {
   WideView.Active = false;
   return (float) WideView.SetFieldOfView + WideView.GetFieldOfView;
  }
  return WideView.GetFieldOfView;
 }
 return old_GetCameraHeightRateValue(instance, type);
}

void (*OnCameraHeightChanged)(void *instance);
void (*old_CameraSystemUpdate)(void *instance);
void CameraSystemUpdate(void *instance) {
 if (instance != NULL && WideView.Active) {
  OnCameraHeightChanged(instance);
 }
 old_CameraSystemUpdate(instance);
}
//hiện hồi chiêu

std::string getToast = "Modder By CakMods X HGMOD";
bool SpamChat;

String *CreateMonoString(const char *str)
{
    return Il2CppString::Create(str);
}

uintptr_t (*AsHero)(...);
void (*_SetPlayerName)(...);
void (*old_Update)(uintptr_t instance);
void (*old_InBattleMsgNetCore_SendInBattleMsg_InputChat)(String *, int);

void AUpdate(uintptr_t instance)
{
    if (instance != NULL)
    {
        uintptr_t SkillControl = AsHero(instance);
        uintptr_t HudControl = *(int *)(instance + 0x3c);
        if (HudControl > 0 && SkillControl > 0)
        {
            uintptr_t Skill1Cd = *(int *)(SkillControl + 0x28) / 1000;
            uintptr_t Skill2Cd = *(int *)(SkillControl + 0x40) / 1000;
            uintptr_t Skill3Cd = *(int *)(SkillControl + 0x58) / 1000;
            uintptr_t Skill4Cd = *(int *)(SkillControl + 0x88) / 1000;

            string openSk = "[";
            string closeSk = "] ";
            string sk1 = to_string(Skill1Cd);
            string sk2 = to_string(Skill2Cd);
            string sk3 = to_string(Skill3Cd);
            string sk4 = to_string(Skill4Cd);
            string ShowSkill = openSk + sk1 + closeSk + openSk + sk2 + closeSk + openSk + sk3 + closeSk;
            string ShowSkill2 = openSk + sk4 + closeSk;

            const char *str1 = ShowSkill.c_str();
            const char *str2 = ShowSkill2.c_str();

            if (Cd)
            {
                String *playerName = CreateMonoString(str1);
                String *prefixName = CreateMonoString(str2);
                _SetPlayerName(HudControl, playerName, prefixName, true);
            }

            // Thêm điều kiện SpamChat
            if (SpamChat)
            {
                String *msg = Il2CppString::Create(getToast.c_str());
                old_InBattleMsgNetCore_SendInBattleMsg_InputChat(msg, 0);
            }
        }
    }

    return old_Update(instance);
}




///



#define OBFUSCATE_METHOD(image, namespaze, clazz, name, args) \
IL2Cpp::Il2CppGetMethodOffset(OBFUSCATE(image), OBFUSCATE(namespaze), OBFUSCATE(clazz), OBFUSCATE(name), args)


class Camera {
	public:
        static Camera *get_main() {
        Camera *(*get_main_) () = (Camera *(*)())OBFUSCATE_METHOD("UnityEngine.CoreModule.dll", "UnityEngine", "Camera", "get_main", 0);
        return get_main_();
    }
	
    Vector3 WorldToScreenPoint(Vector3 position) {
        Vector3 (*WorldToScreenPoint_)(Camera *camera, Vector3 position) = (Vector3 (*)(Camera *, Vector3))OBFUSCATE_METHOD("UnityEngine.CoreModule.dll", "UnityEngine", "Camera", "WorldToScreenPoint", 1);
        return WorldToScreenPoint_(this, position);
    }
};

class ValueLinkerComponent {
    public:
        int get_actorHp() {
            int (*get_actorHp_)(ValueLinkerComponent * objLinkerWrapper) = (int (*)(ValueLinkerComponent *))OBFUSCATE_METHOD("Project_d.dll", "Kyrios.Actor", "ValueLinkerComponent", "get_actorHp", 0);
            return get_actorHp_(this);
        }

        int get_actorHpTotal() {
            int (*get_actorHpTotal_)(ValueLinkerComponent * objLinkerWrapper) =
                (int (*)(ValueLinkerComponent *))OBFUSCATE_METHOD("Project_d.dll", "Kyrios.Actor", "ValueLinkerComponent", "get_actorHpTotal", 0);
            return get_actorHpTotal_(this);
        }
};



class CActorInfo {
    public:
        string *ActorName() {
            return *(string **)((uintptr_t)this + 0xC);
        }

        int hudHeight() {
            return *(int *)((uintptr_t)this + 0x70); 
        }
};


class ActorConfig{
	public:
	
	int ConfigID() {
		return *(int *) ((uintptr_t) this + 0x10); //public Int32 ConfigID; // 0x10
	}
};


class ActorLinker {
    public:
        ValueLinkerComponent *ValueComponent() {
            return *(ValueLinkerComponent **)((uintptr_t)this + 0x18); //public ValueLinkerComponent ValueComponent; // 0x18

        }

        ActorConfig *ObjLinker() {
            return *(ActorConfig **) ((uintptr_t) this + 0x98); //public ActorConfig ObjLinker; // 0x9C
        }

        Vector3 get_position() {
            Vector3 (*get_position_)(ActorLinker * linker) = (Vector3(*)(ActorLinker *))OBFUSCATE_METHOD("Project_d.dll", "Kyrios.Actor", "ActorLinker", "get_position", 0);
            return get_position_(this);
        }
        Quaternion get_rotation() {
            Quaternion (*get_rotation_)(ActorLinker *linker) = (Quaternion (*)(ActorLinker *))OBFUSCATE_METHOD("Project_d.dll", "Kyrios.Actor", "ActorLinker", "get_rotation", 0);
            return get_rotation_(this);
        }
        bool IsHostCamp() {
            bool (*IsHostCamp_)(ActorLinker *linker) = (bool (*)(ActorLinker *))OBFUSCATE_METHOD("Project_d.dll", "Kyrios.Actor", "ActorLinker", "IsHostCamp", 0);
            return IsHostCamp_(this);
        }
        
        bool IsHostPlayer() {
            bool (*IsHostPlayer_)(ActorLinker *linker) = (bool (*)(ActorLinker *))OBFUSCATE_METHOD("Project_d.dll", "Kyrios.Actor", "ActorLinker", "IsHostPlayer", 0);
            return IsHostPlayer_(this);
        }
        bool isMoving() {
            return *(bool *) ((uintptr_t) this + 0x2ee); //	public bool isMoving; // 0x2F2
        }

        Vector3 get_logicMoveForward() {
            Vector3 (*get_logicMoveForward_)(ActorLinker *linker) = (Vector3 (*)(ActorLinker *))OBFUSCATE_METHOD("Project_d.dll", "Kyrios.Actor", "ActorLinker", "get_logicMoveForward", 0);
            return get_logicMoveForward_(this);
        }
        bool get_bVisible() {
            bool (*get_bVisible_)(ActorLinker *linker) = (bool (*)(ActorLinker *))OBFUSCATE_METHOD("Project_d.dll", "Kyrios.Actor", "ActorLinker", "get_bVisible", 0);
            return get_bVisible_(this);
        }
            
};

class ActorManager {
	public:
	
	List<ActorLinker *> *GetAllHeros() {
		List<ActorLinker *> *(*_GetAllHeros)(ActorManager *actorManager) = (List<ActorLinker *> *(*)(ActorManager *))OBFUSCATE_METHOD("Project_d.dll", "Kyrios.Actor", "ActorManager", "GetAllHeros", 0);
		return _GetAllHeros(this);
	}
};

class KyriosFramework {
	public:
	
	static ActorManager *get_actorManager() {
		auto get_actorManager_ = (ActorManager *(*)())OBFUSCATE_METHOD("Project_d.dll", "Kyrios", "KyriosFramework", "get_actorManager", 0);
		return get_actorManager_();
	}
};

class PlayerMovement {
public:
    static Vector3 get_velocity(PlayerMovement* instance) {
        if (instance == nullptr) return Vector3::Zero();

        VInt3 vint3_velocity = ((VInt3(*)(PlayerMovement*))OBFUSCATE_METHOD("Project.Plugins_d.dll", "NucleusDrive.Logic", "PlayerMovement", "get_velocity", 0))(instance);
        return Vector3(vint3_velocity.X / 1000.0f, vint3_velocity.Y / 1000.0f, vint3_velocity.Z / 1000.0f); // Sử dụng X, Y, Z
    }

    static Vector3 VInt3ToVector3(VInt3 vint3) {
        return Vector3(vint3.X / 1000.0f, vint3.Y / 1000.0f, vint3.Z / 1000.0f); // Sử dụng X, Y, Z
    }
};

class LActorRoot {
public:
    // ... (các khai báo khác)

    VInt3* _location() {
        return (VInt3*)((uintptr_t)this + 0x80); //private VInt3 _location; // 0x80
    }

    // Cách gọi thẳng vector3 của get_vec3 thông qua _location
    Vector3 get_location_vec3() {
       // return _location()->get_vec3();
    }
};





struct EntityInfo {
    Vector3 myPos;
	Vector3 enemyPos;
	Vector3 moveForward;
	int ConfigID;
	bool isMoving;
};

EntityInfo EnemyTarget;



Vector3 RotateVectorByQuaternion(Quaternion q) {
	Vector3 v(0.0f, 0.0f, 1.0f);
    float w = q.W, x = q.X, y = q.Y, z = q.Z;

    Vector3 u(x, y, z);
    Vector3 cross1 = Vector3::Cross(u, v);
    Vector3 cross2 = Vector3::Cross(u, cross1);
    Vector3 result = v + 2.0f * cross1 * w + 2.0f * cross2;

    return result;
}

float SquaredDistance(Vector3 v, Vector3 o) {
	return (v.X - o.X) * (v.X - o.X) + (v.Y - o.Y) * (v.Y - o.Y) + (v.Z - o.Z) * (v.Z - o.Z);
}

Vector3 calculateSkillDirection(Vector3 myPosi, Vector3 enemyPosi, bool isMoving, Vector3 moveForward) {
	if (isMoving) {enemyPosi += moveForward;}
    Vector3 direction = enemyPosi - myPosi;
    direction.Normalize();
	return direction;
}

bool AimSkill;
bool AimYue;
bool AimGildur;
bool AimEnzo;
bool AimFlo;
bool AimGrakk;
bool AimNata;
bool isCharging;
int mode = 0, aimType = 1, drawType = 2, skillSlot;


/*
	
Vector3 (*_GetUseSkillDirection)(void *instance, bool isTouchUse);
Vector3 GetUseSkillDirection(void *instance, bool isTouchUse) {
    if (instance != NULL && AimSkill) {
        if (EnemyTarget.myPos != Vector3::Zero() && EnemyTarget.enemyPos != Vector3::Zero()) {
            if ((skillSlot == 1 || skillSlot == 2) && EnemyTarget.ConfigID == 545) {
                return calculateSkillDirection(EnemyTarget.myPos, EnemyTarget.enemyPos, EnemyTarget.isMoving, EnemyTarget.moveForward);
            }
        }
    }
    return _GetUseSkillDirection(instance, isTouchUse);
}
*/

Vector3 (*_GetUseSkillDirection)(void *instance, bool isTouchUse);
Vector3 GetUseSkillDirection(void *instance, bool isTouchUse) {
    if (instance != NULL) {
        if (AimSkill && EnemyTarget.ConfigID == 196 && skillSlot == 2) {
            if (EnemyTarget.myPos != Vector3::Zero() && EnemyTarget.enemyPos != Vector3::Zero()) {
                return calculateSkillDirection(EnemyTarget.myPos, EnemyTarget.enemyPos, EnemyTarget.isMoving, EnemyTarget.moveForward);
            }
        }
        
        if (AimYue && (skillSlot == 1 || skillSlot == 2) && EnemyTarget.ConfigID == 545) {
            if (EnemyTarget.myPos != Vector3::Zero() && EnemyTarget.enemyPos != Vector3::Zero()) {
                return calculateSkillDirection(EnemyTarget.myPos, EnemyTarget.enemyPos, EnemyTarget.isMoving, EnemyTarget.moveForward);
            }
        }
		if (AimGildur && EnemyTarget.ConfigID == 108 && skillSlot == 2) {
            if (EnemyTarget.myPos != Vector3::Zero() && EnemyTarget.enemyPos != Vector3::Zero()) {
                return calculateSkillDirection(EnemyTarget.myPos, EnemyTarget.enemyPos, EnemyTarget.isMoving, EnemyTarget.moveForward);
            }
        }
		if (AimEnzo && EnemyTarget.ConfigID == 195 && skillSlot == 2) {
            if (EnemyTarget.myPos != Vector3::Zero() && EnemyTarget.enemyPos != Vector3::Zero()) {
                return calculateSkillDirection(EnemyTarget.myPos, EnemyTarget.enemyPos, EnemyTarget.isMoving, EnemyTarget.moveForward);
            }
        }
        if (AimFlo && EnemyTarget.ConfigID == 521 && skillSlot == 1) {
            if (EnemyTarget.myPos != Vector3::Zero() && EnemyTarget.enemyPos != Vector3::Zero()) {
                return calculateSkillDirection(EnemyTarget.myPos, EnemyTarget.enemyPos, EnemyTarget.isMoving, EnemyTarget.moveForward);
            }
        }
        if (AimGrakk && EnemyTarget.ConfigID == 175 && skillSlot == 2) {
            if (EnemyTarget.myPos != Vector3::Zero() && EnemyTarget.enemyPos != Vector3::Zero()) {
                return calculateSkillDirection(EnemyTarget.myPos, EnemyTarget.enemyPos, EnemyTarget.isMoving, EnemyTarget.moveForward);
            }
        }
		if (AimNata && EnemyTarget.ConfigID == 142 && skillSlot == 1) {
            if (EnemyTarget.myPos != Vector3::Zero() && EnemyTarget.enemyPos != Vector3::Zero()) {
                return calculateSkillDirection(EnemyTarget.myPos, EnemyTarget.enemyPos, EnemyTarget.isMoving, EnemyTarget.moveForward);
            }
        }
        
    }
    
    return _GetUseSkillDirection(instance, isTouchUse);
}

uintptr_t m_isCharging, m_currentSkillSlotType;
void (*_UpdateLogic)(void *instance, int delta);
void UpdateLogic(void *instance, int delta){
    if (instance != NULL) {
        isCharging = *(bool *)((uintptr_t)instance + 0x62); //private bool m_isCharging; // 0x56
        skillSlot = *(int *)((uintptr_t)instance + 0x30); //private SkillSlotType m_currentSkillSlotType; // 0x2C
    }

    if (AimSkill || AimYue || AimGildur || AimEnzo || AimFlo || AimGrakk || AimNata)  // Thêm AimYue vào điều kiện
    {
        Quaternion rotation;
        float minDistance = std::numeric_limits<float>::infinity();
        float minDirection = std::numeric_limits<float>::infinity();
        float minHealth = std::numeric_limits<float>::infinity();
        float minHealth2 = std::numeric_limits<float>::infinity();
        float minHealthPercent = std::numeric_limits<float>::infinity();
        ActorLinker *Entity = nullptr;

        ActorManager *get_actorManager = KyriosFramework::get_actorManager();
        if (get_actorManager == nullptr) return;

        List<ActorLinker *> *GetAllHeros = get_actorManager->GetAllHeros();
        if (GetAllHeros == nullptr) return;

        ActorLinker **actorLinkers = (ActorLinker **) GetAllHeros->getItems();

        for (int i = 0; i < GetAllHeros->getSize(); i++)
        {
            ActorLinker *actorLinker = actorLinkers[(i *2) + 1];
            if (actorLinker == nullptr) continue;

            if (actorLinker->IsHostPlayer()) {
                rotation = actorLinker->get_rotation();
                EnemyTarget.myPos = actorLinker->get_position();
                EnemyTarget.ConfigID = actorLinker->ObjLinker()->ConfigID();
            }

            if (actorLinker->IsHostCamp() || !actorLinker->get_bVisible() || actorLinker->ValueComponent()->get_actorHp() < 1) continue;

            Vector3 EnemyPos = actorLinker->get_position();
            float Health = actorLinker->ValueComponent()->get_actorHp();
            float MaxHealth = actorLinker->ValueComponent()->get_actorHpTotal();
            int HealthPercent = (int)std::round((float)Health / MaxHealth * 100);
            float Distance = Vector3::Distance(EnemyTarget.myPos, EnemyPos);
            float Direction = SquaredDistance(RotateVectorByQuaternion(rotation), calculateSkillDirection(EnemyTarget.myPos, EnemyPos, actorLinker->isMoving(), actorLinker->get_logicMoveForward()));

            if (Distance < 25.f)
            {
                if (aimType == 0)
                {
                    if (HealthPercent < minHealthPercent)
                    {
                        Entity = actorLinker;
                        minHealthPercent = HealthPercent;
                    }

                    if (HealthPercent == minHealthPercent && Health < minHealth2)
                    {
                        Entity = actorLinker;
                        minHealth2 = Health;
                        minHealthPercent = HealthPercent;
                    }
                }

                if (aimType == 1 && Health < minHealth)
                {
                    Entity = actorLinker;
                    minHealth = Health;
                }

                if (aimType == 2 && Distance < minDistance)
                {
                    Entity = actorLinker;
                    minDistance = Distance;
                }

                if (aimType == 3 && Direction < minDirection && isCharging)
                {
                    Entity = actorLinker;
                    minDirection = Direction;
                }
            }
        }

        if (Entity == nullptr) {
            EnemyTarget.enemyPos = Vector3::Zero();
            EnemyTarget.moveForward = Vector3::Zero();
            EnemyTarget.ConfigID = 0; // Hoặc một giá trị mặc định khác
            EnemyTarget.isMoving = false;
        }

        if (Entity != NULL)
        {
            float nDistance = Vector3::Distance(EnemyTarget.myPos, Entity->get_position());
            if (nDistance > 25.f || Entity->ValueComponent()->get_actorHp() < 1)
            {
                EnemyTarget.enemyPos = Vector3::Zero();
                EnemyTarget.moveForward = Vector3::Zero();
                minDistance = std::numeric_limits<float>::infinity();
                minDirection = std::numeric_limits<float>::infinity();
                minHealth = std::numeric_limits<float>::infinity();
                minHealth2 = std::numeric_limits<float>::infinity();
                minHealthPercent = std::numeric_limits<float>::infinity();
                Entity = nullptr;
            }
            else
            {
                EnemyTarget.enemyPos =  Entity->get_position();
                EnemyTarget.moveForward = Entity->get_logicMoveForward();
                EnemyTarget.isMoving = Entity->isMoving();
            }
        }

        if (Entity != NULL && aimType == 3 && !isCharging)
        {
            EnemyTarget.enemyPos = Vector3::Zero();
            EnemyTarget.moveForward = Vector3::Zero();
            minDirection = std::numeric_limits<float>::infinity();
            Entity = nullptr;
        }

        if ((Entity != NULL || EnemyTarget.enemyPos != Vector3::Zero()) && get_actorManager == nullptr)
        {
            EnemyTarget.enemyPos = Vector3::Zero();
            EnemyTarget.moveForward = Vector3::Zero();
            minDistance = std::numeric_limits<float>::infinity();
            minDirection = std::numeric_limits<float>::infinity();
            minHealth = std::numeric_limits<float>::infinity();
            minHealth2 = std::numeric_limits<float>::infinity();
            minHealthPercent = std::numeric_limits<float>::infinity();
            Entity = nullptr;
        }

        if (drawType != 0 && EnemyTarget.ConfigID == 196) {
            if (EnemyTarget.myPos != Vector3::Zero() && EnemyTarget.enemyPos != Vector3::Zero()) {
                Vector3 EnemySC = Camera::get_main()->WorldToScreenPoint(EnemyTarget.enemyPos);

                // Kiểm tra xem EnemySC.z có lớn hơn 0 hay không 
                // Nếu lớn hơn 0, vị trí địch nằm trong tầm nhìn của camera
                if (EnemySC.Z > 0) {
                    // Tính toán vị trí tia vàng trên màn hình
                }
            }
        }
    }

    return _UpdateLogic(instance, delta);
}

//end


//mod skin

bool unlockskin;

enum class TdrErrorType {
    TDR_NO_ERROR = 0,
    TDR_ERR_SHORT_BUF_FOR_WRITE = -1,
    TDR_ERR_SHORT_BUF_FOR_READ = -2,
    TDR_ERR_STR_LEN_TOO_BIG = -3,
    TDR_ERR_STR_LEN_TOO_SMALL = -4,
    TDR_ERR_STR_LEN_CONFLICT = -5,
    TDR_ERR_MINUS_REFER_VALUE = -6,
    TDR_ERR_REFER_SURPASS_COUNT = -7,
    TDR_ERR_ARG_IS_NULL = -8,
    TDR_ERR_CUTVER_TOO_SMALL = -9,
    TDR_ERR_CUTVER_CONFILICT = -10,
    TDR_ERR_PARSE_TDRIP_FAILED = -11,
    TDR_ERR_INVALID_TDRIP_VALUE = -12,
    TDR_ERR_INVALID_TDRTIME_VALUE = -13,
    TDR_ERR_INVALID_TDRDATE_VALUE = -14,
    TDR_ERR_INVALID_TDRDATETIME_VALUE = -15,
    TDR_ERR_FUNC_LOCALTIME_FAILED = -16,
    TDR_ERR_INVALID_HEX_STR_LEN = -17,
    TDR_ERR_INVALID_HEX_STR_FORMAT = -18,
    TDR_ERR_INVALID_BUFFER_PARAMETER = -19,
    TDR_ERR_NET_CUTVER_INVALID = -20,
    TDR_ERR_ACCESS_VILOATION_EXCEPTION = -21,
    TDR_ERR_ARGUMENT_NULL_EXCEPTION = -22,
    TDR_ERR_USE_HAVE_NOT_INIT_VARIABLE_ARRAY = -23,
    TDR_ERR_INVALID_FORMAT = -24,
    TDR_ERR_HAVE_NOT_SET_SIZEINFO = -25,
    TDR_ERR_VAR_STRING_LENGTH_CONFILICT = -26,
    TDR_ERR_VAR_ARRAY_CONFLICT = -27,
    TDR_ERR_BAD_TLV_MAGIC = -28,
    TDR_ERR_UNMATCHED_LENGTH = -29,
    TDR_ERR_UNION_SELECTE_FIELD_IS_NULL = -30,
    TDR_ERR_SUSPICIOUS_SELECTOR = -31,
    TDR_ERR_UNKNOWN_TYPE_ID = -32,
    TDR_ERR_LOST_REQUIRED_FIELD = -33,
    TDR_ERR_NULL_ARRAY = -34
	//
};

class TdrReadBuf {
private:
    std::vector<uint8_t> beginPtr;
    int32_t position;
    int32_t length;
    bool isNetEndian;
public:
    bool isUseCache;
	//
};

namespace CSProtocol {
	class COMDT_HERO_COMMON_INFO {
    public:
        uint32_t getdwHeroID() {
			if (this == nullptr) {return 0;}
			return *(uint32_t *)((uint64_t)this + Field_AovTdr_dll_CSProtocol_COMDT_HERO_COMMON_INFO_dwHeroID);
		};
        uint16_t getwSkinID() {
			if (this == nullptr) {return 0;}
			return *(uint16_t *)((uint64_t)this + Field_AovTdr_dll_CSProtocol_COMDT_HERO_COMMON_INFO_wSkinID);
		};
		
		void setdwHeroID(uint32_t dwHeroID) {
			if (this == nullptr) {return;}
			*(uint32_t *)((uint64_t)this + Field_AovTdr_dll_CSProtocol_COMDT_HERO_COMMON_INFO_dwHeroID) = dwHeroID;
		};
        void setwSkinID(uint16_t wSkinID) {
			if (this == nullptr) {return;}
			*(uint16_t *)((uint64_t)this + Field_AovTdr_dll_CSProtocol_COMDT_HERO_COMMON_INFO_wSkinID) = wSkinID;
		};
		//
    };
	
	struct saveData {
        static uint32_t heroId;
        static uint16_t skinId;
		static bool enable;
		static std::vector<std::pair<COMDT_HERO_COMMON_INFO*, uint16_t>> arrayUnpackSkin;
		
        static void setData(uint32_t hId, uint16_t sId) {
            heroId = hId;
            skinId = sId;
        }
		
		static void setEnable(bool eb) {
            enable = eb;
        }
		
        static uint32_t getHeroId() {
            return heroId;
        }

        static uint16_t getSkinId() {
            return skinId;
        }
		
		static bool getEnable() {
            return enable;
        }
		
		static void resetArrayUnpackSkin() {
    		if (!saveData::arrayUnpackSkin.empty()) {
        		for (const auto& skinInfo : saveData::arrayUnpackSkin) {
            		COMDT_HERO_COMMON_INFO* heroInfo = skinInfo.first;
            		uint16_t skinId = skinInfo.second;
			
            		heroInfo->setwSkinID(skinId);
        		}
        		saveData::arrayUnpackSkin.clear();
    		}
		}
		//
    };
	
    uint32_t saveData::heroId = 0;
    uint16_t saveData::skinId = 0;
	bool saveData::enable = false;
	std::vector<std::pair<COMDT_HERO_COMMON_INFO*, uint16_t>> saveData::arrayUnpackSkin;
	//
}

void hook_unpack(CSProtocol::COMDT_HERO_COMMON_INFO* instance) {
	if (!CSProtocol::saveData::enable) {return;}
	if (
	instance->getdwHeroID() == CSProtocol::saveData::heroId
	&& CSProtocol::saveData::heroId != 0
	&& CSProtocol::saveData::skinId != 0
	) {
		CSProtocol::saveData::arrayUnpackSkin.emplace_back(instance, instance->getwSkinID());
		instance->setwSkinID(CSProtocol::saveData::skinId);
	}
	//
}

TdrErrorType (*old_unpack)(CSProtocol::COMDT_HERO_COMMON_INFO* instance, TdrReadBuf& srcBuf, int32_t cutVer);
TdrErrorType unpack(CSProtocol::COMDT_HERO_COMMON_INFO* instance, TdrReadBuf& srcBuf, int32_t cutVer) {

	TdrErrorType result = old_unpack(instance, srcBuf, cutVer);
		if (unlockskin) {
	hook_unpack(instance);
	}
    return result;
	//
}



void (*old_RefreshHeroPanel)(void* instance, bool bForceRefreshAddSkillPanel, bool bRefreshSymbol, bool bRefreshHeroSkill);
void (*old_OnClickSelectHeroSkin)(void *instance, uint32_t heroId, uint32_t skinId);
void OnClickSelectHeroSkin(void *instance, uint32_t heroId, uint32_t skinId) {
	if (unlockskin) {
	if (heroId != 0) {
		old_RefreshHeroPanel(instance, 1, 1, 1);
	}
	}
	old_OnClickSelectHeroSkin(instance, heroId, skinId);
}

bool (*old_IsCanUseSkin)(void *instance, uint32_t heroId, uint32_t skinId);
bool IsCanUseSkin(void *instance, uint32_t heroId, uint32_t skinId) {

	if (unlockskin) {
		if (heroId != 0) {
		CSProtocol::saveData::setData(heroId, skinId);
	}
	return 1;
	}
	return old_IsCanUseSkin(instance, heroId, skinId);

}

uint32_t (*old_GetHeroWearSkinId)(void* instance, uint32_t heroId);
uint32_t GetHeroWearSkinId(void* instance, uint32_t heroId) {

if (unlockskin) {
	CSProtocol::saveData::setEnable(true);
	return CSProtocol::saveData::skinId;
	}
	
	return old_GetHeroWearSkinId(instance, heroId);

}

bool (*old_IsHaveHeroSkin)(uint_t heroId, uint_t skinId, bool isIncludeTimeLimited);
bool IsHaveHeroSkin(uint_t heroId, uint_t skinId, bool isIncludeTimeLimited = false) {
if (unlockskin) {
	return 1;
	}
	return old_IsHaveHeroSkin(heroId, skinId, isIncludeTimeLimited);

}



/*

bool B_FireWall;

void (*old_EndGame)(void *instance, bool a1, int a2);
void EndGame(void *instance, bool a1, int a2) {
    if (instance != NULL && B_FireWall) {
        return;
    }
    old_EndGame(instance, a1, a2);
    
}



    ssize_t (*orignal_send)(int, const void *, size_t, int);
    ssize_t hook_send(int sockfd, const void *buf, size_t len, int flags)
    {
        if (B_FireWall)
        {
            return 0;
            }
        return orignal_send(sockfd, buf, len, flags);
}
*/


int heroid2,skinid2;
int (*_get_PersonalBtnId)(void *instance);
int get_PersonalBtnId(void *instance) {
    if (heroid2 != 0 && skinid2 != 0) {
        std::string buttonStr = std::to_string(heroid2) + std::to_string(skinid2);
        int buttonId = std::stoi(buttonStr);
        return buttonId;  
    }
    return _get_PersonalBtnId(instance);
}



bool (*_IsOpen)(void *instance);
 bool IsOpen(void *instance) {
    if (instance != NULL) { 
        return true;
    } 
}



//avatar
int (*_checkTeamLaderGradeMax)(void *instance);
int checkTeamLaderGradeMax(void *instance){
    if (instance != NULL && Avatar) { 
        return 0;
    }
   return _checkTeamLaderGradeMax(instance); 
}


//
void (*_ShowSkillStateInfo)(void *instance, bool bShow);
void ShowSkillStateInfo(void *instance, bool bShow) {
    if (instance != NULL && Ult) {
      bShow = true;
    }
    _ShowSkillStateInfo(instance, bShow);
}
bool ShowIcon;
void (*_ShowHeroInfo)(void *instance,int actor, bool bShow);
void ShowHeroInfo(void *instance, int actor,bool bShow) {
    if (instance != NULL && Ult) {
      bShow = true;
    } 
    _ShowHeroInfo(instance,actor, bShow);
}
bool ShowHP;
void (*_ShowHeroHpInfo)(void *instance, bool bShow);
void ShowHeroHpInfo(void *instance, bool bShow) {
    if (instance != NULL && Ult) {
      bShow = true;
    } 
    _ShowHeroHpInfo(instance, bShow);
}

//
const bool (*_get_Supported60FPSMode)(void *instance);
const bool get_Supported60FPSMode(void *instance) {
   if (instance != NULL && Fps) { 
     return true;
    } 
}

const bool (*_get_SupportedBoth60FPS_CameraHeight)(void *instance);
const bool get_SupportedBoth60FPS_CameraHeight(void *instance) {
    if (instance != NULL && Fps) {
        return true;
    } 
}

const bool (*_IsIPadDevice)(void *instance);
const bool IsIPadDevice(void *instance) {
    if (instance != NULL && Fps) {
       return true;
    }
}


//
bool (*_IsHostProfile)(void *instance);
bool IsHostProfile(void *instance) {
    if (instance != NULL && Lsd) 
    {
        return true;
    }
    return _IsHostProfile(instance);

}
//



static bool dtd = false;
int (*_CheckRoleName)(void *instance, MonoString* inputname);
int CheckRoleName(void *instance, MonoString* inputname)
{
    if (instance != NULL)
    {
        int giatri = _CheckRoleName(instance, inputname);
        if (dtd)
        {
            if (giatri == 2)
                return 0;
        }
        return giatri;
    }
    return _CheckRoleName(instance, inputname);
}

MonoString*(*_RemoveSpace)(void *instance, MonoString* inputname);
MonoString*RemoveSpace(void *instance, MonoString* inputname)
{
    if (instance != NULL)
    {
        if (dtd)
            return inputname;
    }
    return _RemoveSpace(instance, inputname);
}
	 





uintptr_t il2cpp;
void *hack_thread(void *) {
    while (!il2cpp)
    {
        il2cpp = Tools::GetBaseAddress("libil2cpp.so");
        sleep(1);
    }
	///data/data//
    IL2Cpp::Il2CppAttach();
		
	
Tools::Hook((void *) (uintptr_t)IL2Cpp::Il2CppGetMethodOffset(OBFUSCATE("Project.Plugins_d.dll"), OBFUSCATE("NucleusDrive.Logic"), OBFUSCATE("LVActorLinker") , OBFUSCATE("SetVisible"), 3), (void *) SetVisible , (void **) &_SetVisible);
//get offset hack map



OnCameraHeightChanged = (void(*)(void *))IL2Cpp::Il2CppGetMethodOffset(OBFUSCATE("Project_d.dll"), OBFUSCATE(""), OBFUSCATE("CameraSystem") , OBFUSCATE("OnCameraHeightChanged"), 0);
Tools::Hook((void *) (uintptr_t)IL2Cpp::Il2CppGetMethodOffset(OBFUSCATE("Project_d.dll"), OBFUSCATE(""), OBFUSCATE("CameraSystem") , OBFUSCATE("Update"), 0), (void *) CameraSystemUpdate, (void **) &old_CameraSystemUpdate);
Tools::Hook((void *) (uintptr_t)IL2Cpp::Il2CppGetMethodOffset(OBFUSCATE("Project_d.dll"), OBFUSCATE(""), OBFUSCATE("CameraSystem") , OBFUSCATE("GetCameraHeightRateValue"), 1), (void *) GetCameraHeightRateValue, (void **) &old_GetCameraHeightRateValue);
//get offset cam xa


Tools::Hook((void *) (uint64_t)IL2Cpp::Il2CppGetMethodOffset(OBFUSCATE("Project_d.dll"), OBFUSCATE("Kyrios.Actor"), OBFUSCATE("ActorLinker") , OBFUSCATE("Update"), 0), (void *) AUpdate, (void **) &old_Update);
AsHero = (uintptr_t(*)(...)) IL2Cpp::Il2CppGetMethodOffset(OBFUSCATE("Project_d.dll"), OBFUSCATE("Kyrios.Actor"), OBFUSCATE("ActorLinker") , OBFUSCATE("AsHero"), 0);
_SetPlayerName = (void (*)(...)) ((uint64_t)IL2Cpp::Il2CppGetMethodOffset("Project_d.dll","Assets.Scripts.GameLogic","HudComponent3D","SetPlayerName",4));
_SetPlayerName = (void (*)(...))IL2Cpp::Il2CppGetMethodOffset(OBFUSCATE("Project_d.dll"), OBFUSCATE("Assets.Scripts.GameLogic"), OBFUSCATE("HudComponent3D") , OBFUSCATE("SetPlayerName"), 4);
//Tools::Hook((void *) (uintptr_t)IL2Cpp::Il2CppGetMethodOffset(OBFUSCATE("Project_d.dll"), OBFUSCATE("Assets.Scripts.GameSystem"), OBFUSCATE("SettlementHelper") , OBFUSCATE("SetPlayerName"), 2), (void *) SettlementHelper_SetPlayerName, (void **) &_SettlementHelper_SetPlayerName);

// hiện hồi chiêu*/


Tools::Hook((void *) (uintptr_t)IL2Cpp::Il2CppGetMethodOffset(OBFUSCATE("Project_d.dll"), OBFUSCATE("Assets.Scripts.Framework"), OBFUSCATE("GameSettings") , OBFUSCATE("get_Supported60FPSMode"), 0), (void *) get_Supported60FPSMode , (void **) &_get_Supported60FPSMode);
Tools::Hook((void *) (uintptr_t)IL2Cpp::Il2CppGetMethodOffset(OBFUSCATE("Project_d.dll"), OBFUSCATE("Assets.Scripts.Framework"), OBFUSCATE("GameSettings") , OBFUSCATE("get_Supported90FPSMode"), 0), (void *) get_Supported60FPSMode , (void **) &_get_Supported60FPSMode);
Tools::Hook((void *) (uintptr_t)IL2Cpp::Il2CppGetMethodOffset(OBFUSCATE("Project_d.dll"), OBFUSCATE("Assets.Scripts.Framework"), OBFUSCATE("GameSettings") , OBFUSCATE("get_Supported120FPSMode"), 0), (void *) get_Supported60FPSMode , (void **) &_get_Supported60FPSMode);
Tools::Hook((void *) (uintptr_t)IL2Cpp::Il2CppGetMethodOffset(OBFUSCATE("Project_d.dll"), OBFUSCATE("Assets.Scripts.Framework"), OBFUSCATE("GameSettings") , OBFUSCATE("get_SupportedBoth60FPS_CameraHeight"), 0), (void *) get_SupportedBoth60FPS_CameraHeight , (void **) &_get_SupportedBoth60FPS_CameraHeight);
Tools::Hook((void *) (uintptr_t)IL2Cpp::Il2CppGetMethodOffset(OBFUSCATE("Project_d.dll"), OBFUSCATE("Assets.Scripts.Framework"), OBFUSCATE("GameSettings") , OBFUSCATE("IsIPadDevice"), 0), (void *) IsIPadDevice , (void **) &_IsIPadDevice);
	

//fps
  
  
  Tools::Hook((void *) (uintptr_t)IL2Cpp::Il2CppGetMethodOffset(OBFUSCATE("Project_d.dll"), OBFUSCATE("Assets.Scripts.GameSystem"), OBFUSCATE("CPlayerProfile") , OBFUSCATE("get_IsHostProfile"), 0), (void *) IsHostProfile, (void **) &_IsHostProfile);
//lsd
  
  
  //history
  
Tools::Hook((void *) (uintptr_t)IL2Cpp::Il2CppGetMethodOffset(OBFUSCATE("Project_d.dll"), OBFUSCATE("Assets.Scripts.GameSystem"), OBFUSCATE("HeroInfoPanel") , OBFUSCATE("ShowHeroInfo"), 2), (void *) ShowHeroInfo, (void **) &_ShowHeroInfo);
Tools::Hook((void *) (uintptr_t)IL2Cpp::Il2CppGetMethodOffset(OBFUSCATE("Project_d.dll"), OBFUSCATE(""), OBFUSCATE("MiniMapHeroInfo") , OBFUSCATE("ShowSkillStateInfo"), 1), (void *) ShowSkillStateInfo, (void **) &_ShowSkillStateInfo);
Tools::Hook((void *) (uintptr_t)IL2Cpp::Il2CppGetMethodOffset(OBFUSCATE("Project_d.dll"), OBFUSCATE(""), OBFUSCATE("MiniMapHeroInfo") , OBFUSCATE("ShowHeroHpInfo"), 1), (void *) ShowHeroHpInfo, (void **) &_ShowHeroHpInfo);

//show ulti


Tools::Hook((void *) (uintptr_t)IL2Cpp::Il2CppGetMethodOffset(OBFUSCATE("Project_d.dll"), OBFUSCATE("Assets.Scripts.GameLogic"), OBFUSCATE("SkillControlIndicator") , OBFUSCATE("GetUseSkillDirection"), 1), (void *) GetUseSkillDirection, (void **) &_GetUseSkillDirection);

Tools::Hook((void *) (uintptr_t)IL2Cpp::Il2CppGetMethodOffset(OBFUSCATE("Project_d.dll"), OBFUSCATE("Assets.Scripts.GameSystem"), OBFUSCATE("CSkillButtonManager") , OBFUSCATE("UpdateLogic"), 1), (void *) UpdateLogic, (void **) &_UpdateLogic);
	
 //mod

    old_RefreshHeroPanel = (void (*)(void*, bool, bool, bool)) (Method_Project_d_dll_Assets_Scripts_GameSystem_HeroSelectNormalWindow_RefreshHeroPanel_3);//HGMODXHG
    TOOL_HOOK(Method_AovTdr_dll_CSProtocol_COMDT_HERO_COMMON_INFO_unpack_2, unpack, old_unpack);//HGMODXHG
	TOOL_HOOK(Method_Project_d_dll_Assets_Scripts_GameSystem_HeroSelectNormalWindow_OnClickSelectHeroSkin_2, OnClickSelectHeroSkin, old_OnClickSelectHeroSkin);//HGMODXHG
	TOOL_HOOK(Method_Project_d_dll_Assets_Scripts_GameSystem_CRoleInfo_IsCanUseSkin_2, IsCanUseSkin, old_IsCanUseSkin);//HGMODXHG
    TOOL_HOOK(Method_Project_d_dll_Assets_Scripts_GameSystem_CRoleInfo_GetHeroWearSkinId_1, GetHeroWearSkinId, old_GetHeroWearSkinId);//HGMODXHG
	TOOL_HOOK(Method_Project_d_dll_Assets_Scripts_GameSystem_CRoleInfo_IsHaveHeroSkin_3, IsHaveHeroSkin, old_IsHaveHeroSkin);//HGMODXHG
	
	
	TOOL_HOOK(Method_Project_d_dll_Assets_Scripts_GameSystem_PersonalButton_get_PersonalBtnId_0, get_PersonalBtnId, _get_PersonalBtnId);
    TOOL_HOOK(Method_Project_d_dll_Assets_Scripts_GameSystem_PersonalButton_IsOpen_0, IsOpen, _IsOpen);



Tools::Hook((void *) (uintptr_t)IL2Cpp::Il2CppGetMethodOffset(OBFUSCATE("Project_d.dll"), OBFUSCATE("Assets.Scripts.GameSystem"), OBFUSCATE("CMatchingSystem") , OBFUSCATE("checkTeamLaderGradeMax"), 1), (void *) checkTeamLaderGradeMax, (void **) &_checkTeamLaderGradeMax);

old_InBattleMsgNetCore_SendInBattleMsg_InputChat = (void (*)(String*, int)) (Adr_InBattleMsgNetCore_SendInBattleMsg_InputChat);
//auto on

Tools::Hook((void *) (uintptr_t)IL2Cpp::Il2CppGetMethodOffset(OBFUSCATE("Project_d.dll"), OBFUSCATE(""), OBFUSCATE("Utility") , OBFUSCATE("CheckRoleName"), 1), (void *) CheckRoleName, (void **) &_CheckRoleName);
Tools::Hook((void *) (uintptr_t)IL2Cpp::Il2CppGetMethodOffset(OBFUSCATE("Project_d.dll"), OBFUSCATE("Assets.Scripts.UI"), OBFUSCATE("CUIUtility") , OBFUSCATE("RemoveSpace"), 1), (void *) RemoveSpace, (void **) &_RemoveSpace);


HackMap = true;
Cd = true;
Ult = true;
unlockskin = true;
Fps = true; 
//UID = true;



return NULL;
}


jobjectArray GetFeatureList(JNIEnv *env, jobject context) {
    jobjectArray ret;

    const char *features[] = {
		
			
            OBFUSCATE("Category_▼ʜᴀᴄᴋ ɢᴀᴍᴇ #1▼"), 
            
            OBFUSCATE("Collapse_Chức Năng Chính"),
            
            OBFUSCATE("1_CollapseAdd_Toggle_➢ 𝙃𝘼𝘾𝙆 𝙈𝘼𝙋"),
            
            OBFUSCATE("3_CollapseAdd_Toggle_➢ 𝙎𝙃𝙊𝙒 𝙁𝙐𝙇𝙇 𝙃𝙊𝙄 𝘾𝙃𝙄𝙀𝙐"),
            
            OBFUSCATE("4_CollapseAdd_Toggle_➢ 𝙎𝙃𝙊𝙒 𝙐𝙇𝙏𝙄"),
	
			OBFUSCATE("Category_▼ᴄᴀᴍ xᴀ ᴠɪᴇᴡ▼"),
            
            OBFUSCATE("2_SeekBar_➢ 𝘾𝘼𝙈 𝙓𝘼 𝙑𝙄𝙀𝙒_0_20"),
            
            OBFUSCATE("Category_▼ʜᴀᴄᴋ ɢᴀᴍᴇ #2▼"),
            
            OBFUSCATE("Collapse_Chức Năng Bổ Sung"),
            
            OBFUSCATE("21_CollapseAdd_Toggle_➢ 𝙐𝙉𝙇𝙊𝘾𝙆 𝙁𝙐𝙇𝙇 𝙎𝙆𝙄𝙉 "),		
			
            OBFUSCATE("8_CollapseAdd_Toggle_➢ 𝙐𝙉𝙇𝙊𝘾𝙆 𝙈𝘼𝙓 𝙁𝙋𝙎"),
            
            OBFUSCATE("5_CollapseAdd_Toggle_➢ 𝙎𝙃𝙊𝙒 𝙃𝙄𝙎𝙏𝙊𝙍𝙔"),
			
		    OBFUSCATE("7_CollapseAdd_Toggle_➢ 𝙎𝙃𝙊𝙒 𝙉𝘼𝙈𝙀 𝘽𝘼𝙉 𝙋𝙄𝘾𝙆"),
		    
		    OBFUSCATE("6_CollapseAdd_Toggle_➢ 𝙎𝙃𝙊𝙒 𝘼𝙑𝘼𝙏𝘼𝙍"),
			
			OBFUSCATE("2190_CollapseAdd_Toggle_➢ 𝙍𝘼 𝙎𝘼𝙉𝙃 𝙉𝙃𝘼𝙉𝙃"),	
			
			OBFUSCATE("666_CollapseAdd_Toggle_➢ 𝙍𝙚𝙉𝙖𝙢𝙚 𝘿𝙖̀𝙞"),
			
			OBFUSCATE("CollapseAdd_ButtonLink_𝙑𝙞𝙙𝙚𝙤 𝙃𝘿𝙎𝘿 𝙍𝙚𝙉𝙖𝙢𝙚_https://t.me/HGMOD_CS/3371"),
			
			OBFUSCATE("Category_▼Mod Skin Nút Bấm▼"),
			
			OBFUSCATE("Collapse_Mod Button Skin"),
			
			OBFUSCATE("CollapseAdd_Category_▼ Lưu Ý: Chỉ Mở 1 Chức Năng, Mở Chức Năng Khác Phải Tắt Chức Năng Cũ!▼"),
			
			OBFUSCATE("200_CollapseAdd_Toggle_➢ 𝙇𝙖𝙪𝙧𝙞𝙚𝙡 𝙏𝙝𝙪̛́ 𝙉𝙜𝙪𝙮𝙚̂𝙣"),
			
			OBFUSCATE("201_CollapseAdd_Toggle_➢ 𝙀𝙩𝙚𝙧𝙣𝙖𝙡 𝙎𝙖𝙞𝙡𝙤𝙧 𝙈𝙤𝙤𝙣"),
			
			OBFUSCATE("202_CollapseAdd_Toggle_➢ 𝙑𝙞𝙤 𝙏𝙝𝙖̂̀𝙣 𝙇𝙤𝙣𝙜 𝙏𝙮̉ 𝙏𝙮̉"),
			
			OBFUSCATE("203_CollapseAdd_Toggle_➢ 𝙀𝙩𝙚𝙧𝙣𝙖𝙡 𝙎𝙖𝙞𝙡𝙤𝙧 𝘾𝙝𝙞𝙗𝙞 𝙈𝙤𝙤𝙣"),
			
			OBFUSCATE("204_CollapseAdd_Toggle_➢ 𝘽𝙪𝙩𝙩𝙚𝙧𝙛𝙡𝙮 𝙆𝙞𝙢 𝙉𝙜𝙪̛ 𝙏𝙝𝙖̂̀𝙣 𝙉𝙪̛̃"),
			
			OBFUSCATE("205_CollapseAdd_Toggle_➢ 𝘽𝙪𝙩𝙩𝙚𝙧𝙛𝙡𝙮 𝙏𝙝𝙖́𝙣𝙝 𝙉𝙪̛̃"),
			
			OBFUSCATE("206_CollapseAdd_Toggle_➢ 𝙍𝙖𝙯 𝙂𝙤𝙣"),
			
			OBFUSCATE("207_CollapseAdd_Toggle_➢ 𝙉𝙖𝙠𝙧𝙤𝙩𝙝 𝙆𝙞𝙡𝙡𝙪𝙖"),
			
			OBFUSCATE("214_CollapseAdd_Toggle_➢ 𝙉𝙖𝙠 𝘽𝙖̣𝙘𝙝 𝘿𝙞𝙚̣̂𝙣"), 
			
			OBFUSCATE("215_CollapseAdd_Toggle_➢ 𝙉𝙖𝙠 𝙌𝙪𝙮̉ 𝙏𝙝𝙪̛𝙤̛𝙣𝙜"), 
			
			OBFUSCATE("208_CollapseAdd_Toggle_➢ 𝘼𝙞𝙧𝙞 𝙏𝙝𝙪̛́ 𝙉𝙜𝙪𝙮𝙚̂𝙣"),
			
			OBFUSCATE("209_CollapseAdd_Toggle_➢ 𝙔𝙚𝙣𝙖 𝙃𝙪𝙮𝙚̂̀𝙣 𝘾𝙪̛̉𝙪 𝙏𝙝𝙞𝙚̂𝙣"),
			
			OBFUSCATE("216_CollapseAdd_Toggle_➢ 𝙔𝙚𝙣𝙖 𝙏𝙧𝙖̂́𝙣 𝙔𝙚̂𝙪 𝙏𝙝𝙖̂̀𝙣 𝙇𝙤̣̂𝙘"),
			
			OBFUSCATE("210_CollapseAdd_Toggle_➢ 𝙑𝙚𝙧𝙚𝙨 𝙇𝙪̛𝙪 𝙇𝙮 𝙇𝙤𝙣𝙜 𝙈𝙖̂̃𝙪"),
			
			OBFUSCATE("211_CollapseAdd_Toggle_➢ 𝙆𝙧𝙞𝙭𝙞 𝙋𝙝𝙪̀ 𝙏𝙝𝙪̉𝙮 𝙏𝙝𝙤̛̀𝙞 𝙆𝙝𝙤̂𝙣𝙜"),
			
			OBFUSCATE("212_CollapseAdd_Toggle_➢ 𝙇𝙞𝙡𝙞𝙖𝙣𝙖 𝙃𝙤̣𝙘 𝙑𝙞𝙚̣̂𝙣 𝙈𝙖 𝙋𝙝𝙖́𝙥"),
            
            OBFUSCATE("213_CollapseAdd_Toggle_➢ 𝙏𝙪𝙡𝙚𝙣 𝙎𝙖𝙩𝙤𝙧𝙪 𝙂𝙤𝙟𝙤"),
            
            OBFUSCATE("217_CollapseAdd_Toggle_➢ 𝘼𝙮𝙖 𝘾𝙤̂𝙣𝙜 𝙏𝙪́𝙖 𝘾𝙖̂̀𝙪 𝙕𝙤̂̀𝙣𝙜"),
			
			OBFUSCATE("218_CollapseAdd_Toggle_➢ 𝘽𝙖𝙧𝙤𝙣 𝙏𝙝𝙞𝙚̂𝙣 𝙋𝙝𝙪̉"),
			
			OBFUSCATE("219_CollapseAdd_Toggle_➢ 𝘽𝙞𝙡𝙡𝙤𝙬 𝙏𝙝𝙞𝙚̂𝙣 𝙏𝙪̛𝙤̛́𝙣𝙜"),
			
			OBFUSCATE("220_CollapseAdd_Toggle_➢ 𝙑𝙞𝙤𝙡𝙚𝙩 𝙑𝙚̣̂ 𝙏𝙝𝙖̂̀𝙣"),
			
			OBFUSCATE("221_CollapseAdd_Toggle_➢ 𝘽𝙞𝙧𝙤𝙣 𝙏𝙝𝙖̂̀𝙮 𝘾𝙪́𝙣𝙜"),
			
			OBFUSCATE("222_CollapseAdd_Toggle_➢ 𝘾𝙖𝙥𝙝𝙚𝙣𝙮 𝘾𝙖̀𝙣 𝙉𝙜𝙪𝙮𝙚̂𝙣"),
			
			OBFUSCATE("223_CollapseAdd_Toggle_➢ 𝙀𝙡𝙖𝙣𝙙'𝙤𝙧𝙧 𝙏𝙪𝙭𝙚𝙙𝙤 𝙈𝙖𝙨𝙠"),
            
			OBFUSCATE("224_CollapseAdd_Toggle_➢ 𝙀𝙣𝙯𝙤 𝙆𝙪𝙧𝙖𝙥𝙞𝙠𝙖"),
			
			OBFUSCATE("225_CollapseAdd_Toggle_➢ 𝙄𝙡𝙪𝙢𝙞𝙖 𝙇𝙪̛𝙤̛̃𝙣𝙜 𝙉𝙜𝙝𝙞"),
            
			OBFUSCATE("226_CollapseAdd_Toggle_➢ 𝙈𝙪𝙧𝙖𝙙 𝙏𝙝𝙖̂̀𝙣 𝘽𝙞𝙣𝙝"),
			
			OBFUSCATE("227_CollapseAdd_Toggle_➢ 𝙑𝙚𝙧𝙧𝙖 𝙏𝙝𝙖̂̀𝙣 𝙎𝙖́𝙩"),
            
            OBFUSCATE("Category_▼ᴀɪᴍʙᴏᴛ ᴇʟsᴜ▼"), 
            
            OBFUSCATE("Collapse_Các Loại Aim"),
            
            OBFUSCATE("9_CollapseAdd_Toggle_➢ 𝘼𝘾𝙏𝙄𝙑𝙀 𝘼𝙄𝙈 𝙀𝙇𝙎𝙐"),
            
            OBFUSCATE("13_CollapseAdd_Toggle_➢ 𝘼𝘾𝙏𝙄𝙑𝙀 𝘼𝙄𝙈 𝙔𝙐𝙀"),
            
            OBFUSCATE("14_CollapseAdd_Toggle_➢ 𝘼𝘾𝙏𝙄𝙑𝙀 𝘼𝙄𝙈 𝙂𝙄𝙇𝘿𝙐𝙍"),
            
            OBFUSCATE("15_CollapseAdd_Toggle_➢ 𝘼𝘾𝙏𝙄𝙑𝙀 𝘼𝙄𝙈 𝙀𝙉𝙕𝙊"),
            
            OBFUSCATE("16_CollapseAdd_Toggle_➢ 𝘼𝘾𝙏𝙄𝙑𝙀 𝘼𝙄𝙈 𝙃𝙊𝘼 𝙁𝙇𝙊"),
            
            OBFUSCATE("17_CollapseAdd_Toggle_➢ 𝘼𝘾𝙏𝙄𝙑𝙀 𝘼𝙄𝙈 𝙂𝙍𝘼𝙆𝙆"),
			
            OBFUSCATE("10_CollapseAdd_RadioButton_➢ MỤC TIÊU AIM_⮕% 𝙈𝙖́𝙪 𝙏𝙝𝙖̂́𝙥 𝙉𝙝𝙖̂́𝙩,⮕𝙈𝙖́𝙪 𝙏𝙝𝙖̂́𝙥 𝙉𝙝𝙖̂́𝙩,⮕𝙆𝙝𝙤𝙖̉𝙣𝙜 𝘾𝙖́𝙘𝙝 𝙂𝙖̂̀𝙣 𝙉𝙝𝙖̂́𝙩,⮕𝙂𝙖̂̀𝙣 𝙏𝙞𝙖 𝙉𝙝𝙖̂́𝙩"),  
            
            OBFUSCATE("Category_▼sᴘᴀᴍ ᴄʜᴀᴛ▼"),
	
		OBFUSCATE("Collapse_Spam Chat"),
		
		OBFUSCATE("CollapseAdd_Category_❗Lưu Ý: Máy Yếu Không Nên Dùng, Đây Là Chức Năng 1 Người Dùng 10 Người Lag"),
		
		OBFUSCATE("82828_CollapseAdd_Toggle_➢ 𝘼𝘾𝙏𝙄𝙑𝙀 𝙎𝙋𝘼𝙈"),
		
        OBFUSCATE("82829_CollapseAdd_InputText_Nhập Tin Nhắn Muốn Spam (Cân Cả Chửi Tục)"),
        
			
	
			
            };

    //Now you dont have to manually update the number everytime;
    int Total_Feature = (sizeof features / sizeof features[0]);
    ret = (jobjectArray)
            env->NewObjectArray(Total_Feature, env->FindClass(OBFUSCATE("java/lang/String")),
                                env->NewStringUTF(""));

    for (int i = 0; i < Total_Feature; i++)
        env->SetObjectArrayElement(ret, i, env->NewStringUTF(features[i]));

    return (ret);
}

void Changes(JNIEnv *env, jclass clazz, jobject obj,
                                        jint featNum, jstring featName, jint value,
                                        jboolean boolean, jstring str) {

    LOGD(OBFUSCATE("Feature name: %d - %s | Value: = %d | Bool: = %d | Text: = %s"), featNum,
         env->GetStringUTFChars(featName, 0), value,
         boolean, str != NULL ? env->GetStringUTFChars(str, 0) : "");

    //BE CAREFUL NOT TO ACCIDENTLY REMOVE break;

    switch (featNum) {
		
		/*case 4953:
			isBlocklen = false;
			break;
		*/
           case 1:
            HackMap = boolean;
            break;
        case 2:
            WideView.SetFieldOfView = (float) value * 0.1362f;
            WideView.Active = true;
            break;  
        case 3:
           Cd = boolean;
            break;  
            
        case 9:
            AimSkill = boolean;
            break;
        case 13:
            AimYue = boolean;
            break;
        case 14:
            AimGildur = boolean;
            break;
        case 15:
            AimEnzo = boolean;
           break;
           case 16:
            AimFlo = boolean;
           break;
           case 17:
            AimGrakk = boolean;
           break;
        case 10:
             switch (value) {
		     case 1:
		     aimType = 0;
			 break;
			 case 2:
		    aimType = 1;
			 break;
	        case 3:
		     aimType = 2;
			 break;
			 case 4:
		    aimType = 3;
			 break; 
				 
				 
			 }
             break;
        
			
			
			case 21:
			unlockskin = boolean;
			
		
			
			break;
			
			case 8:
            Fps = boolean;
            break;  
            
            
            
            case 5:
            Lsd = boolean;
            break;  
            
            
            case 6:
            
           Avatar = boolean;
           
           break;
            
            
            case 4:
            Ult = boolean;
            break;  
			
			
					
			
			
			//button skin
			
			case 200:
				
			
			heroid2 = 141;           skinid2 = 11;
		
			
			break;
			
			
			case 201:
				
			
			heroid2 = 152;           skinid2 = 12;
		
			
			break;
			
			
			case 202:
				
			
			heroid2 = 111;           skinid2 = 15;
		
			
			break;
			
			
			
			case 203:
			heroid2 = 118;           skinid2 = 12;
			break;
			
			case 204:
			heroid2 = 116;           skinid2 = 14;
			break;
			
			case 205:
			heroid2 = 116;           skinid2 = 16;
			break;
			
			case 206:
			heroid2 = 157;           skinid2 = 11;
			break;
			
			case 207:
			heroid2 = 150;           skinid2 = 12;
			break;
			
			case 208:
			heroid2 = 130;           skinid2 = 15;
			break;
			
			
			case 209:
			heroid2 = 154;           skinid2 = 12;
			break;
			
			case 210:
			heroid2 = 520;           skinid2 = 11;
			break;
			
			case 211:
			heroid2 = 106;           skinid2 = 20;
			break;
			
			case 212:
			heroid2 = 510;           skinid2 = 15;
			break;
			
			
			case 213:
			heroid2 = 190;           skinid2 = 15;
			break;
			
			
			case 214:
			heroid2 = 150;           skinid2 = 15;
			break;
			
			case 215:
			heroid2 = 150;           skinid2 = 13;
			break;
			
			case 216:
			heroid2 = 154;           skinid2 = 13;
			break;
			
			case 217:
			heroid2 = 54;           skinid2 = 307;
			break;
			
			case 218:
			heroid2 = 59;           skinid2 = 802;
			break;
			
			case 219:
			heroid2 = 59;           skinid2 = 901;
			break;
			
			case 220:
			heroid2 = 11;           skinid2 = 107;
			break;
			
			case 221:
			heroid2 = 59;           skinid2 = 702;
			break;
			
			case 222:
			heroid2 = 524;           skinid2 = 14;
			break;
			
			case 223:
			heroid2 = 19;           skinid2 = 906;
			break;
			
			case 224:
			heroid2 = 19;           skinid2 = 508;
			break;
			
			case 225:
			heroid2 = 136;           skinid2 = 13;
			break;
			
			case 226:
			heroid2 = 131;           skinid2 = 16;
			break;
			
			case 227:
			heroid2 = 109;           skinid2 = 15;
			break;
			
		case 82828:
        SpamChat = boolean;       
        
    break;
    
    case 82829:
   getToast = env->GetStringUTFChars(str, 0);
 
    break;       
	
	
case 666 :
dtd = boolean;
break;
	

	
             
             
			

    

    
}

        
    }


__attribute__((constructor))
void lib_main() {


    pthread_t ptid;
   // pthread_create(&ptid, NULL, anogs_thread, NULL);
    pthread_create(&ptid, NULL, hack_thread, NULL);

}
	
	



int RegisterMenu(JNIEnv *env) {
    JNINativeMethod methods[] = {
            {OBFUSCATE("Icon"), OBFUSCATE("()Ljava/lang/String;"), reinterpret_cast<void *>(Icon)},
            {OBFUSCATE("IconWebViewData"),  OBFUSCATE("()Ljava/lang/String;"), reinterpret_cast<void *>(IconWebViewData)},
            {OBFUSCATE("IsGameLibLoaded"),  OBFUSCATE("()Z"), reinterpret_cast<void *>(isGameLibLoaded)},
            {OBFUSCATE("Init"),  OBFUSCATE("(Landroid/content/Context;Landroid/widget/TextView;Landroid/widget/TextView;)V"), reinterpret_cast<void *>(Init)},
            {OBFUSCATE("SettingsList"),  OBFUSCATE("()[Ljava/lang/String;"), reinterpret_cast<void *>(SettingsList)},
            {OBFUSCATE("GetFeatureList"),  OBFUSCATE("()[Ljava/lang/String;"), reinterpret_cast<void *>(GetFeatureList)},
    };

    jclass clazz = env->FindClass(OBFUSCATE("com/android/support/Menu"));
    if (!clazz)
        return JNI_ERR;
    if (env->RegisterNatives(clazz, methods, sizeof(methods) / sizeof(methods[0])) != 0)
        return JNI_ERR;
    return JNI_OK;
}

int RegisterPreferences(JNIEnv *env) {
    JNINativeMethod methods[] = {
            {OBFUSCATE("Changes"), OBFUSCATE("(Landroid/content/Context;ILjava/lang/String;IZLjava/lang/String;)V"), reinterpret_cast<void *>(Changes)},
    };
    jclass clazz = env->FindClass(OBFUSCATE("com/android/support/Preferences"));
    if (!clazz)
        return JNI_ERR;
    if (env->RegisterNatives(clazz, methods, sizeof(methods) / sizeof(methods[0])) != 0)
        return JNI_ERR;
    return JNI_OK;
}

int RegisterMain(JNIEnv *env) {
    JNINativeMethod methods[] = {
            {OBFUSCATE("CheckOverlayPermission"), OBFUSCATE("(Landroid/content/Context;)V"), reinterpret_cast<void *>(CheckOverlayPermission)},
    };
    jclass clazz = env->FindClass(OBFUSCATE("com/android/support/Main"));
    if (!clazz)
        return JNI_ERR;
    if (env->RegisterNatives(clazz, methods, sizeof(methods) / sizeof(methods[0])) != 0)
        return JNI_ERR;

    return JNI_OK;
}

extern "C"
JNIEXPORT jint JNICALL
JNI_OnLoad(JavaVM *vm, void *reserved) {
    JNIEnv *env;
    vm->GetEnv((void **) &env, JNI_VERSION_1_6);
    if (RegisterMenu(env) != 0)
        return JNI_ERR;
    if (RegisterPreferences(env) != 0)
        return JNI_ERR;
    if (RegisterMain(env) != 0)
        return JNI_ERR;
    return JNI_VERSION_1_6;
}
