extern "C" {
#include <soundpipe.h>
#include <sporth.h>
}

#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "MVerb.h"


extern "C" {
typedef struct {
    MVerb<SPFLOAT> mverb;
} sp_mverb;

MVerb<SPFLOAT> g_mverb;

static int sporth_mverb(plumber_data *pd, sporth_stack *stack, void **ud)
{
    sp_mverb *mv;
    SPFLOAT outL, outR;
    SPFLOAT inL, inR;
    SPFLOAT *ins[] = {&inL, &inR};
    SPFLOAT *outs[]  = {&outL, &outR};
    switch(pd->mode) {
        case PLUMBER_CREATE:
            mv = (sp_mverb *)malloc(sizeof(sp_mverb));
            *ud = mv;
            if(sporth_check_args(stack, "ff") != SPORTH_OK) {
                fprintf(stderr,"Not enough arguments for gain\n");
                stack->error++;
                return PLUMBER_NOTOK;
            }

            sporth_stack_pop_float(stack);
            sporth_stack_pop_float(stack);
            sporth_stack_push_float(stack, 0.0);
            sporth_stack_push_float(stack, 0.0);
            
            break;
        case PLUMBER_INIT:
            mv = (sp_mverb *)*ud;
            g_mverb.setSampleRate(pd->sp->sr);
            g_mverb.reset();

            g_mverb.setParameter(MVerb<SPFLOAT>::DAMPINGFREQ, 1.);
            g_mverb.setParameter(MVerb<SPFLOAT>::DENSITY, 0.9);
            g_mverb.setParameter(MVerb<SPFLOAT>::BANDWIDTHFREQ, 1.);
            g_mverb.setParameter(MVerb<SPFLOAT>::DECAY, 0.94);
            g_mverb.setParameter(MVerb<SPFLOAT>::PREDELAY, 0.);
            g_mverb.setParameter(MVerb<SPFLOAT>::SIZE, 1.);
            g_mverb.setParameter(MVerb<SPFLOAT>::GAIN, 1.);
            g_mverb.setParameter(MVerb<SPFLOAT>::MIX, 0.35);
            g_mverb.setParameter(MVerb<SPFLOAT>::EARLYMIX, 0.75);

            sporth_stack_pop_float(stack);
            sporth_stack_pop_float(stack);
            sporth_stack_push_float(stack, 0.0);
            sporth_stack_push_float(stack, 0.0);
            break;
        case PLUMBER_COMPUTE:
            mv = (sp_mverb *)*ud;
            inR = sporth_stack_pop_float(stack);
            inL = sporth_stack_pop_float(stack);
            g_mverb.process(ins, outs, 1);
            sporth_stack_push_float(stack, outL);
            sporth_stack_push_float(stack, outR);
            break;
        case PLUMBER_DESTROY:
            mv = (sp_mverb *)*ud;
            free(mv);
            break;
    }

    return PLUMBER_OK;
}


plumber_dyn_func sporth_return_ugen()
{
    return sporth_mverb;
}

}
