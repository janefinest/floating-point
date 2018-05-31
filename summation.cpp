/*******************************************************************************
This software was developed by employees of the National Institute of Standards
and Technology (NIST), an agency of the Federal Government. Pursuant to Title 17
United States Code Section 105, works of NIST employees are not subject to
copyright protection in the United States and are considered to be in the public
domain. Permission to freely use, copy, modify, and distribute this software and
its documentation without fee is hereby granted, provided that this notice and
disclaimer of warranty appears in all copies.

THE SOFTWARE IS PROVIDED 'AS IS' WITHOUT ANY WARRANTY OF ANY KIND, EITHER
EXPRESSED, IMPLIED, OR STATUTORY, INCLUDING, BUT NOT LIMITED TO, ANY WARRANTY
THAT THE SOFTWARE WILL CONFORM TO SPECIFICATIONS, ANY IMPLIED WARRANTIES OF
MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE, AND FREEDOM FROM INFRINGEMENT,
AND ANY WARRANTY THAT THE DOCUMENTATION WILL CONFORM TO THE SOFTWARE, OR ANY
WARRANTY THAT THE SOFTWARE WILL BE ERROR FREE. IN NO EVENT SHALL NIST BE LIABLE
FOR ANY DAMAGES, INCLUDING, BUT NOT LIMITED TO, DIRECT, INDIRECT, SPECIAL OR
CONSEQUENTIAL DAMAGES, ARISING OUT OF, RESULTING FROM, OR IN ANY WAY CONNECTED
WITH THIS SOFTWARE, WHETHER OR NOT BASED UPON WARRANTY, CONTRACT, TORT, OR
OTHERWISE, WHETHER OR NOT INJURY WAS SUSTAINED BY PERSONS OR PROPERTY OR
OTHERWISE, AND WHETHER OR NOT LOSS WAS SUSTAINED FROM, OR AROSE OUT OF THE
RESULTS OF, OR USE OF, THE SOFTWARE OR SERVICES PROVIDED HEREUNDER.

Distributions of NIST software should also include copyright and licensing
statements of any third-party software that are legally bundled with the code
in compliance with the conditions of those licenses.
*******************************************************************************/

/**
   \file summation.cpp
   \brief map histogram of values

   In principal, \$ 10 \times 10^{-3}
                  +  9 \times 10^{-2}
                  +  9 \times 10^{-1}
                  +  9 \times 10^{0}
                  +  9 \times 10^{1}
                  +  9 \times 10^{2}
                  +  9 \times 10^{3}
                  = 10000.\$
   Similarly, \$ 8 \times 2^{-4}
               + 8 \times 2^{-3}
               + 8 \times 2^{-2}
               + 9 \times 2^{-1}
               + 8 \times 2^{0}
               + 8 \times 2^{1}
               + 8 \times 2^{2}
               + 8 \times 2^{3}
               = 128.\$
   This program demonstrates that the decimal power series is approximate, not
   associative, while the binary power series is exact.
 */

#include <algorithm>
#include <iostream>
#include <map>
#include <random>

int main()
{
    std::map<float,unsigned int> imprecision;
    const int N = 1e6;

    std::random_device Maxwell;
    std::default_random_engine prng;
    prng.seed(Maxwell());

    std::vector<float> x;
    #ifdef DECIMAL
    for (unsigned int i = 0; i < 9; i++) {
        x.push_back(   0.001);
        x.push_back(   0.01);
        x.push_back(   0.1);
        x.push_back(   1.);
        x.push_back(  10.);
        x.push_back( 100.);
        x.push_back(1000.);
    }
    x.push_back(0.001); // there's an extra one of these guys

    #else
    for (unsigned int i = 0; i < 8; i++) {
        x.push_back(0.0625);
        x.push_back(0.1250);
        x.push_back(0.2500);
        x.push_back(0.5000);
        x.push_back(1.0000);
        x.push_back(2.0000);
        x.push_back(4.0000);
        x.push_back(8.0000);
    }
    x.push_back(0.5000); // there's an extra one of these guys

    #endif

    for (unsigned int i = 0; i < N; i++) {
        std::random_shuffle(x.begin(), x.end());
        float z = 0;
        for (unsigned int i = 0; i < x.size(); i++)
            z += x[i];

        imprecision[z]++;
    }

    for (std::map<float,unsigned int>::const_iterator it = imprecision.begin(); it != imprecision.end(); it++) {
        printf("%32.26f: %12.9f %%\n", it->first, double(100*it->second)/N);
    }

    return 0;
}
