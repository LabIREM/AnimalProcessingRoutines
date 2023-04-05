/*** Tools
 *
 * Toolkit ...
 *
 * WARNING: many unused code & brouillon included ... to be cleaned sooner or later ...
 *
 * @author GOYA Yuki
 * @version 2012-08-01
 */

#ifndef TOOLS_H
#define TOOLS_H

#include <string>
#include <vector>

// PI
const double PI = 3.14159265358979224;

// JUNK
#if 0
class GLOBALVARS
{
public:
    static unsigned int HEIGHT;
    static unsigned int WIDTH;
};
double ecartType(std::list<double>, double);
std::list<unsigned int> dilate(std::list<unsigned int>, unsigned int, unsigned int);
unsigned int bin2dec(std::string); //2011-09-28

class fpSlicer
{
    std::string rootname;
    std::string extension;
    unsigned int number;
    unsigned short ndigits;
public:
    fpSlicer(std::string);
    std::string getRootname();
    std::string getExtension();
    unsigned int getNumber();
    unsigned short getNdigits();
    bool isEmpty();
};
#endif

// convert int to string
std::string intToString(int);
// convert int to string with a specific number of digits
std::string numberToString(unsigned int, unsigned int);
// remove trailing white spaces
std::string chomp(std::string);
// convert pixel index into Matlab pixel index
unsigned int indexToMatlabIndex(unsigned int, unsigned int, unsigned int);
// convert index into (x,y) coordinates
std::pair<unsigned int, unsigned int> indexToXY(unsigned int, unsigned int);
// mean angle
double meanAngle(std::vector<double>); //2011-08-31
// median
double median(std::vector<double>); //2012-01-17
// round to closest integer (only for positive value)
unsigned int UIRound(double); // 2012-04-05
// convert tag list to a string
std::string tagVectorToString(std::vector<std::string> &, bool);
// compare 2 tags
bool cmpTAG(std::string, std::string);

bool is4connex(std::pair<unsigned int, unsigned int> &, std::pair<unsigned int, unsigned int> &);
bool is8connex(std::pair<unsigned int, unsigned int> &, std::pair<unsigned int, unsigned int> &);



#endif // TOOLS_H