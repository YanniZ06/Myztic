#pragma once

#include <al.h>
#include <alc.h>
#include <efx.h>
#include <alext.h>

namespace Myztic {
    class ALExt {
    public:
        static void initAllEXT(ALCdevice* device);

        static void initDeviceClock(ALCdevice* device);
        static LPALCGETINTEGER64VSOFT alcGetInteger64vSOFT;

        static void initEFX();
        static LPALGENEFFECTS alGenEffects;
        static LPALDELETEEFFECTS alDeleteEffects;
        static LPALISEFFECT alIsEffect;
        static LPALEFFECTI alEffecti;
        static LPALEFFECTIV alEffectiv;
        static LPALEFFECTF alEffectf;
        static LPALEFFECTFV alEffectfv;
        static LPALGETEFFECTI alGetEffecti;
        static LPALGETEFFECTIV alGetEffectiv;
        static LPALGETEFFECTF alGetEffectf;
        static LPALGETEFFECTFV alGetEffectfv;
        static LPALGENFILTERS alGenFilters;
        static LPALDELETEFILTERS alDeleteFilters;
        static LPALISFILTER alIsFilter;
        static LPALFILTERI alFilteri;
        static LPALFILTERIV alFilteriv;
        static LPALFILTERF alFilterf;
        static LPALFILTERFV alFilterfv;
        static LPALGETFILTERI alGetFilteri;
        static LPALGETFILTERIV alGetFilteriv;
        static LPALGETFILTERF alGetFilterf;
        static LPALGETFILTERFV alGetFilterfv;
        static LPALGENAUXILIARYEFFECTSLOTS alGenAuxiliaryEffectSlots;
        static LPALDELETEAUXILIARYEFFECTSLOTS alDeleteAuxiliaryEffectSlots;
        static LPALISAUXILIARYEFFECTSLOT alIsAuxiliaryEffectSlot;
        static LPALAUXILIARYEFFECTSLOTI alAuxiliaryEffectSloti;
        static LPALAUXILIARYEFFECTSLOTIV alAuxiliaryEffectSlotiv;
        static LPALAUXILIARYEFFECTSLOTF alAuxiliaryEffectSlotf;
        static LPALAUXILIARYEFFECTSLOTFV alAuxiliaryEffectSlotfv;
        static LPALGETAUXILIARYEFFECTSLOTI alGetAuxiliaryEffectSloti;
        static LPALGETAUXILIARYEFFECTSLOTIV alGetAuxiliaryEffectSlotiv;
        static LPALGETAUXILIARYEFFECTSLOTF alGetAuxiliaryEffectSlotf;
        static LPALGETAUXILIARYEFFECTSLOTFV alGetAuxiliaryEffectSlotfv;

        static void initSysEvents(ALCdevice* device);
        static LPALCEVENTISSUPPORTEDSOFT alcEventIsSupportedSOFT;
        static LPALCEVENTCONTROLSOFT alcEventControlSOFT;
        static LPALCEVENTCALLBACKSOFT alcEventCallbackSOFT;
    };
}