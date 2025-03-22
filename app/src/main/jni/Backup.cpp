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
    MemoryPatch srank, antiskin, ulti1, ulti2, ulti3, lsd, fps1, fps2, fps3, fps4, fps5, giamhoichieu, end, end1;

} hexPatches;
bool antiskin;
bool fps;
bool ulti;
bool out;
bool srank;
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

String *CreateMonoString(const char *str)
{
    
    return Il2CppString::Create(str);
}


uintptr_t (*AsHero)(...);
void (*_SetPlayerName)(...);
void (*old_Update)(uintptr_t instance);
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

bool isBlock = false;


ssize_t (*old_send)(int , const void *, size_t , int);
ssize_t send(int sockfd, const void *buf, size_t len, int flags) {
    if (isBlock) {
        return 0;
    }
    return old_send(sockfd, buf, len, flags);
}
void SetHook() {
    if (old_send == NULL) Tools::Hook(DobbySymbolResolver("libc.so", "send"), (void *) send, (void **) &old_send);
}

void (*old_PrepareBattle)(...);
void PrepareBattle(void* instance) {
    isBlock = true;
   // LOGD("BLOCK ON");
    SetHook();
    old_PrepareBattle(instance);
}
void (*old_CountDownHandler)(...);
void CountDownHandler(void* instance, void* parent, void* ts, bool isPlaySound) {
    if (ts != NULL) {
        double totalTime = *(double*)((uintptr_t)ts + 0x38);
        if (totalTime < 15) {
            isBlock = true;
            SetHook();
           // LOGD("BLOCK ON");
        }
    }
    old_CountDownHandler(instance, parent, ts, isPlaySound);
}
void (*old_PVPLoadingViewOnEnter)(void* thiz);
void PVPLoadingViewOnEnter(void* thiz) {
    isBlock = true;
   // LOGD("BLOCK ON");
    SetHook();
    old_PVPLoadingViewOnEnter(thiz);
}


void (*orig_NucleusDrive_Logic_LFrameworkEndGame)(void* _this, bool bSyncUnload, int32_t waitingFinishState);
void hook_NucleusDrive_Logic_LFrameworkEndGame(void* _this, bool bSyncUnload, int32_t waitingFinishState)
{
   // LOGD("END GAME");
   
    std::thread([](){
        std::this_thread::sleep_for(std::chrono::seconds(1));
       isBlock = false;
        //LOGD("BLOCK OFF");
    }).detach();
    return;
}

///

//AUTO USE SKILL

int ActorLinker_MyObjID = 9999, 
m_PhanTram_HP = 40, 
SupportEquipmentIndex,  Hercules_MadnessIndex,
ActorLinker_objCamp, AutoWinLose,
m_SupportEquipmentIndex, mvalue_SupportEquipmentIndex;

std::vector<void*> MyLActorRoot(2, NULL), 
MySkillSlot(14, NULL);

void* MyCBattleEquipSystem, * ActorLinker_MyHudControl;
bool AutoUseSupportEquipment, AutoPunish, AutoExecute, Equipped1138, AutoSellBladeofEternity, AutoSellDeathSickle, AutoSellSupportEquipment;
Vector3 MyPosition;

int (*old_ValuePropertyComponent_get_actorHp)(void*);
int (*old_ValuePropertyComponent_get_actorHpTotal)(void*);
int (*old_ActorLinker_get_ObjID)(void*);
bool (*old_ActorLinker_IsHostPlayer)(void*);
int (*old_LActorRoot_get_ObjID)(void*);
bool (*old_SkillSlot_SendRequestUseSkill)(void*);
bool (*old_SkillSlot_get_IsCDReady)(void*);
VInt3 (*old_LActorRoot_get_location)(void*);
VInt3 (*old_LActorRoot_get_forward)(void*);
void* (*old_LActorRoot_AsHero)(void*);
void* (*old_LActorRoot_AsOrgan)(void*);
void* (*old_LActorRoot_AsMonster)(void*);
void* (*old_CBattleSystem_get_TheBattleEquipSystem)(void*);
void (*old_CBattleEquipSystem_SendSellEquipFrameCommand)(void*, int);
int (*old_ActorLinker_get_objCamp)(void*);
int (*old_LActorRoot_GiveMyEnemyCamp)(void*);
int (*old_ActorLinker_get_objType)(void*);
void (*old_ValuePropertyComponent_set_actorHp)(void*, int);
bool (*old_LObjWrapper_get_IsDeadState)(void*);
struct stEquipInfo
{
    uint16_t m_equipID;
    uint32_t m_amount;
    uint32_t m_maxAmount;
};


// Hàm chuyển đổi VInt3 thành Vector3
Vector3 VInt3ToVector3(VInt3 location, VInt3 forward) {
    // Function to calculate the power of 10 of a number
    auto calculatePowerOfTen = [](int num) {
        int div = 1;
        while (num >= 10) {
            num /= 10;
            div *= 10;
        }
        return div;
    };

    // Calculate Vector3 components
    float factorX = (float)(location.X * calculatePowerOfTen(forward.X) + forward.X) / (1000 * calculatePowerOfTen(forward.X));
    float factorY = (float)(location.Y * calculatePowerOfTen(forward.Y) + forward.Y) / (1000 * calculatePowerOfTen(forward.Y));
    float factorZ = (float)(location.Z * calculatePowerOfTen(forward.Z) + forward.Z) / (1000 * calculatePowerOfTen(forward.Z));
    
    // Return Vector3 object
    return Vector3(factorX, factorY, factorZ);
}


void (*old_ActorLinker_UpdateCn)(void*);
void ActorLinker_UpdateCn(void* instance) {
	old_ActorLinker_UpdateCn(instance);
	if (old_ActorLinker_IsHostPlayer(instance)) {
		ActorLinker_MyObjID = old_ActorLinker_get_ObjID(instance);
		ActorLinker_MyHudControl = *(void**)((uint64_t)instance + Adr_ActorLinker_HudControl);
		ActorLinker_objCamp = old_ActorLinker_get_objCamp(instance);
	}
	old_ActorLinker_UpdateCn(instance);
	
}

