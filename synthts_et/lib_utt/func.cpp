#include "../include/util.h"
#include "../include/func.h"
#include "../include/num.h"
#include "../include/syls.h"
#include "../include/label.h"
#include "../etana/proof.h"


bool PRINT_WORD = false;

CFSWString DealWithText (CFSWString text) {
	CFSWString res;
	text.Trim();
	for (INTPTR i = 0; i < text.GetLength(); i++)	{
		CFSWString c = text.GetAt(i);
		CFSWString pc = res.GetAt(res.GetLength()-1);
		CFSWString nc = text.GetAt(i+1);
		if (is_char(c)) res += c;
			else
		if (is_digit(c)) res += c;
			else			
		if (is_symbol(c)) res += c;
			else			
		if (is_colon(c) && !is_colon(pc)) res += c;
			else			
		if (is_bbracket(c) && !is_bbracket(pc)) res += c;
			else
		if (is_ebracket(c) && !is_ebracket(pc)) res += c;
			else									
		if (is_comma(c) && !is_comma(pc)) res += c;
			else			
		if (is_fchar(c)) res += replace_fchar(c);
			else			
		if (is_space(c) && !is_whitespace(pc)) res += c;
			else
		if (is_break(c) && !is_break(nc)) res += c;
			else
		if (is_tab(c) && !is_whitespace(pc)) res += c;
			else
		if (is_ending(c) && !is_ending(pc) && !is_whitespace(pc)) res += c;
			
	}
	res.Trim();
	return res;
}


CFSArray<CFSWString> do_utterances(CFSWString s) {
		CFSWString res;
		CFSArray<CFSWString> res_array;
		
		for (INTPTR i = 0; i < s.GetLength(); i++)	{
			CFSWString c = s.GetAt(i);
			CFSWString pc = res.GetAt(res.GetLength()-1);
			CFSWString nc = s.GetAt(i+1);
			CFSWString nnc = s.GetAt(i+2);
			
			if (is_ending(c) && is_whitespace(nc) && is_upper(nnc))  {
				res.Trim();
				res_array.AddItem(res);
				res = empty_str;
			}
			else
			if (is_tab(c)) {
				if (res.GetLength() > 0) {
					res.Trim();
					res_array.AddItem(res);					
					res = empty_str;
				}
			}
			else
				res += c;
			}
			res.Trim();
			if (res.GetLength() > 0) {
				res_array.AddItem(res);
			}			
	return res_array;
}

void push_ph_res(utterance_struct &u, phrase_struct &p, CFSWString &res) {
					res.Trim();
					p.s = res;
					u.phr_vector.AddItem(p);
					res = empty_str;
}


void do_phrases(utterance_struct &u) {
	phrase_struct p;
	CFSWString res;
	p.phone_c = 0;
	p.syl_c = 0;
	p.word_c = 0;
		for (INTPTR i = 0; i < u.s.GetLength(); i++)	{
			CFSWString c = u.s.GetAt(i);
			CFSWString pc = res.GetAt(res.GetLength()-1);
			CFSWString nc = u.s.GetAt(i+1);
			CFSWString nnc = u.s.GetAt(i+2);
			if ((is_comma(c)||is_colon(c)||is_semicolon(c)) && is_space(nc) && is_char(nnc)) {
				if (res.GetLength() > 0) {	push_ph_res(u, p, res);	}
			}
			else
			if (is_bbracket(c)) {
				if (res.GetLength() > 0) {
					push_ph_res(u, p, res);
					p.s = L"sulgudes";
					u.phr_vector.AddItem(p);					
				}			
			}
			else
			if (is_ebracket(c)) {
				if (res.GetLength() > 0) {	push_ph_res(u, p, res);	}			
			}			
			else
			if (is_space(c)) { 
				CFSWString tempm = u.s.Mid(i+1, -1);
				if (is_conju(tempm.Left(tempm.Find(L" "))) && res.GetLength() > 0) {
					push_ph_res(u, p, res);
				}
				else				
				res += c;	
			}			
			else
			if (is_bhyphen(c)) {
				if (res.GetLength() > 0 && ( (is_char(pc) && is_space(nc)) || (is_space(nc) && is_char(nnc)) || (is_space(pc) && is_char(nc))   ) ) {
					push_ph_res(u, p, res);
				} else			
				res += c;	
			}			
			else
			res += c;	
		}


			if (res.GetLength() > 0) {
					if (is_ending(res.GetAt(res.GetLength()-1))) {
						res.Delete(res.GetLength()-1, 1);
					}
					push_ph_res(u, p, res);
				}
	
}



void do_phones (syl_struct &ss) {
	CFSArray<phone_struct> pv;
	phone_struct p;
	for (INTPTR i = 0; i < ss.syl.GetLength(); i++ ) {
		CFSWString c = ss.syl.GetAt(i);
		if ((c == L'š') || (c == L'ž')) c = L"sh";
		if (c == L'õ') c = L"q";
		if (c == L'ä') c = L"x";
		if (c == L'ö') c = L"c";
		if (c == L'ü') c = L"y";

		if (is_colon(c)) {
		if (i > 0) 
				pv[pv.GetSize()-1].phone += doq;
		}
		else {
			p.phone = c;
			p.syl_p = i + 1;
			pv.AddItem(p);
		}

	}
	ss.phone_vector = pv;
}



