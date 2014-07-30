/* Generated automatically from make_voice_list */

#include "flite.h"

cst_voice *register_cmu_us_awb(const char *voxdir);

cst_val *flite_set_voice_list(void)
{
   flite_voice_list = cons_val(voice_val(register_cmu_us_awb(NULL)),flite_voice_list);
   flite_voice_list = val_reverse(flite_voice_list);
   return flite_voice_list;
}

