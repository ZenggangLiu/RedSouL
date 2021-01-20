//
//  TODO_Code.hpp
//  Core
//
//  Created by Zenggang Liu on 20/01/21.
//  Copyright © 2021 Zenggang Liu. All rights reserved.
//

#ifndef TODO_Code_h
#define TODO_Code_h

// Checks if the given integer intVal has just ONE bit set: 0001000 NOT 1001000
/// Returns TRUE when intVal just has ONE bit set
#ifndef ZGESINGLEBITSET
#define ZGESINGLEBITSET(intVal) \
    !((uint32)intVal & ((uint32)intVal -1))
#endif // #ifndef ZGESINGLEBITSET


/// Gets value aligned according to the expecting alignment
///
/// ZGEALIGNUP(1, 4)         --> 4
/// ZGEALIGNUP(4, 4)         --> 4
/// ZGEALIGNUP(5, 4)         --> 8
/// ZGEALIGNUP(0xFFFF001, 4) --> 0xFFFF0004
/// ZGEALIGNUP(0xFFFF002, 4) --> 0xFFFF0004
/// ZGEALIGNUP(0xFFFF003, 4) --> 0xFFFF0004
/// ZGEALIGNUP(0xFFFF004, 4) --> 0xFFFF0004
/// ZGEALIGNUP(0xFFFF005, 4) --> 0xFFFF0008
#ifndef ZGEALIGNUP
#define ZGEALIGNUP(value, alignment) \
    ((value + alignment - 1) & (~(alignment - 1)))
#endif // #ifndef ZGEALIGNUP
#endif /* TODO_Code_h */
