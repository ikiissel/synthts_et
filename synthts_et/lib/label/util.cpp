#include <iostream>
#include "../etana/proof.h"

bool is_lvowel (CFSWString c) {
	if (c.FindOneOf(L"aeiouõäöü") > -1)	return true;
		return false;
}

bool is_uvowel (CFSWString c) {
	if (c.FindOneOf(L"AEIOUÕÄÖÜ") > -1)	return true;
		return false;
}

bool is_lconsonant (CFSWString c) {
	if (c.FindOneOf(L"bcdfghjklmnpqrsšžtvwxyz") > -1)	return true;
		return false;
}

bool is_uconsonant (CFSWString c) {
	if (c.FindOneOf(L"BCDFGHJKLMNPQRSŠŽTVWXYZ") > -1) return true;
		return false;
}

bool is_ending (CFSWString c) {
	if (c.FindOneOf(L".?!;") > -1)	return true;
		return false;
}

bool is_tab (CFSWString c) {
	if (c.FindOneOf(L"\t\n") > -1)	return true;
		return false;
}

bool is_break (CFSWString c) {
	if (c.FindOneOf(L"\n") > -1)	return true;
		return false;
}

bool is_space (CFSWString c) {
	if (c.FindOneOf(L" ") > -1)	return true;
		return false;
}

bool is_fchar (CFSWString c) {
	if (c.FindOneOf(L"ÀÁÂÃàáâãÈÉÊËèéêëÌÍÎÏŸÝìíîïÿýÒÓÔÅòóôåÙÚÛùúûæÆØŒøœñÑçÇßÐÞðþ") > -1)	return true;
		return false;
}

bool is_underscore (CFSWString c) { //liitsõna värk, selleks lahku
	if (c.FindOneOf(L"_") > -1)	return true;
		return false;
}

bool is_symbol (CFSWString c) {
	if ( (c.FindOneOf(L"-*@#€$£%&/\\=+~<>§") > -1) || is_underscore(c)) 	return true;
		return false;
}

bool is_digit (CFSWString c) {
	if (c.FindOneOf(L"0123456789") > -1)	return true;
		return false;
}

bool is_colon (CFSWString c) {
	if (c.FindOneOf(L":") > -1)	return true;
		return false;
}

bool is_semicolon (CFSWString c) {
	if (c.FindOneOf(L";") > -1)	return true;
		return false;
}

bool is_comma (CFSWString c) {
	if (c.FindOneOf(L",") > -1)	return true;
		return false;
}

bool is_dash (CFSWString c) {
	if (c.FindOneOf(L"–") > -1)	return true;
		return false;
}

bool is_hyphen (CFSWString c) {
	if (c.FindOneOf(L"-") > -1)	return true;
		return false;
}

bool is_bbracket (CFSWString c) {
	if (c.FindOneOf(L"([{") > -1)	return true;
		return false;
}

bool is_ebracket (CFSWString c) {
	if (c.FindOneOf(L")]}") > -1)	return true;
		return false;
}

bool is_bhyphen (CFSWString c) {
	if (is_hyphen(c) || is_dash(c)) return true;
		return false;
}


bool is_whitespace (CFSWString c) {
	if (is_tab(c) || is_space(c)) return true;
		return false;
}

bool is_vowel (CFSWString c) {
	if (is_lvowel(c) || is_uvowel(c)) return true;
		return false;
}

bool is_consonant (CFSWString c) {
	if (is_lconsonant(c) || is_uconsonant(c))	return true;
		return false;
}

bool is_upper (CFSWString c) {
	if (is_uconsonant(c) || is_uvowel(c))	return true;
		return false;
}

bool is_char (CFSWString c) {
	if (is_vowel(c) || is_consonant(c))	return true;
		return false;
}

bool is_conju (CFSWString c) {
	if (c == L"ja" || c == L"ning" || c == L"ega" || c == L"ehk" || c == L"või")	return true;
		return false;
}

bool is_compound_word (CFSWString c) {
		for (INTPTR i = 0; i < c.GetLength(); i++)
			if (is_underscore(c.GetAt(i))) return true;
		return false;
}

bool is_fsymbol (CFSWString c) { //ainult tüübiarray jaoks
	if (is_symbol(c) || is_ending(c) || is_colon(c) || is_dash(c) || is_hyphen(c) || is_comma(c))	return true;
		return false;
}


