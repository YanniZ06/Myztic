#include <pch.h>

#include <audio/backend/ALExt.h>

LPALCGETINTEGER64VSOFT Myztic::ALExt::alcGetInteger64vSOFT;
LPALGENEFFECTS Myztic::ALExt::alGenEffects;
LPALDELETEEFFECTS Myztic::ALExt::alDeleteEffects;
LPALISEFFECT Myztic::ALExt::alIsEffect;
LPALEFFECTI Myztic::ALExt::alEffecti;
LPALEFFECTIV Myztic::ALExt::alEffectiv;
LPALEFFECTF Myztic::ALExt::alEffectf;
LPALEFFECTFV Myztic::ALExt::alEffectfv;
LPALGETEFFECTI Myztic::ALExt::alGetEffecti;
LPALGETEFFECTIV Myztic::ALExt::alGetEffectiv;
LPALGETEFFECTF Myztic::ALExt::alGetEffectf;
LPALGETEFFECTFV Myztic::ALExt::alGetEffectfv;
LPALGENFILTERS Myztic::ALExt::alGenFilters;
LPALDELETEFILTERS Myztic::ALExt::alDeleteFilters;
LPALISFILTER Myztic::ALExt::alIsFilter;
LPALFILTERI Myztic::ALExt::alFilteri;
LPALFILTERIV Myztic::ALExt::alFilteriv;
LPALFILTERF Myztic::ALExt::alFilterf;
LPALFILTERFV Myztic::ALExt::alFilterfv;
LPALGETFILTERI Myztic::ALExt::alGetFilteri;
LPALGETFILTERIV Myztic::ALExt::alGetFilteriv;
LPALGETFILTERF Myztic::ALExt::alGetFilterf;
LPALGETFILTERFV Myztic::ALExt::alGetFilterfv;
LPALGENAUXILIARYEFFECTSLOTS Myztic::ALExt::alGenAuxiliaryEffectSlots;
LPALDELETEAUXILIARYEFFECTSLOTS Myztic::ALExt::alDeleteAuxiliaryEffectSlots;
LPALISAUXILIARYEFFECTSLOT Myztic::ALExt::alIsAuxiliaryEffectSlot;
LPALAUXILIARYEFFECTSLOTI Myztic::ALExt::alAuxiliaryEffectSloti;
LPALAUXILIARYEFFECTSLOTIV Myztic::ALExt::alAuxiliaryEffectSlotiv;
LPALAUXILIARYEFFECTSLOTF Myztic::ALExt::alAuxiliaryEffectSlotf;
LPALAUXILIARYEFFECTSLOTFV Myztic::ALExt::alAuxiliaryEffectSlotfv;
LPALGETAUXILIARYEFFECTSLOTI Myztic::ALExt::alGetAuxiliaryEffectSloti;
LPALGETAUXILIARYEFFECTSLOTIV Myztic::ALExt::alGetAuxiliaryEffectSlotiv;
LPALGETAUXILIARYEFFECTSLOTF Myztic::ALExt::alGetAuxiliaryEffectSlotf;
LPALGETAUXILIARYEFFECTSLOTFV Myztic::ALExt::alGetAuxiliaryEffectSlotfv;
LPALCEVENTISSUPPORTEDSOFT Myztic::ALExt::alcEventIsSupportedSOFT;
LPALCEVENTCONTROLSOFT Myztic::ALExt::alcEventControlSOFT;
LPALCEVENTCALLBACKSOFT Myztic::ALExt::alcEventCallbackSOFT;

void Myztic::ALExt::initEFX() {
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

void Myztic::ALExt::initDeviceClock(ALCdevice* device) {
    alcGetInteger64vSOFT = (LPALCGETINTEGER64VSOFT)alcGetProcAddress(device, "alcGetInteger64vSOFT");
}

void Myztic::ALExt::initSysEvents(ALCdevice* device) {
    alcEventIsSupportedSOFT = (LPALCEVENTISSUPPORTEDSOFT)alcGetProcAddress(NULL, "alcEventIsSupportedSOFT");
    alcEventControlSOFT = (LPALCEVENTCONTROLSOFT)alcGetProcAddress(NULL, "alcEventControlSOFT");
    alcEventCallbackSOFT = (LPALCEVENTCALLBACKSOFT)alcGetProcAddress(NULL, "alcEventCallbackSOFT");
}

void Myztic::ALExt::initAllEXT(ALCdevice* device) {
    initSysEvents(device);
    initDeviceClock(device);
    initEFX();
}