bool macro;
void (*old_SendUseCommonAttack)(...);
void SendUseCommonAttack(void* instance, void* Start, uint uiRealObjID = 0) { 

    if (macro) {
        Vector3 Position = VInt3ToVector3(old_LActorRoot_get_location(instance), old_LActorRoot_get_forward(instance));
        float DistanceActor = Vector3::Distance(MyPosition, Position);
        if (DistanceActor >= 1.5f) {
            old_SkillSlot_SendRequestUseSkill(MySkillSlot[2]);
        }
    }

    return old_SendUseCommonAttack(instance, Start, uiRealObjID);
}



void (*old_SkillSlot_LateUpdate)(void*, int);
void SkillSlot_LateUpdate(void* instance, int nDelta) {
	old_SkillSlot_LateUpdate(instance, nDelta);
	if (instance != NULL) {
		if (*(int*)((uint64_t)instance + Field_Project_d_dll_Assets_Scripts_GameLogic_SkillSlot_m_skillCDPreperformanceMaxTimeFrameSync) != -1 && *(int*)((uint64_t)instance + Field_Project_d_dll_Assets_Scripts_GameLogic_SkillSlot_m_skillCDPreperformanceMaxTimeFrameSync) != -1) {
			if (*(void**)((uint64_t)instance + Field_Project_d_dll_Assets_Scripts_GameLogic_SkillSlot_SkillObj) != NULL) {
				int SkillSlot = *(int*)((uint64_t)instance + Field_Project_d_dll_Assets_Scripts_GameLogic_SkillSlot_SlotType + 0x0);
				MySkillSlot[SkillSlot] = instance;
				if (SkillSlot == 5) {
					void* SkillObj = *(void**)((uint64_t)instance + Field_Project_d_dll_Assets_Scripts_GameLogic_SkillSlot_SkillObj);
					if (SkillObj != NULL && old_SkillSlot_get_IsCDReady(instance)) {
				        int SkillID = *(int*)((uint64_t)SkillObj + Field_Project_d_dll_Assets_Scripts_GameLogic_SkillSlot__skillInfoData);
						if (AutoPunish && (SkillID == 80104 || SkillID == 80116)) {
                            *(int*)((uint64_t)(*(void**)((uint64_t)instance + Field_Project_d_dll_Assets_Scripts_GameLogic_SkillSlot_skillIndicator)) + Field_Project_d_dll_Assets_Scripts_GameLogic_SkillControlIndicator_bUseAdvanceSelect) = 0;
                            *(int*)((uint64_t)(*(void**)((uint64_t)instance + Field_Project_d_dll_Assets_Scripts_GameLogic_SkillSlot_skillIndicator)) + Field_Project_d_dll_Assets_Scripts_GameLogic_SkillControlIndicator_bSkillBtnDrag) = 0;
						}
					}		
				}
			}
		}
	}
    if (MyLActorRoot[0] != NULL) {
		if (MySkillSlot[9] != NULL) {
		    if (AutoUseSupportEquipment && m_PhanTram_HP > 0) {
		        void *ValueComponent = *(void**)((uint64_t)MyLActorRoot[0] + Adr_LActorRoot_ValueComponent);
				void *ActorControl = *(void**)((uint64_t)MyLActorRoot[0] + Adr_LActorRoot_ActorControl); 
                if (ValueComponent != NULL && ActorControl != NULL) {
		            int actorHp = old_ValuePropertyComponent_get_actorHp(ValueComponent);
                    int actorHpTotal = old_ValuePropertyComponent_get_actorHpTotal(ValueComponent);
				    if (old_SkillSlot_get_IsCDReady(MySkillSlot[9]) && !old_LObjWrapper_get_IsDeadState(ActorControl) && actorHp > 0 && actorHp < m_PhanTram_HP / 100.0 * actorHpTotal) {
			            old_SkillSlot_SendRequestUseSkill(MySkillSlot[9]);
                    }
                }
	        }
	        if (AutoSellSupportEquipment && SupportEquipmentIndex > 0) {
	            if (MyCBattleEquipSystem != NULL) {
					if (!old_SkillSlot_get_IsCDReady(MySkillSlot[9])) {
	                    old_CBattleEquipSystem_SendSellEquipFrameCommand(MyCBattleEquipSystem, SupportEquipmentIndex - 1);
					}
			    }
		    }
			MySkillSlot[9] = NULL;
		}
		void* EquipComponent = *(void **)((uint64_t)MyLActorRoot[0] + Field_Project_Plugins_d_dll_NucleusDrive_Logic_LActorRoot_HurtControl);
	    if (EquipComponent != NULL) {
			monoArray<stEquipInfo*> *m_equipInfos = *(monoArray<stEquipInfo*> **)((uint64_t)EquipComponent + Field_Project_Plugins_d_dll_NucleusDrive_Logic_LEquipComponent_m_equipInfos);
			if (m_equipInfos != NULL) {
				bool is1138 = false;
				for (int i = 0; i < m_equipInfos->getLength(); ++i) {
					stEquipInfo m_equipInfo = m_equipInfos->getPointer()[i];
					if (&m_equipInfo != NULL) {
					    int m_equipID = m_equipInfo.m_equipID;
	                    if (m_equipID == 1138) {
						    is1138 = true;
					    }
					}
		        }
				Equipped1138 = is1138;
			}
	    }
		MyLActorRoot[0] = NULL;
	}
}

bool (*old_PunishPromptDuration_get_isHpUnderPunishValue)(void*);
bool PunishPromptDuration_get_isHpUnderPunishValue(void* instance) {
	bool prevIsHpUnderPunishValue = old_PunishPromptDuration_get_isHpUnderPunishValue(instance);
	if (AutoPunish && prevIsHpUnderPunishValue) {
		old_SkillSlot_SendRequestUseSkill(MySkillSlot[5]);
	}
	return prevIsHpUnderPunishValue;
}

