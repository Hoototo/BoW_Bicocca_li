/**
 * File: FLift128.cpp
 * Date: November 2011
 * Author: Dorian Galvez-Lopez
 * Description: functions for Lift128 descriptors
 * License: see the LICENSE.txt file
 *
 */
 
#include <vector>
#include <string>
#include <sstream>

#include "FClass.h"
#include "FLift128.h"

using namespace std;

namespace DBoW2 {

// --------------------------------------------------------------------------

void FLift128::meanValue(const std::vector<FLift128::pDescriptor> &descriptors, 
  FLift128::TDescriptor &mean)
{
  mean.resize(0);
  mean.resize(FLift128::L, 0);
  
  float s = descriptors.size();
  
  vector<FLift128::pDescriptor>::const_iterator it;
  for(it = descriptors.begin(); it != descriptors.end(); ++it)
  {
    const FLift128::TDescriptor &desc = **it;
    for(int i = 0; i < FLift128::L; i += 4)
    {
      mean[i  ] += desc[i  ] / s;
      mean[i+1] += desc[i+1] / s;
      mean[i+2] += desc[i+2] / s;
      mean[i+3] += desc[i+3] / s;
    }
  }
}

// --------------------------------------------------------------------------
  
double FLift128::distance(const FLift128::TDescriptor &a, const FLift128::TDescriptor &b)
{
  double sqd = 0.;
  for(int i = 0; i < FLift128::L; i += 4)
  {
    sqd += (a[i  ] - b[i  ])*(a[i  ] - b[i  ]);
    sqd += (a[i+1] - b[i+1])*(a[i+1] - b[i+1]);
    sqd += (a[i+2] - b[i+2])*(a[i+2] - b[i+2]);
    sqd += (a[i+3] - b[i+3])*(a[i+3] - b[i+3]);
  }
  return sqd;
}

// --------------------------------------------------------------------------

std::string FLift128::toString(const FLift128::TDescriptor &a)
{
  stringstream ss;
  for(int i = 0; i < FLift128::L; ++i)
  {
    ss << a[i] << " ";
  }
  return ss.str();
}

// --------------------------------------------------------------------------
  
void FLift128::fromString(FLift128::TDescriptor &a, const std::string &s)
{
  a.resize(FLift128::L);
  
  stringstream ss(s);
  for(int i = 0; i < FLift128::L; ++i)
  {
    ss >> a[i];
  }
}

// --------------------------------------------------------------------------

void FLift128::toMat32F(const std::vector<TDescriptor> &descriptors, 
    cv::Mat &mat)
{
  if(descriptors.empty())
  {
    mat.release();
    return;
  }
  
  const int N = descriptors.size();
  const int L = FLift128::L;
  
  mat.create(N, L, CV_32F);
  
  for(int i = 0; i < N; ++i)
  {
    const TDescriptor& desc = descriptors[i];
    float *p = mat.ptr<float>(i);
    for(int j = 0; j < L; ++j, ++p)
    {
      *p = desc[j];
    }
  } 
}

// --------------------------------------------------------------------------

} // namespace DBoW2

