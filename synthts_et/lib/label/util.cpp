#include <iostream>
#include "../etana/proof.h"
#include "util.h"


bool is_lvowel (CFSWString c) {
	if (c.FindOneOf(L"aeiouõäöüy") > -1)	return true;
		return false;
}

bool is_uvowel (CFSWString c) {
	if (c.FindOneOf(L"AEIOUÕÄÖÜY") > -1)	return true;
		return false;
}

bool is_lconsonant (CFSWString c) {
	if (c.FindOneOf(L"bcdfghjklmnpqrsšžtvwxz") > -1)	return true;
		return false;
}

bool is_uconsonant (CFSWString c) {
	if (c.FindOneOf(L"BCDFGHJKLMNPQRSŠŽTVWXZ") > -1) return true;
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
	if (c.FindOneOf(sp) > -1)	return true;
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
	if ( (c.FindOneOf(L"-*@#€$£%&/\\=+~<>§αβγδ") > -1) || is_underscore(c)) 	return true;
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

bool is_upper_word (CFSWString c) {
		for (INTPTR i = 0; i < c.GetLength(); i++)
			if (!is_upper(c.GetAt(i))) return false;
		return true;
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




const char* ccstr (CFSWString s) {
    return FSStrWtoA(s, FSCP_UTF8);
}


INTPTR is_abbreviation(CFSWString s, CFSArray<CFSWString> &a) {
    // Lühendid EKRst
    if (s.GetAt(s.GetLength() - 1) == L'.') s.Delete(s.GetLength() - 1, 1);
    if (s == L"aj") return explode(L"ajutine", sp, a);
    if (s == L"ak") return explode(L"arvelduskonto", sp, a);
    if (s == L"apr") return explode(L"aprill", sp, a);
    if (s == L"AS") return explode(L"aktsiaselts", sp, a);
    if (s == L"aug") return explode(L"august", sp, a);
    if (s == L"aü") return explode(L"ametiühing", sp, a);
    if (s == L"dets") return explode(L"detsember", sp, a);
    if (s == L"dl") return explode(L"dessertlusikatäis", sp, a);
    if (s == L"dots") return explode(L"dotsent", sp, a);
    if (s == L"dr") return explode(L"doktor", sp, a);
    if (s == L"e") return explode(L"ehk", sp, a);
    if (s == L"E") return explode(L"esmaspäev", sp, a);
    if (s == L"eKr") return explode(L"enne Kristuse sündi", sp, a);
    if (s == L"EL") return explode(L"Euroopa Liit", sp, a);
    if (s == L"e.m.a") return explode(L"enne meie ajaarvamist", sp, a);
    if (s == L"end") return explode(L"endine", sp, a);
    if (s == L"FIE") return explode(L"füüsilisest isikust ettevõtja", sp, a);
    if (s == L"hr") return explode(L"härra", sp, a);
    if (s == L"hrl") return explode(L"harilikult", sp, a);
    if (s == L"ik") return explode(L"isikukood", sp, a);
    if (s == L"ins") return explode(L"insener", sp, a);
    if (s == L"it") return explode(L"infotehnoloogia", sp, a);
    if (s == L"IT") return explode(L"infotehnoloogia", sp, a);
    if (s == L"j.a") return explode(L"juures asuv", sp, a);
    if (s == L"jaan") return explode(L"jaanuar", sp, a);
    if (s == L"jj") return explode(L"ja järgmine", sp, a);
    if (s == L"jm") return explode(L"ja muud", sp, a);
    if (s == L"jms") return explode(L"ja muud sellised", sp, a);
    if (s == L"jmt") return explode(L"ja mitmed teised", sp, a);
    if (s == L"jn") return explode(L"joonis", sp, a);
    if (s == L"jne") return explode(L"ja nii edasi", sp, a);
    if (s == L"jpt") return explode(L"ja paljud teised", sp, a);
    if (s == L"jr") return explode(L"juunior", sp, a);
    if (s == L"jrk") return explode(L"järjekord", sp, a);
    if (s == L"jsk") return explode(L"jaoskond", sp, a);
    if (s == L"jt") return explode(L"ja teised", sp, a);
    if (s == L"juh") return explode(L"juhataja", sp, a);
    if (s == L"jun") return explode(L"juunior", sp, a);
    if (s == L"jv") return explode(L"järv", sp, a);
    if (s == L"K") return explode(L"kolmapäev", sp, a);
    if (s == L"k.a") return explode(L"kaasa arvatud", sp, a);
    if (s == L"kd") return explode(L"köide", sp, a);
    if (s == L"khk") return explode(L"kihelkond", sp, a);
    if (s == L"kk") return explode(L"keskkool", sp, a);
    if (s == L"kl") return explode(L"kell", sp, a);
    if (s == L"knd") return explode(L"kandidaat", sp, a);
    if (s == L"kod") return explode(L"kodanik", sp, a);
    if (s == L"kpl") return explode(L"kauplus", sp, a);
    if (s == L"kr") return explode(L"kroon", sp, a);
    if (s == L"kt") return explode(L"kohusetäitja", sp, a);
    if (s == L"kub") return explode(L"kubermang", sp, a);
    if (s == L"kv") return explode(L"kvartal", sp, a);
    if (s == L"KÜ") return explode(L"korteriühistu", sp, a);
    if (s == L"l") return explode(L"liiter", sp, a);
    if (s == L"L") return explode(L"laupäev", sp, a);
    if (s == L"lg") return explode(L"lõige", sp, a);
    if (s == L"lj") return explode(L"linnajagu", sp, a);
    if (s == L"lk") return explode(L"lehekülg", sp, a);
    if (s == L"LK") return explode(L"looduskaitse all", sp, a);
    if (s == L"lm") return explode(L"liidumaa", sp, a);
    if (s == L"lo") return explode(L"linnaosa", sp, a);
    if (s == L"lp") return explode(L"lugupeetud", sp, a);
    if (s == L"lüh") return explode(L"lühemalt", sp, a);
    if (s == L"M") return explode(L"meestele", sp, a);
    if (s == L"mag") return explode(L"magister", sp, a);
    if (s == L"m.a.j") return explode(L"meie ajaarvamise järgi", sp, a);
    if (s == L"min") return explode(L"minut", sp, a);
    if (s == L"mk") return explode(L"maakond", sp, a);
    if (s == L"mld") return explode(L"miljard", sp, a);
    if (s == L"mln") return explode(L"miljon", sp, a);
    if (s == L"mnt") return explode(L"maantee", sp, a);
    if (s == L"mob") return explode(L"mobiiltelefon", sp, a);
    if (s == L"ms") return explode(L"muuseas", sp, a);
    if (s == L"MTÜ") return explode(L"mittetulundusühing", sp, a);
    if (s == L"N") return explode(L"neljapäev", sp, a);
    if (s == L"nim") return explode(L"nimeline", sp, a);
    if (s == L"nn") return explode(L"niinimetatud", sp, a);
    if (s == L"nov") return explode(L"november", sp, a);
    if (s == L"nr") return explode(L"number", sp, a);
    if (s == L"nt") return explode(L"näiteks", sp, a);
    if (s == L"nö") return explode(L"nii öelda", sp, a);
    if (s == L"okt") return explode(L"oktoober", sp, a);
    if (s == L"osk") return explode(L"osakond", sp, a);
    if (s == L"OÜ") return explode(L"osaühing", sp, a);
    if (s == L"P") return explode(L"pühapäev", sp, a);
    if (s == L"pa") return explode(L"poolaasta", sp, a);
    if (s == L"pk") return explode(L"postkast", sp, a);
    if (s == L"pKr") return explode(L"pärast Kristuse sündi", sp, a);
    if (s == L"pms") return explode(L"peamiselt", sp, a);
    if (s == L"p.o") return explode(L"peab olema", sp, a);
    if (s == L"pr") return explode(L"proua", sp, a);
    if (s == L"prl") return explode(L"preili", sp, a);
    if (s == L"prof") return explode(L"professor", sp, a);
    if (s == L"ps") return explode(L"poolsaar", sp, a);
    if (s == L"pst") return explode(L"puiestee", sp, a);
    if (s == L"ptk") return explode(L"peatükk", sp, a);
    if (s == L"R") return explode(L"reede", sp, a);
    if (s == L"raj") return explode(L"rajoon", sp, a);
    if (s == L"rbl") return explode(L"rubla", sp, a);
    if (s == L"RE") return explode(L"riigiettevõte", sp, a);
    if (s == L"reg") return explode(L"registreerimis", sp, a);
    if (s == L"rg") return explode(L"registri", sp, a);
    if (s == L"rmtk") return explode(L"raamatukogu", sp, a);
    if (s == L"rkl") return explode(L"riigikoguliige", sp, a);
    if (s == L"rtj") return explode(L"raudteejaam", sp, a);
    if (s == L"s") return explode(L"sekund", sp, a);
    if (s == L"SA") return explode(L"sihtasutus", sp, a);
    if (s == L"s.a") return explode(L"sel aastal", sp, a);
    if (s == L"saj") return explode(L"sajand", sp, a);
    if (s == L"sh") return explode(L"sealhulgas", sp, a);
    if (s == L"sen") return explode(L"seenior", sp, a);
    if (s == L"sept") return explode(L"september", sp, a);
    if (s == L"skp") return explode(L"selle kuu päeval", sp, a);
    if (s == L"spl") return explode(L"supilusikatäis", sp, a);
    if (s == L"sl") return explode(L"supilusikatäis", sp, a);
    if (s == L"sm") return explode(L"seltsimees", sp, a);
    if (s == L"s.o") return explode(L"see on", sp, a);
    if (s == L"s.t") return explode(L"see tähendab", sp, a);
    if (s == L"st") return explode(L"see tähendab", sp, a);
    if (s == L"stj") return explode(L"saatja", sp, a);
    if (s == L"srn") return explode(L"surnud", sp, a);
    if (s == L"sü") return explode(L"säilitusüksus", sp, a);
    if (s == L"snd") return explode(L"sündinud", sp, a);
    if (s == L"t") return explode(L"tonn", sp, a);
    if (s == L"T") return explode(L"teisipäev", sp, a);
    if (s == L"tehn") return explode(L"tehniline", sp, a);
    if (s == L"tel") return explode(L"telefon", sp, a);
    if (s == L"tk") return explode(L"tükk(i)", sp, a);
    if (s == L"tl") return explode(L"teelusikatäis", sp, a);
    if (s == L"tlk") return explode(L"tõlkija", sp, a);
    if (s == L"Tln") return explode(L"Tallinn", sp, a);
    if (s == L"tn") return explode(L"tänav", sp, a);
    if (s == L"tr") return explode(L"trükk", sp, a);
    if (s == L"Trt") return explode(L"Tartu", sp, a);
    if (s == L"tv") return explode(L"televisioon", sp, a);
    if (s == L"TV") return explode(L"televisioon", sp, a);
    if (s == L"u") return explode(L"umbes", sp, a);
    if (s == L"ukj") return explode(L"uue kalendri järgi", sp, a);
    if (s == L"UÜ") return explode(L"usaldusühing", sp, a);
    if (s == L"v") return explode(L"või", sp, a);
    if (s == L"v.a") return explode(L"välja arvatud", sp, a);
    if (s == L"van") return explode(L"vananenud", sp, a);
    if (s == L"VE") return explode(L"väikeettevõte", sp, a);
    if (s == L"veebr") return explode(L"veebruar", sp, a);
    if (s == L"vkj") return explode(L"vana kalendri järgi", sp, a);
    if (s == L"vm") return explode(L"või muu(d)", sp, a);
    if (s == L"vms") return explode(L"või muu seesugune", sp, a);
    if (s == L"vrd") return explode(L"võrdle", sp, a);
    if (s == L"vt") return explode(L"vaata", sp, a);
    if (s == L"õa") return explode(L"õppeaasta", sp, a);
    if (s == L"õp") return explode(L"õpetaja", sp, a);
    if (s == L"õpil") return explode(L"õpilane", sp, a);
    if (s == L"ÄÜ") return explode(L"äriühing", sp, a);
    if (s == L"ÜE") return explode(L"ühisettevõte", sp, a);
    if (s == L"SEK") return explode(L"Rootsi krooni", sp, a);
    if (s == L"NOK") return explode(L"Norra krooni", sp, a);
    if (s == L"RUR") return explode(L"Vene rubla", sp, a);
    if (s == L"USD") return explode(L"USA dollarit", sp, a);
    if (s == L"GBP") return explode(L"Inglise naela", sp, a);
    if (s == L"LVL") return explode(L"Läti latti", sp, a);
    if (s == L"LTL") return explode(L"Leedu litti", sp, a);
    if (s == L"EEK") return explode(L"Eesti krooni", sp, a);
    return 0;
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
    if (c == L"α") return L"alfa";
    if (c == L"β") return L"beta";
    if (c == L"γ") return L"gamma";
    if (c == L"δ") return L"delta";
    
    return L"tundmatumärk";
}































