#include "../etana/proof.h"

struct phone_struct {
	CFSWString phone;
	CFSWString lpos, pos, rpos;
	INTPTR syl_p, word_p, phr_p, utt_p;
	INTPTR p6, p7, a1, a3, b1, b3, c1, c3, b4, b5, b6, b7, d2, e2, f2, e3, e4, g1, g2, h1, h2, i1, i2, h3, h4, j1, j2, j3;
};

struct syl_struct {
	CFSWString syl;
	INTPTR stress;
	CFSArray<phone_struct> phone_vector;
	INTPTR phone_c, word_p, phr_p, utt_p;
};

struct word_struct {
	CMorphInfo mi;
	CFSArray<syl_struct> syl_vector;
	INTPTR phone_c, syl_c, phr_p, utt_p;
};

struct phrase_struct {
	CFSWString s;
	CFSArray<word_struct> word_vector;
	INTPTR phone_c, syl_c,	word_c, utt_p;
};

struct utterance_struct {
	CFSWString s;
	CFSArray<phrase_struct> phr_vector;
	INTPTR phone_c, syl_c,	word_c,	phra_c;
};

struct text_struct {
	CFSArray<utterance_struct> utt_vector;
};

const CFSWString sp = L" ";
const CFSWString d = L"-";
const CFSWString colon = L":";
const CFSWString no_synth = L"-";
const CFSWString empty_str = L""; //peaks vaatama, et kuidas ilusti nullida
const CFSWString doq = L"w";

bool is_lvowel (CFSWString c);
bool is_uvowel (CFSWString c);
bool is_lconsonant (CFSWString c);
bool is_uconsonant (CFSWString c);
bool is_vowel (CFSWString c);
bool is_consonant (CFSWString c);
bool is_upper (CFSWString c);
bool is_char (CFSWString c);
bool is_fchar (CFSWString c);
bool is_tab (CFSWString c);
bool is_break (CFSWString c);
bool is_space (CFSWString c);
bool is_whitespace (CFSWString c);
bool is_ending (CFSWString c);
bool is_digit (CFSWString c);
bool is_colon (CFSWString c);
bool is_semicolon (CFSWString c);
bool is_comma (CFSWString c);
bool is_symbol (CFSWString c);
bool is_bbracket (CFSWString c);
bool is_ebracket (CFSWString c);
bool is_conju (CFSWString c);
bool is_hyphen (CFSWString c);
bool is_bhyphen (CFSWString c);
bool is_underscore (CFSWString c);
bool is_compound_word (CFSWString c);
bool is_upper_word (CFSWString c);

bool has_vowel(CFSWString s);
void make_ctype_array (CFSWString s, CFSArray<CFSWString> &a);
CFSWString make_char_string (CFSWString c);
INTPTR ctype (CFSWString c);

INTPTR is_abbreviation(CFSWString s, CFSArray<CFSWString> &a);
CFSWString replace_fchar (CFSWString c);
CFSWString replace_schar (CFSWString c);
INTPTR explode(CFSWString s, CFSWString cSplitter, CFSArray<CFSWString> &Array);
CMorphInfo clean_anaroot_output (CMorphInfo MI);


//Muud jubinad

// fprintf sobilik convert
const char* ccstr (CFSWString s);

