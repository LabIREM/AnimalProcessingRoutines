/*** Class IntensityV.
 *
 * Intensity values for a border.
 *
 * @author GOYA Yuki
 * @version 2012-04-06
 */

#ifndef INTENSITYV
#define INTENSITYV

class IntensityV
{
    ///*** number of pixel for dilated border */
    unsigned int nbDilatedPixel;
    /*** sum of intensity for dilated border */
    double sumOfIntensity;
    /*** sum of intensity for dilated border without vertexes */
    double shortSumOfIntensity;
    ///*** number of pixel for dilated border without vertexes */
    unsigned int shortNbDilatedPixel;
    /*** local background */
    double localBackground;
    /*** local intensity */
    double localSumIntensity;
public:
// constructor
    /*** constructor */
    IntensityV();
// Setter & Getter
    /*** access to sumofint */
    double GetSumOfIntensity();
    /*** access to shortsumofint */
    double GetShortSumOfIntensity();
    /*** access to ndilpix */
    unsigned int GetNbDilatedPixel();
    /*** access to shortndilpix */
    unsigned int GetShortNbDilatedPixel();
    /*** access to localSumBackground */
    double GetLocalBackground();
    /*** Set local background */
    void SetLocalBackground(unsigned int);
    /*** access to localSumIntensity */
    double GetLocalSumIntensity();
// other method
    /*** increase sumofint */
    void accumulateF(double);
    /*** increase shortsumofint */
    void accumulateS(double);
    /*** increase localSumIntensity */
    void accumulateLSI(double);
    /*** increase localSumBackground */
    void accumulateLSB(double);
};

#endif // INTENSITYV