void (*old_CBattleSystem_Update)(void*);
void CBattleSystem_Update(void* instance) {
	MyCBattleEquipSystem = old_CBattleSystem_get_TheBattleEquipSystem(instance);
	
	
	old_CBattleSystem_Update(instance);
}

void (*old_HudComponent3D_AddBuffStat)(void*, void*);
void HudComponent3D_AddBuffStat(void* instance, void* newBuffStatInfo) {
	old_HudComponent3D_AddBuffStat(instance, newBuffStatInfo);
	int FromType = *(int*)((uint64_t)newBuffStatInfo + Field_Project_d_dll_Assets_Scripts_GameLogic_CBuffHudStatInfo_FromType + 0x0);
	int PerformanceType = *(int*)((uint64_t)newBuffStatInfo + Field_Project_d_dll_Assets_Scripts_GameLogic_CBuffHudStatInfo_PerformanceType + 0x0);
	if (MyCBattleEquipSystem != NULL && ActorLinker_MyHudControl == instance) {
		if (MyLActorRoot[1] != NULL) {
	        void* EquipComponent = *(void **)((uint64_t)MyLActorRoot[1] + Field_Project_Plugins_d_dll_NucleusDrive_Logic_LActorRoot_HurtControl);
	        if (EquipComponent != NULL) {
				monoArray<stEquipInfo*> *m_equipInfos = *(monoArray<stEquipInfo*> **)((uint64_t)EquipComponent + Field_Project_Plugins_d_dll_NucleusDrive_Logic_LEquipComponent_m_equipInfos);
				if (m_equipInfos != NULL) {
					for (int i = 0; i < m_equipInfos->getLength(); ++i) {
						stEquipInfo m_equipInfo = m_equipInfos->getPointer()[i];
						if (&m_equipInfo != NULL) {
							int m_equipID = m_equipInfo.m_equipID;
	                        if (m_equipID == 1337 && AutoSellBladeofEternity && FromType == 1 && PerformanceType == 19) {
						        old_CBattleEquipSystem_SendSellEquipFrameCommand(MyCBattleEquipSystem, i);
	                        }
	
	                        if (m_equipID == 11271 && AutoSellDeathSickle && FromType == 0 && PerformanceType == 18) {
		                        old_CBattleEquipSystem_SendSellEquipFrameCommand(MyCBattleEquipSystem, i);
	                        }
						}
				    }
				}
				
			}
		}
	}
}

void (*old_BuffProtectRule_AddBuffProtect)(void*, void*, int);
void BuffProtectRule_AddBuffProtect(void* instance, void* inBuff, int protectType) {
	old_BuffProtectRule_AddBuffProtect(instance, inBuff, protectType);
	
	void* buffHolder = *(void**)((uint64_t)instance + Field_Project_Plugins_d_dll_NucleusDrive_Logic_BuffProtectRule_buffHolder);
	if (buffHolder != NULL) {
		int BuffUniqueIdBegining = *(int*)((uint64_t)instance + Field_Project_Plugins_d_dll_NucleusDrive_Logic_BuffProtectRule_m_uiBeProtectedTotalValue);
		//saveLog(std::to_string(BuffUniqueIdBegining) + "&" + std::to_string(protectType));

	}
	if (MyLActorRoot[1] != NULL) {
	    void* EquipComponent = *(void **)((uint64_t)MyLActorRoot[1] + Field_Project_Plugins_d_dll_NucleusDrive_Logic_LActorRoot_HurtControl);
	    if (EquipComponent != NULL) {
	        void* mEquipBuySellLogic = *(void **)((uint64_t)EquipComponent + Field_Project_Plugins_d_dll_NucleusDrive_Logic_BuffProtectRule_m_fromOtherValueDict);
            if (mEquipBuySellLogic != NULL) {
	            //if (Hercules_MadnessIndex > 0 && (UniqueId == 115 || UniqueId == 548) && protectType == 3) {
		            //old_LEquipBuySellLogic_SellEquip(mEquipBuySellLogic, Hercules_MadnessIndex - 1);
	            //}
	        }
			MyLActorRoot[1] = NULL;
		}
	}
}

