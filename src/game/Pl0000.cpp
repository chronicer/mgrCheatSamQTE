#include "shared.h"
#include "Pl0000.h"
#include <cGameUIManager.h>

Pl0000::Pl0000()
{
    ((void (__thiscall *)(Pl0000 *))(shared::base + 0x6C0310))(this);
}

void Pl0000::SetSlowMo(float Duration, float SlowRate)
{
    ((void (__thiscall *)(Pl0000 *, float, float))(shared::base + 0x77AB80))(this, Duration, SlowRate);
}

void Pl0000::EnableRipperMode()
{
    ((void (__thiscall *)(Pl0000 *))(shared::base + 0x785190))(this);
}

void Pl0000::SetFuelContainer(float FuelContainer)
{
    ((void (__thiscall *)(Pl0000 *, float))(shared::base + 0x7C3100))(this, FuelContainer);
}

void Pl0000::DisableRipperMode(bool bUseFade)
{
    ((void (__thiscall *)(Pl0000 *, bool))(shared::base + 0x7D9590))(this, bUseFade);
}



float Pl0000::GetMaxFuelContainer()
{
    return ((float (__thiscall *)(Pl0000 *))(shared::base + 0x7DA020))(this);
}

bool Pl0000::IsParrying()
{
    return ((bool (__thiscall *)(Pl0000 *))(shared::base + 0x6C0B30))(this);
}

bool Pl0000::IsOnGround()
{
    return ((bool (__thiscall *)(Pl0000 *))(shared::base + 0x6C0BB0))(this);
}

bool Pl0000::IsInAir()
{
    return ((bool (__thiscall *)(Pl0000 *))(shared::base + 0x77C730))(this);
}

void Pl0000::CallEffect(int id, cEspControler *esp)
{
    ((void(__thiscall *)(Pl0000*, int, cEspControler *))(shared::base + 0x7C3470))(this, id, esp);
}

void Pl0000::SamTestAttack()
{
    ((void(__thiscall*)(Pl0000*))(shared::base + 0x47C000))(this);
}


//////////////
void Pl0000::SamUpdateInput()
{
    ((void(__thiscall*)(Pl0000*))(shared::base + 0x492EB0))(this);
}

void Pl0000::SamHandleAction()
{
    ((void(__thiscall*)(Pl0000*))(shared::base + 0x49C910))(this);
}

void Pl0000::SamCharges() 
{
    ((void(__thiscall*)(Pl0000*))(shared::base + 0xD8A0))(this);
}

///////////////////////////


void Pl0000::RaidenUpdate()
{
    ((void(__thiscall*)(Pl0000*))(shared::base + 0x8104B0))(this);
}

void Pl0000::RaidenHandleAction()
{
    ((void(__thiscall*)(Pl0000*))(shared::base + 0x805000))(this);
}

void Pl0000::RaidenUpdateInput()
{
    ((void(__thiscall*)(Pl0000*))(shared::base + 0x8104B0))(this);
}

void Pl0000::ArmstrongTick()
{
    ((void(__fastcall*)(Pl0000*))(shared::base + 0x4578B0))(this);
}

void Pl0000::FindAnimation(int animationID, int a3, float a4, float a5, int a6, float a7, float a8) {
    ((void(__thiscall*)(Pl0000*, int animationID, int a3, float a4, float a5, int a6, float a7, float a8))(shared::base + 0x6A4080))(this, animationID, a3, a4, a5, a6, a7, a8);
}

void Pl0000::sub_464400(int a2, int a3, float a4, float a5, int a6) {
    ((void(__thiscall*)(Pl0000*, int a2, int a3, float a4, float a5, int a6))(shared::base + 0x464400))(this, a2, a3, a4, a5, a6);
}

void Pl0000::sub_77B270(float arg0, float a3, float a4, float a5) {
    ((void(__thiscall*)(Pl0000*, float arg0, float a3, float a4, float a5))(shared::base + 0x77B270))(this, arg0, a3, a4, a5);
}

bool Pl0000::sub_68C760(int a1) {
   return ((bool(__thiscall*)(Pl0000*, int a1))(shared::base + 0x68C760))(this,a1);
}

void Pl0000::sub_7884C0() {
    ((void(__thiscall*)(Pl0000*))(shared::base + 0x7884C0))(this);
}

void Pl0000::sub_46F130() {
    ((void(__thiscall*)(Pl0000*))(shared::base + 0x46F130))(this);
}
void Pl0000::sub_786010(int a1) {
    ((void(__thiscall*)(Pl0000*, int a1))(shared::base + 0x786010))(this, a1);
}

void Pl0000::sub_794790(float a2, float a3) {
    ((void(__thiscall*)(Pl0000*, float a2, float a3))(shared::base + 0x794790))(this, a2, a3);
}


void Pl0000::sub_7954D0() {
    ((void(__thiscall*)(Pl0000*))(shared::base + 0x7954D0))(this);
}

void Pl0000::sub_7E9130() {
    ((void(__thiscall*)(Pl0000*))(shared::base + 0x7E9130))(this);
}

void Pl0000::sub_77D1B0() {
    ((void(__thiscall*)(Pl0000*))(shared::base + 0x77D1B0))(this);
}

void Pl0000::sub_788B90() {
    ((void(__thiscall*)(Pl0000*))(shared::base + 0x788B90))(this);
}




void Pl0000::sub_7E9E40() {
    ((void(__thiscall*)(Pl0000*))(shared::base + 0x7E9E40))(this);
}

void Pl0000::sub_7EE8D0() {
    ((void(__thiscall*)(Pl0000*))(shared::base + 0x7EE8D0))(this);
}

