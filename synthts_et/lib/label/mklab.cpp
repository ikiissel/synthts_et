#include "../etana/proof.h"
#include "../../include/mklab.h"

#include "util.h"
#include "word.h"
#include "number.h"
#include "syls.h"
#include "label.h"

CFSWString DealWithText(CFSWString text) {
    /* Proovin kogu sõnniku minema loopida */
    CFSWString res;
    text.Trim();
    text.Replace(L"\n\n", L"\n", 1);    
    for (INTPTR i = 0; i < text.GetLength(); i++) {
        CFSWString c = text.GetAt(i);
        CFSWString pc = res.GetAt(res.GetLength() - 1);
        CFSWString nc = text.GetAt(i + 1);
        if (is_char(c)) res += c;
        else
            if (is_digit(c)) res += c;
        else
            if (is_hyphen(c) && is_char(pc) && is_char(nc)) res += sp;
        else
            if (is_symbol(c)) res += c;
        else
            if (is_colon(c) && !is_colon(pc)) res += c;
        else
            if (is_bbracket(c) && !is_bbracket(pc)) res += c;
        else
            if (is_ebracket(c) && is_ending(nc)) res += L"";
        else
            if (is_ebracket(c) && !is_ebracket(pc)) res += c;
        else
            if (is_comma(c) && !is_comma(pc)) res += c;
        else
            if (is_fchar(c)) res += replace_fchar(c);
        else
            if (is_space(c) && !is_whitespace(pc)) res += c;
        else
            if (is_break(c) && !is_break(pc)) { 
                
                res += c;   
            } //kahtlane
        else
            if (is_tab(c) && !is_whitespace(pc)) res += c;
        else
            if (is_ending(c) && !is_ending(pc) && !is_whitespace(pc)) res += c;

    }
    res.Trim();        
    return res;

}

CFSArray<CFSWString> do_utterances(CFSWString s) {
    CFSWString res = empty_str;
    CFSArray<CFSWString> res_array;

    if (s.GetLength() == 1)
        res_array.AddItem(s);
    else
        for (INTPTR i = 0; i < s.GetLength(); i++) {
            CFSWString c = s.GetAt(i);
            CFSWString pc = res.GetAt(res.GetLength() - 1);
            CFSWString nc = s.GetAt(i + 1);
            CFSWString nnc = s.GetAt(i + 2);

            if (is_ending(c) && is_whitespace(nc) && is_upper(nnc)) {
                res.Trim();
                res_array.AddItem(res);
                res = empty_str;
            } else
                if (is_tab(c)) {
                if (res.GetLength() > 0) {
                    res.Trim();
                    res_array.AddItem(res);
                    res = empty_str;
                }
            } else
                res += c;
        }
    res.Trim();
    
    if (res.GetLength() > 0) {
        while (is_ending(res.GetAt(res.GetLength() - 1))) {
            res.Delete(res.GetLength() - 1, 1);
        }
        
        res_array.AddItem(res);
    }

    for (INTPTR i=0; i < res_array.GetSize(); i++) {
        if (is_ending(res_array[i].GetAt(res_array[i].GetLength()-1))) 
            res_array[i].Delete(  res_array[i].GetLength()-1, 1   );

    }    
    return res_array;
}

void push_ph_res(utterance_struct &u, phrase_struct &p, CFSWString &res) {
    res.Trim();
    p.s = res;
    u.phr_vector.AddItem(p);
    res = empty_str;
} //do_phrases osa

bool is_word(CFSWString s) {
    if ((has_vowel(s) == true) &&
            (s.GetLength() == make_char_string(s).GetLength()) &&
            (s.GetLength() > 1))
        return true;
    return false;
}