bool useHalfSkillInfoData;
void (*old_LActorRoot_UpdateLogicCn)(void *, int);
void LActorRoot_UpdateLogicCn(void* instance, int delta) {
    old_LActorRoot_UpdateLogicCn(instance, delta);
    Vector3 Position = VInt3ToVector3(old_LActorRoot_get_location(instance), old_LActorRoot_get_forward(instance));
	
	if (ActorLinker_MyObjID == old_LActorRoot_get_ObjID(instance)) {
	    MyLActorRoot[0] = instance;
		MyLActorRoot[1] = instance;
		MyPosition = Position;
	} else {
		void *ValueComponent = *(void**)((uint64_t)instance + Adr_LActorRoot_ValueComponent);
		void *ActorControl = *(void**)((uint64_t)instance + Adr_LActorRoot_ActorControl);
        if (ValueComponent != NULL && ActorControl != NULL) {
		    int actorHp = old_ValuePropertyComponent_get_actorHp(ValueComponent);
            int actorHpTotal = old_ValuePropertyComponent_get_actorHpTotal(ValueComponent);
			float DistanceActor = Vector3::Distance(MyPosition, Position);
			int _skillInfoData, SkillID;
			void* SkillObj;
			int LActorRoot_GiveMyEnemyCamp = old_LActorRoot_GiveMyEnemyCamp(instance);
		    if (old_LActorRoot_AsHero(instance) != NULL) {
				if (LActorRoot_GiveMyEnemyCamp == ActorLinker_objCamp) {
							
  
				
				    if (DistanceActor <= 4.5f) {
						if (MySkillSlot[5] != NULL) {
					        _skillInfoData = *(int*)((uint64_t)MySkillSlot[5] + Field_Project_d_dll_Assets_Scripts_GameLogic_SkillSlot__skillInfoData);
						    SkillObj = *(void**)((uint64_t)MySkillSlot[5] + Field_Project_d_dll_Assets_Scripts_GameLogic_SkillSlot_SkillObj);
						    if (SkillObj != NULL) {
				                SkillID = *(int*)((uint64_t)SkillObj + Field_Project_d_dll_Assets_Scripts_GameLogic_Skill_SkillID);
					            if (old_SkillSlot_get_IsCDReady(MySkillSlot[5]) && !old_LObjWrapper_get_IsDeadState(ActorControl) && actorHp > 0 && actorHp <= (actorHpTotal - actorHp) * 16 / 100) {
					                if (AutoExecute && SkillID == 80108) {
					                    old_SkillSlot_SendRequestUseSkill(MySkillSlot[5]);
						            }
					            }
						    }
						}
				    }
				}
		    } else {
				
				if (old_LActorRoot_AsMonster(instance) != NULL) {
					if (DistanceActor <= 5.9f) {
						if (MySkillSlot[5] != NULL) {
						    _skillInfoData = *(int*)((uint64_t)MySkillSlot[5] + Field_Project_d_dll_Assets_Scripts_GameLogic_SkillSlot__skillInfoData);
						    SkillObj = *(void**)((uint64_t)MySkillSlot[5] + Field_Project_d_dll_Assets_Scripts_GameLogic_SkillSlot_SkillObj);
						    if (SkillObj != NULL) {
				                SkillID = *(int*)((uint64_t)SkillObj + Field_Project_d_dll_Assets_Scripts_GameLogic_Skill_SkillID);
								/*
		                        if (
						           (actorHpTotal == 10000 || actorHpTotal > 11000) && actorHp <= (_skillInfoData)
							
						        ) */
								  if ((actorHpTotal == 10000 || actorHpTotal > 11000) && 
                    actorHp <= (useHalfSkillInfoData ? (_skillInfoData / 2) : _skillInfoData)) 
                    
								{
							        if (AutoPunish && (SkillID == 80104 || SkillID == 80116)) {
					                    old_SkillSlot_SendRequestUseSkill(MySkillSlot[5]);
							        }
					            }
						    }
						}
					}
					
				} else {
				    if (old_LActorRoot_AsOrgan(instance) != NULL) {
					    bool isWin = LActorRoot_GiveMyEnemyCamp == ActorLinker_objCamp;
						if (AutoWinLose == 1 && isWin || AutoWinLose == 2 && !isWin) {
							old_ValuePropertyComponent_set_actorHp(ValueComponent, 0);
				        }
				    } else {
					    
				    }
				}
		    }
		}
	}
}

///


void *anogs_thread(void *) {
while (!isLibraryLoaded(OBFUSCATE("libanogs.so")))
    {
        sleep(1);
    }

MemoryPatch::createWithHex("libanogs.so", 0x8b4a6,"00 20 70 47").Modify();
MemoryPatch::createWithHex("libanogs.so", 0x8d27e,"1F FF 2F E1").Modify();
MemoryPatch::createWithHex("libanogs.so", 0x8d2a8,"1F FF 2F E1").Modify();
MemoryPatch::createWithHex("libanogs.so", 0x8d28e,"1F FF 2F E1").Modify();
MemoryPatch::createWithHex("libanogs.so", 0xa1384,"00 BF 00 BF").Modify();
MemoryPatch::createWithHex("libanogs.so", 0xa1348,"00 BF 00 BF").Modify();
return NULL;
}




///


int (*old_AnoSDKOnRecvSignature)(void *instance, const char *name, uint8_t *data, uint32_t data_len, uint32_t crc);
int AnoSDKOnRecvSignature(void *instance, const char *name, uint8_t *data, uint32_t data_len, uint32_t crc)
{
	if(instance != NULL)
    return 0;
     }



//

List<void *> *(*PlrList)(void *ins);
int TypeKill;
void (*_Getuid)(void *ins,int uid);
void Getuid(void *ins,int uid) {
    
    if (ins != NULL && TypeKill != 0) {
    
   List<void *> *target = PlrList(ins);     
   if (target != NULL) {
   void **playerItem = (void **)target->getItems();
   for (int i = 0; i < target->getSize(); i++) {
   void *Player = playerItem[i];
   if (Player != NULL) {
   
  *(int *)((uintptr_t)Player  + IL2Cpp::Il2CppGetFieldOffset("Project.Plugins_d.dll","LDataProvider", "PlayerBase","broadcastID")) = TypeKill;    
 // *(String **)((uintptr_t)Player + IL2Cpp::Il2CppGetFieldOffset("Project.Plugins_d.dll", "LDataProvider", "PlayerBase", "Name")) = Il2CppString::Create("Qwen");    

  
  
  }}}}
   return _Getuid(ins,uid);  
  }
  //


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


    hexPatches.fps1 = MemoryPatch::createWithHex((uintptr_t)IL2Cpp::Il2CppGetMethodOffset(OBFUSCATE("Project_d.dll"), OBFUSCATE("Assets.Scripts.Framework"), OBFUSCATE("GameSettings"), OBFUSCATE("get_Supported60FPSMode"), 0), OBFUSCATE("01 00 A0 E3 1E FF 2F E1"));
	
	hexPatches.fps2 = MemoryPatch::createWithHex((uintptr_t)IL2Cpp::Il2CppGetMethodOffset(OBFUSCATE("Project_d.dll"), OBFUSCATE("Assets.Scripts.Framework"), OBFUSCATE("GameSettings"), OBFUSCATE("get_Supported90FPSMode"), 0), OBFUSCATE("01 00 A0 E3 1E FF 2F E1"));
	
	hexPatches.fps3 = MemoryPatch::createWithHex((uintptr_t)IL2Cpp::Il2CppGetMethodOffset(OBFUSCATE("Project_d.dll"), OBFUSCATE("Assets.Scripts.Framework"), OBFUSCATE("GameSettings"), OBFUSCATE("get_Supported120FPSMode"), 0), OBFUSCATE("01 00 A0 E3 1E FF 2F E1"));
	
	hexPatches.fps4 = MemoryPatch::createWithHex((uintptr_t)IL2Cpp::Il2CppGetMethodOffset(OBFUSCATE("Project_d.dll"), OBFUSCATE("Assets.Scripts.Framework"), OBFUSCATE("GameSettings"), OBFUSCATE("get_SupportedBoth60FPS_CameraHeight"), 0), OBFUSCATE("01 00 A0 E3 1E FF 2F E1"));
	
	hexPatches.fps5 = MemoryPatch::createWithHex((uintptr_t)IL2Cpp::Il2CppGetMethodOffset(OBFUSCATE("Project_d.dll"), OBFUSCATE("Assets.Scripts.Framework"), OBFUSCATE("GameSettings"), OBFUSCATE("IsIPadDevice"), 0), OBFUSCATE("01 00 A0 E3 1E FF 2F E1"));
	

