/*** Class SkelPix.
 *
 * Represents a pixel of the segmentation skeleton.
 *
 * @author GOYA Yuki
 * @version 2012-08-01
 */

#ifndef SKELPIX_H
#define SKELPIX_H

#include <string>
#include <vector>
#include <utility> // for std::pair

class SkelPix
{
    ///pixel id
    unsigned int idPixel;
    /// neighbouring cells id number
    unsigned int neighbours[4];
    /// image width
    static unsigned int WIDTH; //2011-10-25
    /// image height
    static unsigned int HEIGHT; //2011-10-25
public:
// contructors & destructor
    SkelPix();
    SkelPix(unsigned int, unsigned int, unsigned int, unsigned int, unsigned int);
// setters & getters
    unsigned int GetIdPixel();
    unsigned int GetNeighbour(unsigned int);
    unsigned int GetNbNeighbours();
    static void SetWidth(unsigned int); //2011-10-25
    static void SetHeight(unsigned int); //2011-10-25
    static unsigned int GetWidth(); //2011-10-25
    static unsigned int GetHeight(); //2011-10-25
    std::pair<unsigned int, unsigned int> GetXY(); //2012-01-19
    std::vector< std::pair<unsigned int, unsigned int> > GetNeighboursCoupleList();
    void FillNeighboursCoupleList(std::vector<std::pair<unsigned int, unsigned int> > &);
    void Reset(); //2012-06-07
    void Fill(unsigned int, unsigned int, unsigned int, unsigned int, unsigned int); //2012-06-07
// other methods
    bool IsVertex();
    std::string ToString();
    void FillString(std::string &s);
};

#endif // SKELPIX_H
