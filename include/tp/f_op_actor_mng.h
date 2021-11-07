#pragma once

#include "defines.h"

namespace tp::f_op_actor_mng
{
    extern "C"
    {
        s32 createItemForPresentDemo( const float pos[3],
                                      s32 item,
                                      u8 unk3,
                                      s32 unk4,
                                      s32 unk5,
                                      const float unk6[3],
                                      const float unk7[3] );
        s32 createItemForTrBoxDemo( const float pos[3],
                                    s32 item,
                                    s32 unk3,
                                    s32 unk4,
                                    const float unk5[3],
                                    const float unk6[3] );
        s32 createDemoItem( const float pos[3],
                            s32 item,
                            s32 unk3,
                            const float unk4[3],
                            s32 unk5,
                            const float unk6[3],
                            u8 unk7 );
        s32 createItemForBoss( const float pos[3],
                               s32 item,
                               s32 unk3,
                               const float unk4[3],
                               const float unk5[3],
                               float unk6,
                               float unk7,
                               s32 unk8 );
        s32 createItemForMidBoss( const float pos[3],
                                  s32 item,
                                  s32 unk3,
                                  const float unk4[3],
                                  const float unk5[3],
                                  s32 unk6,
                                  s32 unk7 );
        s32 createItemForDirectGet( const float pos[3],
                                    s32 item,
                                    s32 unk3,
                                    const float unk4[3],
                                    const float unk5[3],
                                    float unk6,
                                    float unk7 );
        s32 createItemForSimpleDemo( const float pos[3],
                                     s32 item,
                                     s32 unk3,
                                     const float unk4[3],
                                     const float unk5[3],
                                     float unk6,
                                     float unk7 );
    }
}     // namespace tp::f_op_actor_mng