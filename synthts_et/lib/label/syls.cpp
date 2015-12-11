#include "../etana/proof.h"
#include "util.h"


bool print_syls = false;

const INTPTR pattern_array_size = 55;

CFSWString pattern[pattern_array_size] = { 
L"VV#", L"VVQ#", L"VVQQ#", L"VVC#", L"VVss#", L"VVsQ#", L"VVQs#", L"VVLQ#", L"VVLQQ#", L"VVCC#", L"VVCCC#", L"V#", L"VC#", L"VLQ#", L"VLQQ#", L"VLh#", L"VCC#", L"VLss#", L"VLsQ#", L"VLhv#", L"VLQC#", L"VCCC#", L"VCCCC#", 
L"VV-Q", L"VV-", L"VVs-s", L"VVs-Q", L"VVL-Q", L"VVQ-L", L"VVQ-j", L"VVQ-", L"VVQQ-", L"VVC-", L"VVQs-", L"VVsQ-", L"VVLQ-", L"VVLQ-Q", L"VVLQQ-", L"VVCC-", L"VVCCC-", L"VL-Q", L"VL-h", L"VC-", L"VLQ-", L"VLQ-Q", L"VLQ-s", L"VLs-", L"VLh-", L"VCC-", L"VLsQ-", L"VLQC-", L"VCCC-", L"VLQCC-", L"VCCCC-", L"VC-C"};

const INTPTR shift[pattern_array_size]={2,2,3,2,3,2,3,2,4,2,2,1,2,2,3,3,2,3,3,3,3,2,2,2,2,3,3,5,2,2,3,3,2,3,3,2,4,4,2,2,2,4,2,2,3,3,3,3,2,3,3,2,3,2,2};

/*
void PrintSyls(CFSWString s) {
		if (print_syls) {
			wprintf (L"\t= ");
			wprintf (s);			
			wprintf (L"\n");				
		}
}

void PrintSyls2(INTPTR i, INTPTR pos, CFSWString s) {
		if (print_syls) {
			wprintf (L"\t= ");
			wprintf (s);			
			wprintf (L" (%i)(%i)\n", i, pos);				
		}
}
*/


bool can_palat (CFSWString c) {
	if (c.FindOneOf(L"DLNST") > -1) return true;
		return false;
}

CFSWString shift_pattern (CFSWString s) {
	if (s == L'j') return L'j';
		else
	if (s == L'h') return L'h';
		else
	if (s == L'v') return L'v';
		else
	if (s.FindOneOf(L"sS") > -1)	return L's';
		else
	if (s.FindOneOf(L"lmnrLN") > -1)	return L'L';
		else
	if (s.FindOneOf(L"kptfšT") > -1)	return L'Q';
		else
	if (is_vowel(s)) return L'V';
		else
	if (is_consonant(s)) return L'C';	
	return s;
}

CFSWString chars_to_phones_part_I (CFSWString &s) {
	/*	müüa -> müia siia?
			Kuna vältemärgi nihutamise reeglid on ehitatud selliselt, et kohati kasutatakse 
			foneeme ja kohati ei, siis tuleb täht->foneem teisendus teha kahes jaos.
			Palataliseerimine põhineb ideel, et palataliseeritud foneemi ümbruses ei saa
			olla palataliseeruvaid mittepalataliseeritud foneeme :D
	*/
	CFSWString res;
	for (INTPTR i = 0; i < s.GetLength(); i++) {
		CFSWString c = s.GetAt(i);
		if (c == L']') {
				CFSWString t = CFSWString(s.GetAt(i-1)).ToUpper();
				res.SetAt( res.GetLength()-1, t.GetAt(0));
					//vaatab taha; pole kindel, et kas on vajalik
					t = CFSWString(s.GetAt(i-2)).ToUpper();
					if (can_palat(t)) {
						res.SetAt( res.GetLength()-2, t.GetAt(0));
							t = CFSWString(s.GetAt(i-3)).ToUpper();
							if (can_palat(t)) {
								res.SetAt( res.GetLength()-2, t.GetAt(0));
							}
					}
					//vaatab ette					
					t = CFSWString(s.GetAt(i+1)).ToUpper();
					if (can_palat(t)) {
						s.SetAt(i+1, t.GetAt(0));
						t = CFSWString(s.GetAt(i+2)).ToUpper();
							if (can_palat(t)) {
								s.SetAt(i+2, t.GetAt(0));
							}
					}
			}
		else
		if (c == L'<') {
				CFSWString t = CFSWString(s.GetAt(i+1)).ToUpper();
				s.SetAt(i+1, t.GetAt(0));
		}
		else
		if (c == L'?') {} //Ebanormaalne rõhk. Pärast vaatab, mis teha
		else
		if (c == L'x') res += L"ks";
		else
		if (c == L'y') res += L"i";
		else
		if (c == L'w') res += L"v";
		else
		if (c == L'z') res += L"ts";
		else
		if (c == L'c') {
			 res += L"k";
		}
		else
		if (c == L'ü' && is_vowel(s.GetAt(i+1)) && s.GetAt(i-1) == L'ü')
			res += L"i";
		else
		if (c == L'q') {
			if (is_vowel(s.GetAt(i+1))) {
					res += L"k";
					s.SetAt(i+1, L'v');
				}
				else
				res += L"k";
		}
		else		
		res += c;
	}
return res;
}

