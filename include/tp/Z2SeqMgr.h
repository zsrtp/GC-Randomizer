#pragma once

struct Z2SeqMgr;

namespace tp::Z2AudioLib::SeqMgr
{
    extern "C"
    {
        void startBattleBgm( Z2SeqMgr* seqMgr, bool param_1 );
        void subBgmStart( Z2SeqMgr* seqMgr, u32 id );
    }
}     // namespace tp::Z2AudioLib::SeqMgr