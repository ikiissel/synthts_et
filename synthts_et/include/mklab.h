#include "../lib/etana/proof.h"

extern CLinguistic Linguistic;	
extern CDisambiguator Disambiguator;


CFSWString DealWithText (CFSWString text);
CFSArray<CFSWString> do_utterances(CFSWString s);
CFSArray<CFSWString> do_all (CFSWString text, bool print_label, bool print_utt);