//fps
  
  
  hexPatches.lsd=MemoryPatch::createWithHex((uintptr_t)IL2Cpp::Il2CppGetMethodOffset(OBFUSCATE("Project_d.dll"), OBFUSCATE("Assets.Scripts.GameSystem"), OBFUSCATE("CPlayerProfile"), OBFUSCATE("get_IsHostProfile"), 0), OBFUSCATE("01 00 A0 E3 1E FF 2F E1"));
  
  
  //history
  
   
  //show rank
  
    hexPatches.ulti1 = MemoryPatch::createWithHex((uintptr_t)IL2Cpp::Il2CppGetMethodOffset(OBFUSCATE("Project_d.dll"), OBFUSCATE("Assets.Scripts.GameSystem"), OBFUSCATE("HeroInfoPanel"), OBFUSCATE("ShowHeroInfo"), 2), OBFUSCATE("01 00 A0 E3 1E FF 2F E1"));
	
	hexPatches.ulti2 = MemoryPatch::createWithHex((uintptr_t)IL2Cpp::Il2CppGetMethodOffset(OBFUSCATE("Project_d.dll"), OBFUSCATE(""), OBFUSCATE("MiniMapHeroInfo"), OBFUSCATE("ShowSkillStateInfo"), 1), OBFUSCATE("01 00 A0 E3 1E FF 2F E1"));
	
	hexPatches.ulti3 = MemoryPatch::createWithHex((uintptr_t)IL2Cpp::Il2CppGetMethodOffset(OBFUSCATE("Project_d.dll"), OBFUSCATE(""), OBFUSCATE("MiniMapHeroInfo"), OBFUSCATE("ShowHeroHpInfo"), 1), OBFUSCATE("01 00 A0 E3 1E FF 2F E1"));
	

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


///
/*
Tools::Hook((void *) (uintptr_t)IL2Cpp::Il2CppGetMethodOffset(OBFUSCATE("Project.Plugins_d.dll"), OBFUSCATE("NucleusDrive.Logic"), OBFUSCATE("LFramework") , OBFUSCATE("EndGame"), 2), (void *) EndGame, (void **) &old_EndGame); //hàm end game

Tools::Hook((void *) (uintptr_t)IL2Cpp::Il2CppGetMethodOffset(OBFUSCATE("Project_d.dll"), OBFUSCATE("Assets.Scripts.GameSystem"), OBFUSCATE("HeroSelectBanPickWindow") , OBFUSCATE("OnPrepareBattle"), 0), (void *) EndGame, (void **) &old_EndGame); //hàm đứng không cho thoát ra sảnh

DobbyHook((void *)DobbySymbolResolver(OBFUSCATE("/system/lib/libc.so"), OBFUSCATE("send")), (void *)hook_send, (void **)&orignal_send); //hàm libc
*/

hexPatches.giamhoichieu = MemoryPatch::createWithHex("libil2cpp.so", 0x14ABCF4, "0080BB4443622A05");

Tools::Hook((void *) IL2Cpp::Il2CppGetMethodOffset(OBFUSCATE("Project_d.dll"), OBFUSCATE("Assets.Scripts.GameSystem"), OBFUSCATE("HeroSelectNormalWindow") , OBFUSCATE("CountDownHandler"), 3), (void *) CountDownHandler , (void ** ) &old_CountDownHandler); //HeroSelectNormalWindow - StartEndTimer
Tools::Hook((void *) IL2Cpp::Il2CppGetMethodOffset(OBFUSCATE("Project_d.dll"), OBFUSCATE("Assets.Scripts.GameSystem"), OBFUSCATE("HeroSelectNormalWindow") , OBFUSCATE("PrepareBattle"), 0), (void *) PrepareBattle , (void **) &old_PrepareBattle); //HeroSelectNormalWindow - StartEndTimer
Tools::Hook((void *) IL2Cpp::Il2CppGetMethodOffset(OBFUSCATE("Project_d.dll"), OBFUSCATE("Assets.Scripts.GameSystem"), OBFUSCATE("PVPLoadingView") , OBFUSCATE("OnEnter"), 0), (void *) PVPLoadingViewOnEnter , (void **) &old_PVPLoadingViewOnEnter); //HeroSelectNormalWindow - StartEndTimer
Tools::Hook((void *) (uintptr_t)IL2Cpp::Il2CppGetMethodOffset(OBFUSCATE("Project.Plugins_d.dll"), OBFUSCATE("NucleusDrive.Logic"), OBFUSCATE("LFramework") , OBFUSCATE("EndGame"), 2), (void *) hook_NucleusDrive_Logic_LFrameworkEndGame, (void **) &orig_NucleusDrive_Logic_LFrameworkEndGame); //hàm end game


