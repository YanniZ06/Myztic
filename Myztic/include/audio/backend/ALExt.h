#pragma once

#include <al.h>
#include <alc.h>
#include <efx.h>
#include <alext.h>

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

inline void ALExt::initAllEXT(ALCdevice* device) {
    initSysEvents(device);
    initDeviceClock(device);
    initEFX();
}

inline void ALExt::initSysEvents(ALCdevice* device) {
    alcEventIsSupportedSOFT = (LPALCEVENTISSUPPORTEDSOFT)alcGetProcAddress(device, "alcEventIsSupportedSOFT");
    alcEventControlSOFT = (LPALCEVENTCONTROLSOFT)alcGetProcAddress(device, "alcEventControlSOFT");
    alcEventCallbackSOFT = (LPALCEVENTCALLBACKSOFT)alcGetProcAddress(device, "alcEventControlSOFT");
}

inline void ALExt::initDeviceClock(ALCdevice* device) {
    alcGetInteger64vSOFT = (LPALCGETINTEGER64VSOFT)alcGetProcAddress(device, "alcGetInteger64vSOFT");
}


inline void ALExt::initEFX() {
    alGenEffects = (LPALGENEFFECTS)alGetProcAddress("alGenEffects");
    alDeleteEffects = (LPALDELETEEFFECTS)alGetProcAddress("alDeleteEffects");
    alIsEffect = (LPALISEFFECT)alGetProcAddress("alIsEffect");
    alEffecti = (LPALEFFECTI)alGetProcAddress("alEffecti");
    alEffectiv = (LPALEFFECTIV)alGetProcAddress("alEffectiv");
    alEffectf = (LPALEFFECTF)alGetProcAddress("alEffectf");
    alEffectfv = (LPALEFFECTFV)alGetProcAddress("alEffectfv");
    alGetEffecti = (LPALGETEFFECTI)alGetProcAddress("alGetEffecti");
    alGetEffectiv = (LPALGETEFFECTIV)alGetProcAddress("alGetEffectiv");
    alGetEffectf = (LPALGETEFFECTF)alGetProcAddress("alGetEffectf");
    alGetEffectfv = (LPALGETEFFECTFV)alGetProcAddress("alGetEffectfv");
    alGenFilters = (LPALGENFILTERS)alGetProcAddress("alGenFilters");
    alDeleteFilters = (LPALDELETEFILTERS)alGetProcAddress("alDeleteFilters");
    alIsFilter = (LPALISFILTER)alGetProcAddress("alIsFilter");
    alFilteri = (LPALFILTERI)alGetProcAddress("alFilteri");
    alFilteriv = (LPALFILTERIV)alGetProcAddress("alFilteriv");
    alFilterf = (LPALFILTERF)alGetProcAddress("alFilterf");
    alFilterfv = (LPALFILTERFV)alGetProcAddress("alFilterfv");
    alGetFilteri = (LPALGETFILTERI)alGetProcAddress("alGetFilteri");
    alGetFilteriv = (LPALGETFILTERIV)alGetProcAddress("alGetFilteriv");
    alGetFilterf = (LPALGETFILTERF)alGetProcAddress("alGetFilterf");
    alGetFilterfv = (LPALGETFILTERFV)alGetProcAddress("alGetFilterfv");
    alGenAuxiliaryEffectSlots = (LPALGENAUXILIARYEFFECTSLOTS)alGetProcAddress("alGenAuxiliaryEffectSlots");
    alDeleteAuxiliaryEffectSlots = (LPALDELETEAUXILIARYEFFECTSLOTS)alGetProcAddress("alDeleteAuxiliaryEffectSlots");
    alIsAuxiliaryEffectSlot = (LPALISAUXILIARYEFFECTSLOT)alGetProcAddress("alIsAuxiliaryEffectSlot");
    alAuxiliaryEffectSloti = (LPALAUXILIARYEFFECTSLOTI)alGetProcAddress("alAuxiliaryEffectSloti");
    alAuxiliaryEffectSlotiv = (LPALAUXILIARYEFFECTSLOTIV)alGetProcAddress("alAuxiliaryEffectSlotiv");
    alAuxiliaryEffectSlotf = (LPALAUXILIARYEFFECTSLOTF)alGetProcAddress("alAuxiliaryEffectSlotf");
    alAuxiliaryEffectSlotfv = (LPALAUXILIARYEFFECTSLOTFV)alGetProcAddress("alAuxiliaryEffectSlotfv");
    alGetAuxiliaryEffectSloti = (LPALGETAUXILIARYEFFECTSLOTI)alGetProcAddress("alGetAuxiliaryEffectSloti");
    alGetAuxiliaryEffectSlotiv = (LPALGETAUXILIARYEFFECTSLOTIV)alGetProcAddress("alGetAuxiliaryEffectSlotiv");
    alGetAuxiliaryEffectSlotf = (LPALGETAUXILIARYEFFECTSLOTF)alGetProcAddress("alGetAuxiliaryEffectSlotf");
    alGetAuxiliaryEffectSlotfv = (LPALGETAUXILIARYEFFECTSLOTFV)alGetProcAddress("alGetAuxiliaryEffectSlotfv");
}