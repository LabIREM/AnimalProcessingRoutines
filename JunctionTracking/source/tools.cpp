/*** Tools
 *
 * Toolkit ...
 *
 * @author GOYA Yuki
 * @version 2012-08-01
 */

#include "tools.h"

#include <iostream>
#include <sstream>
#include <cmath>
#include <algorithm>
#include <complex>      // to compute mean angle
#include <queue>        // used in sortPix

using namespace std;

// JUNK
#if 0
unsigned int GLOBALVARS::HEIGHT = 0;
unsigned int GLOBALVARS::WIDTH = 0;

/*** computes the standard deviation.
 * @param elts the list of value
 * @param mean the mean value
 * @return the standard deviation of the list
 */
double ecartType(list<double> elts, double meanValue)
{
    double stdvalue = 0;
    for(list<double>::iterator itlist = elts.begin(); itlist != elts.end(); itlist++)
    {
        stdvalue += (*itlist - meanValue) * (*itlist - meanValue);
    }
    stdvalue = stdvalue / (double)elts.size();
    stdvalue = sqrt(stdvalue);
    if(stdvalue < 1e-10)
    {
        stdvalue = 0;
    }
    return stdvalue;
}

/*** dilate a list of pixels with an elementary 4-connex cercle.
 * @param pixels the list of pixels linear indexes
 * @param width the image width
 * @param height the image height
 * @return the list of dilated pixels linear indexes
 */
list<unsigned int> dilate(list<unsigned int> pixels, unsigned int width, unsigned int height)
{
    list<unsigned int> dilatedPixels(pixels); // copy of pixels
    unsigned int nbPix = width * height;
    for(list<unsigned int>::iterator itPixels = pixels.begin(); itPixels != pixels.end(); itPixels++)
    {
        unsigned int y = *itPixels / width;
        // left
        unsigned int di = (*itPixels) - 1;
        unsigned int diy = di / width;
        if( (y == diy) && (di >= 0) && (find(dilatedPixels.begin(), dilatedPixels.end(), di) == dilatedPixels.end()) )
        {
            dilatedPixels.push_back(di);
        }
        // right
        di = (*itPixels) + 1;
        diy = di / width;
        if( (y == diy) && (di < nbPix) && (find(dilatedPixels.begin(), dilatedPixels.end(), di) == dilatedPixels.end()) )
        {
            dilatedPixels.push_back(di);
        }
        // up
        di = (*itPixels) - width;
        if( (di >= 0) && (find(dilatedPixels.begin(), dilatedPixels.end(), di) == dilatedPixels.end()) )
        {
            dilatedPixels.push_back(di);
        }
        // down
        di = (*itPixels) + width;
        if( (di < nbPix) && (find(dilatedPixels.begin(), dilatedPixels.end(), di) == dilatedPixels.end()) )
        {
            dilatedPixels.push_back(di);
        }
    }
    return dilatedPixels;
}

// 2011-09-28
unsigned int bin2dec(string strbin)
{
    unsigned int res = 0;
    for(int i = strbin.size() - 1; i > -1; i--)
    {
        if(strbin[i] == '1')
        {
            res += (1 << (strbin.size() - 1 - i));
        }
    }
    return res;
}

//2011-11-23
list<unsigned int> dilate(list<unsigned int> pixels, unsigned int width, unsigned int height)
{
    unsigned int nbPix = width * height;
    unsigned int nelmt = pixels->size();
    for(unsigned int i = 0; i < nelmt; i++)
    {

    }
}

/*** dilate a list of pixels with an elementary 4-connex cercle.
 * @param pixels the list of pixels linear indexes
 * @param width the image width
 * @param height the image height
 * @return the list of dilated pixels linear indexes
 */
