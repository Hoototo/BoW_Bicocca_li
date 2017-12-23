/**
 * File: Demo.cpp
 * Date: November 2011
 * Author: Dorian Galvez-Lopez
 * Description: demo application of DBoW2
 * License: see the LICENSE.txt file
 */

#include <iostream>
#include <vector>

// read
#include <cmath>
#include <fstream>

// DBoW2
#include "DBoW2.h" // defines Surf64Vocabulary and Surf64Database

#include "DUtils.h"
#include "DUtilsCV.h" // defines macros CVXX
#include "DVision.h"

// OpenCV
#include <opencv/cv.h>
#include <opencv/highgui.h>
#if CV24
#include <opencv2/nonfree/features2d.hpp>
#endif


using namespace DBoW2;
using namespace DUtils;
using namespace std;

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

void loadFeatures(vector<vector<vector<float> > > &features, vector<string> vTimestamps);
void changeStructure(const vector<float> &plain, vector<vector<float> > &out,
  int L);
void testVocCreation(const vector<vector<vector<float> > > &features);
void testDatabase(const vector<vector<vector<float> > > &features);

vector<float> InputData_To_Vector(string ss);

void LoadDesc(const string &rgbFilename, vector<string> &vstrImageFilenamesRGB, vector<string> &vTimestamps);


// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

// number of training images
const int NIMAGES = 123;  

// extended surf gives 128-dimensional vectors
const bool EXTENDED_SURF = true;  //false

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

void wait()
{
  cout << endl << "Press enter to continue" << endl;
  getchar();
}

// ----------------------------------------------------------------------------

int main(int argc, char **argv)
{
    if(argc != 2)
  {
      cerr << endl << "Usage: ./demolift path_to_rgbtxt" << endl;
      return 1;
  }

  // Retrieve paths to images
  vector<string> vstrImageFilenamesRGB;
  vector<string> vTimestamps;
  string rgbFilename = string(argv[1]);
  LoadDesc(rgbFilename, vstrImageFilenamesRGB, vTimestamps);

  vector<vector<vector<float> > > features;

  loadFeatures(features,vTimestamps);

  testVocCreation(features);

  wait();

  testDatabase(features);

  return 0;
}

// ----------------------------------------------------------------------------

void loadFeatures(vector<vector<vector<float> > > &features, vector<string> vTimestamps)
{
  features.clear();
  features.reserve(NIMAGES);

  //cv::SURF surf(400, 4, 2, EXTENDED_SURF);

  cout << "Extracting LIFT features..." << endl;
  // 需要修改NIMAGES的值,i的初始值为0,因为vector是从0开始的,NIMAGES的值是文件个数.
  for(int i = 0; i < NIMAGES; ++i)
  {
    stringstream ss;
    ss << "images/Bicoccalift_desc/" << vTimestamps[i] << ".txt";

    //cout << "ss.str(): " << ss.str() << endl;

    vector<float> descriptors = InputData_To_Vector(ss.str());

    features.push_back(vector<vector<float> >());
    //changeStructure(descriptors, features.back(), surf.descriptorSize());
    changeStructure(descriptors, features.back(), 128);
  }
}

// ----------------------------------------------------------------------------
vector<float> InputData_To_Vector(string ss)
{
    vector<float> p;
    ifstream infile(ss.c_str());
    float number;
    while(! infile.eof())
    {
        infile >> number;
        p.push_back(number);
    }
    p.pop_back();  //此处要将最后一个数字弹出，是因为上述循环将最后一个数字读取了两次
    return p;
}

// ----------------------------------------------------------------------------
// --------------------------read rgb.txt time stamp---------------------------
void LoadDesc(const string &rgbFilename, vector<string> &vstrImageFilenamesRGB, vector<string> &vTimestamps)
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
            string t;
            string sRGB;
            ss >> t;
            //cout << "stamp:" << t << endl;
            vTimestamps.push_back(t);
            
            ss >> sRGB;
            //cout << "RGBfilename:" << sRGB << endl;
            vstrImageFilenamesRGB.push_back(sRGB);

        }
    }
}

// ----------------------------------------------------------------------------

void changeStructure(const vector<float> &plain, vector<vector<float> > &out,
  int L)
{
  out.resize(plain.size() / L);

  unsigned int j = 0;
  for(unsigned int i = 0; i < plain.size(); i += L, ++j)
  {
    out[j].resize(L);
    std::copy(plain.begin() + i, plain.begin() + i + L, out[j].begin());
  }
}

// ----------------------------------------------------------------------------

void testVocCreation(const vector<vector<vector<float> > > &features)
{
  // branching factor and depth levels
  const int k = 10;  //9
  const int L = 5;   //3
  const WeightingType weight = TF_IDF;
  const ScoringType score = L2_NORM; //L1_NORM

  Lift128Vocabulary voc(k, L, weight, score);

  cout << "Creating a small " << k << "^" << L << " vocabulary..." << endl;
  voc.create(features);
  cout << "... done!" << endl;

  cout << "Vocabulary information: " << endl
  << voc << endl << endl;

  // lets do something with this vocabulary
  cout << "Matching images against themselves (0 low, 1 high): " << endl;
  BowVector v1, v2;
  for(int i = 0; i < NIMAGES; i++)
  {
    voc.transform(features[i], v1);
    for(int j = 0; j < NIMAGES; j++)
    {
      voc.transform(features[j], v2);

      double score = voc.score(v1, v2);
      cout << "Image " << i << " vs Image " << j << ": " << score << endl;
    }
  }

  // save the vocabulary to disk
  cout << endl << "Saving vocabulary..." << endl;
  voc.save("Bicoccalift.yml.gz");
  cout << "Done" << endl;
}

// ----------------------------------------------------------------------------

void testDatabase(const vector<vector<vector<float> > > &features)
{
  cout << "Creating a small database..." << endl;

  // load the vocabulary from disk
  Lift128Vocabulary voc("Bicoccalift.yml.gz");

  Lift128Database db(voc, false, 0); // false = do not use direct index
  // (so ignore the last param)
  // The direct index is useful if we want to retrieve the features that
  // belong to some vocabulary node.
  // db creates a copy of the vocabulary, we may get rid of "voc" now

  // add images to the database
  for(int i = 0; i < NIMAGES; i++)
  {
    db.add(features[i]);
  }

  cout << "... done!" << endl;

  cout << "Database information: " << endl << db << endl;

  // and query the database
  cout << "Querying the database: " << endl;

  QueryResults ret;
  for(int i = 0; i < NIMAGES; i++)
  {
    db.query(features[i], ret, 4);

    // ret[0] is always the same image in this case, because we added it to the
    // database. ret[1] is the second best match.

    cout << "Searching for Image " << i << ". " << ret << endl;
  }

  cout << endl;

  // we can save the database. The created file includes the vocabulary
  // and the entries added
  cout << "Saving database..." << endl;
  db.save("Bicoccalift.yml.gz");
  cout << "... done!" << endl;

  // once saved, we can load it again
  cout << "Retrieving database once again..." << endl;
  Lift128Database db2("Bicoccalift.yml.gz");
  cout << "... done! This is: " << endl << db2 << endl;
}

// ----------------------------------------------------------------------------
