/*** Class IntensityV.
 *
 * Intensity values for a border.
 *
 * @author GOYA Yuki
 * @version 2012-04-06
 */

#include "intensityV.h"


/*** constructor */
IntensityV::IntensityV()
{
    nbDilatedPixel = 0;
    shortNbDilatedPixel = 0;
    sumOfIntensity = 0;
    shortSumOfIntensity = 0;
    localBackground = 0;
    localSumIntensity = 0;
}
/*** access to sumOfIntensity */
double IntensityV::GetSumOfIntensity()
{
    return sumOfIntensity;
}
/*** access to shortSumOfIntensity */
double IntensityV::GetShortSumOfIntensity()
{
    return shortSumOfIntensity;
}
/*** access to ndilpix */
unsigned int IntensityV::GetNbDilatedPixel()
{
    return nbDilatedPixel;
}
/*** access to shortndilpix */
unsigned int IntensityV::GetShortNbDilatedPixel()
{
    return shortNbDilatedPixel;
}
/*** access to localSumBackground */
double IntensityV::GetLocalBackground()
{
    return localBackground;
}
/*** Set local background */
void IntensityV::SetLocalBackground(unsigned int nb)
{
    if(nb != 0)
    {
        localBackground /= (double)nb;
    }
    else
    {
        localBackground = -1;
    }
}
/*** access to localSumIntensity */
double IntensityV::GetLocalSumIntensity()
{
    return localSumIntensity;
}
/*** increase sumOfIntensity */
void IntensityV::accumulateF(double val)
{
    sumOfIntensity += val;
    nbDilatedPixel++;
}
/*** increase shortSumOfIntensity */
void IntensityV::accumulateS(double val)
{
    shortSumOfIntensity += val;
    shortNbDilatedPixel++;
}
/*** increase localSumIntensity */
void IntensityV::accumulateLSI(double val)
{
    localSumIntensity += val;
}
/*** increase localSumBackground */
void IntensityV::accumulateLSB(double val)
{
    localBackground += val;
}

//end of file