CMorphInfo DisambiguateDisambiguationResult(CFSArray<CMorphInfo> a, CFSWString s) {
 CMorphInfo res;
 	res = a[0];
 	for (INTPTR i = 1; i < a.GetSize(); i++) {
 		if (s.GetLength() == a[i].m_szRoot.GetLength())
 			res = a[i];
 	}
 return res;

}

bool is_word (CFSWString s) {
	if ((has_vowel(s) == true) && 
			(s.GetLength() == make_char_string(s).GetLength()) && 
			(s.GetLength() > 1)) 
		return true;	
	return false;
}

CFSArray<CFSWString> tokens2words (CFSArray<CFSWString> a) {
	
	CFSArray<CFSWString> res;
	for (INTPTR i = 0; i < a.GetSize(); i++) {
		CFSWString s = a[i];
		
		if (is_word(s)) 
			res.AddItem (s);
		else {
			CFSArray<CFSWString> carray;
			CFSArray<CFSWString> temp_array;
			make_ctype_array(s, carray); 	
			for (INTPTR ictype=0; ictype < carray.GetSize(); ictype++) {				
				INTPTR c_type = ctype(carray[ictype].GetAt(0));
				if (c_type == 1) { //Tähed
					if (is_word(carray[ictype])) res.AddItem(carray[ictype]);
						else { // kui on sodipodi
							explode(carray[ictype], L"", temp_array);
							for (INTPTR i_temp = 0; i_temp < temp_array.GetSize(); i_temp++) 
								res.AddItem (replace_schar(temp_array[i_temp]));
					}
				}
				else 
				if (c_type == 2) { //Sümbolid
					explode(carray[ictype], L"", temp_array);
						for (INTPTR i_temp = 0; i_temp < temp_array.GetSize(); i_temp++) 
							res.AddItem (replace_schar(temp_array[i_temp]));
				}
				else
				if (c_type == 3) { //Numbrid
					CFSWString nr = int_to_words(carray[ictype]);
					explode(nr, L" ", temp_array);
						for (INTPTR i_temp=0; i_temp < temp_array.GetSize(); i_temp++) 
							if (temp_array[i_temp].GetLength() > 0) 
								res.AddItem (temp_array[i_temp]);
				}	
			}
		} 
		
		
			
	
	} // iga token
	
	return res;
}


void phrase2words (phrase_struct &p) {

	CFSWString res;
	CFSArray<CPTWord> PTWords;
	CFSArray<CFSWString> word_array;

		for (INTPTR i = 0; i < p.s.GetLength(); i++)	{
			CFSWString c = p.s.GetAt(i);
			if (is_space(c) || i == (p.s.GetLength()-1) ) {
				if (i == (p.s.GetLength()-1)) res += c;					
					word_array.AddItem (res);
					res = empty_str;
			}
			else res += c;
		}
	
	word_array = tokens2words(word_array);

	for (INTPTR i = 0; i < word_array.GetSize(); i++)
		PTWords.AddItem(word_array[i]);
	
	
	Linguistic.m_bGuess=true; 
	Linguistic.m_bPhonetic=false;
	Linguistic.m_bProperName=true;


	CFSArray<CMorphInfos> MorphResults = Linguistic.AnalyzeSentense(PTWords);
	MorphResults=Disambiguator.Disambiguate(MorphResults);

	CFSArray<CMorphInfos> MorphResults_temp;

	INTPTR lisatud = 0;
	for (INTPTR i = 0; i < MorphResults.GetSize(); i++) {
		
		CFSArray<CMorphInfo> Analysis = MorphResults[i].m_MorphInfo;
		CFSWString root = Analysis[0].m_szRoot;

		if (is_compound_word(root)) {
	
			CFSArray<CFSWString> compound_word_array;			
			explode (root, L"_", compound_word_array);
			for (INTPTR i2 = 0; i2 < compound_word_array.GetSize()-1; i2++) {
				PTWords.Cleanup();
				word_array.InsertItem (i+lisatud, compound_word_array[i2]);
				lisatud ++;
				PTWords.AddItem(compound_word_array[i2]);				
					MorphResults_temp.AddItem (
							Disambiguator.Disambiguate(Linguistic.AnalyzeSentense(PTWords))[0]
						);
			}
			
			word_array[i+lisatud] = L"+" + word_array[i+lisatud];
			MorphResults[i].m_MorphInfo[0].m_szRoot = compound_word_array [ compound_word_array.GetSize() -1 ];
			MorphResults_temp.AddItem(MorphResults[i]);
			
		} 
		else {			

			MorphResults_temp.AddItem(MorphResults[i]);
		}
	}
	MorphResults = MorphResults_temp;
	
	Linguistic.m_bGuess=true; 
	Linguistic.m_bPhonetic=true;
	Linguistic.m_bCombineWords = false;
	
	word_struct ws;
	
	for (INTPTR i = 0; i < MorphResults.GetSize(); i++) {
		
		CMorphInfo MI = DisambiguateDisambiguationResult(MorphResults[i].m_MorphInfo, word_array[i]);
			
			if ((CFSWString(MI.m_cPOS) == L"V" ) && (MI.m_szRoot != L"ei") && (MI.m_szRoot != L"ära") ) // tegusõnale tuleb endiselt käsitsi keevitada?
				MI.m_szRoot += L"ma";
						
					CFSArray<CMorphInfo> Result = Linguistic.Synthesize(MI, L"");
					INTPTR ResultSize = Result.GetSize();
					if (ResultSize == 0) {
						ws.ai = MI;
					}
					else { 
						ws.ai = Result[0];
						if (ResultSize > 1) //ühestaja :)	
							for (INTPTR i3 = 1; i3 < Result.GetSize(); i3++) 
								if (word_array[i].GetLength() == Result[i3].m_szRoot.GetLength()) 
									ws.ai = Result[i3];						
					
						ws.ai.m_szRoot += ws.ai.m_szEnding + ws.ai.m_szClitic;
					}
					
					if (word_array[i].GetAt(0) != L'+') {
						CFSWString s = make_char_string(ws.ai.m_szRoot);
						if (s.GetLength() != word_array[i].GetLength())
							ws.ai.m_szRoot = word_array[i];
					}
					
					ws.ai.m_szRoot = ws.ai.m_szRoot.ToLower();
					p.word_vector.AddItem(ws);

	}
}



