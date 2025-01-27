#include <pch.h>

#include <audio/backend/ALExt.h>

LPALCGETINTEGER64VSOFT ALExt::alcGetInteger64vSOFT;
LPALGENEFFECTS ALExt::alGenEffects;
LPALDELETEEFFECTS ALExt::alDeleteEffects;
LPALISEFFECT ALExt::alIsEffect;
LPALEFFECTI ALExt::alEffecti;
LPALEFFECTIV ALExt::alEffectiv;
LPALEFFECTF ALExt::alEffectf;
LPALEFFECTFV ALExt::alEffectfv;
LPALGETEFFECTI ALExt::alGetEffecti;
LPALGETEFFECTIV ALExt::alGetEffectiv;
LPALGETEFFECTF ALExt::alGetEffectf;
LPALGETEFFECTFV ALExt::alGetEffectfv;
LPALGENFILTERS ALExt::alGenFilters;
LPALDELETEFILTERS ALExt::alDeleteFilters;
LPALISFILTER ALExt::alIsFilter;
LPALFILTERI ALExt::alFilteri;
LPALFILTERIV ALExt::alFilteriv;
LPALFILTERF ALExt::alFilterf;
LPALFILTERFV ALExt::alFilterfv;
LPALGETFILTERI ALExt::alGetFilteri;
LPALGETFILTERIV ALExt::alGetFilteriv;
LPALGETFILTERF ALExt::alGetFilterf;
LPALGETFILTERFV ALExt::alGetFilterfv;
LPALGENAUXILIARYEFFECTSLOTS ALExt::alGenAuxiliaryEffectSlots;
LPALDELETEAUXILIARYEFFECTSLOTS ALExt::alDeleteAuxiliaryEffectSlots;
LPALISAUXILIARYEFFECTSLOT ALExt::alIsAuxiliaryEffectSlot;
LPALAUXILIARYEFFECTSLOTI ALExt::alAuxiliaryEffectSloti;
LPALAUXILIARYEFFECTSLOTIV ALExt::alAuxiliaryEffectSlotiv;
LPALAUXILIARYEFFECTSLOTF ALExt::alAuxiliaryEffectSlotf;
LPALAUXILIARYEFFECTSLOTFV ALExt::alAuxiliaryEffectSlotfv;
LPALGETAUXILIARYEFFECTSLOTI ALExt::alGetAuxiliaryEffectSloti;
LPALGETAUXILIARYEFFECTSLOTIV ALExt::alGetAuxiliaryEffectSlotiv;
LPALGETAUXILIARYEFFECTSLOTF ALExt::alGetAuxiliaryEffectSlotf;
LPALGETAUXILIARYEFFECTSLOTFV ALExt::alGetAuxiliaryEffectSlotfv;
LPALCEVENTISSUPPORTEDSOFT ALExt::alcEventIsSupportedSOFT;
LPALCEVENTCONTROLSOFT ALExt::alcEventControlSOFT;
LPALCEVENTCALLBACKSOFT ALExt::alcEventCallbackSOFT;

void ALExt::initEFX() {
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

void ALExt::initDeviceClock(ALCdevice* device) {
    alcGetInteger64vSOFT = (LPALCGETINTEGER64VSOFT)alcGetProcAddress(device, "alcGetInteger64vSOFT");
}

void ALExt::initSysEvents(ALCdevice* device) {
    alcEventIsSupportedSOFT = (LPALCEVENTISSUPPORTEDSOFT)alcGetProcAddress(device, "alcEventIsSupportedSOFT");
    alcEventControlSOFT = (LPALCEVENTCONTROLSOFT)alcGetProcAddress(device, "alcEventControlSOFT");
    alcEventCallbackSOFT = (LPALCEVENTCALLBACKSOFT)alcGetProcAddress(device, "alcEventControlSOFT");
}

void ALExt::initAllEXT(ALCdevice* device) {
    initSysEvents(device);
    initDeviceClock(device);
    initEFX();
}