bool has_vowel(CFSWString s) {
		for (INTPTR i = 0; i < s.GetLength(); i++)	{
			if (is_vowel(s.GetAt(i))) return true;
		}
		return false;
}

INTPTR ctype (CFSWString c) {
	if (is_char(c)) return 1;
		else
	if (is_fsymbol(c)) return 2;
	 else
	if (is_digit(c)) return 3;
	return 0;
}

void make_ctype_array (CFSWString s, CFSArray<CFSWString> &a) {
	for (INTPTR i = 0; i < s.GetLength(); i++)	{
		CFSWString c = s.GetAt(i);
		if (i == 0) {
			a.AddItem(c);
		}
		else {
			INTPTR arr_size = a.GetSize()-1;
			CFSWString lc = a[arr_size];
			lc = lc.GetAt(lc.GetLength()-1);
			if ( ctype(c) == ctype(lc) ) 
					a[arr_size] += c;			
				else 
					a.AddItem(c);
		}
	}
}

CFSWString make_char_string (CFSWString c) {
	CFSWString res;
	for (INTPTR i = 0; i < c.GetLength(); i++)	{
		if (is_char(c.GetAt(i))) res += c.GetAt(i);
	}		
	return res;
}


INTPTR explode(CFSWString s, CFSWString cSplitter, CFSArray<CFSWString> &Array) {
	Array.Cleanup();
	INTPTR ipStart=0;
	for (INTPTR ip=0; ip<s.GetLength(); ip++){
		if (s.GetAt(ip)==cSplitter){
			Array.AddItem(s.Mid(ipStart, ip-ipStart));
			ipStart=ip+1;
		}
		else
		if (cSplitter.GetLength() == 0)
			Array.AddItem(s.GetAt(ip));
	}
	if (cSplitter.GetLength() > 0) 
		Array.AddItem(s.Mid(ipStart));
	return Array.GetSize();
}


CMorphInfo clean_anaroot_output (CMorphInfo MI) {
	CFSWString s;
	for (INTPTR i=0; i < MI.m_szRoot.GetLength(); i++) {
		if (MI.m_szRoot.GetAt(i) != L'=') s += MI.m_szRoot.GetAt(i);
	}	
	MI.m_szRoot = s;
	return MI;
}




void Print (CFSWString label, CFSWString s) {
	wprintf(L"\n<");
	wprintf(label);
	wprintf(L">");
	wprintf(s);
	wprintf(L"\n");

}
void PRINDI_YHESTAMINE (CFSArray<CMorphInfo> A) {
		wprintf (L"\n\n\t\tYhestamine(%i)\t\t\n\t\t-------------------\n", A.GetSize());
		for (INTPTR ipRes=0; ipRes<A.GetSize(); ipRes++) {
			wprintf (L"\t\t");
			wprintf (A[ipRes].m_szRoot);			
			wprintf (L" [");
			wprintf (A[ipRes].m_szForm);			
			wprintf (L"] [");
			wprintf (CFSWString(A[ipRes].m_cPOS));
	
			
			wprintf (L"]\n");				
			}
//wprintf (L"\t\t-------------------\n");
}

void PrintAnalyze(CMorphInfo ai) {
		wprintf (L"root: ");
		wprintf (ai.m_szRoot);
		wprintf (L"\tpos: ");
		wprintf (CFSWString(ai.m_cPOS));
		wprintf (L"\tform: ");
		wprintf (CFSWString(ai.m_szForm));
		wprintf (L"\tend: ");
		wprintf (CFSWString(ai.m_szEnding));
		wprintf (L"\tclit: ");
		wprintf (CFSWString(ai.m_szClitic));
		wprintf (L"\n\n");
}




