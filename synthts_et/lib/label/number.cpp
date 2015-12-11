#include "../etana/proof.h"

CFSWString int_to_words (CFSWString numStr) {
    CFSWString rs;

 	CFSWString onesName[] = {
 		L"üks", L"kaks", L"kolm", L"neli", L"viis", L"kuus", L"seitse", L"kaheksa", L"üheksa"
 	};

	CFSWString teensName[] = {
 		L"kümme", L"üksteist", L"kaksteist", L"kolmteist", L"neliteist", L"viisteist", L"kuusteist", L"seitseteist", L"kaheksateist", L"üheksateist"
	};

	CFSWString tensName[] = {
		L"kakskümmend", L"kolmkümmend", L"nelikümmend", L"viiskümmend", L"kuuskümmend", L"seitsekümmend", L"kaheksakümmend", L"üheksakümmend"
	};

 	CFSWString onesNameO[] = {
 		L"esimene", L"teine", L"kolmas", L"neljas", L"viies", L"kuues", L"seitsmes", L"kaheksas", L"üheksas",
	};

 	CFSWString teensNameO[] = {
 		L"kümmes", L"üheteistkümnes", L"kaheteistkümnes", L"kolmeteistkümnes", L"neljateistkümnes", 
 		L"viieteistkümnes", L"kuueteistkümnes", L"seitsmeteistkümnes", L"kaheksateistkümnes", L"üheksateistkümnes"
	};

  CFSWString tensNameO[] = { 
  	L"kahekümne", L"kolmekümne", L"neljakümne", L"viiekümne",   L"kuuekümne",   L"seitsmekümne",   L"kaheksakümne",   L"üheksakümne" 
  };

		
  CFSWString illion_preName[] = { L"m", L"b", L"tr", L"kvadr", L"kvint", L"sekst", L"sept", L"okt", L"non", L"dets" };
  CFSWString decillion_preName[]={ L"un", L"duo", L"tre", L"kvattuor", L"kvin", L"seks", L"septen", L"okto", L"novem" };

      while (numStr.GetAt(0) == L'0') {
      	rs += L"null ";
      	numStr.Delete(0, 1);
      }
      if (numStr.GetLength() == 0) {
      	numStr.Trim();
      	return rs;
      }
      
      // process the validated entry
        while( numStr.GetLength()%3 != 0 )
            numStr = L'0' + numStr;// pad the string with leading '0' until size = multiple of 3

        // for each group of 3 digits from most to least significant
        for( unsigned int i = 0; i < numStr.GetLength(); i += 3 )
        {
            // skip if all 3 digits == '0'
            if( numStr.GetAt(i) == L'0' && numStr.GetAt(i+1) == L'0' && numStr.GetAt(i+2) == L'0' )
                continue;

            if( numStr.GetAt(i + 0) > L'0' ) {// treat the hundreds place
                rs += onesName[ numStr.GetAt(i + 0) - 48 - 1 ] + L"sada ";
               }

            if( numStr.GetAt(i + 1) == L'0' || numStr.GetAt(i + 1) > L'1' )// treat tens and ones digits for non-teens case
            {
                if( numStr.GetAt(i + 1) > L'1' ) { 
                		rs += tensName[ numStr.GetAt(i + 1) - 48 - 2 ] + L" ";
                	}
                if( numStr.GetAt(i + 2) > L'0' ) {
                 		rs += onesName[ numStr.GetAt(i + 2) - 48 - 1 ] + L" ";
                 }
            }
            else {// special teens case
                rs += teensName[ numStr.GetAt(i + 2) - 48 ] + L" ";                
               }

            // naming each factor of 1,000
            unsigned int j = ( numStr.GetLength() - i )/3;
            if( j == 2 ) {
             	rs += L"tuhat ";
             }
            else if( j > 2 )
            {

                if( j <= 12 ) {
                 	rs += illion_preName[ j - 3 ];
                }
                else if( j <= 21 ) {
                	rs += decillion_preName[ j - 13 ] + L"dets";
                }
                else if( j == 22 ){                	
                	rs += L"vigint";
                }
                rs += L"iljon ";
            }
        }

rs.Trim(); 
return rs; 

}






