
#define S_FUNCTION_NAME  control_Hinf
#define S_FUNCTION_LEVEL 2


#include "simstruc.h"
#include "mex.h"
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>
#include <stddef.h>
#include <time.h>


#define dt        		*mxGetPr(ssGetSFcnParam(S,0))		// Software sample time, as an input parameter
#define time			ssGetT(S)							// Current simulation time		

#define N    4
        
static int init = 0;

static double integrator = 0;
static double gain[2] = {0.7,0.1};
static double Yk;
static double SampleTime = 0.01; 

static double ControlHinf(double error);


static void init_control();

static void mdlInitializeSizes(SimStruct *S)
{
    ssSetNumSFcnParams(S, 1);
    if (ssGetNumSFcnParams(S) != ssGetSFcnParamsCount(S)) return;
    
    if (!ssSetNumInputPorts(S, 1)) return;
    ssSetInputPortWidth(S, 0, DYNAMICALLY_SIZED);
    ssSetInputPortDirectFeedThrough(S, 0, 1);
    
    if (!ssSetNumOutputPorts(S, 1)) return;
    ssSetOutputPortWidth(S, 0, DYNAMICALLY_SIZED);

    ssSetNumSampleTimes(S, 1);
}

static void mdlInitializeSampleTimes(SimStruct *S)
{
    ssSetSampleTime(S, 0, dt);
    ssSetOffsetTime(S, 0, 0.0);
}

#define MDL_START  /* Change to #undef to remove function */
#if defined(MDL_START) 

  static void mdlStart(SimStruct *S)
  {
    init_control();
  }
#endif

static void mdlOutputs(SimStruct *S, int_T tid)
{
    InputRealPtrsType In = ssGetInputPortRealSignalPtrs(S,0);
    real_T *Y = ssGetOutputPortRealSignal(S,0);
    *Y = ControlHinf(*In[0]);
}

static void mdlTerminate(SimStruct *S)
{
}

#ifdef  MATLAB_MEX_FILE    
#include "simulink.c"      
#else
#include "cg_sfun.h"       
#endif

static double ControlHinf(double error)
{
    
integrator += error*SampleTime;

Yk = gain[0]*error + gain[1]*integrator;

    return Yk;
}

static void init_control()
{

integrator = 0; 

}