CFSWString replace_fchar (CFSWString c) {
	if (c == L"À") return L"a";
	if (c == L"Á") return L"a";
	if (c == L"Â") return L"a";
	if (c == L"Ã") return L"a";
	if (c == L"à") return L"a";
	if (c == L"á") return L"a";
	if (c == L"â") return L"a";
	if (c == L"ã") return L"a";
	if (c == L"È") return L"e";
	if (c == L"É") return L"e";
	if (c == L"Ê") return L"e";
	if (c == L"Ë") return L"e";
	if (c == L"è") return L"e";
	if (c == L"é") return L"e";
	if (c == L"ê") return L"e";
	if (c == L"ë") return L"e";
	if (c == L"Ì") return L"i";
	if (c == L"Í") return L"i";
	if (c == L"Î") return L"i";
	if (c == L"Ï") return L"i";
	if (c == L"Ÿ") return L"i";
	if (c == L"Ý") return L"i";
	if (c == L"ì") return L"i";
	if (c == L"í") return L"i";
	if (c == L"î") return L"i";
	if (c == L"ï") return L"i";
	if (c == L"ÿ") return L"ü";
	if (c == L"ý") return L"ü";
	if (c == L"Ò") return L"o";
	if (c == L"Ó") return L"o";
	if (c == L"Ô") return L"o";
	if (c == L"Å") return L"o";
	if (c == L"ò") return L"o";
	if (c == L"ó") return L"o";
	if (c == L"ô") return L"o";
	if (c == L"å") return L"o";
	if (c == L"Ù") return L"u";
	if (c == L"Ú") return L"u";
	if (c == L"Û") return L"u";
	if (c == L"ù") return L"u";
	if (c == L"ú") return L"u";
	if (c == L"û") return L"u";
	if (c == L"æ") return L"ä";
	if (c == L"Æ") return L"ä";
	if (c == L"Ø") return L"ö";
	if (c == L"Œ") return L"ö";
	if (c == L"ø") return L"ö";
	if (c == L"œ") return L"ö";
	if (c == L"ñ") return L"n";
	if (c == L"Ñ") return L"n";
	if (c == L"ç") return L"s";
	if (c == L"Ç") return L"s";
	if (c == L"ß") return L"s";
	if (c == L"Ð") return L"k";
	if (c == L"Þ") return L"k";
	if (c == L"ð") return L"k";
	if (c == L"þ") return L"k";	

}

CFSWString replace_schar (CFSWString c) {
    c = c.ToLower();
    if (c == L"a") return L"aa";
    if (c == L"b") return L"bee";
    if (c == L"c") return L"tsee";
    if (c == L"d") return L"dee";
    if (c == L"e") return L"ee";
    if (c == L"f") return L"eff";
    if (c == L"g") return L"gee";
    if (c == L"h") return L"haa";
    if (c == L"i") return L"ii";
    if (c == L"j") return L"jott";
    if (c == L"k") return L"kaa";
    if (c == L"l") return L"ell";
    if (c == L"m") return L"emm";
    if (c == L"n") return L"enn";
    if (c == L"o") return L"oo";
    if (c == L"p") return L"pee";
    if (c == L"q") return L"kuu";
    if (c == L"r") return L"err";
    if (c == L"s") return L"ess";
    if (c == L"š") return L"shaa";
    if (c == L"ž") return L"shee";
    if (c == L"t") return L"tee";
    if (c == L"u") return L"uu";
    if (c == L"v") return L"vee";
    if (c == L"w") return L"kaksisvee";
    if (c == L"õ") return L"õõ";
    if (c == L"ä") return L"ää";
    if (c == L"ö") return L"öö";
    if (c == L"ü") return L"üü";
    if (c == L"x") return L"iks";
    if (c == L"y") return L"igrek";
    if (c == L"z") return L"tsett";
    if (c == L".") return L"punkt";
    if (c == L",") return L"koma";
    if (c == L":") return L"koolon";
    if (c == L";") return L"semikoolon";
    if (c == L"-") return L"kriips";
    if (c == L"_") return L"alakriips";
    if (c == L"*") return L"tärn";
    if (c == L"!") return L"hüüumärk";
    if (c == L"?") return L"küsimärk";
    if (c == L"@") return L"ätt";
    if (c == L"#") return L"trellid";
    if (c == L"€") return L"eurot";
    if (c == L"$") return L"dollarit";
    if (c == L"£") return L"naela";
    if (c == L"%") return L"protsenti";
    if (c == L"&") return L"änd";
    if (c == L"/") return L"kaldkriips";
    if (c == L"\\") return L"tagurpidi kaldkriips";    
    if (c == L"=") return L"võrdub";
    if (c == L"+") return L"pluss";
    if (c == L"~") return L"tilde";
    if (c == L"<") return L"suurem kui";
    if (c == L">") return L"väiksem kui";
    if (c == L"§") return L"paragraaf";
    if (c == L"(") return L"sulgudes";
    if (c == L"[") return L"nurksulgudes";
    if (c == L"{") return L"loogsulgudes";
    return L"tundmatumärk";
}