void Pl0000::sub_787FF0() {
    ((void(__thiscall*)(Pl0000*))(shared::base + 0x787FF0))(this);
}





void Pl0000::sub_805D50() {
    ((void(__thiscall*)(Pl0000*))(shared::base + 0x805D50))(this);
}


void Pl0000::sub_7E8330() { //sword lighting?
    ((void(__thiscall*)(Pl0000*))(shared::base + 0x7E8330))(this);
}

void Pl0000::ArmstrongModelFix() { //sword lighting?
    ((void(__thiscall*)(Pl0000*))(shared::base + 0x1AD6F0))(this);
}


/*void Pl0000::sub_77B270(float a1, float a2, float a3, float a4) {
    ((void(__thiscall*)(Pl0000*, float a1, float a2, float a3, float a4))(shared::base + 0x77B270))(this, a1,a2,a3,a4);
}*/

void Pl0000::ArmstrongHeal() {
    ((void(__thiscall*)(Pl0000*))(shared::base + 0x1BFE80))(this);
}


bool Pl0000::SamTestAttack(int* a2) {
        DWORD* v4; // eax
        BOOL v6; // ebx
        bool v7; // cc
        unsigned int v8; // [esp-18h] [ebp-30h]
        int v9; // [esp+8h] [ebp-10h]
        int v10[3]; // [esp+Ch] [ebp-Ch] BYREF
        BOOL v11; // [esp+1Ch] [ebp+4h]

        this->field_2680 = 0;
       // if (((unsigned int)0x800000 & *(DWORD*)&Trigger::GameFlags) != 0)
         //   return 0;
     //   v4 = sub_A678D0(v10, 2, 1092, -1);
       // if (sub_A7A6E0(v4))
         //   return 0;
        v6 = 1;
        v9 = 0;
        /**/ if (a2[4] || a2[6])
        {
            //  v6 = sub_68C760(this, 1);
              //if (sub_68C760(this, 24))
              //{
                //  if (player->field_39E0)
                  //    v6 = 1;
             // }
        //  }
        //  if (sub_68C760(this, 34))
            //  v9 = 1;
            v11 = a2[7] == 0;
            if (a2[3])
            {
                if (!v6)
                    goto LABEL_20;
                if (((this->field_E3C & this->field_CF8) != 0 || this->field_25AC) && !a2[7]
                    && this->sub_45C910()
                    )
                {
                    this->field_2644 = 0;
                    this->field_25AC = 0;
                    this->field_256C = 0;
                    this->field_2568 = 0;
                LABEL_18:
                    this->field_2628 = 0;
                    this->field_263C = NAN;
                    return 1;
                }
            }
            if (!v6)
            {
            LABEL_20:
                if (!v9)
                    goto LABEL_23;
            }
            if (this->field_E70)
            {
                this->field_E70 = 0;
                this->SetState(0x10001B, 0, 0, 0);
                this->field_2644 = 0;
                return 1;
            }
        LABEL_23:
            if (v6)
            {
                if (this->field_25F8 && ((this->field_E24 & this->field_CFC) != 0 || this->field_256C))
                {
                    v8 = 1048594;
                LABEL_32:
                    this->field_263C = NAN;
                    this->field_2628 = 0;
                    this->field_2620 = 0;
                    this->field_2568 = 0;
                    this->field_25F4 = 0;
                    this->field_25F8 = 0;
                    this->SetState(v8, 0, 0, 0);
                    this->field_2628 = 0;
                    this->field_2644 = 0;
                    return 1;
                }
                if (this->field_25F4 && ((this->field_E20 & this->field_CFC) != 0 || this->field_256C))
                {
                    v8 = 1049595;
                    goto LABEL_32;
                }
            }
            if (!a2[9] || !v6 || 
               // !sub_45C530(player) ||
                !v11)
            {
                if (a2[8])
                {
                    if (!v6)
                        return 0;
                    if (v11 
                       // && sub_45D910(player, *a2)
                        )
                        goto LABEL_18;
                }
                if (v6 
                  //  && player->field_5428
                    )
                {
                    this->field_2620 = 0;
                    //player->field_5428 = 0;
                    this->field_2568 = 0;
                    this->field_256C = 0;
                    this->SetState(0x10001C, 0, 0, 0);
                    return 1;
                }
                return 0;
            }
            if (a2[1])
                this->field_2628 = 0;
            this->field_256C = 0;
            this->field_2630 = 0.0;
            if (a2[2])
            {
                v7 = this->field_2628 < 4;
                this->field_263C = NAN;
                if (!v7)
                {
                    this->field_2628 = 0;
                    this->field_263C = NAN;
                }
            }
           // player->field_5430 = 0;
            this->SetState(0x100011, 0, 0, 0);
            this->field_3350 = 0;
            this->field_2644 = 0;
            return 1;
        }
}

int Pl0000::sub_45C910()
{
    return ((int(__thiscall*)(Pl0000*))(shared::base + 0x45C910))(this);
}


Entity Pl0000::KamaititiCreate(float a2) {
    return ((Entity(*__thiscall)(float))0x46C610)(0.0);
}


bool Pl0000::IsUnarmed() 
{
    return ((bool(__thiscall*)(Pl0000*))(shared::base + 0x780980))(this);
}


int Pl0000::GetMaxHealth()
{
    return ((int(__thiscall *)(Pl0000 *))(shared::base + 0x77C980))(this);
}

cVec4 Pl0000::GetPosForCamera()
{
    return ((cVec4 (__thiscall *)(Pl0000 *))(shared::base + 0x6C0C60))(this);
}

void Pl0000::ForceKill()
{
    ((void (__thiscall *)(Pl0000 *))(shared::base + 0x77CE20))(this);
}
