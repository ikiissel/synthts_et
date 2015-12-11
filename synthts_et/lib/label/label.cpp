#include <vector>
#include <string>
#include "../etana/proof.h"
#include "util.h"

const CFSWString context_signs = L"^-+=@_"; //5
const CFSWString pau = L"pau";
const INTPTR no_v = -2;
const CFSWString xphone = L"x";

phrase_struct pause_struct() {

    phone_struct phs;
    phs.syl_p = no_v;
    phs.word_p = no_v;
    phs.phr_p = no_v;
    phs.utt_p = no_v;
    phs.p6 = -1;
    phs.p7 = -1;
    phs.a1 = 0;
    phs.a3 = 0;
    phs.b1 = -1;
    phs.b3 = -1;
    phs.b4 = -1;
    phs.b5 = -1;
    phs.b6 = -1;
    phs.b7 = -1;
    phs.c1 = 0;
    phs.c3 = 0;
    phs.lpos = L"0";
    phs.d2 = 0;
    phs.pos = L"0";
    phs.e2 = -1;
    phs.e3 = -1;
    phs.e4 = -1;
    phs.rpos = L"0";
    phs.f2 = 0;
    phs.g1 = 0;
    phs.g2 = 0;
    phs.h1 = -1;
    phs.h2 = -1;
    phs.h3 = 1;
    phs.h4 = 1;
    phs.i1 = 0;
    phs.i2 = 0;
    phs.j1 = 1;
    phs.j2 = 1;
    phs.j3 = 1;
    syl_struct ss;
    ss.phone_c = no_v;
    ss.word_p = no_v;
    ss.phr_p = no_v;
    ss.utt_p = no_v;
    ss.stress = 0;
    word_struct ws;
    ws.phone_c = no_v;
    ws.syl_c = no_v;
    ws.phr_p = no_v;
    ws.utt_p = no_v;
    phrase_struct ps;
    ps.phone_c = no_v;
    ps.syl_c = no_v;
    ps.word_c = no_v;
    ps.utt_p = no_v;

    phs.phone = pau;
    ss.phone_vector.AddItem(phs);
    ws.syl_vector.AddItem(ss);
    ps.word_vector.AddItem(ws);
    return ps;
}

CFSWString its(INTPTR i) {
    /* kui foneeme on lausungis rohkem, kui 1M - kukub */
    if (i < 0) return xphone;
    CFSWString s = empty_str;
    wchar_t buffer [8];
    swprintf(buffer, 8, L"%i", i);
    s += buffer;
    return s;
}

void add_pauses(utterance_struct &utt) {
    utterance_struct res;
    res.phone_c = utt.phone_c; // väga halb lahendus. Kõik utt väärtused tuleb ümber pumbata
    res.syl_c = utt.syl_c;
    res.word_c = utt.word_c;
    res.phra_c = utt.phra_c;
    res.phr_vector.AddItem(pause_struct());
    res.phone_c++;
    for (INTPTR i = 0; i < utt.phr_vector.GetSize(); i++) {
        res.phr_vector.AddItem(utt.phr_vector[i]);
        res.phr_vector.AddItem(pause_struct());
        res.phone_c++;
    }
    utt = res;
}

bool is_pho(CFSWString pho) {
    if (pho == pau)
        return false;
    else
        return true;
}

CFSWString gpos(CFSWString c) {
    CFSWString res = L"z";
    if (c.FindOneOf(L"DGIKXY") > -1) return res = L"d";
    else
        if (c.FindOneOf(L"ACHNOPSU") > -1) return res = L"a";
    else
        if (c.FindOneOf(L"V") > -1) return res = L"v";
    else
        if (c.FindOneOf(L"J") > -1) return res = L"j";
    else
        return res;
}

CFSArray<phone_struct> make_phone_array(utterance_struct &utt) {
    CFSArray<phone_struct> pa;
    for (INTPTR i_phr = 0; i_phr < utt.phr_vector.GetSize(); i_phr++) {
        for (INTPTR i_word = 0; i_word < utt.phr_vector[i_phr].word_vector.GetSize(); i_word++) {
            for (INTPTR i_syl = 0; i_syl < utt.phr_vector[i_phr].word_vector[i_word].syl_vector.GetSize(); i_syl++) {
                for (INTPTR i_pho = 0; i_pho < utt.phr_vector[i_phr].word_vector[i_word].syl_vector[i_syl].phone_vector.GetSize(); i_pho++) {
                    pa.AddItem(utt.phr_vector[i_phr].word_vector[i_word].syl_vector[i_syl].phone_vector[i_pho]);
                }
            }
        }
    }
    return pa;
}

