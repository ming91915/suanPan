#ifndef SUANPAN_H
#define SUANPAN_H

#if defined(_DEBUG) || defined(DEBUG)
#define SUANPAN_DEBUG
#else
#define ARMA_NO_DEBUG
#endif

#ifndef CT_NODE
#define CT_NODE 1
#endif
#ifndef CT_ELEMENT
#define CT_ELEMENT 2
#endif
#ifndef CT_MATERIAL
#define CT_MATERIAL 3
#endif
#ifndef CT_SOLVER
#define CT_SOLVER 4
#endif
#ifndef CT_ODE
#define CT_ODE 5
#endif
#ifndef CT_ODESOLVER
#define CT_ODESOLVER 6
#endif
#ifndef CT_LOAD
#define CT_LOAD 7
#endif
#ifndef CT_AMPLITUDE
#define CT_AMPLITUDE 8
#endif
#ifndef CT_STEP
#define CT_STEP 9
#endif
#ifndef CT_ABSDISP
#define CT_ABSDISP 10
#endif
#ifndef CT_CONVERGENCE
#define CT_CONVERGENCE 11
#endif
#ifndef CT_DOMAIN
#define CT_DOMAIN 12
#endif
#ifndef CT_USER_FUNCTION
#define CT_USER_FUNCTION 13
#endif
#ifndef CT_STATIC
#define CT_STATIC 14
#endif
#ifndef CT_DYNAMIC
#define CT_DYNAMIC 15
#endif
#ifndef CT_FREQUENCE
#define CT_FREQUENCE 16
#endif
#ifndef CT_BC
#define CT_BC 17
#endif
#ifndef CT_CONSTRAINT
#define CT_CONSTRAINT 18
#endif
#ifndef CT_TIE
#define CT_TIE 19
#endif
#ifndef CT_RECORDER
#define CT_RECORDER 20
#endif
#ifndef CT_SECTION
#define CT_SECTION 21
#endif
#ifndef CT_REORDERER
#define CT_REORDERER 22
#endif
#ifndef CT_RELDISP
#define CT_RELDISP 23
#endif
#ifndef CT_NEWTON
#define CT_NEWTON 24
#endif
#ifndef CT_RELERROR
#define CT_RELERROR 25
#endif
#ifndef CT_ABSERROR
#define CT_ABSERROR 26
#endif
#ifndef CT_RAMP
#define CT_RAMP 27
#endif
#ifndef CT_TABULAR
#define CT_TABULAR 28
#endif
#ifndef CT_BS23
#define CT_BS23 29
#endif
#ifndef CT_RK23
#define CT_RK23 30
#endif
#ifndef CT_RK45
#define CT_RK45 31
#endif
#ifndef CT_DP45
#define CT_DP45 32
#endif
#ifndef CT_NEWMARK
#define CT_NEWMARK 33
#endif
#ifndef CT_ANALYSIS
#define CT_ANALYSIS 34
#endif
#ifndef CT_MPC
#define CT_MPC 35
#endif
#ifndef CT_CLOAD
#define CT_CLOAD 36
#endif
#ifndef CT_ABSINCREDISP
#define CT_ABSINCREDISP 37
#endif
#ifndef CT_BFGS
#define CT_BFGS 38
#endif
#ifndef CT_RELINCREDISP
#define CT_RELINCREDISP 39
#endif
#ifndef CT_CENTRALDIFFERENCE
#define CT_CENTRALDIFFERENCE 40
#endif
#ifndef CT_ABSRESIDUAL
#define CT_ABSRESIDUAL 41
#endif

#ifndef MT_ELASTIC1D
#define MT_ELASTIC1D 1000
#endif
#ifndef MT_ELASTIC2D
#define MT_ELASTIC2D 1001
#endif
#ifndef MT_ELASTIC3D
#define MT_ELASTIC3D 1002
#endif
#ifndef MT_BILINEAR1D
#define MT_BILINEAR1D 1003
#endif
#ifndef MT_BILINEAR3D
#define MT_BILINEAR3D 1004
#endif

#ifndef ET_TRUSS2D
#define ET_TRUSS2D 2000
#endif
#ifndef ET_CP4
#define ET_CP4 2001
#endif
#ifndef ET_PS
#define ET_PS 2002
#endif
#ifndef ET_QE2
#define ET_QE2 2003
#endif
#ifndef ET_GQ12
#define ET_GQ12 2004
#endif
#ifndef ET_CP3
#define ET_CP3 2005
#endif

#if defined(__cplusplus)
#endif

#ifdef _MSC_VER
#endif

#if defined(__x86_64__) || defined(_WIN64)
#define __ARCH__ 64
#else
#define __ARCH__ 32
#endif

#ifdef __GNUC__
#endif

#include <armadillo>
#include <array>
#include <memory>
#include <unordered_map>
#include <unordered_set>
using namespace arma;
using namespace std;

#endif
