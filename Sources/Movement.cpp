#include "cheats.hpp"

namespace CTRPluginFramework
{
    void    CoordinateModifier(MenuEntry *entry)
    {
        if (Controller::IsKeysDown(A + DPadDown))
            Player::GetInstance()->AddToCoordinates(0.f, 0.f, 0.1f);
        if (Controller::IsKeysDown(A + DPadUp))
            Player::GetInstance()->AddToCoordinates(0.f, 0.f, -0.1f);
        if (Controller::IsKeysDown(A + DPadLeft))
            Player::GetInstance()->AddToCoordinates(-0.1f, 0.f, 0.f);
        if (Controller::IsKeysDown(A + DPadRight))
            Player::GetInstance()->AddToCoordinates(0.1f, 0.f, 0.f);
    }

    void    Teleporter(MenuEntry *entry)
    {
        static Coordinates  savedPos[3] = { 0 };

        int           slot = 0;

        if (Controller::IsKeysDown(L))
            slot = 2;
        else if (Controller::IsKeysDown(R))
            slot = 1;

        if (Controller::IsKeysDown(B + DPadUp))
        {
            savedPos[slot] = Player::GetInstance()->GetCoordinates();
        }
        else if (Controller::IsKeysDown(B + DPadDown))
        {
            Player::GetInstance()->SetCoordinates(savedPos[slot]);
        }
    }

    void    WalkOverThings(MenuEntry *entry)
    {
        u32     offsets[] = { 0x6503FC, 0x650414, 0x650578, 0x6505F0, 0x6506A4, 0x6506BC, 0x6506C0, 0x6506ec };
        u32     original[] = { 0x0A000094, 0x0A000052, 0x0A000001, 0xDA000014, 0xED841A05, 0xED840A07, 0x0A000026, 0x0A000065 };
        u32     patch[] = { 0xEA000094, 0xEA000052, 0xEA000001, 0xEA000014, 0xE1A00000, 0xE1A00000, 0xEA000026, 0xEA000065 };
        
        if (Controller::IsKeysDown(L + DPadUp))
        {
            for (int i = 0; i < 8; i++)
            {
                Process::Patch(offsets[i], (u8 *)&patch[i], 4);
            }
        }

        if (Controller::IsKeysDown(L + DPadDown))
        {
            for (int i = 0; i < 8; i++)
            {
                Process::Patch(offsets[i], (u8 *)&original[i], 4);
            }
        }
    }

    void    SpeedHack(MenuEntry *entry)
    {
        // Init - Set velocity if it's 0
        if (!entry->GetArg())
        {
            entry->SetArg(new float);
            float *arg = static_cast<float *>(entry->GetArg());
            *arg = 0.1f;
        }

        if (Controller::IsKeysDown(B))
        {
            u32     velocity = READU32(Game::Velocity);
            float   speed = *static_cast<float *>(entry->GetArg());
 
            if (velocity >= 0x41A79DB3)
            {
                WRITEU32(Game::Velocity, 0x41A79DB3);
            }
            else if (velocity > 0)
            {
                ADDTOFLOAT(Game::Velocity, speed);
            }
        }
    }

    bool    CheckSpeedInput(const void *in, std::string &error)
    {
        float  input = *reinterpret_cast<const float *>(in);

        // If input >= 1.f => ERROR
        if (input >= 1.f)
        {
            error = "Error !\n\nThe speed can't be superior to 0.9";
            return (false);
        }

        // If input <= 0.f => ERROR
        if (input <= 0.f)
        {
            error = "Error !\n\nThe speed must be superior to 0 and can't be negative";
            return (false);
        }

        // Else we're good
        return (true);
    }

    void    SpeedHackEditor(MenuEntry *entry)
    {
        // Init - Set velocity if it's 0
        if (!entry->GetArg())
        {
            entry->SetArg(new float);
            float *arg = static_cast<float *>(entry->GetArg());
            *arg = 0.1f;
        }

        Keyboard kb("Speed Hack Editor\n\nEnter the speed you want to use:");

        float   *initialSpeed = static_cast<float *>(entry->GetArg());
        float   newSpeed;

        kb.SetCompareCallback(CheckSpeedInput);

        if (kb.Open(newSpeed, *initialSpeed) != -1)
        {
            *initialSpeed = newSpeed;
        }
    }

    void    MoonJump(MenuEntry *entry)
    {
        if (Controller::IsKeysDown(L + DPadUp))
        {
                Process::Write16(Game::Gravity, 0xFFFF);
                Player::GetInstance()->AddToCoordinates(0.f, 0.05f, 0.f);
        }
        if (Controller::IsKeysDown(L + DPadDown))
        {
            Player::GetInstance()->AddToCoordinates(0.f, -0.05f, 0.f);
        }
    }
}