CFSArray<CFSWString> tokens2words(CFSArray<CFSWString> a) {
    CFSArray<CFSWString> temp_array;
    CFSArray<CFSWString> res;
    for (INTPTR i = 0; i < a.GetSize(); i++) {
        CFSWString s = a[i];

        if (is_abbreviation(s, temp_array) > 0) {
            for (INTPTR i = 0; i < temp_array.GetSize(); i++)
                res.AddItem(temp_array[i]);
        } else
            if (is_word(s))
            res.AddItem(s);
        else {
            CFSArray<CFSWString> carray;
            make_ctype_array(s, carray);
            for (INTPTR ictype = 0; ictype < carray.GetSize(); ictype++) {
                INTPTR c_type = ctype(carray[ictype].GetAt(0));
                if (c_type == 1) { //Tähed
                    if (is_word(carray[ictype])) res.AddItem(carray[ictype]);
                    else { // kui on sodipodi
                        explode(carray[ictype], L"", temp_array);
                        for (INTPTR i_temp = 0; i_temp < temp_array.GetSize(); i_temp++)
                            res.AddItem(replace_schar(temp_array[i_temp]));
                    }
                } else
                    if (c_type == 2) { //Sümbolid
                    explode(carray[ictype], L"", temp_array);
                    for (INTPTR i_temp = 0; i_temp < temp_array.GetSize(); i_temp++)
                        res.AddItem(replace_schar(temp_array[i_temp]));
                } else
                    if (c_type == 3) { //Numbrid
                    CFSWString nr = int_to_words(carray[ictype]);
                    explode(nr, sp, temp_array);
                    for (INTPTR i_temp = 0; i_temp < temp_array.GetSize(); i_temp++)
                        if (temp_array[i_temp].GetLength() > 0)
                            res.AddItem(temp_array[i_temp]);
                }
            }
        }
    } // iga token

    return res;
}

void phrase2words(phrase_struct &p, CFSArray<CPTWord> &PTW) {

    CFSWString res;
    CFSArray<CPTWord> PTWords;
    CFSArray<CFSWString> word_array;

    for (INTPTR i = 0; i < p.s.GetLength(); i++) {
        CFSWString c = p.s.GetAt(i);
        if (is_space(c) || i == (p.s.GetLength() - 1)) {
            if (i == (p.s.GetLength() - 1)) res += c;
            word_array.AddItem(res);
            res = empty_str;
        } else res += c;
    }

    word_array = tokens2words(word_array);
    p.s = empty_str;

    for (INTPTR i = 0; i < word_array.GetSize(); i++) {
        p.s += word_array[i];
        if (i < word_array.GetSize() - 1) p.s += sp;
        PTW.AddItem(word_array[i]); //.ToLower()); Kui väiketähtedeks, siis siin
    }
    p.word_c = word_array.GetSize();
}

INTPTR do_phrases(utterance_struct &u) {
    phrase_struct p;
    CFSWString res;
    p.phone_c = 0;
    p.syl_c = 0;
    p.word_c = 0;

    for (INTPTR i = 0; i < u.s.GetLength(); i++) {
        CFSWString c = u.s.GetAt(i);
        CFSWString pc = res.GetAt(res.GetLength() - 1);
        CFSWString nc = u.s.GetAt(i + 1);
        CFSWString nnc = u.s.GetAt(i + 2);
        if ((is_comma(c) || is_colon(c) || is_semicolon(c)) && is_space(nc) && is_char(nnc)) {
            res.Trim();
            if (res.GetLength() > 0) {
                push_ph_res(u, p, res);
            }
        } else
            if (is_bbracket(c)) {
               res.Trim();
            if (res.GetLength() > 0) {                
                push_ph_res(u, p, res);
            }
               p.s = L"sulgudes";
               u.phr_vector.AddItem(p);

        } else
            if (is_ebracket(c)) {
             res.Trim();
            if (res.GetLength() > 0) {                
                push_ph_res(u, p, res);
            }
        } else
            if (is_space(c)) { // komatud sidesõnad
            CFSWString tempm = u.s.Mid(i + 1, -1);
            if (is_conju(tempm.Left(tempm.Find(sp))) && res.GetLength() > 0) {
                push_ph_res(u, p, res);
            } else
                res += c;
        } else
            if (is_bhyphen(c)) {
            if (res.GetLength() > 0 && ((is_char(pc) && is_space(nc)) || (is_space(nc) && is_char(nnc)) || (is_space(pc) && is_char(nc)))) {
                push_ph_res(u, p, res);
            } else
                res += c;
        } else
            res += c;
    }

    if (res.GetLength() > 0) {
        //        if (is_ending(res.GetAt(res.GetLength() - 1))) {
        //            res.Delete(res.GetLength() - 1, 1);
        //        }        
        push_ph_res(u, p, res);
    }        
    return u.phr_vector.GetSize();
}