CFSArray<CFSWString> do_all (CFSWString text) {	
	
	utterance_struct u;
		u.phone_c = 0;
		u.syl_c = 0;
		u.word_c = 0;
		u.phra_c = 0;
		u.s = text;	
	
		do_phrases(u);
		INTPTR phra_c = u.phr_vector.GetSize();		
		u.phra_c += phra_c;
			
		INTPTR sylinutt = 1, wordinutt = 1, phrinutt = 1;
			
			for (INTPTR i1 = 0; i1 < u.phr_vector.GetSize(); i1++) {

				phrase2words (u.phr_vector[i1]);
				/**/				
				INTPTR word_c = u.phr_vector[i1].word_vector.GetSize();	
				u.phr_vector[i1].word_c += word_c;
				u.word_c += word_c;
				
				u.phr_vector[i1].utt_p = phrinutt++;				
				
				INTPTR sylinphr = 1, wordinphr = 1;

					for (INTPTR i2 = 0; i2 < u.phr_vector[i1].word_vector.GetSize(); i2++) {
						do_syls(u.phr_vector[i1].word_vector[i2]); //syls.cpp
						INTPTR syl_c = u.phr_vector[i1].word_vector[i2].syl_vector.GetSize();	
							u.phr_vector[i1].word_vector[i2].syl_c += syl_c;
							u.phr_vector[i1].syl_c += syl_c;
							u.syl_c += syl_c;
							
							u.phr_vector[i1].word_vector[i2].phr_p = wordinphr++;
							u.phr_vector[i1].word_vector[i2].utt_p = wordinutt++;
							
							
							for (INTPTR i3 = 0; i3 < u.phr_vector[i1].word_vector[i2].syl_vector.GetSize(); i3++) {
								do_phones(u.phr_vector[i1].word_vector[i2].syl_vector[i3]);
										
										u.phr_vector[i1].word_vector[i2].syl_vector[i3].word_p = i3+1;
										
										u.phr_vector[i1].word_vector[i2].syl_vector[i3].phr_p = sylinphr++;
										u.phr_vector[i1].word_vector[i2].syl_vector[i3].utt_p = sylinutt++;
										
										for (INTPTR i4 = 0; i4 < u.phr_vector[i1].word_vector[i2].syl_vector[i3].phone_vector.GetSize(); i4++) {
											phone_struct pho = u.phr_vector[i1].word_vector[i2].syl_vector[i3].phone_vector[i4];
											pho.syl_p = i4 + 1;
											pho.word_p = u.phr_vector[i1].word_vector[i2].phone_c + i4 + 1;
											pho.phr_p = u.phr_vector[i1].phone_c + i4 + 1;
											pho.utt_p = u.phone_c + i4 + 1;
											u.phr_vector[i1].word_vector[i2].syl_vector[i3].phone_vector[i4] = pho;
										}
								
								INTPTR phone_c = u.phr_vector[i1].word_vector[i2].syl_vector[i3].phone_vector.GetSize();
								u.phr_vector[i1].word_vector[i2].syl_vector[i3].phone_c += phone_c;
								u.phr_vector[i1].word_vector[i2].phone_c += phone_c;
								u.phr_vector[i1].phone_c += phone_c;
								u.phone_c += phone_c;
							}					
					}
			}

return do_label(u);



}




























