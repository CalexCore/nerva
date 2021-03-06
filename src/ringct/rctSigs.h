// Copyright (c) 2016, Monero Research Labs
//
// Author: Shen Noether <shen.noether@gmx.com>
//
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without modification, are
// permitted provided that the following conditions are met:
//
// 1. Redistributions of source code must retain the above copyright notice, this list of
//    conditions and the following disclaimer.
//
// 2. Redistributions in binary form must reproduce the above copyright notice, this list
//    of conditions and the following disclaimer in the documentation and/or other
//    materials provided with the distribution.
//
// 3. Neither the name of the copyright holder nor the names of its contributors may be
//    used to endorse or promote products derived from this software without specific
//    prior written permission.
//
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY
// EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
// MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL
// THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
// SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
// PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
// INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
// STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF
// THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

#pragma once

//#define DBG

#ifndef RCTSIGS_H
#define RCTSIGS_H

#include <cstddef>
#include <vector>
#include <tuple>

#include "crypto/generic-ops.h"

extern "C" {
#include "crypto/random.h"
#include "crypto/keccak.h"
}
#include "crypto/crypto.h"


#include "rctTypes.h"
#include "rctOps.h"

//Define this flag when debugging to get additional info on the console
#ifdef DBG
#define DP(x) dp(x)
#else
#define DP(x)
#endif

namespace hw {
    class device;
}


namespace rct {

    boroSig genBorromean(const key64 x, const key64 P1, const key64 P2, const bits indices);
    bool verifyBorromean(const boroSig &bb, const key64 P1, const key64 P2);

    mgSig MLSAG_Gen(const key &message, const keyM & pk, const keyV & xx, const multisig_kLRki *kLRki, key *mscout, const unsigned int index, size_t dsRows, hw::device &hwdev);
    bool MLSAG_Ver(const key &message, const keyM &pk, const mgSig &sig, size_t dsRows);

    rangeSig proveRange(key & C, key & mask, const xmr_amount & amount);
    bool verRange(const key & C, const rangeSig & as);

    mgSig proveRctMG(const ctkeyM & pubs, const ctkeyV & inSk, const keyV &outMasks, const ctkeyV & outPk, const multisig_kLRki *kLRki, key *mscout, unsigned int index, const key &txnFee, const key &message, hw::device &hwdev);
    mgSig proveRctMGSimple(const key & message, const ctkeyV & pubs, const ctkey & inSk, const key &a , const key &Cout, const multisig_kLRki *kLRki, key *mscout, unsigned int index, hw::device &hwdev);
    bool verRctMG(const mgSig &mg, const ctkeyM & pubs, const ctkeyV & outPk, const key &txnFee, const key &message);
    bool verRctMGSimple(const key &message, const mgSig &mg, const ctkeyV & pubs, const key & C, bool v2);
	bool verRctMGSimple_v1(const key &message, const mgSig &mg, const ctkeyV & pubs, const key & C);
	bool verRctMGSimple_v2(const key &message, const mgSig &mg, const ctkeyV & pubs, const key & C);    

    void getKeyFromBlockchain(ctkey & a, size_t reference_index);
    std::tuple<ctkeyM, xmr_amount> populateFromBlockchain(ctkeyV inPk, int mixin);

    rctSig genRct(const key &message, const ctkeyV & inSk, const keyV & destinations, const std::vector<xmr_amount> & amounts, const ctkeyM &mixRing, const keyV &amount_keys, const multisig_kLRki *kLRki, multisig_out *msout, unsigned int index, ctkeyV &outSk, const RCTConfig &rct_config, hw::device &hwdev);
    rctSig genRct(const key &message, const ctkeyV & inSk, const ctkeyV  & inPk, const keyV & destinations, const std::vector<xmr_amount> & amounts, const keyV &amount_keys, const multisig_kLRki *kLRki, multisig_out *msout, const int mixin, const RCTConfig &rct_config, hw::device &hwdev);

    //v1/v2 switch
    rctSig genRctSimple(const key & message, const ctkeyV & inSk, const keyV & destinations, const std::vector<xmr_amount> & inamounts, const std::vector<xmr_amount> & outamounts, 
        xmr_amount txnFee, const ctkeyM & mixRing, const keyV &amount_keys, const std::vector<multisig_kLRki> *kLRki, multisig_out *msout, 
        const std::vector<unsigned int> & index, ctkeyV &outSk, const RCTConfig &rct_config, hw::device &hwdev);
	rctSig genRctSimple_v1(const key & message, const ctkeyV & inSk, const keyV & destinations, const std::vector<xmr_amount> & inamounts, const std::vector<xmr_amount> & outamounts, 
        xmr_amount txnFee, const ctkeyM & mixRing, const keyV &amount_keys, const std::vector<multisig_kLRki> *kLRki, multisig_out *msout, 
        const std::vector<unsigned int> & index, ctkeyV &outSk, const RCTConfig &rct_config, hw::device &hwdev);
    rctSig genRctSimple_v2(const key & message, const ctkeyV & inSk, const keyV & destinations, const std::vector<xmr_amount> & inamounts, const std::vector<xmr_amount> & outamounts, 
        xmr_amount txnFee, const ctkeyM & mixRing, const keyV &amount_keys, const std::vector<multisig_kLRki> *kLRki, multisig_out *msout, 
        const std::vector<unsigned int> & index, ctkeyV &outSk, const RCTConfig &rct_config, hw::device &hwdev);

	bool verRct(const rctSig & rv, bool semantics);

    bool verRctSimple(const rctSig & rv, bool semantics, bool v2);
    bool verRctSimple_v1(const rctSig & rv, bool semantics);

	bool verRctSemanticsSimple(const rctSig & rv);
    bool verRctSemanticsSimple(const std::vector<const rctSig*> & rv);
    bool verRctNonSemanticsSimple(const rctSig & rv);

    xmr_amount decodeRct(const rctSig & rv, const key & sk, unsigned int i, key & mask, hw::device &hwdev);
    xmr_amount decodeRct(const rctSig & rv, const key & sk, unsigned int i, hw::device &hwdev);
    xmr_amount decodeRctSimple(const rctSig & rv, const key & sk, unsigned int i, key & mask, hw::device &hwdev);
    xmr_amount decodeRctSimple(const rctSig & rv, const key & sk, unsigned int i, hw::device &hwdev);

    key get_pre_mlsag_hash(const rctSig &rv, hw::device &hwdev);
    bool signMultisig(rctSig &rv, const std::vector<unsigned int> &indices, const keyV &k, const multisig_out &msout, const key &secret_key);
}
#endif  /* RCTSIGS_H */