list<unsigned int> dilate(list<unsigned int> pixels, unsigned int width, unsigned int height)
{
    list<unsigned int> dilatedPixels(pixels); // copy of pixels
    unsigned int nbPix = width * height;
    for(list<unsigned int>::iterator itPixels = pixels.begin(); itPixels != pixels.end(); itPixels++)
    {
        unsigned int y = *itPixels / width;
        // left
        unsigned int di = (*itPixels) - 1;
        unsigned int diy = di / width;
        if((y == diy) && (di >= 0))
        {
            dilatedPixels.push_back(di);
        }
        // right
        di = (*itPixels) + 1;
        diy = di / width;
        if((y == diy) && (di < nbPix))
        {
            dilatedPixels.push_back(di);
        }
        // up
        di = (*itPixels) - width;
        if(di >= 0)
        {
            dilatedPixels.push_back(di);
        }
        // down
        di = (*itPixels) + width;
        if(di < nbPix)
        {
            dilatedPixels.push_back(di);
        }
    }
    // sort the list
    dilatedPixels.sort();
    // remove duplicate elements and resize the list
    dilatedPixels.erase(unique(dilatedPixels.begin(), dilatedPixels.end()), dilatedPixels.end());

    return dilatedPixels;
}



fpSlicer::fpSlicer(string path)
{
    // slicing ...
    size_t found = path.find_last_of(".");
    if(found == string::npos) // something wrong: no extension?
    {
        return;
    }

    // set file extension
    extension = path.substr(found);
    // slice for rootname
    string fileroot = path.substr(0, found);
    found = fileroot.find_last_of("_");
    if(found == string::npos) // format must be [root]_[filenumber].[extension]"
    {
        return;
    }

    // file rootname
    rootname = fileroot.substr(0, found + 1); //include the '_'
    // file number
    string nfile_s = fileroot.substr(found + 1);
    // number of digit
    ndigits = nfile_s.size();
    // file number
    number = (unsigned int)atoi(nfile_s.c_str());
}


// ----------------------------- //
//* elements to be sorted later *//
// ----------------------------- //

string fpSlicer::getRootname()
{
    return rootname;
}

string fpSlicer::getExtension()
{
    return extension;
}

unsigned int fpSlicer::getNumber()
{
    return number;
}

unsigned short fpSlicer::getNdigits()
{
    return ndigits;
}

bool fpSlicer::isEmpty()
{
    return rootname.empty();
}

#endif

/*** convert an int to a string
 * @param i the int to convert
 * @return the string representation of i
 */
string intToString(int i)
{
    ostringstream out;
    out << i;
    return out.str();
}

/*** convert an int to a string with a specific number of digit
 * @param i the int to convert
 * @param ndigit the number of digit
 * @return the representation of i with ndigit digits.
 */
string numberToString(unsigned int i, unsigned int ndigit)
{
    string s_i = intToString(i);
    unsigned int padding = ndigit - (unsigned int) s_i.size();
    while(padding-- != 0)
    {
        s_i = "0" + s_i;
    }
    return s_i;
}

/*** chop the trailing white spaces and new lines of a string.
 * aiming to be similar to Perl's chomp function ...
 * @param the string to chop
 * @return the chopped string
 */
string chomp(string s)
{
    int i = s.size() - 1;
    while(i >= 0 && (s[i] == '\n' || s[i] == ' ' || s[i] == '\r'))
    {
        i--;
    }
    return s.substr(0, i + 1);
}

/*** convert the pixel index (used within this program) into its Matlab equivalent.
 * @param index the pixel index to convert
 * @param height the image height
 * @param width the image width
 * @return the equivalent pixel index in Matlab
 */
unsigned int indexToMatlabIndex(unsigned int index, unsigned int height, unsigned int width)
{
    unsigned int x_c, y_c;
    x_c = index % width;
    y_c = (index - x_c) / width;
    return x_c * height + y_c + 1;
}

/*** convert the pixel index into (X,Y) coordinates.
 * @param index the pixel index
 * @param width the image width
 * @return the (X,Y) coordinates of index.
 */
pair<unsigned int, unsigned int> indexToXY(unsigned int index, unsigned int width)
{
    unsigned int x = index % width;
    unsigned int y = (index - x) / width;
    return make_pair(x, y);
}

/*** mean angle
 * % References:
%   Statistical analysis of circular data, N. I. Fisher
%   Topics in circular statistics, S. R. Jammalamadaka et al.
%   Biostatistical Analysis, J. H. Zar
 * @param elts the vector of values
 * @return the mean angle
 */