CFSWString syllabify(CFSWString s) {
	CFSWString res;

	for (INTPTR i = 0; i < s.GetLength(); i++) {
		CFSWString c = s.GetAt(i);
		if (is_consonant(c) && is_vowel(s.GetAt(i-1)) && is_vowel(s.GetAt(i+1)))
			res += d;
		if (is_vowel(c) && is_vowel(s.GetAt(i-1)) && is_vowel(s.GetAt(i+1)) && c == s.GetAt(i+1))
			res += d;
		if (is_vowel(c) && is_vowel(s.GetAt(i-1)) && is_vowel(s.GetAt(i-2)) && c != s.GetAt(i-1))
			res += d;
		if (is_consonant(c) && is_consonant(s.GetAt(i-1)) && is_vowel(s.GetAt(i+1)) && has_vowel(res) ) //küsitav
			res += d;
		
		res += c;
	}	
//	PrintSyls(res);
	return res;		
}

INTPTR pattern_lookup (CFSWString s) {
	INTPTR res = -1;
	for (INTPTR i = 0; i < (pattern_array_size-1); i++) {
		if (s == pattern[i]) {
			res = shift[i];
		}
	}
	return res;
}

CFSWString simplify_pattern (CFSWString s) {
	CFSWString res;
	for (INTPTR i = 0; i < (s.GetLength()); i++) {
		CFSWString c = s.GetAt(i);
		if (c.FindOneOf(L"jhvsLQ") > -1) res += L"C";
			else
			res += c;
	}
	return res;
}





CFSWString the_shift(CFSWString s) {
/*
	On mingi võimalus, et lihtsustus tuleb teha kahes astmes. LQ-ta ja LQ-ga (vt shift_pattern). Kõik 
	seotud sellega, et pole	vältenihutusreeglitest lõpuni aru saanud. Eksisteerib Mihkla versioon ja 
	ametlik versioon. Tänud	Mihklale, kes kala asemel annab tattninale õnge, see õpetab ujuma.
	Maadlesin õngega pikalt.
*/	
	
	CFSWString res;
	CFSWString code;
	INTPTR pos;
	INTPTR i = 0;
	INTPTR x;
	
	while (s.GetLength() > 0) { // kui kokku jookseb on alati süüdi WHILE
		CFSWString c = s.GetAt(0);
		s.Delete(0, 1);
		if (is_uvowel(c)) {
			c = c.ToLower();
			code += shift_pattern(c);
			res += c;
			pos = i;
		}
		else
		if (c == d && code.GetLength() > 0) {
			res += c;
			code += c;
			CFSWString t_code = code;
			t_code += shift_pattern(s.GetAt(0));			
			
			x = pattern_lookup(t_code); //orig üle silbipiiri
//			if (print_syls) PrintSyls2(x, pos, t_code);
			if (x > -1) {
				x += pos;
				if (x > res.GetLength()) { // kui kargab järgmisse silpi
					x = x - res.GetLength();
					s.Insert(x, colon);
				}
				else
				res.Insert(x, colon);
				i++;
			}
			else {
				t_code = simplify_pattern(t_code);
				x = pattern_lookup(t_code); //liht üle silbipiiri
//				if (print_syls) PrintSyls2(x, pos, t_code);
				if (x > -1) {
					x += pos;
					if (x > res.GetLength()) { // kui kargab järgmisse silpi
						x = x - res.GetLength();
						s.Insert(x, colon);
					}
					else
					res.Insert(x, colon);
					i++;
				}
				else {
					x = pattern_lookup(code); //orig 
//					if (print_syls) PrintSyls2(x, pos, code);
					if (x > -1) {
						x += pos;
						res.Insert(x, colon);
						i++;
					}
					else {
						code = simplify_pattern(code);
						x = pattern_lookup(code); //liht
//						if (print_syls) PrintSyls2(x, pos, code);
							if (x > -1) {
								x += pos;
								res.Insert(x, colon);
								i++;
							}
					}
				}
			}

			code = empty_str;
		}
		else
		{
			res += c;
			if (code.GetLength() > 0) {
				code += shift_pattern(c);
			}
		}
		i ++;		
	} //while

	// sõna lõpus
	if (code.GetLength() > 0) {
		code += L"#";
		//imelik koht ainult "lonksu" pärast
		if ((code.Left(3) == L"VLQ")  && ((code.GetAt(3) == L's') || (code.GetAt(3) == L'h') ||(code.GetAt(3) == L'v') ||(code.GetAt(3) == L'j'))) {
			code = L"VLQC#";
		}		
		INTPTR x = pattern_lookup(code);
		//if (print_syls) PrintSyls2(x, pos, code);
		if (x > -1) {
			x += pos;
			res.Insert(x, colon);
		}
		else {
			code = simplify_pattern(code);
			x = pattern_lookup(code);
			//if (print_syls) PrintSyls2(x, pos, code);
			if (x > -1) {
				x += pos;
				res.Insert(x, colon);
			}			
		}
		code = empty_str;
	}
	return res;
}