CMorphInfo DisambiguateSynthesisResult(CFSArray<CMorphInfo> MIs, CPTWord s) {
    CFSWString s1;
    CMorphInfo x;

    for (INTPTR i = 0; i < MIs.GetSize(); i++) {
        s1 = MIs[i].m_szRoot + MIs[i].m_szEnding + MIs[i].m_szClitic;
        if (s.m_szWord == make_char_string(s1)) {
            MIs[i].m_szRoot = s1;
            return MIs[i];
        }
    }
    x.m_szRoot = s.m_szWord;
    return x;
}

void do_phones(syl_struct &ss) {
    CFSArray<phone_struct> pv;
    phone_struct p;
    p.utt_p = 0;
    p.phr_p = 0;
    p.word_p = 0;
    p.syl_p = 0;
    INTPTR phone_syl_p = 1;
    for (INTPTR i = 0; i < ss.syl.GetLength(); i++) {
        CFSWString c = ss.syl.GetAt(i);
        if ((c == L'š') || (c == L'ž')) c = L"sh";
        if (c == L'õ') c = L"q";
        if (c == L'ä') c = L"x";
        if (c == L'ö') c = L"c";
        if (c == L'ü') c = L"y";

        if (is_colon(c)) {
            // 1. nihutusvigade kaitseks (vt "piirkonda")
            // 2-3. kolmandas vältes v ja j on kõnebaasis sedavõrd haruldased,
            // et väljundis kuuleb nende asemel mingit r-i laadset hääikut.
            // Kellel on parem baas, kommenteerigu 2. ja 3. tingimus välja.            
				if ((i > 0) && (pv[pv.GetSize() - 1].phone != L"j") && (pv[pv.GetSize() - 1].phone != L"v"))
                pv[pv.GetSize() - 1].phone += doq;
        } else {
            p.phone = c;
            p.syl_p = phone_syl_p++;
            pv.AddItem(p);
        }

    }
    ss.phone_vector = pv;
}

void print_u(utterance_struct u) {
    for (INTPTR i = 0; i < u.phr_vector.GetSize(); i++)
        for (INTPTR i1 = 0; i1 < u.phr_vector[i].word_vector.GetSize(); i1++) {
            fprintf(stderr, "%s\n\n", ccstr(u.phr_vector[i].word_vector[i1].mi.m_szRoot));
            for (INTPTR i2 = 0; i2 < u.phr_vector[i].word_vector[i1].syl_vector.GetSize(); i2++) {
                fprintf(stderr, "\t%s\n", ccstr(u.phr_vector[i].word_vector[i1].syl_vector[i2].syl));

                for (INTPTR i3 = 0; i3 < u.phr_vector[i].word_vector[i1].syl_vector[i2].phone_vector.GetSize(); i3++) {
                    CFSWString w = u.phr_vector[i].word_vector[i1].syl_vector[i2].phone_vector[i3].phone;
                    while (w.GetLength() < 6) w += sp;
                    fprintf(stderr, "\t\t%s", ccstr(w));
                    wprintf(L"%i %i %i %i\t", u.phra_c, u.word_c, u.syl_c, u.phone_c);

                    wprintf(L"%i %i %i [%i]\t", u.phr_vector[i].word_c, u.phr_vector[i].syl_c, u.phr_vector[i].phone_c, u.phr_vector[i].utt_p);

                    wprintf(L"%i %i [%i %i]\t", 
                            u.phr_vector[i].word_vector[i1].syl_c,
                            u.phr_vector[i].word_vector[i1].phone_c,
                            u.phr_vector[i].word_vector[i1].utt_p,
                            u.phr_vector[i].word_vector[i1].phr_p
                            );

                    wprintf(L"%i [%i %i %i]\t", 
                            u.phr_vector[i].word_vector[i1].syl_vector[i2].phone_c,
                            u.phr_vector[i].word_vector[i1].syl_vector[i2].utt_p,
                            u.phr_vector[i].word_vector[i1].syl_vector[i2].phr_p,
                            u.phr_vector[i].word_vector[i1].syl_vector[i2].word_p
                            );

                    phone_struct p = u.phr_vector[i].word_vector[i1].syl_vector[i2].phone_vector[i3];
                    wprintf(L"[%i %i %i %i]\t", p.utt_p, p.phr_p, p.word_p, p.syl_p);

                    wprintf(L"{ %i }", u.phr_vector[i].word_vector[i1].syl_vector[i2].stress);

                    wprintf(L"\n");
                }
            }
        }
    wprintf(L"\n");
}