//MemoryPatch::createWithHex((uintptr_t)IL2Cpp::Il2CppGetMethodOffset(OBFUSCATE("Project.Plugins_d.dll"), OBFUSCATE("NucleusDrive.Logic"), OBFUSCATE("LSynchrReport") , OBFUSCATE("get_Enable"), 0), OBFUSCATE("1E FF 2F E1")).Modify();
//MemoryPatch::createWithHex((uintptr_t)IL2Cpp::Il2CppGetMethodOffset(OBFUSCATE("Project.Plugins_d.dll"), OBFUSCATE("NucleusDrive.Logic"), OBFUSCATE("LSynchrReport") , OBFUSCATE("set_Enable"), 0), OBFUSCATE("1E FF 2F E1")).Modify();
MemoryPatch::createWithHex((uintptr_t)IL2Cpp::Il2CppGetMethodOffset(OBFUSCATE("Project.Plugins_d.dll"), OBFUSCATE("NucleusDrive.Logic"), OBFUSCATE("LDedicatedSvrConnector") , OBFUSCATE("SetLogin"), 2), OBFUSCATE("1E FF 2F E1")).Modify();
MemoryPatch::createWithHex((uintptr_t)IL2Cpp::Il2CppGetMethodOffset(OBFUSCATE("Project.Plugins_d.dll"), OBFUSCATE("NucleusDrive.Logic"), OBFUSCATE("LDedicatedSvrConnector") , OBFUSCATE("PrintTraffic"), 2), OBFUSCATE("1E FF 2F E1")).Modify();
//MemoryPatch::createWithHex((uintptr_t)IL2Cpp::Il2CppGetMethodOffset(OBFUSCATE("Project_d.dll"), OBFUSCATE("GCloud.AnoSDK"), OBFUSCATE("AnoSDK") , OBFUSCATE("AnoSDKOnRecvSignature"), 4), OBFUSCATE("00 00 A0 E3 1E FF 2F E1")).Modify();
Tools::Hook((void *) (uintptr_t)IL2Cpp::Il2CppGetMethodOffset(OBFUSCATE("Project_d.dll"), OBFUSCATE("GCloud.AnoSDK"), OBFUSCATE("AnoSDK") , OBFUSCATE("AnoSDKOnRecvSignature"), 4), (void *) AnoSDKOnRecvSignature, (void **) &old_AnoSDKOnRecvSignature); //




    TOOL_HOOK(Adr_LActorRoot_UpdateLogicCn, LActorRoot_UpdateLogicCn, old_LActorRoot_UpdateLogicCn)
	TOOL_HOOK(Adr_ActorLinker_UpdateCn, ActorLinker_UpdateCn, old_ActorLinker_UpdateCn)
    old_ActorLinker_IsHostPlayer = (bool (*)(void*)) (Adr_ActorLinker_IsHostPlayer);
	old_ActorLinker_get_ObjID = (int (*)(void*)) (Adr_ActorLinker_get_ObjID);
    old_LActorRoot_get_ObjID = (int (*)(void*)) (Adr_LActorRoot_get_ObjID);
    old_ValuePropertyComponent_get_actorHp = (int (*)(void*)) (Adr_ValuePropertyComponent_get_actorHp);
    old_ValuePropertyComponent_get_actorHpTotal = (int (*)(void*)) (Adr_ValuePropertyComponent_get_actorHpTotal);
    old_ValuePropertyComponent_set_actorHp = (void (*)(void*, int)) (Adr_ValuePropertyComponent_set_actorHp);
	old_LActorRoot_GiveMyEnemyCamp = (int (*)(void*)) (Adr_LActorRoot_GiveMyEnemyCamp);  
    old_SkillSlot_SendRequestUseSkill = (bool (*)(void*)) ((void*)(Method_Project_d_dll_Assets_Scripts_GameLogic_SkillSlot_SendRequestUseSkill_0));
	old_SkillSlot_get_IsCDReady = (bool (*)(void*)) ((void*)(Method_Project_d_dll_Assets_Scripts_GameLogic_SkillSlot_get_IsCDReady_0));
	old_LActorRoot_get_location = (VInt3 (*)(void*)) (Adr_LActorRoot_get_location);
	old_LActorRoot_get_forward = (VInt3 (*)(void*)) (Adr_LActorRoot_get_forward);
    old_LActorRoot_AsHero = (void *(*)(void*)) (Adr_LActorRoot_AsHero);
    old_LActorRoot_AsOrgan = (void *(*)(void*)) (Adr_LActorRoot_AsOrgan);
	old_LActorRoot_AsMonster = (void *(*)(void*)) (Adr_LActorRoot_AsMonster);
	old_CBattleSystem_get_TheBattleEquipSystem = (void *(*)(void*)) (Method_Project_d_dll_Assets_Scripts_GameSystem_CBattleSystem_get_TheBattleEquipSystem_0);
    old_CBattleEquipSystem_SendSellEquipFrameCommand = (void (*)(void*, int)) (Method_Project_d_dll_Assets_Scripts_GameSystem_CBattleEquipSystem_SendSellEquipFrameCommand_1);
    old_ActorLinker_get_objCamp = (int (*)(void *)) (Adr_ActorLinker_get_objCamp);
    
	old_ActorLinker_get_objType = (int (*)(void*)) (Adr_ActorLinker_get_objType);
	old_LObjWrapper_get_IsDeadState = (bool (*)(void*)) (Adr_LObjWrapper_get_IsDeadState);

TOOL_HOOK(Method_Project_d_dll_Assets_Scripts_GameLogic_SkillSlot_LateUpdate_1, SkillSlot_LateUpdate, old_SkillSlot_LateUpdate);
TOOL_HOOK(Method_Project_d_dll_Assets_Scripts_GameLogic_HudComponent3D_AddBuffStat_1, HudComponent3D_AddBuffStat, old_HudComponent3D_AddBuffStat);
TOOL_HOOK(Method_Project_Plugins_d_dll_NucleusDrive_Logic_BuffProtectRule_AddBuffProtect_2, BuffProtectRule_AddBuffProtect, old_BuffProtectRule_AddBuffProtect);
TOOL_HOOK(Method_Project_d_dll_Assets_Scripts_GameSystem_CBattleSystem_Update_0, CBattleSystem_Update, old_CBattleSystem_Update);
Tools::Hook((void *) (uintptr_t)IL2Cpp::Il2CppGetMethodOffset(OBFUSCATE("Project_d.dll"), OBFUSCATE("Assets.Scripts.GameSystem"), OBFUSCATE("CSkillButtonManager") , OBFUSCATE("SendUseCommonAttack"), 2), (void *) SendUseCommonAttack, (void **) &old_SendUseCommonAttack); //