CFSWString word_to_syls(CFSWString word) {
	word = chars_to_phones_part_I(word);
	CFSWString s = make_char_string(word);
	s = syllabify(s);
	s = the_shift(s);
	if (print_syls) {
		//PrintSyls(s);
		//wprintf (L"\n");
	}
	return s;
}

bool is_stressed_syl (CFSWString syl) {
	bool res = false;
	for (INTPTR i = 0; i < syl.GetLength(); i++) {
		if ((syl.GetAt(i) == colon) || ((is_vowel(syl.GetAt(i))) && (is_vowel(syl.GetAt(i+1)))))
			res = true;
	}
	return res;
}

// et oleks lihtsam 2sid tagasi lülitada ja saada 1 astmlised rõhud

INTPTR extra_stress (CFSArray<syl_struct> &sv, INTPTR size) {
    if (size==1) return 0;
    else 
        for (INTPTR i = 1; i < size; i++)
            for (INTPTR i1 = 0; i1 < sv[i].syl.GetLength(); i1++) {
                if (sv[i].syl.GetAt(i1) == colon) return i;
                if (i1 > 0) 
                    if ( is_vowel(sv[i].syl.GetAt(i1)) &&  is_vowel(sv[i].syl.GetAt(i1-1)) )
                        return i;
               }                    
    return 0;
}


void add_stress2 (CFSArray<syl_struct> &sv, INTPTR wp) {
/* Kõige radikaalsem rõhutus siiani. 
 * wp = kui on liitsõna esimene liige siis on seal pearõhk.
 */
    INTPTR main_stress = 2;
    INTPTR stress = 1;
    INTPTR size = sv.GetSize();
    INTPTR stress_type = extra_stress(sv, size);
    if (stress_type == 0) {
	for (INTPTR i = 0; i < size; i++) {
            if (i % 2 == 0) {                
                if ((i == 0) && (wp == 0))
                    sv[i].stress = main_stress;
                else
                    sv[i].stress = stress;
            }
        }
        if (size > 1) sv[size-1].stress = 0;
    } 
    else {
        if (wp == 0)
            sv[stress_type].stress = main_stress;
        else
            sv[stress_type].stress = stress;
        
        //esimene pool
        if (stress_type == 1) sv[0].stress = 0;
        else       
        for (INTPTR i = stress_type-1; i >= 0; i--) 
            if (i % 2 == 0)
                sv[i].stress = stress;

        if ((stress_type % 2 != 0) && (stress_type > 1))
            sv[stress_type-1].stress = 0;
        
        //teine pool
        
        INTPTR lopp = size - stress_type; 
        
        if (lopp > 3) {
            for (INTPTR i = stress_type+1; i < size; i++)
                if (i % 2 != 0) sv[i].stress = stress;

            sv[size-1].stress = 0; 
        } 
    }      
 }
    






void do_syls(word_struct &w) {
	CFSArray<syl_struct> sv, sv_temp;
	syl_struct ss;	
	CFSArray<CFSWString> temp_arr, c_words;
	ss.phone_c=0, ss.word_p=0, ss.phr_p=0, ss.utt_p = 0; w.syl_c=0;
        INTPTR word_p = 1;
        
        explode (w.mi.m_szRoot, L"_", c_words);
        
        for (INTPTR cw = 0; cw < c_words.GetSize(); cw++) {

            CFSWString s = word_to_syls(c_words[cw]);
            
            //MINGI ERROR paindliKkus
            s.Replace(L"K", L"k", 1);
            s.Replace(L"R", L"r", 1);
            s.Replace(L"V", L"v", 1);
                                   
            explode (s, d, temp_arr);
            ss.stress = 0;
            sv_temp.Cleanup();
            for (INTPTR i = 0;  i < temp_arr.GetSize(); i++) {
		ss.syl = temp_arr[i];
                ss.stress = 0; // rõhu algväärtus
                ss.word_p = word_p++;
                sv_temp.AddItem (ss);
            }
            
            add_stress2(sv_temp, cw);
            for (INTPTR i = 0; i < sv_temp.GetSize(); i++)
                sv.AddItem(sv_temp[i]);
        }
        
	w.syl_vector = sv;

}



