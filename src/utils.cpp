#include "utils.h"

// Get the minimum t and index of the closest hit in a vector of hits. If no
// hit occurs, then returns -1 for the index.
std::tuple<Hit *, int>
getClosestHit (std::vector<Hit *> hits)
{
  double minT = std::numeric_limits<double>::max ();
  Hit *minHit = nullptr;
  int index = -1;
  for (size_t i = 0; i < hits.size (); ++i)
    {
      if (hits[i] && hits[i]->isInfront && hits[i]->minusT < minT)
        {
          minT = hits[i]->minusT;
          minHit = hits[i];
          index = i;
        }
    }
  return { minHit, index };
}