return NULL;
}


jobjectArray GetFeatureList(JNIEnv *env, jobject context) {
    jobjectArray ret;

    const char *features[] = {
		
			
            OBFUSCATE("Category_▼Socıal Medıa▼"), //Not Counted
             
            OBFUSCATE("ButtonLink_𝗧𝗘𝗟𝗘𝗚𝗥𝗔𝗠 𝗚𝗥𝗢𝗨𝗣 𝗛𝗚𝗠𝗢𝗗_https://t.me/HGMOD_CS"),
            
		    OBFUSCATE("ButtonLink_𝗬𝗢𝗨𝗧𝗨𝗕𝗘 𝗖𝗛𝗔𝗡𝗡𝗘𝗟 𝗖𝗔𝗞𝗠𝗢𝗗𝗦_https://youtube.com/@cakmodsgame"),
			
			OBFUSCATE("Category_▼ᴀɴᴛɪ ʀᴇᴘᴏʀᴛ▼"), 
			
			OBFUSCATE("RichWebView_<html><body><span style=\"color: aqua; font-weight: bold;\">⮕ ANTIREPORT: </span><span style=\"color: red; font-weight: bold;\"> ACTIVE✓</span></body></html>"),
			
			OBFUSCATE("Category_❗Lưu Ý: Chống Tố Auto Bật Tắt Hết Trận Chờ Vài Giây Khi Nào Hiện Trận Đấu Kết Thúc Thì Thoát Game Vào Lại"),
	
		/*    OBFUSCATE("Category_▼ᴀɴᴛɪ ʀᴇᴘᴏʀᴛ▼"), 
			
         	OBFUSCATE("Category_▼ᴀɴᴛɪ ʀᴇᴘᴏʀᴛ▼"), 
		    
		    OBFUSCATE("77_Toggle_➢ 𝘼𝙉𝙏𝙄 𝙍𝙀𝙋𝙊𝙍𝙏.𝙚𝙭𝙚"),
			
			OBFUSCATE("Category_▼ Cách dùng: Chọn tướng xong, còn 5 giây bật xoá tố lên - Hết trận hiện Win/Loss chờ 5 giây rồi tắt xoá tố đi (Chờ hiện trận đấu kết thúc thì thoát game vào lại).▼"),
			*/
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
			
			OBFUSCATE("12_CollapseAdd_Toggle_➢ 𝙂𝙄𝘼𝙈 𝟯𝟬% 𝙃𝙊𝙄 𝘾𝙃𝙄𝙀𝙐"),
			
			OBFUSCATE("Category_▼Mod Skin Nút Bấm▼"),
			
			OBFUSCATE("Collapse_Mod Button Skin"),
			
			//OBFUSCATE("1031_CollapseAdd_Spinner_𝙐𝙣𝙡𝙤𝙘𝙠 𝙏𝙝𝙤̂𝙣𝙜 𝘽𝙖́𝙤 𝙃𝙖̣_Chọn Thông Báo,Nak Quỷ Thương, Allain, Capheny, Violet Vệ Thần, Ilumia, Tulen Gojo, Biron Gojo, Liliana, Nak Bạch Diện, Krixi Học Viện, Aya Công Túa 🌈, Veres Lưu Ly, Mu Lỏ Tuyệt Thế Thần Binh, Yena Cửu Huyền Thiên, Raz Gon, Nak Killua"),
			
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
			
			OBFUSCATE("1743_CollapseAdd_Toggle_➢ 𝘼𝘾𝙏𝙄𝙑𝙀 𝘼𝙄𝙈 𝙉𝘼𝙏𝘼"),     
          
            OBFUSCATE("10_CollapseAdd_RadioButton_➢ MỤC TIÊU AIM_⮕% 𝙈𝙖́𝙪 𝙏𝙝𝙖̂́𝙥 𝙉𝙝𝙖̂́𝙩,⮕𝙈𝙖́𝙪 𝙏𝙝𝙖̂́𝙥 𝙉𝙝𝙖̂́𝙩,⮕𝙆𝙝𝙤𝙖̉𝙣𝙜 𝘾𝙖́𝙘𝙝 𝙂𝙖̂̀𝙣 𝙉𝙝𝙖̂́𝙩,⮕𝙂𝙖̂̀𝙣 𝙏𝙞𝙖 𝙉𝙝𝙖̂́𝙩"),  
            
       OBFUSCATE("Category_▼ᴘʟᴀʏᴇʀ ғᴜɴᴄᴛɪᴏɴ▼"),
       
       OBFUSCATE("Collapse_Chức Năng Tuyển Thủ"),
        
        OBFUSCATE("28_CollapseAdd_Toggle_➢ 𝙈𝙖𝙘𝙧𝙤 𝘾𝙝𝙞𝙚̂𝙪 𝟮 𝙁𝙡𝙤𝙧𝙚𝙣𝙩𝙞𝙣𝙤"), 
        
        OBFUSCATE("20_CollapseAdd_Toggle_➢ 𝘼𝙪𝙩𝙤 𝘿𝙪̀𝙣𝙜 𝙋𝙝𝙪̀ 𝙏𝙧𝙤̛̣"),
        
        OBFUSCATE("2191_CollapseAdd_SeekBar_% 𝙃𝙋 𝙎𝙪̛̉ 𝘿𝙪̣𝙣𝙜_0_50"),
        
        OBFUSCATE("23_CollapseAdd_Toggle_➢ 𝘼𝙪𝙩𝙤 𝘽𝙤̣̂𝙘 𝙋𝙝𝙖́"),
			
        OBFUSCATE("29_CollapseAdd_RadioButton_𝘼𝙪𝙩𝙤 𝙏𝙧𝙪̛̀𝙣𝙜 𝙏𝙧𝙞̣_𝙏𝙖̆́𝙩 𝘼𝙪𝙩𝙤,𝘽𝙞̣ 𝙂𝙞𝙖̉𝙢 𝙎𝙏 𝙍𝙤̂̀𝙣𝙜 - 𝙏𝙖̀,𝙆𝙝𝙤̂𝙣𝙜 𝘽𝙞̣ 𝙂𝙞𝙖̉𝙢 𝙎𝙏"),
      
        OBFUSCATE("24_CollapseAdd_Toggle_➢ 𝙏𝙪̛̣ 𝘿𝙤̣̂𝙣𝙜 𝘽𝙖́𝙣 𝘿𝙤̂̀ 𝙎𝙖𝙪 𝙆𝙝𝙞 𝘿𝙪̀𝙣𝙜 𝙋𝙝𝙪̀ 𝙏𝙧𝙤̛̣"),
        
        OBFUSCATE("25_CollapseAdd_SeekBar_➢ 𝙊̂ 𝘿𝙤̂̀ 𝙈𝙪𝙤̂́𝙣 𝘽𝙖́𝙣(0 = off)_0_6"),
            
	
			
	
			
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
            fps = boolean;
            if (fps) {
                hexPatches.fps1.Modify(); 
                hexPatches.fps2.Modify(); 
                hexPatches.fps3.Modify(); 
                hexPatches.fps4.Modify(); 
                hexPatches.fps5.Modify(); 
                
          
            } else {
                hexPatches.fps1.Restore();
                hexPatches.fps2.Restore();
                hexPatches.fps3.Restore();
                hexPatches.fps4.Restore();
                hexPatches.fps5.Restore();


            }
            break;  
            
            
            
            case 5:
            Lsd = boolean;
            if (Lsd) {
                hexPatches.lsd.Modify(); 
                
                
          
            } else {
                hexPatches.lsd.Restore();
                


            }
            break;  
            
            
            case 6:
            
           Avatar = boolean;
           
           break;
            
            
            case 4:
            ulti = boolean;
            if (ulti) {
                hexPatches.ulti1.Modify(); 
                hexPatches.ulti2.Modify(); 
                hexPatches.ulti3.Modify(); 
                
          
            } else {
                hexPatches.ulti1.Restore();
                hexPatches.ulti2.Restore();
                hexPatches.ulti3.Restore();
                
            }
            break;  
			
			
			
			
			
			
			
	/*		
			case 77:
				
			B_FireWall = boolean;
			
			break;
			
			
			*/
            
            
            case 12:
            giamhoichieu = boolean;
            if (giamhoichieu) {
                hexPatches.giamhoichieu.Modify(); 
                
          
            } else {
                hexPatches.giamhoichieu.Restore();


            }
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
			
		
	



/*
case 8888:
      switch (value) {
     case 1: TypeKill = 16; break; case 2: TypeKill = 30; break; case 3: TypeKill = 29; break; case 4: TypeKill = 28; break; case 5: TypeKill = 26; break; case 6: TypeKill = 25; break; case 7: TypeKill = 24; break; case 8: TypeKill = 23; break; case 9: TypeKill = 20; break; case 10: TypeKill = 19; break; case 11: TypeKill = 17; break; case 12: TypeKill = 15; break; case 13: TypeKill = 13; break; case 14: TypeKill = 11; break; case 15: TypeKill = 10; break; case 16: TypeKill = 9; break;     
      }	
*/
  
    case 20:
    AutoUseSupportEquipment = boolean;
    break;
    case 2191:
    m_PhanTram_HP = value;
    break;

    
    
    
                    case 29:
            switch (value) {
            
                case 1:
                useHalfSkillInfoData = false;
                AutoPunish = false;
                break;
                
                case 2:
                useHalfSkillInfoData = true;
                AutoPunish = true;
                break;
                
                case 3:
                useHalfSkillInfoData = false;
                AutoPunish = true;
                break;
                
                }
    
    
    
    
    case 23:
    AutoExecute = boolean;
    break;
    case 24:
    AutoSellSupportEquipment = boolean;
    break;
    case 25:
    SupportEquipmentIndex = 0;
    mvalue_SupportEquipmentIndex = value;
    if (m_SupportEquipmentIndex < 100) {
     m_SupportEquipmentIndex = 100;
        std::thread([]() {
         while (m_SupportEquipmentIndex < 102) {
       sleep(1);
       m_SupportEquipmentIndex ++;
      }
      SupportEquipmentIndex = mvalue_SupportEquipmentIndex;
      m_SupportEquipmentIndex = mvalue_SupportEquipmentIndex;
        }).detach();
    }
    break;
    


case 1743:

AimNata = boolean;
break;

	

case 28:
macro = boolean;
break;
			
			
/*
case 1031:
      switch (value) {
   case 0: TypeKill = 0; break;  case 1: TypeKill = 16; break; case 2: TypeKill = 30; break; case 3: TypeKill = 29; break; case 4: TypeKill = 28; break; case 5: TypeKill = 26; break; case 6: TypeKill = 25; break; case 7: TypeKill = 24; break; case 8: TypeKill = 23; break; case 9: TypeKill = 20; break; case 10: TypeKill = 19; break; case 11: TypeKill = 17; break; case 12: TypeKill = 15; break; case 13: TypeKill = 13; break; case 14: TypeKill = 11; break; case 15: TypeKill = 10; break; case 16: TypeKill = 9; break;     
      }
   
   
*/

             
             
			

    

    
}

        
    }


__attribute__((constructor))
void lib_main() {


    pthread_t ptid;
    pthread_create(&ptid, NULL, anogs_thread, NULL);
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
