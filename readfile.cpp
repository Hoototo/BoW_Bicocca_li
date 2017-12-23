#include <iostream>
//#include <algorithm>
#include <fstream>
//#include <chrono>

//#include <opencv2/core/core.hpp>

//#include <System.h>
#include <vector>
#include <sstream>
using namespace std;

void LoadDesc(const string &rgbFilename, vector<string> &vstrImageFilenamesRGB, vector<double> &vTimestamps);

int main(int argc, char **argv)
{
    if(argc != 2)
    {
        cerr << endl << "Usage: ./a.out path_to_rgbtxt" << endl;
        return 1;
    }

    // Retrieve paths to images
    vector<string> vstrImageFilenamesRGB;
    vector<double> vTimestamps;
    string rgbFilename = string(argv[1]);
    LoadDesc(rgbFilename, vstrImageFilenamesRGB, vTimestamps);
}

void LoadDesc(const string &rgbFilename, vector<string> &vstrImageFilenamesRGB, vector<double> &vTimestamps)
{
    ifstream fAssociation;
    fAssociation.open(rgbFilename.c_str());
    int i =0;
    while(!fAssociation.eof())
    {
        string s;
        getline(fAssociation,s);
        
        if(!s.empty())
        {
            stringstream ss;
            ss << s;
            double t;
            string sRGB;
            ss >> t;
            //cout << "stamp:" << t << endl;
            vTimestamps.push_back(t);
            
            ss >> sRGB;
            //cout << "RGBfilename:" << sRGB << endl;
            vstrImageFilenamesRGB.push_back(sRGB);

        }
    }
    cout.setf(ios::fixed,ios::floatfield);
    cout << "stampnum:" << vTimestamps.size() << " " << "stamp 1:" << vTimestamps[0] << "stamp 2965:" << vTimestamps[2964] <<endl;
    cout << "RGBnum:" << vstrImageFilenamesRGB.size() << "RGB 1:" << vstrImageFilenamesRGB[0] <<  "RGB 2:" << vstrImageFilenamesRGB[1] <<endl;
}