CFSArray<CFSWString> do_all(CFSWString utt, bool print_label, bool print_utt) {
    CFSArray<CFSWString> res;
    CFSArray<CPTWord> PTW;
    utterance_struct u;
    u.s = utt;
    u.syl_c = 0;
    u.phone_c = 0;
    u.phra_c = do_phrases(u);
    INTPTR word_count = 0;
	 if (print_utt) fprintf(stderr, "%s\n", ccstr(utt));    
    for (INTPTR i = 0; i < u.phr_vector.GetSize(); i++) {
        u.phr_vector[i].utt_p = i;
        phrase2words(u.phr_vector[i], PTW);
        word_count += u.phr_vector[i].word_c;
    }

    CFSArray<CMorphInfos> MRs = Disambiguator.Disambiguate(Linguistic.AnalyzeSentense(PTW));

    CFSArray<CMorphInfo> words;

    for (INTPTR i = 0; i < MRs.GetSize(); i++)
        //for (INTPTR i1 = 0; i1 < MRs[i].m_MorphInfo.GetSize(); i1++)
        words.AddItem(MRs[i].m_MorphInfo[0]); //Ühestamistulemuse ühestamise koht


    u.word_c = words.GetSize();


    word_struct w;

    INTPTR utt_phone_c = 1;
    INTPTR syl_utt_p = 1;
    INTPTR phone_utt_p = 1;



    for (INTPTR i = 0; i < u.phr_vector.GetSize(); i++) {
        u.phr_vector[i].utt_p = i + 1;
        INTPTR syl_phr_p = 1;
        INTPTR phone_phr_p = 1;
        INTPTR phrase_pho_c = 1;
        for (INTPTR i1 = 0; i1 < u.phr_vector[i].word_c; i1++) {
            w.utt_p = utt_phone_c++;
            w.phr_p = i1 + 1;
            w.mi = words[0];
            w.mi.m_szRoot += make_char_string(w.mi.m_szEnding) + w.mi.m_szClitic;            
            w.mi.m_szRoot = w.mi.m_szRoot.ToLower();
            // sidesõnad + ei välteta
            if ((CFSWString(w.mi.m_cPOS) == L"J") || (w.mi.m_szRoot == L"<ei")) w.mi.m_szRoot.Replace(L"<", L"", 1);
            do_syls(w);
            u.phr_vector[i].word_vector.AddItem(w);

            INTPTR phone_word_p = 1;
            INTPTR word_pho_c = 1;
            for (INTPTR i2 = 0; i2 < u.phr_vector[i].word_vector[i1].syl_vector.GetSize(); i2++) {
                u.syl_c++;
                u.phr_vector[i].syl_c++;
                u.phr_vector[i].word_vector[i1].syl_c++;
                u.phr_vector[i].word_vector[i1].syl_vector[i2].phr_p = syl_phr_p++;
                u.phr_vector[i].word_vector[i1].syl_vector[i2].utt_p = syl_utt_p++;
                INTPTR syl_phone_c = 1;

                do_phones(u.phr_vector[i].word_vector[i1].syl_vector[i2]);

                for (INTPTR i3 = 0; i3 < u.phr_vector[i].word_vector[i1].syl_vector[i2].phone_vector.GetSize(); i3++) {
                    u.phone_c++;
                    u.phr_vector[i].phone_c = phrase_pho_c++;
                    u.phr_vector[i].word_vector[i1].phone_c = word_pho_c++;
                    u.phr_vector[i].word_vector[i1].syl_vector[i2].phone_c = syl_phone_c++;
                    phone_struct p = u.phr_vector[i].word_vector[i1].syl_vector[i2].phone_vector[i3];
                    p.utt_p = phone_utt_p++;
                    p.phr_p = phone_phr_p++;
                    p.word_p = phone_word_p++;
                    u.phr_vector[i].word_vector[i1].syl_vector[i2].phone_vector[i3] = p;

                }

            }
            words.RemoveItem(0, 1);
        }
    }




    if (print_label) print_u(u);
    res = do_label(u);
    return res;
}




















