#ifndef AUDIO_FLITE_H
#define AUDIO_FLITE_H


extern "C" {
#include <flite/include/flite.h>
#include <flite/include/flite_version.h>
cst_voice *register_cmu_us_awb(const char *voxdir);
cst_voice *register_cmu_us_rms(const char *voxdir);
}

#endif