CFSArray<CFSWString> make_label(CFSArray<phone_struct> pa) {
    CFSArray<CFSWString> res;
    CFSWString s = empty_str;

    INTPTR size = pa.GetSize();
    for (INTPTR i = 0; i < size; i++) {
        CFSWString pp, p, ph, c, cc;
        ph = pa[i].phone;

        if (i == 0) {
            p = xphone;
            pp = xphone;
            c = pa[i + 1].phone;
            cc = pa[i + 2].phone;
        } else
            if (i == 1) {
            pp = xphone;
            p = pa[i - 1].phone;
            c = pa[i + 1].phone;
            cc = pa[i + 2].phone;
        } else
            if (i == size - 2) {
            pp = pa[i - 2].phone;
            p = pa[i - 1].phone;
            c = pa[i + 1].phone;
            ;
            cc = xphone;
        } else
            if (i == size - 1) {
            pp = pa[i - 2].phone;
            p = pa[i - 1].phone;
            c = xphone;
            cc = xphone;
        } else {
            pp = pa[i - 2].phone;
            p = pa[i - 1].phone;
            c = pa[i + 1].phone;
            ;
            cc = pa[i + 2].phone;
        }

        s = pp + context_signs[0] + p + context_signs[1] + ph + context_signs[2] + c + context_signs[3] + cc + context_signs[4];
        s += its(pa[i].p6) + d + its(pa[i].p7);
        s += L"/A:";
        s += its(pa[i].a1) + L"_" + its(pa[i].a3);
        s += L"/B:";
        s += its(pa[i].b1) + d + its(pa[i].b3);
        s += L"@";
        s += its(pa[i].b4) + d + its(pa[i].b5);
        s += L"&";
        s += its(pa[i].b6) + d + its(pa[i].b7);
        s += L"/C:";
        s += its(pa[i].c1) + L"+" + its(pa[i].c3);
        s += L"/D:";
        s += pa[i].lpos + L"_" + its(pa[i].d2);
        s += L"/E:";
        s += pa[i].pos + L"+" + its(pa[i].e2);
        s += L"@";
        s += its(pa[i].e3) + L"+" + its(pa[i].e4);
        s += L"/F:";
        s += pa[i].rpos + L"_" + its(pa[i].f2);
        s += L"/G:";
        s += its(pa[i].g1) + L"_" + its(pa[i].g2);
        s += L"/H:";
        s += its(pa[i].h1) + L"=" + its(pa[i].h2);
        s += L"^";
        s += its(pa[i].h3) + L"=" + its(pa[i].h4); //ei lähe küsimusega pärsi kokku, võib olla ka 0
        s += L"/I:";
        s += its(pa[i].i1) + L"=" + its(pa[i].i2);
        s += L"/J:";
        if (pa[i].phone == L"pau") {
            if (i == 0) {
                s += its(pa[i + 1].j1) + L"+" + its(pa[i + 1].j2) + L"-" + its(pa[i + 1].j3);
            } else {
                s += its(pa[i - 1].j1) + L"+" + its(pa[i - 1].j2) + L"-" + its(pa[i - 1].j3);
            }
        } else {
            s += its(pa[i].j1) + L"+" + its(pa[i].j2) + L"-" + its(pa[i].j3);
        }

        res.AddItem(s);
    }
    return res;
}