double meanAngle(vector<double> elts) // 2011-08-31
{
    complex<double> tot = 0;
    for(vector<double>::iterator it = elts.begin(); it != elts.end(); it++)
    {
        tot += exp(complex<double>(0, *it));
    }
    return atan2(tot.imag(), tot.real());
}

/*** median
 * @param v the vector of values
 * @return the median value
 */
double median(vector<double> v)
{
    vector<double>sv = v;
    sort(sv.begin(), sv.end());
    if(sv.size() % 2 != 0)
    {
        return sv[(sv.size() - 1) / 2];
    }
    else
    {
        return (sv[sv.size() / 2] + sv[sv.size() / 2 - 1]) / 2;
    }
}

/*** round to the closest integer (positive value only)
 * @param a the value
 * @return the closest integer value of a
 */
unsigned int UIRound(double a)
{
    if(a <= 0)
    {
        return 0;
    }
    else
    {
        return (unsigned int)(floor(a + 0.5));
    }
}

/*** comparison between 2 elements of the tag list. lower first order
 * @param s_i the first tag
 * @param s_j the second tag
 * @return true if s_i < s_j
 */
bool cmpTAG(string s_i, string s_j)
{
    string ss_i = s_i;
    string ss_j = s_j;
    // remove non-digit prefix
    while(!isdigit(ss_i.front()))
    {
        ss_i = ss_i.substr(1);
    }
    while(!isdigit(ss_j.front()))
    {
        ss_j = ss_j.substr(1);
    }
    // convert to int
    int i_i = atoi(ss_i.c_str());
    int i_j = atoi(ss_j.c_str());
    if(i_i < i_j)
    {
        return true;
    }
    else if(i_i == i_j)
    {

        //std::cout << "ss_i : " << ss_i << "\tss_j : " << ss_j << endl;

        size_t it = ss_i.find_first_of("_");

        while(it != string::npos)
        {
            ss_i = ss_i.substr(it + 1);
            ss_j = ss_j.substr(it + 1);
            i_i = atoi(ss_i.c_str());
            i_j = atoi(ss_j.c_str());
            if(i_i != i_j)
            {
                return (i_i < i_j);
            }
            it = ss_i.find_first_of("_");
        }
    }
    return false;
}

/*** convertion of the neighbours tag vector to string
 * @param vNeighbour the reference to the neighbour tags vector
 * @param toSort true if the vector needs to be sorted
 * @return a string representation of the tags vector
 */
string tagVectorToString(vector<string> &vNeighbour, bool toSort)
{
    if(toSort)
    {
        sort(vNeighbour.begin(), vNeighbour.end(), cmpTAG);
    }
    if(vNeighbour.empty())
    {
        return "";
    }
    else if(vNeighbour.size() > 1) // several matches
    {
        string s = "(";
        vector<string>::iterator itVNeighbour;
        for(itVNeighbour = vNeighbour.begin(); itVNeighbour != vNeighbour.end(); itVNeighbour++)
        {
            s += (*itVNeighbour) + " ";
        }
        s[s.size() - 1] = ')'; // replace the last white space by a parenthesis
        return s;
    }
    else // only one match
    {
        return vNeighbour.front();
    }
}

/*** test if two pixels are 4-connex
* @param p1 pair unsigned int coordinate x,y
* @param p2 pair unsigned int coordinate x,y
* @return true if they are 4-connex
*/
bool is4connex(std::pair<unsigned int, unsigned int> &p1, std::pair<unsigned int, unsigned int> &p2)
{
    bool flag = false;
    int dx = p1.first - p2.first;
    int dy = p1.second - p2.second;
    if ((dx == 1 || dx == -1) && dy == 0)
    {
        flag = true;
    }
    else if ((dy == 1 || dy == -1) && dx == 0)
    {
        flag = true;
    }
    return flag;
}

// return true p1(x,y) & p2(x,y) are 8-connex
bool is8connex(std::pair<unsigned int, unsigned int> &p1, std::pair<unsigned int, unsigned int> &p2)
{
    bool flag = false;
    int dx = p1.first - p2.first;
    int dy = p1.second - p2.second;
    if (((dx >= -1) && (dx <= 1)) && ((dy >= -1) && (dy <= 1)))
    {
        flag = true;
    }
    return flag;
}

//end of file
