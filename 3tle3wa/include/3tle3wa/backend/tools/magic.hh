#pragma once

/**
 * magic number
*/

struct MaS {
    int magic_number;
    int shift_amount;
};

static constexpr MaS Magika(int d) {
    int p{};
    unsigned ad{}, anc{}, delta{}, q1{}, r1{}, q2{}, r2{}, t{};

    const unsigned two31 = 0x8000'0000;
    MaS magic{.magic_number = 0, .shift_amount = 0};

    ad = d > 0 ? d : -d;
    t = two31 + ((unsigned)d >> 31);
    anc = t - 1 - t % ad;
    p = 31;
    q1 = two31 / anc;
    r1 = two31 - q1 * anc;

    q2 = two31 / ad;
    r2 = two31 - q2 * ad;

    do {
        p = p + 1;
        q1 = 2 * q1;
        r1 = 2 * r1;
        if (r1 >= anc) {
            q1 = q1 + 1;
            r1 = r1 - anc;
        }
        q2 = 2 * q2;
        r2 = 2 * r2;
        if (r2 >= ad) {
            q2 = q2 + 1;
            r2 = r2 - ad;
        }
        delta = ad - r2;
    } while (q1 < delta or (q1 == delta and r1 == 0));

    magic.magic_number = q2 + 1;
    if (d < 0) {
        magic.magic_number = -magic.magic_number;
    }

    magic.shift_amount = p - 32;
    return magic;
}