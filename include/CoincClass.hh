#ifndef CoincClass_H__
#define CoincClass_H__

#include "globalheaders.hh"
#include "HitClass.hh"

class CoincClass
{
public:
					CoincClass() : m_classification(-1) {};
	virtual 		~CoincClass(){};
	
	HitClass 		hit1;
	HitClass 		hit2;

    int             m_classification;
                        // See: include/VIPglobalheaders.hh (in VIPCommonLib)
                        //  1 = RANDOM
                        //  2 = SCATTERED
                        //  0 = TRUE
};


#endif // #define CoincClass_H__