CFSArray<CFSWString> do_label(utterance_struct &utt) {

    syl_struct lsyl, rsyl;
    lsyl.stress = 0;
    rsyl.stress = 0;
    word_struct lword, rword;
    lword.syl_c = 0;
    rword.syl_c = 0;

    phrase_struct lphr, rphr;
    lphr.syl_c = 0;
    rphr.syl_c = 0;
    lphr.word_c = 0;
    rphr.word_c = 0;

    INTPTR i_phr_size = utt.phr_vector.GetSize();
    for (INTPTR i_phr = 0; i_phr < i_phr_size; i_phr++) {
        INTPTR i_word_size = utt.phr_vector[i_phr].word_vector.GetSize();

        if (i_phr == 0)
            lphr.s = d;
        else
            lphr = utt.phr_vector[i_phr - 1];

        if (i_phr < (i_phr_size - 1))
            rphr = utt.phr_vector[i_phr + 1];
        else {
            rphr.s = d;
            rphr.word_c = 0;
            rphr.syl_c = 0;
        }

        for (INTPTR i_word = 0; i_word < i_word_size; i_word++) {

            INTPTR i_syl_size = utt.phr_vector[i_phr].word_vector[i_word].syl_vector.GetSize();

            if (i_word == 0) {
                if (i_phr > 0)
                    lword = utt.phr_vector[i_phr - 1].word_vector[utt.phr_vector[i_phr - 1].word_vector.GetSize() - 1];
                else
                    lword.mi.m_szRoot = d;
            } else
                lword = utt.phr_vector[i_phr].word_vector[i_word - 1];



            if (i_word < (i_word_size - 1))
                rword = utt.phr_vector[i_phr].word_vector[i_word + 1];
            else
                if (i_phr < (i_phr_size - 1))
                rword = utt.phr_vector[i_phr + 1].word_vector[0];
            else {
                rword.syl_c = 0; //mõistatus - eeldefineeritud rwordi nullimine ei tööta
                rword.mi.m_szRoot = d;
            }

            for (INTPTR i_syl = 0; i_syl < i_syl_size; i_syl++) {
                syl_struct syl = utt.phr_vector[i_phr].word_vector[i_word].syl_vector[i_syl];

                if (i_syl == 0) {
                    if (i_word > 0)
                        lsyl = utt.phr_vector[i_phr].word_vector[i_word - 1].syl_vector[utt.phr_vector[i_phr].word_vector[i_word - 1].syl_vector.GetSize() - 1];
                    else
                        if (i_phr > 0)
                        lsyl = utt.phr_vector[i_phr - 1].
                            word_vector[utt.phr_vector[i_phr - 1].word_vector.GetSize() - 1 ].
                        syl_vector[utt.phr_vector[i_phr - 1].word_vector[utt.phr_vector[i_phr - 1].word_vector.GetSize() - 1].syl_vector.GetSize() - 1];
                    else {
                        lsyl.stress = 0;
                        lsyl.syl = d;
                    }
                } else {
                    lsyl = utt.phr_vector[i_phr].word_vector[i_word].syl_vector[i_syl - 1];
                }

                if (i_syl < (i_syl_size - 1))
                    rsyl = utt.phr_vector[i_phr].word_vector[i_word].syl_vector[i_syl + 1];
                else
                    if (i_word < (i_word_size - 1))
                    rsyl = utt.phr_vector[i_phr].word_vector[i_word + 1].syl_vector[0];
                else
                    if (i_phr < (i_phr_size - 1))
                    rsyl = utt.phr_vector[i_phr + 1].word_vector[0].syl_vector[0];
                else {
                    rsyl.stress = 0;
                    rsyl.syl = d;
                }

                INTPTR i_pho_size = utt.phr_vector[i_phr].word_vector[i_word].syl_vector[i_syl].phone_vector.GetSize();
                for (INTPTR i_pho = 0; i_pho < i_pho_size; i_pho++) {
                    phone_struct pho = utt.phr_vector[i_phr].word_vector[i_word].syl_vector[i_syl].phone_vector[i_pho];

                    //l.p3 = pho.phone;
                    pho.p6 = pho.syl_p;
                    pho.p7 = syl.phone_c - pho.syl_p + 1;
                    if (lsyl.syl != d) {
                        pho.a1 = lsyl.stress;
                        pho.a3 = lsyl.phone_c;
                    } else {
                        pho.a1 = 0;
                        pho.a3 = 0;
                    }

                    pho.b1 = utt.phr_vector[i_phr].word_vector[i_word].syl_vector[i_syl].stress;
                    pho.b3 = utt.phr_vector[i_phr].word_vector[i_word].syl_vector[i_syl].phone_c;

                    if (rsyl.syl != d) {
                        pho.c1 = rsyl.stress;
                        pho.c3 = rsyl.phone_c;
                    } else {
                        pho.c1 = 0;
                        pho.c3 = 0;
                    }
                    pho.b4 = utt.phr_vector[i_phr].word_vector[i_word].syl_vector[i_syl].word_p;
                    pho.b5 = utt.phr_vector[i_phr].word_vector[i_word].syl_c - pho.b4 + 1;
                    pho.b6 = utt.phr_vector[i_phr].word_vector[i_word].syl_vector[i_syl].phr_p;
                    pho.b7 = utt.phr_vector[i_phr].syl_c - pho.b6 + 1;
                    pho.d2 = lword.syl_c;
                    pho.e2 = utt.phr_vector[i_phr].word_vector[i_word].syl_c;
                    pho.f2 = rword.syl_c;
                    pho.e3 = utt.phr_vector[i_phr].word_vector[i_word].phr_p;
                    pho.e4 = utt.phr_vector[i_phr].word_c - pho.e3 + 1;

                    pho.g1 = lphr.syl_c;
                    pho.g2 = lphr.word_c;

                    pho.h1 = utt.phr_vector[i_phr].syl_c;
                    pho.h2 = utt.phr_vector[i_phr].word_c;

                    pho.i1 = rphr.syl_c;
                    pho.i2 = rphr.word_c;

                    pho.h3 = i_phr + 1;
                    pho.h4 = utt.phra_c - pho.h3 + 1;

                    pho.j1 = utt.syl_c;
                    pho.j2 = utt.word_c;
                    pho.j3 = utt.phra_c;

                    if (lword.mi.m_szRoot == d) pho.lpos = L"0";
                    else
                        pho.lpos = gpos(CFSWString(lword.mi.m_cPOS));

                    pho.pos = gpos(CFSWString(utt.phr_vector[i_phr].word_vector[i_word].mi.m_cPOS));

                    if (rword.mi.m_szRoot == d) pho.rpos = L"0";
                    else
                        pho.rpos = gpos(CFSWString(rword.mi.m_cPOS));


                    utt.phr_vector[i_phr].word_vector[i_word].syl_vector[i_syl].phone_vector[i_pho] = pho;
                }

                //lsyl = syl;	

            }
        }
    }

    add_pauses(utt);

    CFSArray<phone_struct> pa = make_phone_array(utt);
    
    CFSArray<CFSWString> res = make_label(pa);

    return res;


}





