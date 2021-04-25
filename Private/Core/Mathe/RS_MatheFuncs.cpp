// Precompiled header
#include "PrecompiledH.hpp"
// System headers
// Lib headers
#include "Core/Mathe/RS_FloatPointFuncs.hpp"
// Self
#include "Core/Mathe/RS_MatheFuncs.hpp"


namespace Core
{

    /* 此次的函数没有进行测试
    UInt32
    Mathe::absolute (
        const SInt32 val)
    {
        return val > 0 ? val : -val;
    }


    Real32
    Mathe::absolute (
        const Real32 val)
    {
#if 1
        union FNNumber
        {
            Real32 fVal;
            SInt32 iVal;
        } _fni_val;

        _fni_val.fVal  = val;
        _fni_val.iVal &= 0x7FFFFFFF;

        return _fni_val.fVal;
#else
        return std::abs(val);
#endif
    }


    Real64
    Mathe::absolute (
        const Real64 val)
    {
#if 1
        union FNNumber
        {
            Real64 fVal;
            SInt64 iVal;
        } _fni_val;

        _fni_val.fVal  = val;
        _fni_val.iVal &= 0x7FFFFFFFFFFFFFFFUL;

        return _fni_val.fVal;
#else
        return std::abs(val);
#endif
    }


    Real32
    Mathe::arccosine (
        const Real32 cos_val)
    {
        // 参考：
        // - http://developer.download.nvidia.com/cg/acos.html
        // - Doc/APIDocs/Cg-3.1_Reference_Manual.pdf
        // - Handbook of Mathematical Functions: M. Abramowitz and I.A. Stegun, Ed.
        //
        const Real32 _neg_flag     = Real32(cos_val < 0);
        const Real32 _positive_val = absolute(cos_val < -1 ? -1 : (cos_val > 1 ? 1 : cos_val));
        Real32 res  = -0.0187293f;
               res *= _positive_val;
               res += 0.0742610f;
               res *= _positive_val;
               res -= 0.2121144f;
               res *= _positive_val;
               res += 1.5707288f;
               res *= squaredRoot(1 - _positive_val);
               res -= 2 * _neg_flag * res;

        return _neg_flag * (Real32)ONE_PI + res;
    }


    Real64
    Mathe::arccosine (
        const Real64 cos_val)
    {
        return std::acos(cos_val);
    }


    Real32
    Mathe::arcsine (
        const Real32 sin_val)
    {
        // 参考：
        // - http://developer.download.nvidia.com/cg/asin.html
        // - Doc/APIDocs/Cg-3.1_Reference_Manual.pdf
        // - Handbook of Mathematical Functions: M. Abramowitz and I.A. Stegun, Ed.
        //
        const Real32 _neg_flag     = Real32(sin_val < 0);
        const Real32 _positive_val = absolute(sin_val < -1 ? -1 : (sin_val > 1 ? 1 : sin_val));
        Real32 res  = -0.0187293f;
               res *= _positive_val;
               res += 0.0742610f;
               res *= _positive_val;
               res -= 0.2121144f;
               res *= _positive_val;
               res += 1.5707288f;
               res = (Real32)HALF_PI - squaredRoot(1 - _positive_val) * res;

        return res - 2 * _neg_flag * res;
    }


    Real64
    Mathe::arcsine (
        const Real64 sin_val)
    {
        return std::asin(sin_val);
    }


    Real32
    Mathe::average (
        const Real32 curent_avg,
        const Real32 new_sample,
        const UInt32 total_samples)
    {
        RUNTIME_ASSERT(total_samples >= 1, "Wrong total sampling count");

        // 参考：http://jvminside.blogspot.co.nz/2010/01/incremental-average-calculation.html#more
        // A_n+1   = A_n   +       (v_n+1 - A_n)/(n+1)
        // A_{n+1} = A_{n} + \frac{v_{n+1}-A_{n}}{n+1}
        return curent_avg + (new_sample - curent_avg) / total_samples;
    }


    Real64
    Mathe::average (
        const Real64 curent_avg,
        const Real64 new_sample,
        const UInt32 total_samples)
    {
        RUNTIME_ASSERT(total_samples >= 1, "Wrong total sampling count");

        // 参考：http://jvminside.blogspot.co.nz/2010/01/incremental-average-calculation.html#more
        // A_n+1   = A_n   +       (v_n+1 - A_n)/(n+1)
        // A_{n+1} = A_{n} + \frac{v_{n+1}-A_{n}}{n+1}
        return curent_avg + (new_sample - curent_avg) / total_samples;
    }


    Real32
    Mathe::cosine (
        const Real32 angle_in_rads)
    {
        // 我们知道：cos(x) = sine(x + PI*0.5)
        // 所以此处计算x
        Real32 _x_in_rads = angle_in_rads + (Real32)HALF_PI;
        if (_x_in_rads > TWO_PI)
        {
            _x_in_rads -= TWO_PI;
        }

        return sine(_x_in_rads);
    }


    Real64
    Mathe::cosine (
        const Real64 angle_in_rads)
    {
        return std::cos(angle_in_rads);
    }


    Real32
    Mathe::fraction (
        const Real32 val)
    {
        return val - std::floor(val);
    }


    Real64
    Mathe::fraction (
        const Real64 val)
    {
        return val - std::floor(val);
    }



    Real32
    Mathe::reciprocal (
        const Real32 val)
    {
        return FloatPoint::equals(val, 0) ? 0 : 1/val;
    }


    Real64
    Mathe::reciprocal (
        const Real64 val)
    {
        return FloatPoint::equals(val, 0) ? 0 : 1/val;
    }


    Real32
    Mathe::sine (
        const Real32 angle_in_rads)
    {
        // 参考：
        // http://forum.devmaster.net/t/fast-and-accurate-sine-cosine/9648
        // 我们使用如下函数来近似Sine
        // sine(x) = Q * (4/pi * x - 4/pi^2 * x^2) + P * (4/pi * x - 4/pi^2 * x^2)^2
        //         = Q * A + P * A * A
        // 此处： Q = 0.775, P = 0.225
        //
        // 输入角度的定义区间：[0, 2PI]
        // 将此角度转换到[-PI, +PI]，因为随后的逻辑使用此区间的角度
        const Real32 _angle_used = (angle_in_rads > ONE_PI) ?
                                   (angle_in_rads - TWO_PI) : angle_in_rads;

        // 计算：4/PI，4/(PI*PI)
        const Real32 _4_pi   =  4 / (Real32)ONE_PI;
        const Real32 _4_pipi = -4 / (Real32)SQUARED_PI;

        // 计算A的数值：(4/pi * x - 4/pi^2 * x^2)
        // NOTE：对于负角度，absolute()返回正角
        Real32 _curve_val = _4_pi * _angle_used + _4_pipi * _angle_used * absolute(_angle_used);

        // 我们使用P来计算：
        // 0.775*A + 0.225*A*A = (1 - 0.225)*A + 0.225*A*A
        // = A - 0.225*A + 0.225*A*A
        // = A + 0.225(A*A - A)
        // = A + P*(A*A - A)
        //  const Real32 _q_val = 0.775f;
        const Real32 _p_val = 0.225f;

        // A + P*(A*A - A)
        _curve_val = _curve_val + _p_val * (_curve_val * absolute(_curve_val) - _curve_val);

        return _curve_val;
    }


    Real64
    Mathe::sine (
        const Real64 angle_in_rads)
    {
        return std::sin(angle_in_rads);
    }


    Real32
    Mathe::squaredRoot (
        const Real32 val)
    {
        // 参考：
        // Babylonian算法 + IEEE 32位浮点数的操控
        // 同std::sqrt()相同精度，但是快34%
        // http://www.codeproject.com/Articles/69941/Best-Square-Root-Method-Algorithm-Function-Precisi
        //
        union FNNumber
        {
            Real32 fval;
            SInt32 ival;
        } _fni_val;

        _fni_val.fval = val;
        _fni_val.ival = (1 << 29) + (_fni_val.ival >> 1) - (1 << 22);

        // Two Babylonian Steps (simplified from:)
        // u.x = 0.5f * (u.x + x/u.x);
        // u.x = 0.5f * (u.x + x/u.x);
        _fni_val.fval = _fni_val.fval + val / _fni_val.fval;
        _fni_val.fval = 0.25f * _fni_val.fval + val / _fni_val.fval;

        return _fni_val.fval;

//        Real64
//        squaredRoot (
//            const Real64 val)
//        {
//            __asm
//            {
//                fld val
//                fsqrt
//            }
//        }
    }


    Real32
    Mathe::squaredRoot_fast (
        const Real32 val)
    {
        const Real32 half_val = 0.5f * val;

        union FNNumber
        {
            Real32 fval;
            SInt32 ival;
        } _fni_val;

        _fni_val.fval = val;
        // gives initial guess y0
        _fni_val.ival = 0x5F3759DF - (_fni_val.ival >> 1);

        // Newton step, repeating increases accuracy
        return val * _fni_val.fval * (1.5f - half_val * _fni_val.fval * _fni_val.fval);

//         Real64
//         __declspec (naked) __fastcall
//         squaredRoot_fast (
//             Real64 val)
//         {
//             _asm fld qword ptr [esp+4]
//             _asm fsqrt
//             _asm ret 8
//         }
    }


    Real64
    Mathe::squaredRoot (
        const Real64 val)
    {
        return std::sqrt(val);
    }*/

} // namespace Core
